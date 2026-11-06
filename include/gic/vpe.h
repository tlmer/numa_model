#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace numa::gic {

class VpeTable {
public:
    struct Config {
        unsigned max_vpes = 16;
        unsigned max_vlpis = 256;
        std::uint32_t lpi_base = 8192;
        unsigned lpi_count = 256;
        bool require_vpt = false;
    };

    struct Prop {
        std::uint8_t priority = 0xA0;
        std::uint8_t group = 1;
        bool pending = false;
    };

    VpeTable();
    explicit VpeTable(const Config& cfg);

    bool enable_vpe(std::uint32_t vpe);
    bool disable_vpe(std::uint32_t vpe);

    bool set_vpt_bases(std::uint32_t vpe, std::uint64_t propbaser,
                       std::uint64_t pendbaser);
    std::uint64_t vpropbaser(std::uint32_t vpe) const;
    std::uint64_t vpendbaser(std::uint32_t vpe) const;
    bool vpt_configured(std::uint32_t vpe) const;
    unsigned vpt_entries() const { return cfg_.lpi_count; }
    std::uint8_t read_vpt_property(std::uint32_t vpe, unsigned index) const;
    bool read_vpt_pending(std::uint32_t vpe, unsigned index) const;
    bool map_vlpi(std::uint32_t vpe, std::uint32_t vlpi,
                  std::uint8_t priority, std::uint8_t group);
    bool set_vlpi_pending(std::uint32_t vpe, std::uint32_t vlpi, bool pending);
    bool vlpi_pending(std::uint32_t vpe, std::uint32_t vlpi) const;
    bool take_vlpi(std::uint32_t vpe, std::uint32_t vlpi);
    std::size_t pending_count(std::uint32_t vpe) const;
    std::size_t vpe_count() const { return vpes_.size(); }

    std::uint64_t rejected() const { return rejected_; }
    std::uint64_t taken() const { return taken_; }

private:
    struct Vpe {
        std::unordered_map<std::uint32_t, Prop> vlpis;
        std::uint64_t vpropbaser = 0;
        std::uint64_t vpendbaser = 0;
        std::vector<std::uint8_t> vprop;
        std::vector<std::uint8_t> vpend;
    };

    Config cfg_;
    std::unordered_map<std::uint32_t, Vpe> vpes_;
    std::uint64_t rejected_ = 0;
    std::uint64_t taken_ = 0;
};

}
