/* SPDX-License-Identifier: MIT */
#ifndef NATIVE_ADRENO_CHIP_H
#define NATIVE_ADRENO_CHIP_H
#include <stdint.h>

/* The register facts that differ between the GPU generations the generic components drive: dword offsets in the GPU
 * register space, from the pinned registers/adreno/a6xx.xml (each the entry whose variants cover the generation) and,
 * for A8XX, checked against Qualcomm's gen8_reg.h. Ring base/control/rptr-address/rptr/wptr, the RBBM interrupt bits and
 * every PM4 opcode and event the ring sends are the same on both and stay in the components. A7XX is the default of
 * every component, so a caller that never names a chip gets exactly the A740 behaviour. The tables are header-only
 * (inline variables), so no build that lists NativeAdreno's sources by name needs a new file. */
struct NativeAdrenoChipRegs {
    uint32_t generation;              /* 7 or 8 */
    uint32_t cp_sqe_instr_base;       /* 64-bit CP_SQE_INSTR_BASE */
    uint32_t cp_scratch2;             /* the fence scratch: CP_SCRATCH(2), CP_SCRATCH_GLOBAL(2) on A8XX */
    uint32_t perfctr_sram_init_cmd;   /* RBBM_PERFCTR_SRAM_INIT_CMD */
    uint32_t perfctr_sram_init_status;/* RBBM_PERFCTR_SRAM_INIT_STATUS */
    uint32_t slice_perfctr_sram_init_cmd;   /* RBBM_SLICE_PERFCTR_SRAM_INIT_CMD (A8XX), 0 where absent */
    uint32_t rbbm_int_status;         /* RBBM_INT_0_STATUS */
    uint32_t rbbm_int_clear;          /* RBBM_INT_CLEAR_CMD */
    uint32_t rbbm_int_mask;           /* RBBM_INT_0_MASK */
    uint32_t cp_int_status;           /* CP_INTERRUPT_STATUS, CP_INTERRUPT_STATUS_GLOBAL on A8XX */
    uint32_t cp_error_bits;           /* the error bits of cp_int_status */
    uint32_t cp_protect_status;       /* CP_PROTECT_STATUS, CP_PROTECT_STATUS_PIPE (current aperture) on A8XX */
    uint32_t cp_always_on_counter;    /* 64-bit CP_ALWAYS_ON_COUNTER */
};

/* A7XX (A740): the offsets the components always used. CP_INTERRUPT_STATUS error bits are the A6XX_CP_INT opcode, ucode,
 * HW fault, protection, AHB, VSD parity and illegal-instruction bits with their A7XX LPAC and BV replicas (0x3fff7). */
inline constexpr NativeAdrenoChipRegs NativeAdrenoChipA7xx = {
    7,
    0x0830,     /* CP_SQE_INSTR_BASE */
    0x0885,     /* CP_SCRATCH(2) */
    0x050e,     /* RBBM_PERFCTR_SRAM_INIT_CMD */
    0x050f,     /* RBBM_PERFCTR_SRAM_INIT_STATUS */
    0,
    0x0201,     /* RBBM_INT_0_STATUS */
    0x0037,     /* RBBM_INT_CLEAR_CMD */
    0x0038,     /* RBBM_INT_0_MASK */
    0x0823,     /* CP_INTERRUPT_STATUS */
    0x3fff7,
    0x0824,     /* CP_PROTECT_STATUS */
    0x0980,     /* CP_ALWAYS_ON_COUNTER */
};

/* A8XX (A840, gen8): the variants="A8XX-" entries. CP_INTERRUPT_STATUS_GLOBAL is the A8XX_CP_GLOBAL_INT_MASK bitset: a
 * hardware and a software fault bit per pipe (BR, BV, LPAC, AQE0, AQE1, DDE BR, DDE BV), all errors (0x007f007f, the
 * value a8xx_gpu.c hw_init enables). */
inline constexpr NativeAdrenoChipRegs NativeAdrenoChipA8xx = {
    8,
    0x0816,     /* CP_SQE_INSTR_BASE */
    0x082c,     /* CP_SCRATCH_GLOBAL(2) */
    0x0449,     /* RBBM_PERFCTR_SRAM_INIT_CMD */
    0x019f,     /* RBBM_PERFCTR_SRAM_INIT_STATUS */
    0x05e8,     /* RBBM_SLICE_PERFCTR_SRAM_INIT_CMD */
    0x006a,     /* RBBM_INT_0_STATUS */
    0x0061,     /* RBBM_INT_CLEAR_CMD */
    0x0062,     /* RBBM_INT_0_MASK */
    0x08b5,     /* CP_INTERRUPT_STATUS_GLOBAL */
    0x007f007f,
    0x084f,     /* CP_PROTECT_STATUS_PIPE */
    0x08e7,     /* CP_ALWAYS_ON_COUNTER */
};
#endif
