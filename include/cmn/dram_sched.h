// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include <cstdint>
#include <vector>

namespace numa::cmn {

struct DramConfig {
    unsigned line_bytes     = 64;
    unsigned bytes_per_tick = 32;   
    unsigned service_ticks  = 2;    
    unsigned banks          = 1;
    unsigned aging_ticks    = 16;   
    unsigned qos_levels     = 4;
};

struct DramDone {
    std::uint64_t id      = 0;
    std::uint64_t arrive  = 0;
    std::uint64_t done    = 0;      
    std::uint64_t latency = 0;      
    unsigned      qos     = 0;      
    unsigned      bytes   = 0;
};

class DramScheduler {
public:
    explicit DramScheduler(const DramConfig& cfg = DramConfig());

    
    std::uint64_t submit(unsigned bytes, unsigned qos);

    
    void tick(std::vector<DramDone>& done);

    std::uint64_t now() const { return now_; }
    std::uint64_t waiting() const;
    const DramConfig& config() const { return cfg_; }

    std::uint64_t submitted()   const { return submitted_; }
    std::uint64_t completed()   const { return completed_; }
    std::uint64_t bytes()       const { return bytes_; }
    std::uint64_t busy_ticks()  const { return busy_ticks_; }
    std::uint64_t total_latency() const { return total_latency_; }
    std::uint64_t max_latency()   const { return max_latency_; }
    std::uint64_t qos_submitted(unsigned q) const;
    std::uint64_t qos_completed(unsigned q) const;

private:
    struct Req {
        std::uint64_t id = 0;
        std::uint64_t arrive = 0;
        unsigned bytes = 0;
        unsigned qos = 0;
        unsigned service = 1;   
        unsigned remaining = 1; 
        bool     started = false;
    };

    unsigned effective_qos(const Req& r) const;   
    Req* pick(unsigned bank);

    DramConfig cfg_;
    std::vector<std::vector<Req>> banks_;
    std::uint64_t now_ = 0;
    std::uint64_t next_id_ = 0;
    std::uint64_t submitted_ = 0;
    std::uint64_t completed_ = 0;
    std::uint64_t bytes_ = 0;
    std::uint64_t busy_ticks_ = 0;
    std::uint64_t total_latency_ = 0;
    std::uint64_t max_latency_ = 0;
    std::vector<std::uint64_t> qos_sub_;
    std::vector<std::uint64_t> qos_done_;
};

}
