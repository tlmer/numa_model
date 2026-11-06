#pragma once

#include "cmn/numa_system.h"
#include "cmn/ra_agent.h"

#include <cstdint>
#include <deque>

namespace numa::cmn {

class VcpuAgent {
public:

    VcpuAgent(NumaSystem& sys, int node, std::uint16_t txnid_base = 0,
              unsigned max_outstanding = 16,
              std::uint32_t timeout_ticks = 1000);

    bool read(std::uint64_t line);
    bool write(std::uint64_t line, const std::uint8_t data[kLineBytes]);


    void pump();

    void tick();
    bool pop(RaCompletion& c);

    std::size_t   pending() const { return ra_.pending(); }
    int           node()    const { return node_; }
    std::uint64_t reads()   const { return reads_; }
    std::uint64_t writes()  const { return writes_; }
    std::uint64_t rejected() const { return rejected_; }
    std::uint64_t timed_out() const { return ra_.timed_out(); }

private:
    std::uint16_t next_txnid();
    cmn::WireNode::Flit mk_req(std::uint8_t reqop, std::uint16_t txnid,
                               std::uint64_t line, std::uint8_t length) const;
    bool finish(std::uint16_t txnid);

    struct Rx {
        std::uint16_t txnid    = 0;
        std::uint32_t rsp_dw   = 0;
        bool          has_data = false;
        std::uint8_t  data[kLineBytes] = {};
    };

    NumaSystem& sys_;
    int           node_;
    std::uint16_t base_;
    std::uint16_t next_ = 0;
    RaAgent       ra_;
    std::deque<Rx> rx_;
    std::deque<RaCompletion> done_;
    std::uint64_t reads_    = 0;
    std::uint64_t writes_   = 0;
    std::uint64_t rejected_ = 0;
};

}
