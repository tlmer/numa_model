# numa_model user guide

## 1. What the model is

`numa_model` is a plain C++17 transaction-level model of a multi-node
NUMA/C2C fabric. A node owns a line range, a home agent serves requests, and a
CPU port issues reads, writes and invalidations. Protocol messages are encoded
through the included CML/CHI-C2C codecs. The model has no kernel, simulator or
device-tree dependency.

## 2. Build and link

Requirements: a C++17 compiler, pthread and libdl.

```
g++ -std=c++17 -Iinclude app.cpp lib/libnumacml.a -lpthread -ldl -o app
```

`build_example.sh` builds the example and console from this repository.

## 3. Core objects

- `numa::cmn::NumaSystem`: node container and address router.
  `add_node(base_line, lines)`, `route(line)`, `send()`/`send_to()`, GIC-D
  delivery and GIC MMIO attachment.
- `numa::cmn::CpuPort`: CPU-side port to a node. Supports line read/write,
  byte-enable/partial-line semantics, TLB invalidation hooks and no DMI.
- `numa::cmn::MigrationManager`: page-granular migration and line-owner update.
- `numa::perf::LatencyMatrix` / `SlitCalibrator`: latency matrix to SLIT
  distance bytes with error reporting.
- `numa::gic::Gicv3Cpu`, `GicdGicr`, `Its`, `VpeTable`: interrupt-controller
  and translation-table models.
- `numa::cml`, `numa::proto::chi_c2c`: message codecs and C2C bridge models.

The public headers under `include/` are the authoritative interface. All names
are C++ and no kernel UAPI is exposed.

## 4. Minimal example

The full working example is `example/numa_example.cpp`: it creates two nodes,
writes and reads a line, invalidates a TLB entry, migrates a page, and
calibrates a two-node SLIT matrix. Its final result is `NUMA_EXAMPLE: PASS`.

## 5. Console

`console/numa_console.cpp` is an interactive front end built only from the
public API and library:

```
info
read <line>
write <line> <byte>
migrate <line> <node>
slit
help
quit
```

It is intended for quick bring-up and for checking a customer port without a
Linux guest.

## 6. Configuration

Address ranges, node count and topology are constructed through the C++
API. The licensed package additionally contains ACPI table generation tools
(SRAT/SLIT/MADT/PPTT/IORT) driven by a YAML topology file. A device-tree
template is not used by this standalone model.

## 7. Performance and testcases

The library does not contain a mandatory trace or performance counter API in
this subset. The licensed package includes model test binaries, including
performance and soak cases. The console exposes functional operations only.

## 8. Error handling and limits

- Invalid node indices, out-of-range lines and unsupported widths return false
  or are counted; no silent truncation is intended.
- The library is single-threaded per object; external synchronization is
  required for concurrent callers.
- The prebuilt archive is x86-64 Linux / GCC 8 C++17 ABI. Rebuild or request a
  matching build under the commercial license for other toolchains.

## 9. Integration boundaries

This repository does not include customer RTL, PHY, board files, QEMU or
SystemC shells, or a Linux driver. GICv4.1 virtual interrupt execution, real
ITS/VPT memory tables and customer interrupt-controller integration are not
part of this standalone subset.

## 10. Support

Report API or integration issues with the API version printed by
`numa_example`, the console `info` output and the exact compiler/library
toolchain used.
