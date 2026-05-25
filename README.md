# rxs

A high-performance, lightweight [XMRig](https://github.com/xmrig/xmrig) fork focused exclusively on RandomX. By dropping support for other algorithms, rxs is able to achieve better performance and a smaller binary than XMRig. For the most part, rxs should be a drop in replacement for XMRig.

For KawPow, CryptoNight, or GhostRider support, use XMRig instead.

## Supported platforms

- **OS:** Linux, Windows, macOS, Android, BSDs
- **CPU:** x86/x64, ARMv7, ARMv8, RISC-V

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Release builds enable platform-aware compiler optimizations. `WITH_LTO=ON` is enabled by default and turns on interprocedural optimization when the compiler supports it. Windows and macOS use the portable RandomX JIT path by default so they build cleanly with native toolchains.

## Donations

rxs does not mine donations and never will. All of your processing power goes toward mining for yourself.
No donation wallet is shipped, printed, suggested, whispered, juggled, or hidden under the floorboards.
