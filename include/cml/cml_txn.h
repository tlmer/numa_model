#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace numa::cml {

constexpr std::uint16_t kTxnIdMask = 0x0FFF;

enum class TxnState : std::uint8_t {
    Pending   = 0,
    Completed = 1,
    TimedOut  = 2,
};

struct Txn {
    std::uint16_t txnid  = 0;
    std::uint64_t addr   = 0;
    std::uint8_t  reqop  = 0;
    std::uint32_t age    = 0;
    TxnState      state  = TxnState::Pending;
};

class TxnTable {
public:
    explicit TxnTable(unsigned max_outstanding = 16,
                      std::uint32_t timeout_ticks = 1000);


    bool allocate(std::uint16_t txnid, std::uint64_t addr, std::uint8_t reqop);


    bool complete(std::uint16_t txnid);

    bool     pending(std::uint16_t txnid) const;
    std::size_t outstanding() const { return table_.size(); }
    std::uint64_t completed() const { return completed_; }
    std::uint64_t timed_out() const { return timed_out_; }
    std::uint64_t rejected()  const { return rejected_; }
    unsigned    max_outstanding() const { return max_outstanding_; }


    void tick();

    const Txn* find(std::uint16_t txnid) const;


    const std::vector<Txn>& expired() const { return expired_; }
    void clear_expired() { expired_.clear(); }

private:
    std::unordered_map<std::uint16_t, Txn> table_;
    std::vector<Txn> expired_;
    unsigned      max_outstanding_;
    std::uint32_t timeout_ticks_;
    std::uint64_t completed_ = 0;
    std::uint64_t timed_out_ = 0;
    std::uint64_t rejected_  = 0;
};

}
