#pragma once

#include "cmn/wire_node.h"
#include "cml/cml_tx_scheduler.h"
#include "gic/endpoint.h"
#include "gic/gicd.h"
#include "gic/gicv3_cpu.h"

#include <array>
#include <cstdint>
#include <deque>
#include <memory>
#include <unordered_map>
#include <vector>

namespace numa::gic {
class GicdGicr;
}

namespace numa::cmn {

class NumaSystem {
public:

    int add_node(std::uint64_t base_line, std::uint64_t lines);

    std::size_t node_count() const { return nodes_.size(); }
    WireNode&       node(std::size_t i)       { return *nodes_.at(i).node; }
    const WireNode& node(std::size_t i) const { return *nodes_.at(i).node; }


    int route(std::uint64_t line) const;

    int static_route(std::uint64_t line) const;


    bool set_line_owner(std::uint64_t line, int node);
    std::size_t line_overrides() const { return owners_.size(); }


    bool send(const WireNode::Flit& in, std::vector<WireNode::Flit>& out);


    bool send_to(int node, const WireNode::Flit& in,
                 std::vector<WireNode::Flit>& out);


    void set_tx_queued(bool v) { tx_queued_ = v; }
    bool tx_queued_enabled() const { return tx_queued_; }
    bool enqueue_tx(int node, const WireNode::Flit& f);
    unsigned flush_tx(std::vector<WireNode::Flit>& out,
                      std::size_t max = static_cast<std::size_t>(-1));
    std::size_t tx_queued() const;
    const std::vector<int>& tx_order() const { return tx_order_; }
    std::uint64_t tx_overflow() const { return tx_overflow_; }
    std::uint64_t tx_flushed() const { return tx_flushed_; }


    bool send_gicd(int src, int tgt, const gic::Message& m);


    bool send_gicd2(int src, int tgt, const gic::Message& a,
                    const gic::Message& b);
    std::uint64_t gicd_multi_sent() const { return gicd_multi_sent_; }

    bool enqueue_gicd(int src, int tgt, const gic::Message& m);
    unsigned flush_gicd(std::size_t max = static_cast<std::size_t>(-1));
    std::size_t gicd_queued() const { return gicd_sched_.occupancy(); }
    const std::vector<int>& gicd_flush_order() const { return gicd_flush_order_; }
    gic::Proxy&       gicd(int node)       { return gicd_.at(static_cast<std::size_t>(node)); }
    const gic::Proxy& gicd(int node) const { return gicd_.at(static_cast<std::size_t>(node)); }


    bool send_gicd_raw(int tgt, std::uint64_t payload33);


    void attach_gicv3(int node, gic::Gicv3Cpu* cpu);


    bool attach_gicv3_pe(int node, gic::Gicv3Cpu* cpu);
    std::size_t gicv3_count(int node) const;
    gic::Gicv3Cpu* gicv3(int node);
    gic::Endpoint&       gicd_endpoint(int node) {
        return endpoints_.at(static_cast<std::size_t>(node));
    }
    const gic::Endpoint& gicd_endpoint(int node) const {
        return endpoints_.at(static_cast<std::size_t>(node));
    }
    std::uint64_t gicd_beats() const { return gicd_beats_; }


    void attach_gic_mmio(gic::GicdGicr* gic) { gic_mmio_ = gic; }
    gic::GicdGicr* gic_mmio() { return gic_mmio_; }
    const gic::GicdGicr* gic_mmio() const { return gic_mmio_; }
    bool mmio_read(std::uint64_t address, unsigned size,
                   std::uint64_t& value) const;
    bool mmio_write(std::uint64_t address, unsigned size,
                    std::uint64_t value);
    std::uint64_t mmio_reads() const { return mmio_reads_; }
    std::uint64_t mmio_writes() const { return mmio_writes_; }
    std::uint64_t mmio_rejects() const { return mmio_rejects_; }

    std::uint64_t delivered() const { return delivered_; }
    std::uint64_t unroutable() const { return unroutable_; }
    std::uint64_t responses() const { return responses_; }
    std::uint64_t dvm_broadcasts() const { return dvm_broadcasts_; }
    std::uint64_t gicd_sent() const { return gicd_sent_; }
    std::uint64_t gicd_rejected() const { return gicd_rejected_; }

private:
    struct Entry {
        std::uint64_t base_line = 0;
        std::uint64_t lines     = 0;
        std::unique_ptr<WireNode> node;
    };

    std::vector<Entry> nodes_;
    std::vector<gic::Proxy> gicd_;
    std::vector<gic::Endpoint> endpoints_;
    std::vector<gic::Gicv3Cpu*> gicv3_;
    std::vector<std::vector<gic::Gicv3Cpu*>> gicv3_pe_;
    gic::GicdGicr* gic_mmio_ = nullptr;
    mutable std::uint64_t mmio_reads_ = 0;
    mutable std::uint64_t mmio_rejects_ = 0;
    std::uint64_t mmio_writes_ = 0;
    std::unordered_map<std::uint64_t, int> owners_;
    unsigned  adf_left_ = 0;
    int       adf_node_ = -1;
    bool      adf_broadcast_ = false;
    std::uint64_t delivered_ = 0;
    std::uint64_t unroutable_ = 0;
    std::uint64_t responses_ = 0;
    std::uint64_t dvm_broadcasts_ = 0;
    std::uint64_t gicd_sent_ = 0;
    std::uint64_t gicd_multi_sent_ = 0;
    std::uint64_t gicd_rejected_ = 0;
    bool tx_queued_ = false;
    std::array<std::deque<WireNode::Flit>, 4> tx_q_;
    std::vector<int> tx_order_;
    std::uint64_t tx_overflow_ = 0;
    std::uint64_t tx_flushed_ = 0;
    int tx_last_q_ = -1;
    cml::TxScheduler gicd_sched_;
    std::vector<int> gicd_flush_order_;
    std::uint64_t gicd_beats_ = 0;
};

}
