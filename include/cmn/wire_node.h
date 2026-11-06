#pragma once

#include "cml/cml_credit.h"
#include "cml/cml_wire.h"
#include "cmn/numa_node.h"

#include <array>
#include <cstdint>
#include <vector>

namespace numa::cmn {

class WireNode {
public:
    using Flit = std::array<std::uint32_t, 16>;

    explicit WireNode(int node_id = 1,
                      std::uint64_t base_line = 0,
                      std::uint64_t lines = 4096);


    void on_flit(const Flit& in, std::vector<Flit>& out);

    NumaNode&     node()      { return node_; }
    std::uint64_t flits_in()  const { return flits_in_; }
    std::uint64_t flits_out() const { return flits_out_; }
    std::uint64_t llctrl_in() const { return llctrl_in_; }
    std::uint64_t adf_in()    const { return adf_in_; }
    std::uint64_t msg_in()    const { return msg_in_; }
    unsigned      adf_left()  const { return adf_left_; }
    std::uint64_t crd_short() const { return crd_short_; }
    std::uint64_t adf_proto_err() const { return adf_proto_err_; }
    std::uint64_t ucmisc_in()  const { return ucmisc_in_; }
    std::uint64_t ucmisc_bad() const { return ucmisc_bad_; }


    std::uint64_t multi_msg_ignored() const { return multi_msg_ignored_; }
    cml::CreditSet& credit()  { return credit_; }

private:
    void emit_rsp_with_data(const cml::wire::Rsp& rsp,
                            const std::uint8_t data[kLineBytes],
                            std::vector<Flit>& out);

    void process_msg(const std::uint32_t md[3], const std::uint8_t* data_in,
                     int srcid, std::vector<Flit>& out);

    NumaNode      node_;
    unsigned      adf_left_ = 0;
    std::uint8_t  adf_buf_[kLineBytes] = {};
    unsigned      adf_pos_  = 0;
    std::uint64_t flits_in_ = 0;
    std::uint64_t flits_out_ = 0;
    std::uint64_t llctrl_in_ = 0;
    std::uint64_t adf_in_ = 0;
    std::uint64_t msg_in_ = 0;


    bool          pending_  = false;
    std::uint32_t pending_md_[3] = {};
    int           pending_srcid_ = 0;
    std::uint64_t adf_proto_err_ = 0;
    std::uint64_t ucmisc_in_ = 0;
    std::uint64_t ucmisc_bad_ = 0;
    std::uint64_t multi_msg_ignored_ = 0;


    cml::CreditSet credit_;
    std::uint64_t  crd_short_ = 0;
};

}
