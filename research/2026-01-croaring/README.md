# Vulnerability Disclosure: Integer Underflow in CRoaring `readSafe()`

- **Date:** January 2026
- **Target:** [CRoaring](https://github.com/RoaringBitmap/CRoaring) (v4.5.1)
- **Vulnerability Type:** Integer Underflow -> OOB Read
- **Impact:** Memory disclosure, including heap metadata and adjacent allocations (secrets/pointers).
- **Status:** Fixed in v4.5.2

## Vulnerability Summary
An integer underflow exists in the `readSafe()` parsing loop of `Roaring64Map`. The vulnerability occurs when the portable serialization size (`tz`) of a bitmap entry is larger than the remaining buffer size (`maxbytes`).

## Root Cause Analysis
In `cpp/roaring64map.hh`, the following logic is used to decrement the remaining bytes during a safe read:

```cpp
size_t tz = read_var.getSizeInBytes(true);
buf += tz;
maxbytes -= tz; // <--- Vulnerable Line
```

When tz exceeds maxbytes, the unsigned subtraction wraps around to a value approaching UINT64_MAX. This effectively bypasses all subsequent maxbytes-based bounds checks, allowing for Out-of-Bounds (OOB) memory access during subsequent iterations of the parsing loop.
Exploitation Primitive

The underflow provides a reliable OOB read primitive. During triage, this was demonstrated to facilitate:

    Heap Metadata Leakage: Disclosure of adjacent chunk headers.

    Sensitive Data Disclosure: Retrieval of adjacent heap allocation contents.

    Pointer Leakage: Leakage of libc addresses, enabling ASLR bypass.

## Remediation

The issue was resolved by implementing a boundary check prior to the subtraction operation.

A regression test, safe_test_lazari, was integrated into the CRoaring test suite to ensure the arithmetic safety of the readSafe loop.
