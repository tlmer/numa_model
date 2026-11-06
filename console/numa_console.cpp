// numa_console: small interactive front end for numa_model.
// Commands: help, info, read <line>, write <line> <byte>, migrate <line> <node>,
//           slit, perf, tc list|run <id> [n]|all [n]|coverage, quit
#include "cmn/cpu_port.h"
#include "cmn/migration.h"
#include "cmn/numa_system.h"
#include "cmn/numa_version.h"
#include "perf/slit.h"
#include "tc/numa_tc.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

int main() {
    numa::cmn::NumaSystem sys;
    sys.add_node(0, 32);
    sys.add_node(32, 32);
    numa::cmn::CpuPort cpu(sys, 0, 0x000, 4);
    auto registry = numa::tc::make_builtin_registry();
    std::uint8_t last[64] = {};
    std::printf("numa_model console API %u.%u.%u; type help\n",
                numa::kApiVersionMajor, numa::kApiVersionMinor,
                numa::kApiVersionPatch);
    std::string line;
    while (std::getline(std::cin, line)) {
        char cmd[32] = {};
        unsigned long a = 0, b = 0;
        int n = std::sscanf(line.c_str(), "%31s %lu %lu", cmd, &a, &b);
        if (n <= 0) continue;
        if (!std::strcmp(cmd, "quit") || !std::strcmp(cmd, "exit")) break;
        if (!std::strcmp(cmd, "help")) {
            std::printf("help | info | read <line> | write <line> <byte> | "
                        "migrate <line> <node> | slit | perf | "
                        "tc list|run <id> [n]|all [n]|coverage | quit\n");
        } else if (!std::strcmp(cmd, "info")) {
            std::printf("nodes=%zu api=%u abi=%u dmi=%d\n",
                        sys.node_count(), numa::kApiVersion,
                        numa::kAbiVersion, cpu.dmi_allowed() ? 1 : 0);
        } else if (!std::strcmp(cmd, "read") && n >= 2) {
            std::uint8_t buf[64] = {};
            if (cpu.read(a, buf)) {
                std::printf("line %lu:", a);
                for (unsigned i = 0; i < 64; ++i) std::printf(" %02x", buf[i]);
                std::printf("\n");
            } else {
                std::printf("read failed\n");
            }
        } else if (!std::strcmp(cmd, "write") && n >= 3) {
            std::memset(last, static_cast<int>(b & 0xff), sizeof(last));
            std::printf("write line %lu byte 0x%02lx: %s\n", a, b & 0xff,
                        cpu.write(a, last) ? "ok" : "failed");
        } else if (!std::strcmp(cmd, "migrate") && n >= 3) {
            numa::cmn::MigrationManager mm(sys, nullptr, 4);
            std::printf("migrate line %lu -> node %lu: %s\n", a, b,
                        mm.migrate_page(a, static_cast<int>(b)) ? "ok" : "failed");
        } else if (!std::strcmp(cmd, "slit")) {
            numa::perf::LatencyMatrix m;
            m.resize(2);
            m.at(0,0) = m.at(1,1) = 80.0;
            m.at(0,1) = m.at(1,0) = 120.0;
            numa::perf::SlitCalibrator cal;
            const auto s = cal.build(m);
            std::printf("SLIT 0->1=%u max_err=%.6f valid=%d\n",
                        static_cast<unsigned>(s.at(0,1)),
                        s.max_rel_error_vs(m), s.valid ? 1 : 0);
        } else if (!std::strcmp(cmd, "perf")) {
            std::printf("perf: use test_perf_cases in the licensed test set; "
                        "console exposes functional operations only\n");
        } else if (!std::strcmp(cmd, "tc")) {
            char sub[32] = {};
            unsigned long arg = 0, iters = 1;
            const int sn = std::sscanf(line.c_str(), "%31s %31s %lu %lu", cmd, sub,
                                       &arg, &iters);
            if (sn >= 2 && !std::strcmp(sub, "list")) {
                const auto cov = registry.coverage();
                for (const auto& item : cov.items) {
                    if (item.implemented) {
                        std::printf("%u %s%s\n", item.id, item.name.c_str(),
                                    item.tested ? " [tested]" : " [untested]");
                    } else {
                        std::printf("N/A %s: %s\n", item.name.c_str(),
                                    item.reason.c_str());
                    }
                }
            } else if (sn >= 3 && !std::strcmp(sub, "run")) {
                const auto r = registry.run(static_cast<unsigned>(arg),
                                            static_cast<unsigned>(iters));
                std::printf("tc run %lu: %s (ran=%u passed=%u failed=%u)\n", arg,
                            r.ok() ? "PASS" : "FAIL", r.ran, r.passed, r.failed);
            } else if (sn >= 2 && !std::strcmp(sub, "all")) {
                const unsigned n = (sn >= 3) ? static_cast<unsigned>(arg) : 1u;
                const auto r = registry.run_all(n);
                std::printf("tc all: %s (cases=%u ran=%u passed=%u failed=%u)\n",
                            r.ok() ? "PASS" : "FAIL", r.cases, r.ran, r.passed,
                            r.failed);
            } else if (sn >= 2 && !std::strcmp(sub, "coverage")) {
                std::cout << registry.coverage_text();
            } else {
                std::printf("usage: tc list|run <id> [n]|all [n]|coverage\n");
            }
        } else {
            std::printf("unknown command\n");
        }
    }
    return 0;
}
