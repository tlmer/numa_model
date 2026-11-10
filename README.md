# numa_model

`numa_model` is a standalone C++17 transaction-level model of a multi-node
NUMA/C2C fabric. It provides link/message codecs, node and home-agent models,
cache-coherence and DVM handling, interrupt-controller models, migration and
SLIT calibration utilities. It is intended for pre-silicon software bring-up
and for driving a device under test without an operating system.

The core is delivered as a prebuilt static library plus public headers. Model
implementation sources are not part of this repository.

## Scope

- Multi-node memory routing with per-line ownership and migration overrides.
- CML_SMP low-version message path and CHI C2C Issue-B style model path.
- Read, write, partial write, atomics, CMO, DVM, snoop, retry and credit models.
- GICv3-compatible distributor, redistributor, ITS/vPE and LPI models.
- Public testcase registry with repeat/all/coverage, JSON/JUnit export and
  documented N/A gaps (`include/tc/numa_tc.h`; console `tc` command).
- ACPI SRAT/SLIT/MADT/PPTT/IORT generation utilities in the licensed package.
- Plain C++17 API; no simulator, kernel or operating-system dependency.

## Repository layout

```
include/            public API headers (including tc/numa_tc.h)
lib/libnumacml.a    prebuilt model library (x86-64, GCC 8 C++17 ABI)
example/            small API example
console/            interactive console built on the public API
docs/USER_GUIDE.md  integration and API guide
build_example.sh    independent build of example and console
```

## Quick build

```
g++ -std=c++17 -Iinclude example/numa_example.cpp lib/libnumacml.a \
    -lpthread -ldl -o numa_example
./numa_example
```

The example and console only need the headers and static library. No source
tree, simulator or kernel headers are required.

## API version

The headers expose `NUMA_MODEL_API_VERSION_MAJOR/MINOR/PATCH`, a combined
integer and `numa::kAbiVersion`. This release is API 1.33.0 / ABI 1.

## Boundaries

- The model is transaction-level. It does not include RTL, PHY, board files,
  customer IP, a QEMU/SystemC shell, or a Linux driver.
- The prebuilt library targets x86-64 Linux and the GCC 8 C++17 ABI. Rebuild
  for a different toolchain or ABI as needed under the commercial license.
- GICv4.1 virtual interrupt execution, real ITS/VPT memory tables and customer
  GIC700 integration are outside this standalone model.
- Full system-level soak results and full regression are not part of this
  repository; delivered test binaries are described in the licensed package.

## License

Commercial/proprietary. See `LICENSE_NOTICE.md`. No open-source license is
granted by this repository.
