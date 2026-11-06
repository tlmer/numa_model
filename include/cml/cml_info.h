// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include <cstdint>

namespace numa::cml {

constexpr unsigned kReqInfoW  = 16;
constexpr unsigned kRespInfoW = 18;
constexpr unsigned kDatInfoW  = 7;

constexpr unsigned kReqOpLsb    = 0,  kReqOpW    = 8;
constexpr unsigned kMemTypeLsb  = 8,  kMemTypeW  = 3;
constexpr unsigned kStrongNcLsb = 11, kStrongNcW = 1;
constexpr unsigned kReqSizeLsb  = 12, kReqSizeW  = 3;
constexpr unsigned kReqPad2Lsb  = 15;

constexpr unsigned kSnpOpLsb   = 0, kSnpOpW   = 4;
constexpr unsigned kDrLsb      = 4;
constexpr unsigned kSnpCastLsb = 5, kSnpCastW = 2;

constexpr unsigned kRespOpLsb   = 0, kRespOpW   = 3;
constexpr unsigned kRespAttrLsb = 3, kRespAttrW = 3;
constexpr unsigned kRespErrLsb  = 6, kRespErrW  = 2;
constexpr unsigned kRespSrcLsb  = 8;

constexpr unsigned kDatRespOpLsb   = 0;
constexpr unsigned kDatRespAttrLsb = 3;
constexpr unsigned kDatPadLsb      = 6;

enum class ReqType : std::uint8_t { MEM = 0, SNP = 1 };

enum class SmpReqOp : std::uint8_t {
    READUNIQUE           = 0x04,  
    READNOTSHAREDDIRTY   = 0x06,  
    READPREFERUNIQUE     = 0x0E,  
    MAKEREADUNIQUE       = 0x0F,  
    WRITENOSNPFULL       = 0x21,  
    WRITEUNIQUEPTL       = 0x22,  
    WRITEUNIQUEFULL      = 0x23,  
    WRITENOSNPZERO       = 0x2E,  
    WRITEUNIQUEZERO      = 0x2F,  
    WRITEUNIQUEPTLSTASH  = 0x32,  
    WRITEUNIQUEFULLSTASH = 0x33,  
};

enum class SmpRespOp : std::uint8_t {
    SMPSNPRESP2 = 0x5,
    SMPREQRESP1 = 0x6,
    SMPSNPRESP1 = 0x7,
};

enum class SmpRespAttr : std::uint8_t {
    NCBWRDATA        = 0x0,
    DBID             = 0x0,
    NCBWRDATACOMPACK = 0x1,
    COMPDBID         = 0x1,
    WRITEDATACANCEL  = 0x2,
    SMPCOMPACK       = 0x3,
    STASHDONE        = 0x6,
    PERSIST          = 0x7,
};

struct ReqInfoMem {
    std::uint8_t reqop     = 0;
    std::uint8_t memtype   = 0;   
    bool         strong_nc = false;
    std::uint8_t size      = 0;   
};

struct ReqInfoSnp {
    std::uint8_t snpop   = 0;
    bool         dr      = false;
    std::uint8_t snpcast = 0;
};

struct RespInfo {
    std::uint8_t respop   = 0;
    std::uint8_t respattr = 0;
    std::uint8_t resperr  = 0;
    bool         respsrc  = false;
};

struct DatInfo {
    std::uint8_t respop   = 0;
    std::uint8_t respattr = 0;
};

ReqInfoMem    decode_reqinfo_mem(std::uint16_t v);
std::uint16_t encode_reqinfo_mem(const ReqInfoMem& r);
ReqInfoSnp    decode_reqinfo_snp(std::uint16_t v);
std::uint16_t encode_reqinfo_snp(const ReqInfoSnp& r);

RespInfo      decode_rspinfo(std::uint32_t v);
std::uint32_t encode_rspinfo(const RespInfo& r);

DatInfo       decode_datinfo(std::uint32_t v);
std::uint32_t encode_datinfo(const DatInfo& d);

bool is_smp_respop(unsigned respop);

}
