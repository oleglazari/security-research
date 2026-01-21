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
