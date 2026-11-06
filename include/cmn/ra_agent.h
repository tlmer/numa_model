#pragma once

#include "cml/cml_txn.h"
#include "cml/cml_wire.h"

#include <cstdint>
#include <deque>

namespace numa::cmn {

constexpr unsigned kRaLineBytes = 64;


struct RaCompletion {
    std::uint16_t txnid    = 0;
    std::uint64_t addr     = 0;
    std::uint8_t  reqop    = 0;
    bool          ok       = false;
    bool          has_data = false;
    std::uint8_t  data[kRaLineBytes] = {};
};

class RaAgent {
public:
    explicit RaAgent(unsigned max_outstanding = 16,
                     std::uint32_t timeout_ticks = 1000);


    bool issue(std::uint16_t txnid, std::uint8_t reqop, std::uint64_t line);


    bool on_rsp(std::uint32_t rsp_dw, bool has_data,
                const std::uint8_t* data, RaCompletion& out);


    void tick();


    bool pop(RaCompletion& c);

    std::size_t pending() const { return txn_.outstanding(); }
    std::uint64_t issued()          const { return issued_; }
    std::uint64_t responded()       const { return responded_; }
    std::uint64_t timed_out()       const { return timed_out_; }
    std::uint64_t issue_rejected()  const { return issue_rejected_; }
    std::uint64_t rsp_rejected()    const { return rsp_rejected_; }
    std::size_t   done_size()       const { return done_.size(); }
    cml::TxnTable& txn() { return txn_; }

private:
    cml::TxnTable txn_;
    std::deque<RaCompletion> done_;
    std::uint64_t issued_ = 0;
    std::uint64_t responded_ = 0;
    std::uint64_t timed_out_ = 0;
    std::uint64_t issue_rejected_ = 0;
    std::uint64_t rsp_rejected_ = 0;
};

}
