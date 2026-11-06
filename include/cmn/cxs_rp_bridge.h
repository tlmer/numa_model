#pragma once

#include "cml/cml_credit.h"
#include "cml/cml_credit_link.h"
#include "cmn/numa_system.h"
#include "cxs/cxs_link_model.h"

#include <cstdint>
#include <vector>

namespace numa::cmn {

class CxsRpBridge {
public:
    struct Config {
        int srcid = 1;
        int tgtid = 2;
        int latency_cycles = 1;
        int link_credits = 8;
        unsigned cml_dedicated = 4;
        unsigned cml_shared = 8;
        unsigned cml_shared_max = 64;
        std::uint64_t max_ticks_per_txn = 4096;

        bool bypass_link = false;
    };

    explicit CxsRpBridge(NumaSystem& sys);
    CxsRpBridge(NumaSystem& sys, const Config& cfg);


    bool read(std::uint64_t addr, std::uint8_t* dst, std::uint32_t n,
              std::uint32_t txnid);
    bool write(std::uint64_t addr, const std::uint8_t* src, std::uint32_t n,
               std::uint32_t txnid);

    cxs::CxsLinkModel&      link()        { return link_; }
    cml::SharedCreditBank&  bank()        { return bank_; }
    cml::CmlCreditLink&     credit_link() { return cl_; }

    std::uint64_t transactions() const { return transactions_; }
    std::uint64_t failed()       const { return failed_; }
    std::uint64_t timeouts()     const { return timeouts_; }
    std::uint64_t ticks()        const { return ticks_; }

private:

    bool transact(const std::vector<WireNode::Flit>& req,
                  std::vector<WireNode::Flit>& rsp);
    bool read_line(std::uint64_t line, std::uint8_t data[64]);
    bool write_line(std::uint64_t line, const std::uint8_t data[64]);
    std::uint16_t alloc_txnid();

    WireNode::Flit make_req_flit(std::uint8_t reqop, std::uint16_t txnid,
                                 std::uint64_t line, std::uint8_t length) const;
    void ensure_link_credits(int need_credits);

    NumaSystem&            sys_;
    Config                 cfg_;
    cxs::CxsLinkModel      link_;
    cml::SharedCreditBank  bank_;
    cml::CmlCreditLink     cl_;
    std::uint32_t          next_pkt_ = 1;
    std::uint16_t          next_txnid_ = 1;

    std::uint64_t transactions_ = 0;
    std::uint64_t failed_ = 0;
    std::uint64_t timeouts_ = 0;
    std::uint64_t ticks_ = 0;
};

}
