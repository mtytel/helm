/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_117127501_INCLUDED
#define BINARYDATA_H_117127501_INCLUDED

namespace BinaryData
{
    extern const char*   twytch_logo_16_1x_png;
    const int            twytch_logo_16_1x_pngSize = 1258;

    extern const char*   twytch_logo_16_2x_png;
    const int            twytch_logo_16_2x_pngSize = 2512;

    extern const char*   twytch_logo_32_1x_png;
    const int            twytch_logo_32_1x_pngSize = 2512;

    extern const char*   twytch_logo_32_2x_png;
    const int            twytch_logo_32_2x_pngSize = 5595;

    extern const char*   twytch_logo_128_1x_png;
    const int            twytch_logo_128_1x_pngSize = 11982;

    extern const char*   twytch_logo_128_2x_png;
    const int            twytch_logo_128_2x_pngSize = 27160;

    extern const char*   twytch_logo_256_1x_png;
    const int            twytch_logo_256_1x_pngSize = 27160;

    extern const char*   twytch_logo_256_2x_png;
    const int            twytch_logo_256_2x_pngSize = 66399;

    extern const char*   twytch_logo_512_1x_png;
    const int            twytch_logo_512_1x_pngSize = 66352;

    extern const char*   twytch_logo_512_2x_png;
    const int            twytch_logo_512_2x_pngSize = 140557;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 10;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
