# Publication checklist notes

| Item | Status |
|---|---|
| AXI4 TLM / configurable ID / IRQ per DT | N/A: the model has no AXI4 port and is not a Linux device driver. |
| ioctl testcase channel | N/A: no kernel/UAPI surface. Test binaries are in the licensed package. |
| User guide + example | Included (`docs/USER_GUIDE.md`, `example/`). |
| Console | Included (`console/`). |
| Independent build | Example and console build from `include/` + `lib/` only. |
| Binary core | Public headers + `lib/libnumacml.a`; model sources are not published. |
| Linux driver print replay | N/A: no Linux driver counterpart. |
| English-only delivery | All repository text and public headers are English; no CJK. |
| DTS template | N/A: topology is C++/YAML/ACPI based, not device tree. |
| Linux utilities | N/A: no lspci/setpci equivalent. |
| SystemC | N/A for this subset: plain C++17 by design. |
