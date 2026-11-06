#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace numa::cmn {


enum class SnoopOp : std::uint8_t {
    SNPTOANY = 0x0,
    SNPTOC   = 0x1,
    SNPTOS   = 0x2,
    SNPTOSC  = 0x3,
    SNPTOI   = 0x4,
    SNPMAKEI = 0x5,
};

struct SnoopReq {
    SnoopOp       op     = SnoopOp::SNPMAKEI;
    std::uint64_t line   = 0;
    std::uint16_t txnid  = 0;
    int           target = -1;
};

struct SnoopLine {
    std::uint32_t     version = 0;
    std::vector<int>  sharers;
    int               owner   = -1;
};

class SnoopDirectory {
public:

    void note_read(std::uint64_t line, int node);

    std::vector<int> note_write(std::uint64_t line, int node);


    std::vector<SnoopReq> snoops_for_exclusive(std::uint64_t line,
                                               std::uint16_t txnid,
                                               int requester) const;

    std::vector<SnoopReq> snoops_for_shared(std::uint64_t line,
                                            std::uint16_t txnid,
                                            int requester) const;


    void apply_snoop_resp(const SnoopReq& req);


    void invalidate_line(std::uint64_t line);


    bool is_stale(std::uint64_t line, int node) const;
    std::uint32_t version(std::uint64_t line) const;
    const std::vector<int>& sharers(std::uint64_t line) const;
    int owner(std::uint64_t line) const;

    std::uint64_t snoops_sent() const { return snoops_sent_; }
    std::uint64_t stale_checks() const { return stale_checks_; }
    std::uint64_t stale_hits() const { return stale_hits_; }

private:
    struct Cache { int node; std::uint32_t version; };
    std::unordered_map<std::uint64_t, SnoopLine>          lines_;
    std::unordered_map<std::uint64_t, std::vector<Cache>> cache_;
    mutable std::uint64_t snoops_sent_ = 0;
    mutable std::uint64_t stale_checks_ = 0;
    mutable std::uint64_t stale_hits_ = 0;
};

}
