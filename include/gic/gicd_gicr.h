#pragma once

#include "gic/gicv3_cpu.h"

#include <cstdint>
#include <vector>

namespace numa::gic {

class GicdGicr {
public:
    struct Config {
        std::uint64_t gicd_base = 0x08000000;
        std::uint64_t gicr_base = 0x080A0000;
        std::uint32_t gicd_length = 0x10000;
        std::uint32_t gicr_stride = 0x20000;
        unsigned cpu_count = 1;
        unsigned spi_count = 32;
        unsigned ppi_count = 16;
        std::uint32_t lpi_base = 8192;
        unsigned lpi_count = 256;
        bool security_enabled = false;
        bool secure_access = false;

        bool affinity_routing = true;


        std::vector<std::uint32_t> affinities;
    };

    GicdGicr();
    explicit GicdGicr(const Config& cfg);

    unsigned cpu_count() const { return static_cast<unsigned>(cpus_.size()); }
    Gicv3Cpu& cpu(unsigned index) { return cpus_.at(index); }
    const Gicv3Cpu& cpu(unsigned index) const { return cpus_.at(index); }

    int spi_target(std::uint32_t intid) const;

    void set_secure_access(bool secure) { secure_ = secure; }
    bool secure_access() const { return secure_; }
    std::uint64_t security_rejects() const { return security_rejects_; }


    bool set_lpi_property(unsigned cpu, std::uint32_t intid,
                          std::uint8_t prio, std::uint8_t group,
                          bool enabled = true);
    bool set_lpi_pending(unsigned cpu, std::uint32_t intid, bool pending);
    bool lpi_pending(unsigned cpu, std::uint32_t intid) const;


    bool read(std::uint64_t address, unsigned size, std::uint64_t& value) const;
    bool write(std::uint64_t address, unsigned size, std::uint64_t value);

    std::uint64_t mmio_reads() const { return reads_; }
    std::uint64_t mmio_writes() const { return writes_; }
    std::uint64_t rejects() const { return rejects_; }

    const Config& config() const { return cfg_; }

private:
    struct LpiProp {
        std::uint8_t prio = 0xA0;
        std::uint8_t group = 1;
        bool enabled = true;
    };

    struct Frame {
        std::uint32_t ctlr = 0;
        bool processor_sleep = false;
        bool lpi_enabled = false;
        std::uint64_t propbaser = 0;
        std::uint64_t pendbaser = 0;
        std::uint32_t grpmodr = 0;
        std::uint32_t nsacr = 0;
        std::vector<LpiProp> lpi_prop;
        std::vector<bool> lpi_pending;
    };

    struct Decode {
        bool valid = false;
        bool distributor = false;
        unsigned cpu = 0;
        bool sgi_base = false;
        std::uint32_t off = 0;
    };

    bool decode(std::uint64_t address, unsigned size, Decode& d) const;
    bool gicd_read(std::uint32_t off, unsigned size, std::uint64_t& v) const;
    bool gicd_write(std::uint32_t off, unsigned size, std::uint64_t v);
    bool gicr_read(unsigned cpu, bool sgi_base, std::uint32_t off,
                   unsigned size, std::uint64_t& v) const;
    bool gicr_write(unsigned cpu, bool sgi_base, std::uint32_t off,
                    unsigned size, std::uint64_t v);

    bool spi_valid(std::uint32_t intid) const;
    bool local_valid(unsigned cpu, std::uint32_t intid) const;
    bool lpi_valid(std::uint32_t intid) const;
    bool secure_regs_enabled() const;
    bool spi_pending_bank(std::uint32_t intid, bool secure) const;
    void set_spi_pending_bank(std::uint32_t intid, bool secure, bool value);
    bool spi_active_bank(std::uint32_t intid, bool secure) const;
    void set_spi_active_bank(std::uint32_t intid, bool secure, bool value);
    std::uint8_t spi_priority_bank(std::uint32_t intid, bool secure) const;
    void set_spi_priority_bank(std::uint32_t intid, bool secure,
                               std::uint8_t prio);
    unsigned lpi_index(std::uint32_t intid) const;
    void sync_lpi(unsigned cpu, unsigned index);
    void sync_all_lpi(unsigned cpu);
    unsigned spi_words() const;
    unsigned local_ppi_count() const;
    std::uint64_t gicr_typer(unsigned cpu) const;
    void apply_ctlr();

    Config cfg_;
    std::uint32_t ctlr_ = 0;
    bool secure_ = false;
    std::uint32_t gicd_nsacr_ = 0;
    std::vector<std::uint32_t> spi_pending_s_;
    std::vector<std::uint32_t> spi_pending_ns_;
    std::vector<std::uint32_t> spi_active_s_;
    std::vector<std::uint32_t> spi_active_ns_;
    std::vector<std::uint8_t> spi_prio_s_;
    std::vector<std::uint8_t> spi_prio_ns_;
    std::vector<std::uint32_t> spi_grpmodr_;
    std::vector<std::uint64_t> route_;
    std::vector<Frame> frames_;
    std::vector<Gicv3Cpu> cpus_;

    mutable std::uint64_t reads_ = 0;
    mutable std::uint64_t writes_ = 0;
    mutable std::uint64_t rejects_ = 0;
    mutable std::uint64_t security_rejects_ = 0;
};

}
