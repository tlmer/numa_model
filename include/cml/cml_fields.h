#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace numa::cml {

constexpr unsigned kSidW  = 10;
constexpr unsigned kFlitW = 512;
constexpr unsigned kDw    = 32;


constexpr std::uint32_t kCntl = 0x3C41;

enum class Llctrl : std::uint8_t {
    LCRD    = 0x0,
    RETRY   = 0x1,
    INIT    = 0xC,
};

enum class Subtype : std::uint8_t {
    LCRD    = 0x1,
    INIT_VD = 0xE,
};

enum class Vdop : std::uint8_t {
    ACTIVATE   = 0x1,
    DEACTIVATE = 0x2,
};

enum class CrdClass : unsigned {
    REQ     = 0,
    DAT     = 1,
    SNP     = 2,
    HA_MISC = 3,
    RA_MISC = 4,
    COUNT   = 5,
};

struct Field {
    const char* name;
    unsigned lsb;
    unsigned width;
};

struct MsgDef {
    const char* name;
    unsigned total;
    std::vector<Field> fields;
};


inline const MsgDef kReqin{
    "REQIN", 182,
    {
        {"VALID", 0, 1}, {"REQTYPE", 1, 1}, {"DATAPRESENT", 2, 1},
        {"SRCID", 3, kSidW}, {"TGTID", 13, kSidW}, {"TXNID", 23, 16},
        {"REQINFO", 39, 16}, {"CLADDR", 55, 46}, {"NS", 101, 1},
        {"QOS", 102, 4}, {"REQCHAINCOUNT", 106, 2}, {"BYT9RSVD", 108, 4},
        {"BYT2RSVD", 112, 2}, {"LO_ADDR", 114, 6}, {"EXCL", 120, 1},
        {"GROUPID", 121, 8}, {"EXT1USER", 129, 9}, {"USER", 138, 28},
        {"RESPERR", 166, 2}, {"EXT6USER", 168, 14},
    }};

inline const MsgDef kNdReqout{
    "ND_REQOUT", 171,
    {
        {"VALID", 0, 1}, {"REQTYPE", 1, 1}, {"EXTCOUNT", 2, 3},
        {"SRCID", 5, kSidW}, {"TGTID", 15, kSidW}, {"TXNID", 25, 16},
        {"REQINFO", 41, 16}, {"CLADDR", 57, 46}, {"NS", 103, 1},
        {"QOS", 104, 4}, {"REQCHAINVALID", 108, 1}, {"BYT9RSVD", 109, 4},
        {"BYT2RSVD", 113, 2}, {"EXT1_VLD", 115, 1}, {"LO_ADDR", 116, 6},
        {"EXCL", 122, 1}, {"GROUPID", 123, 8}, {"EXT1USER", 131, 9},
        {"EXT2_VLD", 140, 1}, {"USER", 141, 28}, {"EXT3_VLD", 169, 1},
        {"ACK_ME", 170, 1},
    }};

inline const MsgDef kRspin{
    "RSPIN", 85,
    {
        {"VALID", 0, 1}, {"RSPTYPE", 1, 1}, {"DATAPRESENT", 2, 1},
        {"SRCID", 3, kSidW}, {"TGTID", 13, kSidW}, {"TXNID", 23, 16},
        {"RSPINFO", 39, 18}, {"USER", 57, 28},
    }};

inline const MsgDef kNdRspout{
    "ND_RSPOUT", 91,
    {
        {"VALID", 0, 1}, {"RSPTYPE", 1, 1}, {"EXTCOUNT", 2, 3},
        {"SRCID", 5, kSidW}, {"TGTID", 15, kSidW}, {"TXNID", 25, 16},
        {"RSPINFO", 41, 18}, {"EXT2_VLD", 59, 1}, {"USER", 60, 28},
        {"EXT3_VLD", 88, 1}, {"EXT6_VLD", 89, 1}, {"ACK_ME", 90, 1},
    }};

inline const MsgDef kStlDatin{
    "STL_DATIN", 664,
    {
        {"VALID", 0, 1}, {"DATTYPE", 1, 1}, {"DATASIZE", 2, 2},
        {"TXNID", 4, 16}, {"BE_VLD", 20, 1}, {"DATINFO", 21, 7},
        {"DATA", 28, 512}, {"BE", 540, 64}, {"METADATA", 604, 24},
        {"POISON", 628, 8}, {"USER", 636, 28},
    }};

inline const MsgDef kUcmiscin{
    "UCMISCIN", 87,
    {
        {"VALID", 0, 1}, {"MSGCOUNT", 1, 1}, {"DATAPRESENT", 2, 1},
        {"SRCID", 3, kSidW}, {"TGTID", 13, kSidW},
        {"PAYLOAD_DW0", 23, 32}, {"PAYLOAD_DW1", 55, 32},
    }};

inline const MsgDef kCrdin{
    "CRDIN", 37,
    {
        {"VALID", 0, 1}, {"SRCID", 1, kSidW}, {"TGTID", 11, kSidW},
        {"REQCRD", 21, 4}, {"DATCRD", 25, 4},
        {"SNPCRD", 29, 4}, {"MISCCRD", 33, 4},
    }};

inline const MsgDef kCrdout{
    "CRDOUT", 60,
    {
        {"VALID", 0, 1}, {"SRCID", 1, kSidW}, {"TGTID", 11, kSidW},
        {"LINKEND", 21, 6}, {"CRDOP", 27, 1},
        {"REQCRD", 28, 8}, {"DATCRD", 36, 8},
        {"SNPCRD", 44, 8}, {"MISCCRD", 52, 8},
    }};

inline const MsgDef kLaAckin{
    "LA_ACKIN", 19,
    {
        {"VALID", 0, 1}, {"TYPE", 1, 2}, {"TXNID", 3, 16},
    }};


inline const MsgDef kUcmiscCrdDw0{
    "UCMISC_CRD_DW0", 32,
    {
        {"MSGLEN_H", 0, 2}, {"BE_V", 2, 1}, {"MSGTYPE", 3, 4},
        {"MSG_V", 7, 1}, {"MISCOP", 8, 4}, {"PAD", 12, 2},
        {"MSGLEN_L", 14, 2}, {"DATCRD", 16, 8}, {"REQCRD", 24, 8},
    }};

inline const MsgDef kUcmiscCrdDw1{
    "UCMISC_CRD_DW1", 32,
    {
        {"PAD", 0, 16}, {"MISCCRD", 16, 8}, {"SNPCRD", 24, 8},
    }};


inline const MsgDef kLinkCtrl{
    "LINK_CTRL", 512,
    {
        {"TYPE", 0, 1}, {"RSVD1", 1, 1}, {"ACK1", 2, 1}, {"RSVD2", 3, 17},
        {"RSPCRD", 20, 4}, {"REQCRD", 24, 4}, {"DATACRD", 28, 4},
        {"LLCTRL", 32, 4}, {"SUBTYPE", 36, 4}, {"SET_ZEROES", 40, 24},
        {"PAYLOAD", 64, 64},
    }};


inline const MsgDef kHdr{
    "CCIX20_HDR", 64,
    {
        {"TYPE", 0, 1}, {"LLL", 1, 2}, {"MSGPTRVAL", 3, 14},
        {"PAD1", 17, 3}, {"LINKCRD", 20, 4}, {"PAD2", 24, 3},
        {"DATASZ", 27, 2}, {"LENGTH", 29, 3},
        {"MSGCRDS", 32, 12}, {"SRCID", 44, 6}, {"PAD3", 50, 4},
        {"TGTID", 54, 6}, {"PAD4", 60, 4},
    }};

}
