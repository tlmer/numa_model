// numa_model example: CpuPort read/write, TLB invalidation, page migration,
// and SLIT calibration. Builds against include/ + libnumacml.a only.
#include "cmn/cpu_port.h"
#include "cmn/migration.h"
#include "cmn/numa_system.h"
#include "cmn/numa_version.h"
#include "perf/slit.h"

#include <cstdio>
#include <cstring>

int main() {
    bool ok = true;
    std::printf("API %u.%u.%u (0x%x) ABI %u\n",
                numa::kApiVersionMajor, numa::kApiVersionMinor,
                numa::kApiVersionPatch, numa::kApiVersion,
                numa::kAbiVersion);

    numa::cmn::NumaSystem sys;
    sys.add_node(0, 32);
    sys.add_node(32, 32);

    std::uint8_t pattern[64];
    for (unsigned i = 0; i < sizeof(pattern); ++i) {
        pattern[i] = static_cast<std::uint8_t>(0x30 + i);
    }

    numa::cml::Tlb tlb;
    tlb.insert(0x4000, 1, 0);
    numa::cmn::CpuPort cpu(sys, 0, 0x000, 4);
    cpu.attach_tlb(&tlb);
    unsigned callbacks = 0;
    cpu.on_tlb_invalidate([&](std::uint64_t) { ++callbacks; });

    std::uint8_t got[64] = {};
    ok = cpu.write(5, pattern) && cpu.read(5, got) &&
         std::memcmp(got, pattern, sizeof(pattern)) == 0 && ok;
    std::printf("read/write: %s\n", ok ? "PASS" : "FAIL");

    const std::size_t cleared = cpu.invalidate_tlb(0x4000);
    ok = ok && cleared == 1 && callbacks == 1 && tlb.size() == 0;
    std::printf("tlb invalidation: cleared=%zu callbacks=%u\n",
                cleared, callbacks);

    for (std::uint64_t line = 8; line < 12; ++line) {
        ok = cpu.write(line, pattern) && ok;
    }
    numa::cmn::MigrationManager mm(sys, nullptr, 4);
    std::uint8_t moved[64] = {};
    ok = mm.migrate_page(8, 1) && cpu.read(8, moved) &&
         std::memcmp(moved, pattern, sizeof(pattern)) == 0 && ok;
    std::printf("page migration: %s (route(8)=%d)\n",
                ok ? "PASS" : "FAIL", sys.route(8));

    numa::perf::LatencyMatrix matrix;
    matrix.resize(2);
    matrix.at(0, 0) = matrix.at(1, 1) = 80.0;
    matrix.at(0, 1) = matrix.at(1, 0) = 120.0;
    numa::perf::SlitCalibrator cal;
    const auto slit = cal.build(matrix);
    std::printf("SLIT 0->1=%u expected=15 max_err=%.6f\n",
                static_cast<unsigned>(slit.at(0, 1)),
                slit.max_rel_error_vs(matrix));
    ok = ok && slit.valid && slit.at(0, 1) == 15;

    std::printf("NUMA_EXAMPLE: %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}
