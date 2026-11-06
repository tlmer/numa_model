#pragma once

#include "cml/cml_codec.h"

#include <cstdint>

namespace numa::cml::wire {


struct Req {
    std::uint8_t  reqop   = 0;
    std::uint16_t txnid   = 0;
    std::uint8_t  type    = 0;
    std::uint8_t  len     = 0;
    std::uint8_t  reqattr = 0;
    std::uint8_t  byt2    = 0;
    std::uint8_t  byt9    = 0;
    bool          be_val  = false;
    bool          ns      = false;
    std::uint8_t  qos     = 0;
    std::uint64_t addr    = 0;
    bool          valid   = false;


    std::uint8_t  lo_addr = 0;
    bool          excl    = false;
};


struct Rsp {
    std::uint8_t  respop   = 0;
    std::uint8_t  respattr = 0;
    std::uint16_t txnid    = 0;
    std::uint8_t  rsptype  = 0;
    std::uint8_t  len      = 0;
    bool          be_val   = false;
    bool          valid    = false;
};


void decode_req(const std::uint32_t d[3], Req& r);
void encode_req(const Req& r, std::uint32_t d[3]);

Rsp  decode_rsp(std::uint32_t dw);
std::uint32_t encode_rsp(const Rsp& r);


struct FlitHdr {
    std::uint8_t  type     = 0;
    std::uint8_t  hops     = 0;

    std::uint16_t msgptr   = 0;
    std::uint8_t  linkcrd  = 0;
    std::uint8_t  length   = 0;
    std::uint8_t  datasz   = 0;
    std::uint16_t msgcrds  = 0;
    std::uint8_t  srcid6   = 0;
    std::uint8_t  tgtid6   = 0;
};

void encode_flit_hdr(const FlitHdr& h, std::uint32_t d[2]);
FlitHdr decode_flit_hdr(const std::uint32_t d[2]);


constexpr unsigned kAdfDwords = 16;
constexpr unsigned kAdfBytes  = kAdfDwords * 4;

void encode_adf(const std::uint8_t data[kAdfBytes],
                std::uint32_t d[kAdfDwords]);
void decode_adf(const std::uint32_t d[kAdfDwords],
                std::uint8_t data[kAdfBytes]);


constexpr std::uint8_t kRespOpComp          = 0x0;
constexpr std::uint8_t kRespOpCompDataUcSc  = 0x1;
constexpr std::uint8_t kRespOpSmpReqResp1   = 0x6;
constexpr std::uint8_t kRespAttrNcbWrData   = 0x0;
constexpr std::uint8_t kRespAttrSmpCompAck  = 0x3;


constexpr std::uint8_t kReqOpReadPreferUnique = 0x0E;
constexpr std::uint8_t kReqOpWriteUniqueZero  = 0x2F;


constexpr std::uint8_t kWireTypeMem = 0b0001;
constexpr std::uint8_t kWireTypeSnp = 0b0010;
constexpr std::uint8_t kWireRspTypeNd  = 0b1000;
constexpr std::uint8_t kWireRspTypeDat = 0b1100;


inline std::uint8_t make_wire_rsptype(bool dat, unsigned rt) {
    const std::uint8_t kind = dat ? kWireRspTypeDat : kWireRspTypeNd;
    return static_cast<std::uint8_t>(kind | ((rt & 1u) << 1) | ((rt & 1u) ? 0u : 1u));
}

inline unsigned ccl_rsptype_from_wire(std::uint8_t wire_rsptype) {
    return (wire_rsptype >> 1) & 1u;
}


cml::Values to_ccl_nd_req(const FlitHdr& h, const Req& r);


Req from_ccl_nd_req(const cml::Values& v);


cml::Values to_ccl_nd_rsp(const FlitHdr& h, const Rsp& r);
Rsp from_ccl_nd_rsp(const cml::Values& v);


Rsp tag_rsp_kind(const Rsp& r, bool dat);

}
