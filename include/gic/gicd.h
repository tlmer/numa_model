#pragma once

#include <cstdint>
#include <deque>
#include <vector>

namespace numa::gic {

constexpr std::uint8_t kMsgTypeUcmisc2Dw = 0x8;
constexpr std::uint8_t kMiscOpGeneric    = 0x8;
constexpr unsigned      kPayloadBits     = 33;

struct Message {
    std::uint8_t  msglen_h = 0;
    std::uint8_t  msglen_l = 0;
    std::uint8_t  msgtype  = 0;
    std::uint8_t  miscop   = 0;
    bool          msg_v    = false;
    bool          be_v     = false;
    bool          genop    = false;
    std::uint64_t payload  = 0;
};


Message make(std::uint64_t payload, std::uint8_t msglen_h = 0);


void encode(const Message& m, std::uint32_t& dw0, std::uint32_t& dw1);


bool decode(std::uint32_t dw0, std::uint32_t dw1, Message& m,
            bool data_present = false);


class Proxy {
public:
    struct Delivery {
        int     target = -1;
        Message msg;
    };

    bool submit(int target, const Message& m);
    bool pop(Delivery& d);

    std::size_t pending() const { return q_.size(); }
    std::uint64_t submitted()  const { return submitted_; }
    std::uint64_t delivered()  const { return delivered_; }
    std::uint64_t rejected()   const { return rejected_; }

private:
    std::deque<Delivery> q_;
    std::uint64_t submitted_ = 0;
    std::uint64_t delivered_ = 0;
    std::uint64_t rejected_  = 0;
};


struct Beat {
    std::uint32_t flit = 0;
    bool          last = false;
};

std::uint64_t pack_beat(const Beat& b);
Beat          unpack_beat(std::uint64_t v);


struct AxpControl {
    std::uint8_t  tstrb = 0;
    bool          tlast = false;
    std::uint8_t  tsize = 0;
    std::uint8_t  tri   = 0;
    std::uint16_t tid   = 0;
};

std::uint32_t pack_axp(const AxpControl& c);
AxpControl    unpack_axp(std::uint32_t w);

std::uint8_t  axp_keep(std::uint8_t tsize, unsigned tdb_bytes = 8);


class BeatReassembler {
public:
    void reset();


    bool push(std::uint64_t payload33);

    bool has_control() const { return has_control_; }
    const AxpControl& control() const { return control_; }
    const std::vector<std::uint32_t>& data() const { return data_; }
    std::size_t data_beats() const { return data_.size(); }


    std::uint64_t word(std::size_t index) const;

    std::uint64_t completed() const { return completed_; }
    std::uint64_t overflow()  const { return overflow_; }

private:


    static constexpr unsigned kMaxDataBeats = 9;
    AxpControl control_;
    bool has_control_ = false;
    bool finished_ = false;
    std::vector<std::uint32_t> data_;
    std::uint64_t completed_ = 0;
    std::uint64_t overflow_  = 0;
};

}
