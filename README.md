# rxs

rxs is a modified version of the popular miner [XMRig](https://github.com/xmrig/xmrig) with the goal of delivering the highest possible performance for the user. It specifically focuses only on RandomX. Doing this means it can also offer a much smaller binary size. If you would like to use other mining algorithms, such as KawPow, CryptoNight, and GhostRider, use xmrig. If you're only planning to do RandomX mining, then rxs should be the same or faster than xmrig.

## Mining backends
- **CPU** (x86/x64/ARMv7/ARMv8/RISC-V)
- **OpenCL** for AMD GPUs.
- **CUDA** for NVIDIA GPUs via an external [CUDA plugin](https://github.com/xmrig/xmrig-cuda).
