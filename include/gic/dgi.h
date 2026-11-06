#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace numa::gic {


constexpr std::uint8_t kDgiLpi      = 0x0;
constexpr std::uint8_t kDgiLpiAck   = 0x1;
constexpr std::uint8_t kDgiCmd      = 0x2;
constexpr std::uint8_t kDgiCmdAck   = 0x3;
constexpr std::uint8_t kDgiRegWrite = 0x4;
constexpr std::uint8_t kDgiRegWack  = 0x5;
constexpr std::uint8_t kDgiRegRead  = 0x6;
constexpr std::uint8_t kDgiRegRdata = 0x7;
constexpr std::uint8_t kDgiSpi      = 0x8;
constexpr std::uint8_t kDgiSgi      = 0x9;
constexpr std::uint8_t kDgiPtb      = 0xA;
constexpr std::uint8_t kDgiRwp      = 0xB;
constexpr std::uint8_t kDgiSpiDeact = 0xC;
constexpr std::uint8_t kDgiVsgi     = 0xD;
constexpr std::uint8_t kDgiVmovp    = 0xE;


constexpr std::uint8_t kSpiNewReq      = 0;
constexpr std::uint8_t kSpiAck         = 1;
constexpr std::uint8_t kSpiUpdReq      = 2;
constexpr std::uint8_t kSpiDeactNotify = 3;
constexpr std::uint8_t kSpiReturn      = 4;
constexpr std::uint8_t kSpiReturnAck   = 5;
constexpr std::uint8_t kSpiDeact       = 6;
constexpr std::uint8_t kSpiDeactAck    = 7;


struct SgiMessage {
    std::uint8_t  opcode = kDgiSgi;
    bool          state  = false;
    std::uint8_t  sub    = 0;
    bool          all    = false;
    std::uint8_t  sgi    = 0;
    std::uint8_t  sgt    = 0;
    std::uint16_t list   = 0;
    std::uint8_t  aff1   = 0;
    std::uint8_t  aff2   = 0;
    std::uint8_t  chip   = 0;
};

std::uint64_t encode_sgi(const SgiMessage& m);


bool decode_sgi(std::uint64_t w, SgiMessage& m);


bool decode_sgi_beats(const std::uint32_t* beats, std::size_t n, SgiMessage& m);


struct SpiMessage {
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
};

std::uint64_t encode_spi(const SpiMessage& m);
bool decode_spi(std::uint64_t w, SpiMessage& m);
bool decode_spi_beats(const std::uint32_t* beats, std::size_t n, SpiMessage& m);


struct LpiMessage {
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
};

std::uint64_t encode_lpi(const LpiMessage& m);
bool decode_lpi(std::uint64_t w, LpiMessage& m);
bool decode_lpi_beats(const std::uint32_t* beats, std::size_t n, LpiMessage& m);


struct LpiAckMessage {
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
};

std::uint64_t encode_lpi_ack(const LpiAckMessage& m);
bool decode_lpi_ack(std::uint64_t w, LpiAckMessage& m);
bool decode_lpi_ack_beats(const std::uint32_t* beats, std::size_t n,
                          LpiAckMessage& m);


struct VsgiMessage {
    std::uint8_t  opcode    = kDgiVsgi;
    bool          allow_hzd = false;
    std::uint8_t  sub       = 0;
    std::uint8_t  slot      = 0;
    std::uint16_t id        = 0;
    std::uint16_t vpe       = 0;
    bool          vpe_bad   = false;
    std::uint8_t  chip      = 0;
};

std::uint64_t encode_vsgi(const VsgiMessage& m);
bool decode_vsgi(std::uint64_t w, VsgiMessage& m);
bool decode_vsgi_beats(const std::uint32_t* beats, std::size_t n, VsgiMessage& m);


struct VsgiAckMessage {
    std::uint8_t opcode = kDgiVsgi;
    bool         reject = false;
    std::uint8_t sub    = 1;
    bool         abort  = false;
    std::uint8_t req_id = 0;
    std::uint8_t chip   = 0;
};

std::uint64_t encode_vsgi_ack(const VsgiAckMessage& m);
bool decode_vsgi_ack(std::uint64_t w, VsgiAckMessage& m);
bool decode_vsgi_ack_beats(const std::uint32_t* beats, std::size_t n,
                           VsgiAckMessage& m);


constexpr std::size_t kVmovpBeats = 6;

struct VmovpMessage {
    std::uint8_t  opcode    = kDgiVmovp;
    bool          db        = false;
    std::uint8_t  sub       = 0;
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
};


std::size_t encode_vmovp_beats(const VmovpMessage& m, std::uint32_t* out,
                               std::size_t cap);

bool decode_vmovp_beats(const std::uint32_t* beats, std::size_t n,
                        VmovpMessage& m);


struct VmovpAckMessage {
    std::uint8_t opcode = kDgiVmovp;
    std::uint8_t sub    = 1;
    std::uint8_t err    = 0;
    std::uint8_t chip   = 0;
};

std::uint64_t encode_vmovp_ack(const VmovpAckMessage& m);
bool decode_vmovp_ack(std::uint64_t w, VmovpAckMessage& m);
bool decode_vmovp_ack_beats(const std::uint32_t* beats, std::size_t n,
                            VmovpAckMessage& m);


constexpr std::uint8_t kLpiCmdMovi  = 0x0;
constexpr std::uint8_t kLpiCmdInv   = 0x1;
constexpr std::uint8_t kLpiCmdClr   = 0x2;
constexpr std::uint8_t kLpiCmdMapc  = 0x3;
constexpr std::uint8_t kLpiCmdVmovi = 0x4;
constexpr std::uint8_t kLpiCmdVinv  = 0x5;
constexpr std::uint8_t kLpiCmdVclr  = 0x6;
constexpr std::uint8_t kLpiCmdVmapp = 0x8;
constexpr std::uint8_t kLpiCmdVmovp = 0xC;
constexpr std::uint8_t kLpiCmdInvdb = 0xD;
constexpr std::uint8_t kLpiCmdVsgi  = 0xE;

constexpr unsigned kCmdAddrWidthN2 = 36;
constexpr std::size_t kCmdBeats = 5;


struct CmdMessage {
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
};

std::size_t encode_cmd_beats(const CmdMessage& m, std::uint32_t* out,
                             std::size_t cap);
bool decode_cmd_beats(const std::uint32_t* beats, std::size_t n,
                      CmdMessage& m);


struct CmdAckMessage {
    std::uint8_t opcode = kDgiCmdAck;
    std::uint8_t err    = 0;
    std::uint8_t its    = 0;
    std::uint8_t type   = 0;
    bool         all    = false;
    bool         reject = false;
    bool         abort  = false;
    std::uint8_t chip   = 0;
};

std::uint64_t encode_cmd_ack(const CmdAckMessage& m);
bool decode_cmd_ack(std::uint64_t w, CmdAckMessage& m);
bool decode_cmd_ack_beats(const std::uint32_t* beats, std::size_t n,
                          CmdAckMessage& m);


struct RwpMessage {
    std::uint8_t opcode = kDgiRwp;
    std::uint8_t state  = 1;
    std::uint8_t chip   = 0;
};

std::size_t encode_rwp_beats(const RwpMessage& m, std::uint32_t* out,
                             std::size_t cap);
bool decode_rwp_beats(const std::uint32_t* beats, std::size_t n,
                      RwpMessage& m);


struct RegAccessMessage {
    std::uint8_t  opcode = kDgiRegWrite;
    bool          ns     = false;
    std::uint8_t  offset = 0;
    std::uint16_t addr   = 0;
    std::uint8_t  size   = 0;
    std::uint8_t  chip   = 0;
    std::uint32_t data   = 0;
};


std::size_t encode_reg_access_beats(const RegAccessMessage& m,
                                    std::uint32_t* out, std::size_t cap);
bool decode_reg_access_beats(const std::uint32_t* beats, std::size_t n,
                             RegAccessMessage& m);

struct RegWackMessage {
    std::uint8_t opcode = kDgiRegWack;
    bool         defer  = false;
    bool         err    = false;
    std::uint8_t chip   = 0;
};

std::uint64_t encode_reg_wack(const RegWackMessage& m);
bool decode_reg_wack(std::uint64_t w, RegWackMessage& m);
bool decode_reg_wack_beats(const std::uint32_t* beats, std::size_t n,
                           RegWackMessage& m);

struct RegDataMessage {
    std::uint8_t  opcode = kDgiRegRdata;
    std::uint8_t  chip   = 0;
    std::uint64_t data   = 0;
};


std::size_t encode_reg_data_beats(const RegDataMessage& m,
                                  std::uint32_t* out, std::size_t cap);
bool decode_reg_data_beats(const std::uint32_t* beats, std::size_t n,
                           RegDataMessage& m);


constexpr std::size_t kPtbBeats = 9;
constexpr std::uint8_t kPtbReq     = 0;
constexpr std::uint8_t kPtbAck     = 1;
constexpr std::uint8_t kPtbRead    = 2;
constexpr std::uint8_t kPtbReadAck = 3;

struct PtbMessage {
    std::uint8_t opcode    = kDgiPtb;
    std::uint8_t subop     = kPtbReq;
    bool         comp      = false;
    std::uint16_t target   = 0;
    std::uint8_t block     = 0;
    std::uint8_t stream_id = 0;
    std::uint8_t chip      = 0;
    bool         pend_en   = false;
    std::array<std::uint8_t, 32> map{};
};

std::size_t encode_ptb_beats(const PtbMessage& m, std::uint32_t* out,
                             std::size_t cap);
bool decode_ptb_beats(const std::uint32_t* beats, std::size_t n,
                      PtbMessage& m);

}
