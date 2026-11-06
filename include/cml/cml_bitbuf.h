// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>

namespace numa::cml {

class BitBuf {
public:
    static constexpr unsigned kBits = 1024;
    static constexpr unsigned kWords = kBits / 64;

    void clear() { w_.fill(0); }

    void set(unsigned lsb, unsigned width, std::uint64_t value) {
        if (width == 0 || width > 64 || lsb + width > kBits) {
            throw std::out_of_range("BitBuf::set range");
        }
        for (unsigned done = 0; done < width;) {
            const unsigned bit = lsb + done;
            const unsigned wi = bit / 64;
            const unsigned off = bit % 64;
            const unsigned n = std::min<unsigned>(64 - off, width - done);
            const std::uint64_t mask = (n == 64) ? ~0ull : ((1ull << n) - 1);
            w_[wi] = (w_[wi] & ~(mask << off)) |
                     (((value >> done) & mask) << off);
            done += n;
        }
    }

    std::uint64_t get(unsigned lsb, unsigned width) const {
        if (width == 0 || width > 64 || lsb + width > kBits) {
            throw std::out_of_range("BitBuf::get range");
        }
        std::uint64_t value = 0;
        for (unsigned done = 0; done < width;) {
            const unsigned bit = lsb + done;
            const unsigned wi = bit / 64;
            const unsigned off = bit % 64;
            const unsigned n = std::min<unsigned>(64 - off, width - done);
            const std::uint64_t mask = (n == 64) ? ~0ull : ((1ull << n) - 1);
            value |= ((w_[wi] >> off) & mask) << done;
            done += n;
        }
        return value;
    }

    std::uint64_t word(unsigned i) const { return w_.at(i); }
    void set_word(unsigned i, std::uint64_t v) { w_.at(i) = v; }

    bool operator==(const BitBuf& other) const { return w_ == other.w_; }
    bool operator!=(const BitBuf& other) const { return !(*this == other); }

private:
    std::array<std::uint64_t, kWords> w_{};
};

}
