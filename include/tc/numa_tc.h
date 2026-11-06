#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace numa::tc {


inline constexpr std::size_t kMaxTestCases = 32;


inline constexpr std::size_t kMaxNameLength = 64;


inline constexpr unsigned kAllTestCases = 0xFFFFFFFFu;


using TestFunction = std::function<bool()>;


struct TestCaseInfo {
    bool        valid = false;
    unsigned    id = 0;
    std::string name;
    std::string description;
};


struct RunResult {
    unsigned    id = 0;
    std::string name;
    unsigned    iterations = 0;
    unsigned    ran = 0;
    unsigned    passed = 0;
    unsigned    failed = 0;

    std::vector<unsigned> failed_iterations;


    bool        known = false;

    std::string error;

    bool ok() const {
        return known && failed == 0 && ran == iterations;
    }
};


struct RunAllResult {
    unsigned cases = 0;
    unsigned iterations = 0;
    unsigned ran = 0;
    unsigned passed = 0;
    unsigned failed = 0;
    std::vector<RunResult> results;

    bool ok() const {
        return cases != 0 && failed == 0;
    }
};


struct CoverageItem {
    unsigned    id = 0;
    std::string name;
    std::string description;
    bool        implemented = true;
    bool        tested = false;
    unsigned    runs = 0;
    unsigned    passes = 0;
    unsigned    failures = 0;


    std::string reason;
};


struct CoverageReport {
    unsigned total = 0;
    unsigned tested = 0;
    unsigned untested = 0;
    unsigned gaps = 0;
    std::vector<CoverageItem> items;


    std::string text() const;
};

class Registry {
public:


    bool add(unsigned id, std::string name, std::string description,
             TestFunction fn, std::string* error = nullptr);


    bool add_gap(std::string name, std::string reason,
                 std::string* error = nullptr);

    std::size_t count() const { return entries_.size(); }
    bool contains(unsigned id) const;


    TestCaseInfo info(std::size_t index) const;


    RunResult run(unsigned id, unsigned iterations = 1);


    RunAllResult run_all(unsigned iterations = 1);


    CoverageReport coverage() const;
    std::string coverage_text() const { return coverage().text(); }


    void clear();

private:
    struct Entry {
        unsigned     id = 0;
        std::string  name;
        std::string  description;
        TestFunction fn;
        unsigned     runs = 0;
        unsigned     passes = 0;
        unsigned     failures = 0;
    };

    struct Gap {
        std::string name;
        std::string reason;
    };

    Entry*       find(unsigned id);
    const Entry* find(unsigned id) const;

    std::vector<Entry> entries_;
    std::vector<Gap>   gaps_;
};


Registry make_builtin_registry();


bool tc_api_version();
bool tc_system_route_rw();
bool tc_tlb_invalidate();
bool tc_page_migration();
bool tc_slit_calibration();
bool tc_c2c_request_response();

}
