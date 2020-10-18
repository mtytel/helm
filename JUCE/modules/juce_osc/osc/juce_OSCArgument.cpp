/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

OSCArgument::OSCArgument (int32 v)              : type (OSCTypes::int32),   intValue (v) {}
OSCArgument::OSCArgument (float v)              : type (OSCTypes::float32), floatValue (v) {}
OSCArgument::OSCArgument (const String& s)      : type (OSCTypes::string),  stringValue (s) {}
OSCArgument::OSCArgument (MemoryBlock b)        : type (OSCTypes::blob),    blob (std::move (b)) {}
OSCArgument::OSCArgument (OSCColour c)          : type (OSCTypes::colour),  intValue ((int32) c.toInt32()) {}

//==============================================================================
String OSCArgument::getString() const noexcept
{
    if (isString())
        return stringValue;

    jassertfalse; // you must check the type of an argument before attempting to get its value!
    return {};
}

int32 OSCArgument::getInt32() const noexcept
{
    if (isInt32())
        return intValue;

    jassertfalse; // you must check the type of an argument before attempting to get its value!
    return 0;
}

float OSCArgument::getFloat32() const noexcept
{
    if (isFloat32())
        return floatValue;

    jassertfalse; // you must check the type of an argument before attempting to get its value!
    return 0.0f;
}

const MemoryBlock& OSCArgument::getBlob() const noexcept
{
    // you must check the type of an argument before attempting to get its value!
    jassert (isBlob());

    return blob;
}

OSCColour OSCArgument::getColour() const noexcept
{
    if (isColour())
        return OSCColour::fromInt32 ((uint32) intValue);

    jassertfalse; // you must check the type of an argument before attempting to get its value!
    return { 0, 0, 0, 0 };
}


//==============================================================================
//==============================================================================
#if JUCE_UNIT_TESTS

class OSCArgumentTests  : public UnitTest
{
public:
    OSCArgumentTests()
         : UnitTest ("OSCArgument class", UnitTestCategories::osc)
    {}


    MemoryBlock getMemoryBlockWithRandomData (size_t numBytes)
    {
        MemoryBlock block (numBytes);

        Random rng = getRandom();

        for (size_t i = 0; i < numBytes; ++i)
            block[i] = (char) rng.nextInt (256);

        return block;
    }

    void runTest()
    {
        runTestInitialisation();
    }

    void runTestInitialisation()
    {
        beginTest ("Int32");
        {
            int value = 123456789;

            OSCArgument arg (value);

            expect (arg.getType() == OSCTypes::int32);
            expect (arg.isInt32());
            expect (! arg.isFloat32());
            expect (! arg.isString());
            expect (! arg.isBlob());
            expect (! arg.isColour());

            expect (arg.getInt32() == value);
        }

        beginTest ("Float32");
        {
            float value = 12345.6789f;

            OSCArgument arg (value);

            expect (arg.getType() == OSCTypes::float32);
            expect (! arg.isInt32());
            expect (arg.isFloat32());
            expect (! arg.isString());
            expect (! arg.isBlob());
            expect (! arg.isColour());

            expect (arg.getFloat32() == value);
        }

        beginTest ("String");
        {
            String value = "Hello, World!";
            OSCArgument arg (value);

            expect (arg.getType() == OSCTypes::string);
            expect (! arg.isInt32());
            expect (! arg.isFloat32());
            expect (arg.isString());
            expect (! arg.isBlob());
            expect (! arg.isColour());

            expect (arg.getString() == value);
        }

        beginTest ("String (from C string)");
        {
            OSCArgument arg ("Hello, World!");

            expect (arg.getType() == OSCTypes::string);
            expect (! arg.isInt32());
            expect (! arg.isFloat32());
            expect (arg.isString());
            expect (! arg.isBlob());
            expect (! arg.isColour());

            expect (arg.getString() == "Hello, World!");
        }

        beginTest ("Blob");
        {
            auto blob = getMemoryBlockWithRandomData (413);
            OSCArgument arg (blob);

            expect (arg.getType() == OSCTypes::blob);
            expect (! arg.isInt32());
            expect (! arg.isFloat32());
            expect (! arg.isString());
            expect (arg.isBlob());
            expect (! arg.isColour());

            expect (arg.getBlob() == blob);
        }

        beginTest ("Colour");
        {
            Random rng = getRandom();

            for (int i = 100; --i >= 0;)
            {
                OSCColour col = { (uint8) rng.nextInt (256),
                                  (uint8) rng.nextInt (256),
                                  (uint8) rng.nextInt (256),
                                  (uint8) rng.nextInt (256) };

                OSCArgument arg (col);

                expect (arg.getType() == OSCTypes::colour);
                expect (! arg.isInt32());
                expect (! arg.isFloat32());
                expect (! arg.isString());
                expect (! arg.isBlob());
                expect (arg.isColour());

                expect (arg.getColour().toInt32() == col.toInt32());
            }
        }

        beginTest ("Copy, move and assignment");
        {
            {
                int value = -42;
                OSCArgument arg (value);

                OSCArgument copy = arg;
                expect (copy.getType() == OSCTypes::int32);
                expect (copy.getInt32() == value);

                OSCArgument assignment ("this will be overwritten!");
                assignment = copy;
                expect (assignment.getType() == OSCTypes::int32);
                expect (assignment.getInt32() == value);
           }
           {
                const size_t numBytes = 412;
                MemoryBlock blob = getMemoryBlockWithRandomData (numBytes);
                OSCArgument arg (blob);

                OSCArgument copy = arg;
                expect (copy.getType() == OSCTypes::blob);
                expect (copy.getBlob() == blob);

                OSCArgument assignment ("this will be overwritten!");
                assignment = copy;
                expect (assignment.getType() == OSCTypes::blob);
                expect (assignment.getBlob() == blob);
           }
        }
    }
};

static OSCArgumentTests OSCArgumentUnitTests;

#endif

} // namespace juce
