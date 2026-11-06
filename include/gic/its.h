// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include "gic/gicd_gicr.h"

#include <cstdint>
#include <deque>
#include <unordered_map>
#include <vector>

namespace numa::gic {

class Its {
public:
    enum class Op {
        MapCollection,   
        MapDevice,       
        MapEvent,        
        MoveEvent,       
        Sync,            
        Invalidate,      
        Discard          
    };

    struct Command {
        Op op = Op::Sync;
        std::uint32_t device = 0;
        std::uint32_t event = 0;
        std::uint32_t lpi = 0;
        std::uint32_t collection = 0;
        std::uint32_t cpu = 0;
        std::uint8_t priority = 0xA0;
        std::uint8_t group = 1;
        bool all = false;
        std::uint64_t itt_addr = 0;
        std::uint64_t target_addr = 0;
        std::uint32_t size = 0;
    };

    struct Config {
        GicdGicr* gic = nullptr;
        unsigned cpu_count = 1;
        std::size_t max_commands = 256;
        std::uint64_t gits_base = 0x08080000;
        std::uint32_t gits_length = 0x20000;   
        std::uint32_t queue_bytes = 4096;
        std::uint64_t gicr_base = 0x080A0000;
        std::uint32_t gicr_stride = 0x20000;
    };

    Its();
    explicit Its(const Config& cfg);

    void attach(GicdGicr* gic) { cfg_.gic = gic; }
    bool enqueue(const Command& c);
    unsigned run();                 
    std::size_t pending_commands() const { return q_.size(); }

    
    bool translate(std::uint32_t device, std::uint32_t event,
                   std::uint32_t& lpi, unsigned& cpu) const;
    
    bool deliver(std::uint32_t device, std::uint32_t event);

    
    static void encode(const Command& c, std::uint64_t w[4]);
    static bool decode(const std::uint64_t w[4], Command& c,
                       std::uint64_t gicr_base = 0x080A0000,
                       std::uint32_t gicr_stride = 0x20000);

    
    bool read_gits(std::uint64_t address, unsigned size, std::uint64_t& v) const;
    bool write_gits(std::uint64_t address, unsigned size, std::uint64_t v);
    void set_translater_device(std::uint32_t device) {
        translater_device_ = device;
    }
    void write_command_memory(std::uint32_t offset, const std::uint64_t w[4]);
    unsigned process_commands();
    std::uint64_t gits_commands() const { return gits_commands_; }
    std::uint64_t gits_errors() const { return gits_errors_; }

    std::uint64_t enqueued() const { return enqueued_; }
    std::uint64_t processed() const { return processed_; }
    std::uint64_t rejected() const { return rejected_; }
    std::uint64_t errors() const { return errors_; }
    std::uint64_t translations() const { return translations_; }
    std::uint64_t unmapped() const { return unmapped_; }
    std::uint64_t syncs() const { return syncs_; }

private:
    struct Event {
        std::uint32_t lpi = 0;
        std::uint8_t priority = 0xA0;
        std::uint8_t group = 1;
    };
    struct Device {
        std::uint32_t collection = 0;
        std::uint64_t itt_addr = 0;
        std::uint32_t itt_size = 0;
        std::unordered_map<std::uint32_t, Event> events;
    };

    void execute(const Command& c);
    bool collection_cpu(std::uint32_t collection, unsigned& cpu) const;

    Config cfg_;
    std::unordered_map<std::uint32_t, unsigned> collections_; 
    std::unordered_map<std::uint32_t, Device> devices_;
    std::deque<Command> q_;
    std::vector<std::uint8_t> gits_queue_;
    std::uint32_t gits_ctlr_ = 0;
    std::uint64_t gits_cbaser_ = 0;
    std::uint64_t gits_creadr_ = 0;
    std::uint64_t gits_cwriter_ = 0;
    std::uint32_t translater_device_ = 0;
    std::uint64_t gits_commands_ = 0;
    std::uint64_t gits_errors_ = 0;
    std::uint64_t enqueued_ = 0;
    std::uint64_t processed_ = 0;
    std::uint64_t rejected_ = 0;
    std::uint64_t errors_ = 0;
    mutable std::uint64_t translations_ = 0;
    mutable std::uint64_t unmapped_ = 0;
    std::uint64_t syncs_ = 0;
};

}
