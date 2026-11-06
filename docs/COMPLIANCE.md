# Publication checklist notes

This file records applicability against the onetest customer checklist.
"Licensed package" means the private delivery archive, not this repository.

| Checklist item | Status in this repository |
|---|---|
| 1. AXI4 master/slave TLM | N/A: no AXI4 port in this model. |
| 2. AXI ID/outstanding configurable | N/A: no AXI4 port. |
| 3. IRQ width selected by DT | N/A: no device-tree binding. |
| 4. Bandwidth/latency/throughput statistics | PARTIAL: performance and soak cases are in the licensed test set; the public subset exposes functional APIs only. |
| 6. ioctl testcase channel | N/A: no kernel/UAPI surface. Test binaries are in the licensed package. |
| 7. Independent build, no OS | PASS: example/console build from `include/` + `lib/`; dependencies are pthread and libdl only. |
| 8. Host/root IP coverage | N/A: not a Linux host-controller model. |
| 9. User guide + example | PASS: `docs/USER_GUIDE.md`, `example/numa_example.cpp`. |
| 10. Console | PASS: `console/numa_console.cpp`, `help/info/read/write/migrate/slit/quit`. |
| 11. Linux replay | N/A: no Linux driver counterpart. |
| 12. Linux proxy with no OS | N/A: the model is a C++ transaction model, not a Linux driver proxy. |
| 13. RTL bridge / VEO template | N/A: no AXI bridge in this subset; CXS adapter RTL is not published. |
| 14. Binary core release | PASS: public headers + `lib/libnumacml.a`; model implementation sources are not published. |
| 15. Original Linux driver prints | N/A: no Linux driver counterpart. |
| 16. English only | PASS: public text is English; public headers are comment-stripped; CJK text and CJK binary string scans are zero. |
| 17. DTS template | N/A: topology is C++/YAML/ACPI based. |
| 18. Linux utilities | N/A: no lspci/setpci equivalent. |
| 19. Remove vendor feature words | PASS: public text has no vendor names; protocol names are retained. |
| 20. Packed RTL signal groups | N/A: no RTL is published. |
| 21. SystemC or plain C++17 | PASS: plain C++17 by design; no SystemC dependency. |
| 22. Linux 6.18 driver alignment | N/A: no Linux driver in this subset. |

## Unified test clauses

| Clause | Status |
|---|---|
| T1 DTS template/load entry | N/A: no device tree. |
| T2 testcase registry + independent ioctl | Not implemented in this public subset; test binaries are in the licensed package. |
| T3 MODEL_TRACE | PARTIAL: example/console print English summaries; no `MODEL_TRACE` environment switch. |
| T4 coverage output | Not implemented in this public subset. |
| T5 run-all | N/A: no testcase registry in this subset. |
| T6 repeat/iterations | N/A: no testcase registry. |
| T7 user-defined tests | N/A: no testcase registry. |
| T8 console | PASS. |
| T9 reuse Linux user-space components | N/A: this is a fabric model, not a Linux driver client. |
| T10 shader delivery | N/A. |

## Verification performed for this release

- `sh verify.sh`: builds example + console from `include/` + `lib/`; example
  prints `NUMA_EXAMPLE: PASS`.
- 34 public headers compile in one translation unit.
- Public text CJK scan: 0.
- Binary CJK string scan: 0.
- Absolute path leak scan in the archive: 0.
- Remote blob SHA verification: 43/43 match the release tree.
