// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include <cstdint>
#include <unordered_map>

namespace numa::cmn {

class ExclusiveMonitor {
public:
    
    void grant(int node, std::uint64_t line, std::uint16_t txnid);

    
    std::size_t break_line(std::uint64_t line, int writer);

    
    bool commit(int node, std::uint64_t line);

    
    void clear(int node);

    bool held(int node, std::uint64_t line) const;
    bool any(int node) const;
    std::uint64_t line(int node) const;      
    std::size_t holders() const { return res_.size(); }

    std::uint64_t granted()     const { return granted_; }
    std::uint64_t replaced()    const { return replaced_; }
    std::uint64_t broken()      const { return broken_; }
    std::uint64_t commits_ok()  const { return commits_ok_; }
    std::uint64_t commits_fail() const { return commits_fail_; }
    std::uint64_t cleared()     const { return cleared_; }

private:
    struct Res {
        std::uint64_t line  = 0;
        std::uint16_t txnid = 0;
    };
    std::unordered_map<int, Res> res_;
    std::uint64_t granted_      = 0;
    std::uint64_t replaced_     = 0;
    std::uint64_t broken_       = 0;
    std::uint64_t commits_ok_   = 0;
    std::uint64_t commits_fail_ = 0;
    std::uint64_t cleared_      = 0;
};

}
