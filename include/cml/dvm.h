// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace numa::cml {

enum class DvmType : std::uint8_t {
    TLBI  = 0,
    BPI   = 1,
    PICI  = 2,
    VICI  = 3,
    SYNC  = 4,
};

struct DvmAddr {
    bool     partnum   = false;   
    bool     vavalid   = false;   
    bool     vmidvalid = false;   
    bool     asidvalid = false;   
    std::uint8_t secure = 0;      
    std::uint8_t hyp    = 0;      
    std::uint8_t type   = 0;      
    std::uint8_t vmid   = 0;      
    std::uint16_t asid  = 0;      
    std::uint8_t s2s1   = 0;      
    bool     l          = false;  
    bool     range      = false;  
    bool     num4       = false;  
};

std::uint64_t encode_dvm_addr(const DvmAddr& a);   
DvmAddr       decode_dvm_addr(std::uint64_t v);

struct DvmData {
    std::uint8_t  num3_0 = 0;     
    std::uint8_t  scale  = 0;     
    std::uint8_t  ttl    = 0;     
    std::uint8_t  tg     = 0;     
    std::uint64_t addr   = 0;     
    std::uint8_t  vmidext = 0;    
};

std::uint64_t encode_dvm_data(const DvmData& d);
DvmData       decode_dvm_data(std::uint64_t v);

struct TlbOpDesc {
    const char* scope    = "Unknown";  
    const char* target   = "all";      
    bool        leaf_only = false;
    bool        is_tlbi   = false;
    bool        gpt       = false;     
};

TlbOpDesc   classify_tlbi(const DvmAddr& a);
std::string describe_tlbi(const DvmAddr& a);   

struct TlbEntry {
    std::uint64_t va   = 0;       
    std::uint16_t asid = 0;
    std::uint8_t  vmid = 0;
    std::uint32_t gen  = 0;       
};

class Tlb {
public:
    void insert(const TlbEntry& e);
    void insert(std::uint64_t va, std::uint16_t asid, std::uint8_t vmid);

    
    
    
    
    
    
    
    
    
    std::size_t invalidate(const DvmAddr& a, const DvmData& d);

    std::size_t size() const { return entries_.size(); }
    bool lookup(std::uint64_t va, std::uint16_t asid, std::uint8_t vmid) const;
    std::uint64_t invalidations() const { return invalidations_; }

private:
    struct Key {
        std::uint64_t va;
        std::uint16_t asid;
        std::uint8_t  vmid;
        bool operator==(const Key& o) const {
            return va == o.va && asid == o.asid && vmid == o.vmid;
        }
    };
    struct KeyHash {
        std::size_t operator()(const Key& k) const {
            return std::hash<std::uint64_t>()(k.va * 1000003u ^
                                              (std::uint64_t(k.asid) << 8) ^
                                              k.vmid);
        }
    };
    std::unordered_map<Key, TlbEntry, KeyHash> entries_;
    std::uint64_t invalidations_ = 0;
};

class DvmController {
public:
    void add_node(int id, Tlb* tlb);

    
    
    std::size_t broadcast(const DvmAddr& a, const DvmData& d);

    std::size_t nodes() const { return nodes_.size(); }
    std::uint64_t broadcasts() const { return broadcasts_; }
    std::uint64_t acks()       const { return acks_; }
    std::uint64_t invalidated() const { return invalidated_; }

private:
    std::vector<std::pair<int, Tlb*>> nodes_;
    std::uint64_t broadcasts_ = 0;
    std::uint64_t acks_ = 0;
    std::uint64_t invalidated_ = 0;
};

}
