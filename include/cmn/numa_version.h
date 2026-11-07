#pragma once

#define NUMA_MODEL_API_VERSION_MAJOR 1
#define NUMA_MODEL_API_VERSION_MINOR 31
#define NUMA_MODEL_API_VERSION_PATCH 0
#define NUMA_MODEL_API_VERSION \
    (NUMA_MODEL_API_VERSION_MAJOR * 10000 + \
     NUMA_MODEL_API_VERSION_MINOR * 100 + \
     NUMA_MODEL_API_VERSION_PATCH)

#define NUMA_MODEL_ABI_VERSION 1

namespace numa {

inline constexpr unsigned kApiVersionMajor = NUMA_MODEL_API_VERSION_MAJOR;
inline constexpr unsigned kApiVersionMinor = NUMA_MODEL_API_VERSION_MINOR;
inline constexpr unsigned kApiVersionPatch = NUMA_MODEL_API_VERSION_PATCH;
inline constexpr unsigned kApiVersion = NUMA_MODEL_API_VERSION;
inline constexpr unsigned kAbiVersion = NUMA_MODEL_ABI_VERSION;

}
