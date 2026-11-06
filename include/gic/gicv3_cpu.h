// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include "gic/endpoint.h"

#include <cstdint>
#include <functional>
#include <utility>
#include <vector>

namespace numa::gic {

class Gicv3Cpu {
public:
    struct Config {
        unsigned spi_count = 32;          
        unsigned ppi_count = 16;          
        std::uint32_t lpi_base = 8192;
        unsigned lpi_count = 256;
        std::uint8_t default_priority = 0xA0;
        std::uint8_t pmr = 0xFF;          
        bool group0_enabled = true;
        bool group1_enabled = true;
        std::uint8_t pe_id = 0;           
    };

    Gicv3Cpu();
    explicit Gicv3Cpu(const Config& cfg);

    
    bool set_enable(std::uint32_t intid, bool en);
    bool set_priority(std::uint32_t intid, std::uint8_t prio);
    bool set_group(std::uint32_t intid, std::uint8_t group);
    
    bool set_pending(std::uint32_t intid, bool pending);
    bool set_active(std::uint32_t intid, bool active);
    bool set_trigger_level(std::uint32_t intid, bool level);
    void set_pmr(std::uint8_t pmr) { cfg_.pmr = pmr; }
    void set_group_enable(bool g0, bool g1);
    bool enabled(std::uint32_t intid) const;
    std::uint8_t priority(std::uint32_t intid) const;
    std::uint8_t group(std::uint32_t intid) const;
    bool pending(std::uint32_t intid) const;
    bool trigger_level(std::uint32_t intid) const;

    
    
    bool inject_spi(unsigned spi_index, std::uint8_t group,
                    std::uint8_t prio = 0, bool level = false);
    bool inject_sgi(unsigned sgi, std::uint8_t group,
                    std::uint8_t prio = 0);
    
    
    
    bool inject_sgi_cfg(unsigned sgi, std::uint8_t group);
    bool inject_ppi(unsigned ppi, std::uint8_t group,
                    std::uint8_t prio = 0);
    
    bool inject_lpi(std::uint32_t lpi, std::uint8_t group,
                    std::uint8_t prio = 0);
    
    bool deassert_spi(unsigned spi_index);

    
    bool pending() const;
    std::uint32_t ack();               
    bool eoi(std::uint32_t intid);
    bool active(std::uint32_t intid) const;
    
    void set_take_callback(std::function<void(std::uint32_t)> cb) {
        take_cb_ = std::move(cb);
    }
    
    void set_eoi_callback(std::function<void(std::uint32_t)> cb) {
        eoi_cb_ = std::move(cb);
    }
    
    std::uint8_t running_priority() const { return running_priority_; }
    std::size_t pending_count() const;

    
    unsigned drain(Endpoint& ep);

    
    
    
    
    bool deliver_sgi(const DeliveredSgi& s);
    bool deliver_spi(const DeliveredSpi& s);
    bool deliver_lpi(const DeliveredLpi& s);
    bool deliver_lpi_ack(const DeliveredLpiAck& a);
    std::uint8_t pe_id() const { return cfg_.pe_id; }

    std::uint64_t injected() const { return injected_; }
    std::uint64_t taken()    const { return taken_; }
    std::uint64_t spurious() const { return spurious_; }
    std::uint64_t rejected() const { return rejected_; }
    std::uint64_t drained()  const { return drained_; }
    std::uint64_t lpi_acks()        const { return lpi_acks_; }
    std::uint64_t lpi_ack_errors()  const { return lpi_ack_errors_; }
    
    std::uint64_t vlpi_dropped()    const { return vlpi_dropped_; }
    std::uint64_t vsgi_seen()       const { return vsgi_seen_; }
    std::uint64_t vsgi_ack_seen()   const { return vsgi_ack_seen_; }
    std::uint64_t vmovp_seen()      const { return vmovp_seen_; }
    std::uint64_t vmovp_ack_seen()  const { return vmovp_ack_seen_; }
    std::uint64_t cmd_seen()        const { return cmd_seen_; }
    std::uint64_t cmd_ack_seen()    const { return cmd_ack_seen_; }
    std::uint64_t rwp_seen()        const { return rwp_seen_; }
    std::uint64_t reg_seen()        const { return reg_seen_; }
    std::uint64_t reg_wack_seen()   const { return reg_wack_seen_; }
    std::uint64_t reg_data_seen()   const { return reg_data_seen_; }
    std::uint64_t ptb_seen()        const { return ptb_seen_; }

private:
    struct Irq {
        bool enabled = false;
        bool pending = false;
        bool active  = false;
        bool level   = false;   
        bool level_cfg = false; 
        std::uint8_t prio  = 0xA0;
        std::uint8_t group = 1;
    };

    Irq*       find(std::uint32_t intid);
    const Irq* find(std::uint32_t intid) const;
    bool       take(Irq& irq);
    void       recompute_running_priority();

    Config cfg_;
    std::vector<Irq> sgi_;   
    std::vector<Irq> ppi_;   
    std::vector<Irq> spi_;   
    std::vector<Irq> lpi_;   

    std::uint8_t running_priority_ = 0xFF;
    std::function<void(std::uint32_t)> take_cb_;
    std::function<void(std::uint32_t)> eoi_cb_;
    std::uint64_t injected_ = 0;
    std::uint64_t taken_ = 0;
    std::uint64_t spurious_ = 0;
    std::uint64_t rejected_ = 0;
    std::uint64_t drained_ = 0;
    std::uint64_t lpi_acks_ = 0;
    std::uint64_t lpi_ack_errors_ = 0;
    std::uint64_t vlpi_dropped_ = 0;
    std::uint64_t vsgi_seen_ = 0;
    std::uint64_t vsgi_ack_seen_ = 0;
    std::uint64_t vmovp_seen_ = 0;
    std::uint64_t vmovp_ack_seen_ = 0;
    std::uint64_t cmd_seen_ = 0;
    std::uint64_t cmd_ack_seen_ = 0;
    std::uint64_t rwp_seen_ = 0;
    std::uint64_t reg_seen_ = 0;
    std::uint64_t reg_wack_seen_ = 0;
    std::uint64_t reg_data_seen_ = 0;
    std::uint64_t ptb_seen_ = 0;
};

}
