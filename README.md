# NativeAdreno

Native XNU Adreno 740 driver. Owns GMU startup, the CP ring, BOs, native 4 KiB SMMU contexts, fences and cache maintenance.
The ring, interrupt decoder, timer and CP start also drive A8XX (Adreno 840) when given `NativeAdrenoChipA8xx` from
`NativeAdrenoChip.h`; A7XX stays every component's default.

Current state: GPU commands and the scanout copy run on sheng hardware. Shader compilation is slow and there is no complete precompiled UI pack.

Linked into XNU by `unixnu build kernel`.
