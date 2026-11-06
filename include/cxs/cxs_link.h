// numa_model public API. Commercial license; see LICENSE_NOTICE.md.
// SPDX-License-Identifier: LicenseRef-Proprietary

#pragma once

#include "cml/cml_codec.h"

#include <array>
#include <cstddef>

namespace numa::cxs {

enum class LinkState {
    DOWN,
    ACTIVATING,
    ACTIVE,
    DEACTIVATING,
};

const char* to_string(LinkState state);

struct Credits {
    
    int l = 256;
    
    std::array<int, static_cast<std::size_t>(cml::CrdClass::COUNT)> p{
        0, 0, 0, 0, 0};
};

class LinkModel {
public:
    explicit LinkModel(int l_credit = 256) { credits_.l = l_credit; }

    LinkState state() const { return state_; }
    const Credits& credits() const { return credits_; }

    void request_activate() {
        if (state_ == LinkState::DOWN) {
            state_ = LinkState::ACTIVATING;
        }
    }
    void ack_activate() {
        if (state_ == LinkState::ACTIVATING) {
            state_ = LinkState::ACTIVE;
        }
    }
    void request_deactivate() { state_ = LinkState::DEACTIVATING; }
    void down() { state_ = LinkState::DOWN; }

    
    void grant(cml::CrdClass c, int n);

    bool can_send(cml::CrdClass c) const;
    bool consume(cml::CrdClass c);

    
    cml::BitBuf init_vd(cml::Vdop op) const { return cml::pack_init_vd(op); }
    cml::BitBuf grant_flit(unsigned req, unsigned dat, unsigned snp,
                           unsigned misc) const {
        return cml::pack_ucmisc_crd(req, dat, snp, misc);
    }

private:
    static std::size_t idx(cml::CrdClass c) {
        return static_cast<std::size_t>(c);
    }

    LinkState state_ = LinkState::DOWN;
    Credits credits_{};
};

}
