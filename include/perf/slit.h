// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace numa::perf {

struct LatencyMatrix {
    unsigned            nodes = 0;
    std::vector<double> ns;          

    void resize(unsigned n) {
        nodes = n;
        ns.assign(static_cast<std::size_t>(n) * n, 0.0);
    }
    double&       at(unsigned s, unsigned d) {
        return ns[static_cast<std::size_t>(s) * nodes + d];
    }
    double        at(unsigned s, unsigned d) const {
        return ns[static_cast<std::size_t>(s) * nodes + d];
    }
    bool complete() const;
};

struct SlitResult {
    unsigned                 nodes = 0;
    std::vector<std::uint8_t> dist;    
    std::vector<double>      predicted_ns;
    bool                     valid = false;
    std::string              reason;   

    std::uint8_t at(unsigned s, unsigned d) const {
        return dist[static_cast<std::size_t>(s) * nodes + d];
    }

    
    
    double max_rel_error_vs(const LatencyMatrix& truth) const;

    
    std::string to_yaml() const;
};

class SlitCalibrator {
public:
    
    
    explicit SlitCalibrator(double asymmetry_tol = 0.05);

    SlitResult build(const LatencyMatrix& measured) const;

private:
    double tol_;
};

}
