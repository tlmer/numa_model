// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include "cml/cml_credit.h"
#include "cxs/cxs_link_model.h"

#include <array>
#include <cstdint>
#include <deque>
#include <unordered_map>
#include <vector>

namespace numa::cml {

class CmlCreditLink {
public:
    CmlCreditLink(SharedCreditBank& bank, cxs::CxsLinkModel& link)
        : bank_(bank), link_(link) {}

    
    
    bool submit(CrdClass c, const cxs::CxsFlit& flit, std::uint32_t packet_id);

    
    
    bool submit_packet(CrdClass c, const std::vector<cxs::CxsFlit>& flits,
                       std::uint32_t packet_id);

    
    unsigned pump();

    
    bool complete(std::uint32_t packet_id);

    
    
    bool cancel(std::uint32_t packet_id);

    std::size_t queued() const;
    std::size_t inflight() const { return inflight_.size(); }
    bool is_inflight(std::uint32_t packet_id) const {
        return inflight_.find(packet_id) != inflight_.end();
    }

    std::uint64_t submitted()        const { return submitted_; }
    std::uint64_t sent()             const { return sent_; }
    std::uint64_t completed()        const { return completed_; }
    std::uint64_t rejected_submit()  const { return rejected_submit_; }
    std::uint64_t rejected_complete() const { return rejected_complete_; }
    std::uint64_t starved_credit()   const { return starved_credit_; }
    std::uint64_t starved_link()     const { return starved_link_; }
    std::uint64_t cancelled()        const { return cancelled_; }

private:
    struct Item {
        std::vector<cxs::CxsFlit> flits;
        std::size_t next = 0;         
        std::uint32_t packet_id = 0;
        bool credit_held = false;
    };

    bool packet_known(std::uint32_t packet_id) const;

    SharedCreditBank& bank_;
    cxs::CxsLinkModel& link_;
    std::array<std::deque<Item>, SharedCreditBank::kClasses> queues_;
    std::unordered_map<std::uint32_t, CrdClass> inflight_;
    unsigned rr_ = 0;

    std::uint64_t submitted_ = 0;
    std::uint64_t sent_ = 0;
    std::uint64_t completed_ = 0;
    std::uint64_t rejected_submit_ = 0;
    std::uint64_t rejected_complete_ = 0;
    std::uint64_t starved_credit_ = 0;
    std::uint64_t starved_link_ = 0;
    std::uint64_t cancelled_ = 0;
};

}
