// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include "cmn/numa_system.h"
#include "cml/dvm.h"

#include <cstdint>

namespace numa::cmn {

class MigrationManager {
public:
    struct Stats {
        std::uint64_t pages    = 0;
        std::uint64_t lines    = 0;
        std::uint64_t bytes    = 0;
        std::uint64_t rejected = 0;
        std::uint64_t dvm      = 0;
    };

    
    explicit MigrationManager(NumaSystem& sys,
                              cml::DvmController* dvm = nullptr,
                              unsigned lines_per_page = 64);

    
    
    bool migrate_page(std::uint64_t page_first_line, int to_node,
                      std::uint64_t va = 0);

    
    bool migrate_line(std::uint64_t line, int to_node);

    const Stats& stats() const { return stats_; }
    unsigned lines_per_page() const { return lpp_; }

private:
    NumaSystem&         sys_;
    cml::DvmController* dvm_;
    unsigned            lpp_;
    Stats               stats_;
};

}
