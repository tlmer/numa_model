#pragma once

#include "gic/dgi.h"
#include "gic/gicd.h"

#include <cstdint>
#include <deque>

namespace numa::gic {


struct DeliveredSgi {
    std::uint8_t  opcode = kDgiSgi;
    bool          state  = false;
    bool          all    = false;
    std::uint8_t  sgi    = 0;
    std::uint8_t  sgt    = 0;
    std::uint16_t list   = 0;
    std::uint8_t  chip   = 0;
    std::uint8_t  tri    = 0;
    std::uint8_t  tsize  = 0;
    std::uint8_t  keep   = 0;
};


struct DeliveredSpi {
    std::uint8_t opcode   = kDgiSpi;
    std::uint8_t subop    = 0;
    std::uint8_t update   = 0;
    bool         err      = false;
    bool         pend     = false;
    bool         active   = false;
    bool         pend_lvl = false;
    bool         reg      = false;
    bool         one_of_n = false;
    std::uint8_t id       = 0;
    std::uint8_t priority = 0;
    bool         aff0 = false;
    bool         aff1 = false;
    bool         aff2 = false;
    std::uint8_t group = 0;
    std::uint8_t klass = 0;
    std::uint8_t chip  = 0;
    std::uint8_t tri   = 0;
    std::uint8_t tsize = 0;
    std::uint8_t keep  = 0;
};


struct DeliveredLpi {
    std::uint8_t  opcode    = kDgiLpi;
    std::uint8_t  stream    = 0;
    bool          virt      = false;
    bool          allow_hzd = false;
    std::uint8_t  stream_id = 0;
    std::uint16_t pid       = 0;
    std::uint16_t tgt       = 0;
    bool          prop_en   = false;
    std::uint8_t  prop_pri  = 0;
    bool          prop_vld  = false;
    std::uint8_t  slot      = 0;
    std::uint8_t  chip      = 0;
    std::uint8_t  tri       = 0;
    std::uint8_t  tsize     = 0;
    std::uint8_t  keep      = 0;
};


struct DeliveredLpiAck {
    std::uint8_t  opcode    = kDgiLpiAck;
    std::uint8_t  err       = 0;
    std::uint8_t  stream    = 0;
    bool          virt      = false;
    std::uint8_t  req_id    = 0;
    bool          reject    = false;
    bool          abort     = false;
    std::uint8_t  stream_id = 0;
    std::uint8_t  chip      = 0;
    std::uint16_t data      = 0;
    std::uint8_t  tri       = 0;
    std::uint8_t  tsize     = 0;
    std::uint8_t  keep      = 0;
};


struct DeliveredVsgi {
    std::uint8_t  opcode    = kDgiVsgi;
    bool          allow_hzd = false;
    std::uint8_t  slot      = 0;
    std::uint16_t id        = 0;
    std::uint16_t vpe       = 0;
    bool          vpe_bad   = false;
    std::uint8_t  chip      = 0;
    std::uint8_t  tri       = 0;
    std::uint8_t  tsize     = 0;
    std::uint8_t  keep      = 0;
};


struct DeliveredVsgiAck {
    std::uint8_t opcode = kDgiVsgi;
    bool         reject = false;
    bool         abort  = false;
    std::uint8_t req_id = 0;
    std::uint8_t chip   = 0;
    std::uint8_t tri    = 0;
    std::uint8_t tsize  = 0;
    std::uint8_t keep   = 0;
};


struct DeliveredVmovp {
    std::uint8_t  opcode    = kDgiVmovp;
    bool          db        = false;
    std::uint16_t tgt       = 0;
    std::uint16_t vpe       = 0;
    std::uint16_t sgi_pend  = 0;
    std::uint8_t  chip      = 0;
    std::uint16_t db_id     = 0;
    std::uint64_t vpt_addr  = 0;
    std::uint8_t  stream_id = 0;
    std::uint16_t hppi1     = 0;
    std::uint8_t  hppi1_pri = 0;
    std::uint16_t hppi2     = 0;
    std::uint8_t  hppi2_pri = 0;
    std::uint16_t hppi3     = 0;
    std::uint8_t  hppi3_pri = 0;
    std::uint8_t  tri       = 0;
    std::uint8_t  tsize     = 0;
    std::uint8_t  keep      = 0;
};


struct DeliveredVmovpAck {
    std::uint8_t opcode = kDgiVmovp;
    std::uint8_t err    = 0;
    std::uint8_t chip   = 0;
    std::uint8_t tri    = 0;
    std::uint8_t tsize  = 0;
    std::uint8_t keep   = 0;
};


struct DeliveredCmd {
    std::uint8_t  opcode    = kDgiCmd;
    std::uint8_t  type      = 0;
    std::uint8_t  its       = 0;
    bool          allow_hzd = false;
    bool          db        = false;
    std::uint8_t  chip      = 0;
    std::uint16_t id        = 0;
    std::uint16_t src_tgt   = 0;
    std::uint16_t dst_tgt   = 0;
    std::uint64_t vpt_addr  = 0;
    std::uint8_t  vpt_size  = 0;
    std::uint64_t vconf_addr = 0;
    std::uint8_t  tri       = 0;
    std::uint8_t  tsize     = 0;
    std::uint8_t  keep      = 0;
};


struct DeliveredCmdAck {
    std::uint8_t opcode = kDgiCmdAck;
    std::uint8_t err    = 0;
    std::uint8_t its    = 0;
    std::uint8_t type   = 0;
    bool         all    = false;
    bool         reject = false;
    bool         abort  = false;
    std::uint8_t chip   = 0;
    std::uint8_t tri    = 0;
    std::uint8_t tsize  = 0;
    std::uint8_t keep   = 0;
};


struct DeliveredRwp {
    std::uint8_t opcode = kDgiRwp;
    std::uint8_t state  = 1;
    std::uint8_t chip   = 0;
    std::uint8_t tri    = 0;
    std::uint8_t tsize  = 0;
    std::uint8_t keep   = 0;
};


struct DeliveredRegAccess {
    std::uint8_t  opcode = kDgiRegWrite;
    bool          ns     = false;
    std::uint8_t  offset = 0;
    std::uint16_t addr   = 0;
    std::uint8_t  size   = 0;
    std::uint8_t  chip   = 0;
    std::uint32_t data   = 0;
    std::uint8_t  tri    = 0;
    std::uint8_t  tsize  = 0;
    std::uint8_t  keep   = 0;
};


struct DeliveredPtb {
    std::uint8_t  opcode    = kDgiPtb;
    std::uint8_t  subop     = kPtbReq;
    bool          comp      = false;
    std::uint16_t target    = 0;
    std::uint8_t  block     = 0;
    std::uint8_t  stream_id = 0;
    std::uint8_t  chip      = 0;
    bool          pend_en   = false;
    std::array<std::uint8_t, 32> map{};
    std::uint8_t  tri       = 0;
    std::uint8_t  tsize     = 0;
    std::uint8_t  keep      = 0;
};


struct DeliveredRegWack {
    std::uint8_t opcode = kDgiRegWack;
    bool         defer  = false;
    bool         err    = false;
    std::uint8_t chip   = 0;
    std::uint8_t tri    = 0;
    std::uint8_t tsize  = 0;
    std::uint8_t keep   = 0;
};


struct DeliveredRegData {
    std::uint8_t  opcode = kDgiRegRdata;
    std::uint8_t  chip   = 0;
    std::uint64_t data   = 0;
    std::uint8_t  tri    = 0;
    std::uint8_t  tsize  = 0;
    std::uint8_t  keep   = 0;
};

class Endpoint {
public:

    explicit Endpoint(std::uint8_t tri = 0) : tri_(tri) {}


    bool on_message(std::uint64_t payload33);

    bool pop(DeliveredSgi& s);
    bool pop_spi(DeliveredSpi& s);
    bool pop_lpi(DeliveredLpi& s);
    bool pop_lpi_ack(DeliveredLpiAck& s);
    bool pop_vsgi(DeliveredVsgi& s);
    bool pop_vsgi_ack(DeliveredVsgiAck& s);
    bool pop_vmovp(DeliveredVmovp& s);
    bool pop_vmovp_ack(DeliveredVmovpAck& s);
    bool pop_cmd(DeliveredCmd& s);
    bool pop_cmd_ack(DeliveredCmdAck& s);
    bool pop_rwp(DeliveredRwp& s);
    bool pop_reg(DeliveredRegAccess& s);
    bool pop_reg_wack(DeliveredRegWack& s);
    bool pop_reg_data(DeliveredRegData& s);
    bool pop_ptb(DeliveredPtb& s);
    std::size_t pending() const { return inbox_.size(); }
    std::size_t spi_pending() const { return spi_inbox_.size(); }
    std::size_t lpi_pending() const { return lpi_inbox_.size(); }
    std::size_t lpi_ack_pending() const { return lpi_ack_inbox_.size(); }
    std::size_t vsgi_pending() const { return vsgi_inbox_.size(); }
    std::size_t vsgi_ack_pending() const { return vsgi_ack_inbox_.size(); }
    std::size_t vmovp_pending() const { return vmovp_inbox_.size(); }
    std::size_t vmovp_ack_pending() const { return vmovp_ack_inbox_.size(); }
    std::size_t cmd_pending() const { return cmd_inbox_.size(); }
    std::size_t cmd_ack_pending() const { return cmd_ack_inbox_.size(); }
    std::size_t rwp_pending() const { return rwp_inbox_.size(); }
    std::size_t reg_pending() const { return reg_inbox_.size(); }
    std::size_t reg_wack_pending() const { return reg_wack_inbox_.size(); }
    std::size_t reg_data_pending() const { return reg_data_inbox_.size(); }
    std::size_t ptb_pending() const { return ptb_inbox_.size(); }
    const std::deque<DeliveredSgi>& inbox() const { return inbox_; }
    const std::deque<DeliveredSpi>& spi_inbox() const { return spi_inbox_; }
    const std::deque<DeliveredLpi>& lpi_inbox() const { return lpi_inbox_; }
    const std::deque<DeliveredLpiAck>& lpi_ack_inbox() const {
        return lpi_ack_inbox_;
    }
    const std::deque<DeliveredVsgi>& vsgi_inbox() const {
        return vsgi_inbox_;
    }
    const std::deque<DeliveredVsgiAck>& vsgi_ack_inbox() const {
        return vsgi_ack_inbox_;
    }
    const std::deque<DeliveredVmovp>& vmovp_inbox() const {
        return vmovp_inbox_;
    }
    const std::deque<DeliveredVmovpAck>& vmovp_ack_inbox() const {
        return vmovp_ack_inbox_;
    }
    const std::deque<DeliveredCmd>& cmd_inbox() const { return cmd_inbox_; }
    const std::deque<DeliveredCmdAck>& cmd_ack_inbox() const {
        return cmd_ack_inbox_;
    }
    const std::deque<DeliveredRwp>& rwp_inbox() const { return rwp_inbox_; }
    const std::deque<DeliveredRegAccess>& reg_inbox() const {
        return reg_inbox_;
    }
    const std::deque<DeliveredRegWack>& reg_wack_inbox() const {
        return reg_wack_inbox_;
    }
    const std::deque<DeliveredRegData>& reg_data_inbox() const {
        return reg_data_inbox_;
    }
    const std::deque<DeliveredPtb>& ptb_inbox() const { return ptb_inbox_; }

    std::uint8_t tri() const { return tri_; }
    std::uint64_t packets()    const { return packets_; }
    std::uint64_t delivered()  const { return delivered_; }
    std::uint64_t spi_delivered() const { return spi_delivered_; }
    std::uint64_t lpi_delivered() const { return lpi_delivered_; }
    std::uint64_t lpi_ack_delivered() const { return lpi_ack_delivered_; }
    std::uint64_t vsgi_delivered() const { return vsgi_delivered_; }
    std::uint64_t vsgi_ack_delivered() const { return vsgi_ack_delivered_; }
    std::uint64_t vmovp_delivered() const { return vmovp_delivered_; }
    std::uint64_t vmovp_ack_delivered() const { return vmovp_ack_delivered_; }
    std::uint64_t cmd_delivered() const { return cmd_delivered_; }
    std::uint64_t cmd_ack_delivered() const { return cmd_ack_delivered_; }
    std::uint64_t rwp_delivered() const { return rwp_delivered_; }
    std::uint64_t reg_delivered() const { return reg_delivered_; }
    std::uint64_t reg_wack_delivered() const { return reg_wack_delivered_; }
    std::uint64_t reg_data_delivered() const { return reg_data_delivered_; }
    std::uint64_t ptb_delivered() const { return ptb_delivered_; }

    std::uint64_t other_op     () const { return other_op_; }
    std::uint64_t tri_mismatch () const { return tri_mismatch_; }
    std::uint64_t overflow     () const { return overflow_; }
    std::uint64_t short_packet () const { return short_packet_; }

private:
    std::uint8_t tri_;
    BeatReassembler ra_;
    std::deque<DeliveredSgi> inbox_;
    std::deque<DeliveredSpi> spi_inbox_;
    std::deque<DeliveredLpi> lpi_inbox_;
    std::deque<DeliveredLpiAck> lpi_ack_inbox_;
    std::deque<DeliveredVsgi> vsgi_inbox_;
    std::deque<DeliveredVsgiAck> vsgi_ack_inbox_;
    std::deque<DeliveredVmovp> vmovp_inbox_;
    std::deque<DeliveredVmovpAck> vmovp_ack_inbox_;
    std::deque<DeliveredCmd> cmd_inbox_;
    std::deque<DeliveredCmdAck> cmd_ack_inbox_;
    std::deque<DeliveredRwp> rwp_inbox_;
    std::deque<DeliveredRegAccess> reg_inbox_;
    std::deque<DeliveredRegWack> reg_wack_inbox_;
    std::deque<DeliveredRegData> reg_data_inbox_;
    std::deque<DeliveredPtb> ptb_inbox_;
    std::uint64_t packets_ = 0;
    std::uint64_t delivered_ = 0;
    std::uint64_t spi_delivered_ = 0;
    std::uint64_t lpi_delivered_ = 0;
    std::uint64_t lpi_ack_delivered_ = 0;
    std::uint64_t vsgi_delivered_ = 0;
    std::uint64_t vsgi_ack_delivered_ = 0;
    std::uint64_t vmovp_delivered_ = 0;
    std::uint64_t vmovp_ack_delivered_ = 0;
    std::uint64_t cmd_delivered_ = 0;
    std::uint64_t cmd_ack_delivered_ = 0;
    std::uint64_t rwp_delivered_ = 0;
    std::uint64_t reg_delivered_ = 0;
    std::uint64_t reg_wack_delivered_ = 0;
    std::uint64_t reg_data_delivered_ = 0;
    std::uint64_t ptb_delivered_ = 0;
    std::uint64_t other_op_ = 0;
    std::uint64_t tri_mismatch_ = 0;
    std::uint64_t overflow_ = 0;
    std::uint64_t short_packet_ = 0;
    std::uint64_t last_overflow_ = 0;
};

}
