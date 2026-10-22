/* SPDX-License-Identifier: MIT */
#ifndef NATIVE_ADRENO_TIMER_H
#define NATIVE_ADRENO_TIMER_H
#include <IOKit/IOTypes.h>
#include <stdint.h>
#include "NativeAdrenoChip.h"

/* GPU always-on counter reader for the user-visible timestamp. Two sources are
 * distinguished because their address spaces differ:
 *  - GMU window: byte offsets inside the already bound GMU window. The A740
 *    offset is derived from the pinned a6xx_gmu.xml dword offset 0x1f888 and
 *    the SM8550 DT layout: (0x1f888 << 2) - (gmu 0x03d6a000 - gpu 0x03d00000)
 *    = 0x14220 for the low word and 0x14224 for the high word.
 *  - CP window: dword register indices in the GPU register window, from
 *    a6xx.xml CP_ALWAYS_ON_COUNTER at 0x0980 for A6XX..A7XX and 0x08e7 for
 *    A8XX (the chip table's; the GMU offsets stay A740's).
 * The caller binds exactly one already powered window; this object performs no
 * power, reset or window discovery. Reads retry while the high word changes,
 * so a counter rollover cannot return a torn value; exceeding the retry bound
 * is an error. A failed read reports an I/O error and never fabricates ticks. */
enum NativeAdrenoTimeSource { NativeAdrenoTimeGmu = 0, NativeAdrenoTimeCp = 1 };
struct NativeAdrenoTimeIo {
    void *cookie;
    int (*read32)(void *,uint32_t address,uint32_t *value);
};
class NativeAdrenoTimer final {
    NativeAdrenoTimeIo io_{};
    uint32_t source_=NativeAdrenoTimeGmu;
    bool attached_=false;
    const NativeAdrenoChipRegs *chip_=&NativeAdrenoChipA7xx;
public:
    NativeAdrenoTimer()=default;
    ~NativeAdrenoTimer()=default;
    NativeAdrenoTimer(const NativeAdrenoTimer &)=delete;
    NativeAdrenoTimer &operator=(const NativeAdrenoTimer &)=delete;
    /* Before attach: the generation whose CP_ALWAYS_ON_COUNTER the CP source reads (A7XX unless named). */
    IOReturn useChip(const NativeAdrenoChipRegs *);
    /* Source is a numeric ABI boundary value and is range-checked. */
    IOReturn attach(const NativeAdrenoTimeIo *,uint32_t source);
    IOReturn detach();
    IOReturn read(uint64_t *ticks) const;
    static uint32_t gmuLowOffset();
    static uint32_t gmuHighOffset();
    static uint32_t cpLowRegister();
    static uint32_t cpHighRegister();
};
#endif
