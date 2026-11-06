#pragma once

#include "cml/cml_bitbuf.h"
#include "cml/cml_fields.h"

#include <cstdint>
#include <map>
#include <string>

namespace numa::cml {

using Values = std::map<std::string, std::uint64_t>;


void pack_msg(BitBuf& buf, const MsgDef& def, const Values& values,
              unsigned base_lsb = 0);


Values unpack_msg(const BitBuf& buf, const MsgDef& def,
                  unsigned base_lsb = 0);


void pack_header(BitBuf& buf, std::uint8_t srcid6, std::uint8_t tgtid6,
                 std::uint16_t msgptr = 0x0001, std::uint8_t length = 0,
                 std::uint8_t datasz = 0, std::uint8_t lcrd = 0,
                 std::uint16_t msgcrds = 0, std::uint8_t lll = 0,
                 std::uint8_t type = 0);

Values unpack_header(const BitBuf& buf);


BitBuf pack_flit(const MsgDef& def, const Values& values,
                 unsigned base_dw = 2, std::uint8_t srcid6 = 0,
                 std::uint8_t tgtid6 = 0);


BitBuf pack_flit2(const MsgDef& def0, const Values& values0,
                  const MsgDef& def1, const Values& values1,
                  unsigned base0_dw = 2, unsigned base1_dw = 5,
                  std::uint8_t srcid6 = 0, std::uint8_t tgtid6 = 0,
                  std::uint8_t length = 0);


BitBuf pack_init_vd(Vdop op);


BitBuf pack_ucmisc_crd(unsigned req, unsigned dat, unsigned snp,
                       unsigned misc);


struct UcmiscCrd {
    unsigned req  = 0;
    unsigned dat  = 0;
    unsigned snp  = 0;
    unsigned misc = 0;
};


BitBuf pack_flit_ucmisc_msg(const UcmiscCrd& crd, const MsgDef& def,
                            const Values& values, unsigned msg_dw = 5,
                            std::uint8_t srcid6 = 0,
                            std::uint8_t tgtid6 = 0);


bool unpack_ucmisc_crd(const BitBuf& buf, UcmiscCrd& out);

}
