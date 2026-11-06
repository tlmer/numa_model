#pragma once

#include "cmn/numa_system.h"
#include "cmn/vcpu_agent.h"
#include "cml/dvm.h"

#include <cstdint>
#include <functional>
#include <vector>

namespace numa::cmn {

class CpuPort {
public:

    using TlbListener = std::function<void(std::uint64_t va)>;

    CpuPort(NumaSystem& sys, int node, std::uint16_t txnid_base = 0,
            unsigned outstanding = 4);

    bool read(std::uint64_t line, std::uint8_t data[kLineBytes]);
    bool write(std::uint64_t line, const std::uint8_t data[kLineBytes]);


    bool read_mmio(std::uint64_t address, unsigned size, std::uint64_t& value) {
        return sys_.mmio_read(address, size, value);
    }
    bool write_mmio(std::uint64_t address, unsigned size, std::uint64_t value) {
        return sys_.mmio_write(address, size, value);
    }


    bool  dmi_allowed() const { return false; }
    void* get_direct_mem_ptr(std::uint64_t ) { return nullptr; }

    void attach_tlb(cml::Tlb* tlb) { tlb_ = tlb; }
    void on_tlb_invalidate(TlbListener cb) { listeners_.push_back(std::move(cb)); }


    std::size_t invalidate_tlb(std::uint64_t va = ~0ull);

    VcpuAgent&       agent()       { return agent_; }
    const VcpuAgent& agent() const { return agent_; }

private:
    NumaSystem& sys_;
    VcpuAgent agent_;
    cml::Tlb* tlb_ = nullptr;
    std::vector<TlbListener> listeners_;
};

}
