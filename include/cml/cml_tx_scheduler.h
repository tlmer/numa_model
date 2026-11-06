#pragma once

#include "cml/cml_codec.h"

#include <array>
#include <cstdint>
#include <vector>

namespace numa::cml {

class TxScheduler {
public:
    static constexpr unsigned kQueues = 4;
    static constexpr unsigned kDepth  = 4;

    TxScheduler() = default;


    bool push(unsigned queue, const MsgDef& def, const Values& values,
              std::uint8_t srcid = 0, std::uint8_t tgtid = 0);


    bool pop_round_robin(MsgDef& def, Values& values,
                         std::uint8_t* srcid = nullptr,
                         std::uint8_t* tgtid = nullptr);


    bool pack_next(BitBuf& out, unsigned base_dw = 2);


    unsigned size(unsigned queue) const;
    bool empty(unsigned queue) const;
    bool all_empty() const;
    unsigned occupancy() const;


    unsigned high_water(unsigned queue) const;

    std::uint64_t queued()   const { return queued_; }
    std::uint64_t dequeued() const { return dequeued_; }
    std::uint64_t rejected() const { return rejected_; }


    int last_queue() const { return last_queue_; }


    const std::vector<unsigned>& pop_order() const { return pop_order_; }

private:
    struct Entry {
        MsgDef def{};
        Values values;
        std::uint8_t srcid = 0;
        std::uint8_t tgtid = 0;
    };

    struct Queue {
        std::array<Entry, kDepth> items{};
        unsigned head  = 0;
        unsigned count = 0;
    };


    int next_queue() const;


    void consume(unsigned queue);

    void check_queue(unsigned queue) const;

    std::array<Queue, kQueues> queues_{};
    int last_queue_ = -1;

    std::uint64_t queued_   = 0;
    std::uint64_t dequeued_ = 0;
    std::uint64_t rejected_ = 0;

    std::array<unsigned, kQueues> high_water_{};
    std::vector<unsigned> pop_order_;
};

}
