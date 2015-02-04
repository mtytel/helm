/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== Makefile.am ==================
static const unsigned char temp_binary_data_0[] =
"noinst_LIBRARIES = libmopo.a\n"
"libmopo_a_SOURCES = alias.cpp \\\n"
"\t\t\t\t\t\t\t\t\t\talias.h \\\n"
"\t\t\t\t\t\t\t\t\t\tarpeggiator.cpp \\\n"
"\t\t\t\t\t\t\t\t\t\tarpeggiator.h \\\n"
"\t\t\t\t\t\t\t\t\t\tbit_crush.cpp \\\n"
"\t\t\t\t\t\t\t\t\t\tbit_crush.h \\\n"
"\t\t\t\t\t\t\t\t\t\tdelay.cpp \\\n"
"                    delay.h \\\n"
"                    distortion.cpp \\\n"
"                    distortion.h \\\n"
"                    envelope.cpp \\\n"
"                    envelope.h \\\n"
"                    feedback.cpp \\\n"
"                    feedback.h \\\n"
"                    filter.cpp \\\n"
"                    filter.h \\\n"
"                    linear_slope.cpp \\\n"
"                    linear_slope.h \\\n"
"                    magnitude_lookup.cpp \\\n"
"                    magnitude_lookup.h \\\n"
"                    memory.h \\\n"
"                    midi_lookup.cpp \\\n"
"                    midi_lookup.h \\\n"
"                    mono_panner.cpp \\\n"
"                    mono_panner.h \\\n"
"                    mopo.h \\\n"
"                    operators.cpp \\\n"
"                    operators.h \\\n"
"                    oscillator.cpp \\\n"
"                    oscillator.h \\\n"
"                    phaser.cpp \\\n"
"                    phaser.h \\\n"
"                    processor.cpp \\\n"
"                    processor.h \\\n"
"                    processor_router.cpp \\\n"
"                    processor_router.h \\\n"
"                    resonance_lookup.cpp \\\n"
"                    resonance_lookup.h \\\n"
"                    smooth_filter.cpp \\\n"
"                    smooth_filter.h \\\n"
"                    smooth_value.cpp \\\n"
"                    smooth_value.h \\\n"
"                    step_generator.cpp \\\n"
"                    step_generator.h \\\n"
"                    tick_router.h \\\n"
"                    trigger_operators.cpp \\\n"
"                    trigger_operators.h \\\n"
"                    value.cpp \\\n"
"                    value.h \\\n"
"                    utils.h \\\n"
"                    voice_handler.cpp \\\n"
"                    voice_handler.h \\\n"
"                    wave.cpp \\\n"
"                    wave.h\n";

const char* Makefile_am = (const char*) temp_binary_data_0;


const char* getNamedResource (const char*, int&) throw();
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes) throw()
{
    unsigned int hash = 0;
    if (resourceNameUTF8 != 0)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x1a187401:  numBytes = 1905; return Makefile_am;
        default: break;
    }

    numBytes = 0;
    return 0;
}

const char* namedResourceList[] =
{
    "Makefile_am"
};

}
