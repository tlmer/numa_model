// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include "cxs/cxs_link.h"

#include <array>
#include <cstdint>
#include <deque>
#include <vector>

namespace numa::cxs {

struct CxsFlit {
    std::array<std::uint64_t, 8> data{};  
    std::uint32_t cntl = 0;
    std::uint32_t prcltype = 0;
    bool last = false;
};

struct LinkStats {
    
    std::uint64_t activations = 0;
    std::uint64_t deactivations = 0;
    std::uint64_t resets = 0;
    std::uint64_t activation_timeouts = 0;
    std::uint64_t bad_acks = 0;
    std::uint64_t deactivation_lost = 0;

    
    std::uint64_t from_dut_flits = 0;
    std::uint64_t from_dut_packets_started = 0;
    std::uint64_t from_dut_packets_completed = 0;
    std::uint64_t rx_delivered = 0;
    std::uint64_t rx_blocked_no_credit = 0;
    std::uint64_t rx_blocked_full = 0;
    std::uint64_t rx_blocked_link = 0;

    
    std::uint64_t to_dut_flits = 0;
    std::uint64_t to_dut_packets_started = 0;
    std::uint64_t to_dut_packets_completed = 0;
    std::uint64_t tx_delivered = 0;
    std::uint64_t tx_blocked_no_credit = 0;
    std::uint64_t tx_blocked_full = 0;
    std::uint64_t tx_blocked_link = 0;

    
    std::uint64_t credits_granted_to_dut = 0;
    std::uint64_t credits_granted_to_model = 0;
    std::uint64_t credits_returned_to_dut = 0;
    std::uint64_t credit_returns_withheld = 0;
    std::uint64_t credits_withheld_capacity = 0;
    std::uint64_t credit_overflow = 0;
    std::uint64_t credit_underflow = 0;

    
    std::uint64_t last_errors = 0;
    std::uint64_t interleave_errors = 0;

    
    std::uint64_t total_latency_ticks = 0;
    std::uint64_t max_latency_ticks = 0;
    std::uint64_t latency_samples = 0;

    
    int max_from_dut_occupancy = 0;
    int max_to_dut_occupancy = 0;
};

class CxsLinkModel {
public:
    struct Config {
        int latency_cycles = 2;
        int from_dut_depth = 16;  
        int to_dut_depth = 16;    
        int initial_dut_to_model_credits = 4;
        int max_dut_to_model_credits = 8;
        int initial_model_to_dut_credits = 0;
        int max_model_to_dut_credits = 8;
        int activation_timeout_cycles = 100;
        int max_packet_flits = 64;
        
        
        int max_flits_per_tick = 0;
        bool auto_return_rx_credits = true;
        bool check_interleave = true;
    };

    CxsLinkModel();
    explicit CxsLinkModel(Config cfg);

    
    void request_activate();
    void ack_activate();
    void request_deactivate();
    bool deactivation_drained() const;
    
    void finish_deactivate();
    
    void reset();

    LinkState state() const { return state_; }
    bool link_up() const { return state_ == LinkState::ACTIVE; }
    bool config_ok() const { return config_ok_; }
    std::uint64_t now() const { return now_; }
    void tick();

    
    
    void grant_model_to_dut_credits(int n);
    
    
    int grant_dut_to_model_credits(int n);
    void set_auto_return_rx_credits(bool on) {
        cfg_.auto_return_rx_credits = on;
    }

    int model_to_dut_credits() const { return m2d_credits_; }
    int dut_to_model_credits() const { return d2m_credits_; }

    
    
    
    bool from_dut(const CxsFlit& f, std::uint32_t packet_id = 0);
    bool to_dut(const CxsFlit& f, std::uint32_t packet_id = 0);

    
    bool rx_ready() const { return !rx_ready_q_.empty(); }
    bool rx_pop(CxsFlit& out);
    bool tx_ready() const { return !tx_ready_q_.empty(); }
    bool tx_pop(CxsFlit& out);

    
    bool rx_packet_ready() const { return !rx_packets_.empty(); }
    std::vector<CxsFlit> rx_packet();
    bool rx_packet_open() const { return rx_pkt_open_; }
    bool tx_packet_open() const { return tx_pkt_open_; }

    
    const LinkStats& stats() const { return stats_; }
    void clear_stats() { stats_ = LinkStats{}; }
    int from_dut_occupancy() const;
    int to_dut_occupancy() const;
    int from_dut_depth() const { return cfg_.from_dut_depth; }
    int to_dut_depth() const { return cfg_.to_dut_depth; }
    const Config& config() const { return cfg_; }

private:
    struct Pending {
        CxsFlit f;
        std::uint32_t packet_id = 0;
        std::uint64_t injected = 0;
        int remaining = 0;
    };
    struct Delivered {
        CxsFlit f;
        std::uint32_t packet_id = 0;
        std::uint64_t injected = 0;
    };

    void deliver(Pending& p, std::deque<Delivered>& q);
    void advance(std::deque<Pending>& pipe, std::deque<Delivered>& ready);
    bool has_inflight() const;
    void note_latency(std::uint64_t injected);

    Config cfg_{};
    bool config_ok_ = true;
    LinkState state_ = LinkState::DOWN;
    std::uint64_t now_ = 0;

    std::deque<Pending> down_pipe_;    
    std::deque<Pending> up_pipe_;      
    std::deque<Delivered> rx_ready_q_; 
    std::deque<Delivered> tx_ready_q_; 

    int d2m_credits_ = 0;  
    int m2d_credits_ = 0;  

    
    bool rx_pkt_open_ = false;
    std::uint32_t rx_pkt_id_ = 0;
    std::vector<CxsFlit> rx_pkt_buf_;
    std::deque<std::vector<CxsFlit>> rx_packets_;

    
    bool tx_pkt_open_ = false;
    std::uint32_t tx_pkt_id_ = 0;
    int tx_pkt_flits_ = 0;

    std::uint64_t activation_start_ = 0;
    bool activation_timeout_reported_ = false;

    LinkStats stats_{};
};

}
