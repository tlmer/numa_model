#pragma once

#include "cml/cml_fields.h"

#include <array>
#include <cstdint>

namespace numa::cml {


std::uint8_t lcrd_encode(unsigned count);
unsigned     lcrd_decode(std::uint8_t code);


class CreditPool {
public:
    explicit CreditPool(unsigned init = 0, unsigned max = 0xFFFFFFFFu)
        : count_(init), max_(max) {}


    bool grant(unsigned n = 1);

    bool consume(unsigned n = 1);

    unsigned count() const { return count_; }
    unsigned max()   const { return max_; }
    std::uint64_t granted()   const { return granted_; }
    std::uint64_t consumed()  const { return consumed_; }
    std::uint64_t rejected()  const { return rejected_; }

private:
    unsigned      count_;
    unsigned      max_;
    std::uint64_t granted_  = 0;
    std::uint64_t consumed_ = 0;
    std::uint64_t rejected_ = 0;
};


class CreditSet {
public:
    CreditSet(unsigned l_init = 256, unsigned p_init = 0);

    CreditPool& l(CrdClass c) { return l_[static_cast<unsigned>(c)]; }
    CreditPool& p(CrdClass c) { return p_[static_cast<unsigned>(c)]; }


    bool can_send(CrdClass c) const {
        const auto i = static_cast<unsigned>(c);
        return l_[i].count() > 0 && p_[i].count() > 0;
    }
    bool send(CrdClass c);
    bool grant_p(CrdClass c, unsigned n = 1);


    bool grant_p_bundle(unsigned req, unsigned dat, unsigned snp,
                        unsigned misc);
    std::uint64_t rejected_bundle() const { return rejected_bundle_; }

private:
    std::array<CreditPool, static_cast<unsigned>(CrdClass::COUNT)> l_;
    std::array<CreditPool, static_cast<unsigned>(CrdClass::COUNT)> p_;
    std::uint64_t rejected_bundle_ = 0;
};


class SharedCreditBank {
public:
    static constexpr unsigned kClasses =
        static_cast<unsigned>(CrdClass::COUNT);

    explicit SharedCreditBank(unsigned dedicated_each = 1,
                              unsigned shared_init = 0,
                              unsigned shared_max = 0xFFFFFFFFu);


    bool consume(CrdClass c, unsigned n = 1);

    bool give_back(CrdClass c, unsigned n = 1);

    unsigned dedicated(CrdClass c) const;
    unsigned shared() const { return shared_; }
    unsigned shared_max() const { return shared_max_; }
    unsigned outstanding(CrdClass c) const;
    unsigned total_outstanding() const;

    std::uint64_t consumed()          const { return consumed_; }
    std::uint64_t returned()          const { return returned_; }
    std::uint64_t rejected_consume()  const { return rejected_consume_; }
    std::uint64_t rejected_return()   const { return rejected_return_; }

private:
    struct ClassState {
        unsigned dedicated    = 0;
        unsigned out_dedicated = 0;
        unsigned out_shared    = 0;
    };

    std::array<ClassState, kClasses> st_;
    unsigned      shared_     = 0;
    unsigned      shared_max_ = 0;
    std::uint64_t consumed_   = 0;
    std::uint64_t returned_   = 0;
    std::uint64_t rejected_consume_ = 0;
    std::uint64_t rejected_return_  = 0;
};

}
