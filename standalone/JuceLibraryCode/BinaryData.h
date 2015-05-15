/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_117127501_INCLUDED
#define BINARYDATA_H_117127501_INCLUDED

namespace BinaryData
{
    extern const char*   twytch_logo_16_1x_png;
    const int            twytch_logo_16_1x_pngSize = 1180;

    extern const char*   twytch_logo_16_2x_png;
    const int            twytch_logo_16_2x_pngSize = 2522;

    extern const char*   twytch_logo_32_1x_png;
    const int            twytch_logo_32_1x_pngSize = 2522;

    extern const char*   twytch_logo_32_2x_png;
    const int            twytch_logo_32_2x_pngSize = 5399;

    extern const char*   twytch_logo_128_1x_png;
    const int            twytch_logo_128_1x_pngSize = 11315;

    extern const char*   twytch_logo_128_2x_png;
    const int            twytch_logo_128_2x_pngSize = 24335;

    extern const char*   twytch_logo_256_1x_png;
    const int            twytch_logo_256_1x_pngSize = 24335;

    extern const char*   twytch_logo_256_2x_png;
    const int            twytch_logo_256_2x_pngSize = 51514;

    extern const char*   twytch_logo_512_1x_png;
    const int            twytch_logo_512_1x_pngSize = 51514;

    extern const char*   twytch_logo_512_2x_png;
    const int            twytch_logo_512_2x_pngSize = 116642;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 10;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
