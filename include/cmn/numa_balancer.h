#pragma once

#include "cmn/migration.h"
#include "cmn/numa_system.h"

#include <cstdint>
#include <unordered_map>

namespace numa::cmn {

struct NumaBalancerConfig {
    unsigned lines_per_page        = 64;
    unsigned min_samples           = 8;
    unsigned remote_ratio_percent  = 60;
    unsigned cooldown_ticks        = 4;
    unsigned max_migrations_per_tick = 2;
};

class NumaBalancer {
public:
    struct Stats {
        std::uint64_t samples      = 0;
        std::uint64_t candidates   = 0;
        std::uint64_t migrated     = 0;
        std::uint64_t rejected     = 0;
        std::uint64_t cooldown     = 0;
        std::uint64_t budget       = 0;
        std::uint64_t no_owner     = 0;
    };

    NumaBalancer(NumaSystem& sys, MigrationManager& mm,
                 NumaBalancerConfig cfg = {});


    void record_access(std::uint64_t line, int accessor_node);


    unsigned tick();

    const Stats& stats() const { return stats_; }
    unsigned     epoch() const { return epoch_; }
    const NumaBalancerConfig& config() const { return cfg_; }

private:
    struct PageStat {
        std::unordered_map<int, unsigned> count;
        std::uint64_t last_migrate_epoch = 0;
        bool          migrated_once = false;
    };

    std::uint64_t page_base(std::uint64_t line) const;

    NumaSystem&        sys_;
    MigrationManager&  mm_;
    NumaBalancerConfig cfg_;
    std::unordered_map<std::uint64_t, PageStat> pages_;
    Stats      stats_;
    unsigned   epoch_ = 0;
};

}
