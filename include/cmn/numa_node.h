// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include "cml/cml_txn.h"
#include "cml/cml_wire.h"
#include "cml/dvm.h"
#include "cmn/excl.h"
#include "cmn/snoop.h"

#include <array>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace numa::cmn {

constexpr unsigned kLineBytes = 64;

class Dram {
public:
    
    
    explicit Dram(std::uint64_t base_line = 0, std::uint64_t lines = 4096,
                  std::uint8_t* external = nullptr);

    void write_line(std::uint64_t line, const std::uint8_t data[kLineBytes]);
    void read_line(std::uint64_t line, std::uint8_t data[kLineBytes]) const;

    std::uint64_t base()  const { return base_; }
    std::uint64_t lines() const { return lines_; }
    std::uint64_t reads() const { return reads_; }
    std::uint64_t writes() const { return writes_; }
    
    std::size_t   foreign_lines() const { return foreign_.size(); }

private:
    std::uint64_t             base_;
    std::uint64_t             lines_;
    std::vector<std::uint8_t> mem_;      
    std::uint8_t*             external_ = nullptr;  
    
    mutable std::unordered_map<std::uint64_t, std::array<std::uint8_t, kLineBytes>>
        foreign_;
    mutable std::uint64_t     reads_  = 0;
    std::uint64_t             writes_ = 0;
};

class Sam {
public:
    struct Region {
        std::uint64_t lo_line;   
        std::uint64_t hi_line;   
        int           node;
    };

    void add_region(std::uint64_t lo_line, std::uint64_t hi_line, int node);
    
    int  route(std::uint64_t line_addr) const;
    std::size_t region_count() const { return regions_.size(); }

private:
    std::vector<Region> regions_;
};

enum class LineState : std::uint8_t { I = 0, S = 1 };

class HnF {
public:
    explicit HnF(Dram& dram) : dram_(dram) {}

    
    cml::wire::Rsp handle_read(const cml::wire::Req& req, std::uint8_t data[kLineBytes]);
    
    cml::wire::Rsp handle_write(const cml::wire::Req& req, const std::uint8_t data[kLineBytes]);

    
    
    
    
    
    bool handle_atomic(const cml::wire::Req& req, std::uint8_t data[kLineBytes],
                       cml::wire::Rsp& rsp, bool& has_data);

    LineState state(std::uint64_t line) const;
    
    
    void cmo_clean(std::uint64_t line);
    void cmo_invalidate(std::uint64_t line);
    std::uint64_t reads()  const { return reads_; }
    std::uint64_t writes() const { return writes_; }
    std::uint64_t atomics() const { return atomics_; }
    std::uint64_t atomic_rejects() const { return atomic_rejects_; }

private:
    Dram&                                    dram_;
    std::unordered_map<std::uint64_t, LineState> dir_;
    std::uint64_t                            reads_  = 0;
    std::uint64_t                            writes_ = 0;
    std::uint64_t                            atomics_ = 0;
    std::uint64_t                            atomic_rejects_ = 0;
};

class NumaNode {
public:
    explicit NumaNode(int node_id = 1,
                      std::uint64_t base_line = 0,
                      std::uint64_t lines = 4096);

    
    
    
    
    bool on_req(const std::uint32_t d[3], std::uint32_t& rsp_dw,
                bool& has_data, std::uint8_t data64[kLineBytes],
                int requester = -1, std::uint8_t lo_addr = 0);

    
    
    bool store_exclusive(const std::uint32_t d[3],
                         const std::uint8_t data[kLineBytes],
                         std::uint32_t& rsp_dw, bool& ok,
                         int requester = -1);

    
    
    bool on_cmo(std::uint64_t line, std::uint8_t chi_opcode,
                int requester = -1);

    
    
    void abort_line(std::uint64_t line);

    int   id()   const { return node_id_; }
    Sam&  sam()        { return sam_; }
    HnF&  hnf()        { return hnf_; }
    Dram& dram()       { return dram_; }
    cml::TxnTable& txn() { return txn_; }
    SnoopDirectory& snoop() { return snoop_; }
    ExclusiveMonitor& excl() { return excl_; }

    
    void attach_tlb(cml::Tlb* tlb) { tlb_ = tlb; }
    cml::Tlb* tlb() const { return tlb_; }
    std::uint64_t dvm_handled() const { return dvm_handled_; }

    
    void adopt_line(std::uint64_t line) { adopted_.insert(line); }
    bool hosts(std::uint64_t line) const { return accepts(line); }
    std::size_t adopted_lines() const { return adopted_.size(); }

    std::uint64_t  dup_dropped() const { return dup_dropped_; }
    std::uint64_t  snoops_issued() const { return snoops_issued_; }

private:
    
    bool accepts(std::uint64_t line) const {
        return sam_.route(line) == node_id_ || adopted_.count(line) != 0;
    }

    
    static constexpr unsigned kRecent = 8;
    struct Recent { std::uint16_t txnid = 0xFFFF; std::uint64_t addr = ~0ull; };

    int   node_id_;
    Dram  dram_;
    Sam   sam_;
    HnF   hnf_;
    cml::TxnTable txn_;     
    Recent        recent_[kRecent];
    unsigned      recent_pos_ = 0;
    std::uint64_t dup_dropped_ = 0;
    SnoopDirectory snoop_;  
    std::unordered_set<std::uint64_t> adopted_;   
    std::uint64_t  snoops_issued_ = 0;
    ExclusiveMonitor excl_; 
    cml::Tlb*      tlb_ = nullptr;   
    std::uint64_t  dvm_handled_ = 0;
};

bool is_exclusive_op(std::uint8_t reqop);

bool is_read_op(std::uint8_t reqop);

bool is_atomic_op(std::uint8_t reqop);
bool is_atomic_load(std::uint8_t reqop);   

bool is_write_op(std::uint8_t reqop);

}
