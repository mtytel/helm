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
namespace dsp
{

/**
    Classes for state variable filter processing.
*/
namespace StateVariableFilter
{
    template <typename NumericType>
    struct Parameters;

    /**
        An IIR filter that can perform low, band and high-pass filtering on an audio
        signal, with 12 dB of attenuation / octave, using a TPT structure, designed
        for fast modulation (see Vadim Zavalishin's documentation about TPT
        structures for more information). Its behaviour is based on the analog
        state variable filter circuit.

        Note: The bandpass here is not the one in the RBJ CookBook, its gain can be
        higher than 0 dB. For the classic 0 dB bandpass, we need to multiply the
        result with R2

        @tags{DSP}
    */
    template <typename SampleType>
    class Filter
    {
    public:
        //==============================================================================
        /** The NumericType is the underlying primitive type used by the SampleType (which
            could be either a primitive or vector)
        */
        using NumericType = typename SampleTypeHelpers::ElementType<SampleType>::Type;

        /** A typedef for a ref-counted pointer to the coefficients object */
        using ParametersPtr = typename Parameters<NumericType>::Ptr;

        //==============================================================================
        /** Creates a filter with default parameters. */
        Filter()                               : parameters (new Parameters<NumericType>) { reset(); }

        Filter (ParametersPtr parametersToUse) : parameters (std::move (parametersToUse)) { reset(); }

        /** Creates a copy of another filter. */
        Filter (const Filter&) = default;

        /** Move constructor */
        Filter (Filter&&) = default;

        //==============================================================================
        /** Initialization of the filter */
        void prepare (const ProcessSpec&) noexcept     { reset(); }

        /** Resets the filter's processing pipeline. */
        void reset() noexcept                          { s1 = s2 = SampleType {0}; }

        /** Ensure that the state variables are rounded to zero if the state
            variables are denormals. This is only needed if you are doing
            sample by sample processing.
        */
        void snapToZero() noexcept                     { util::snapToZero (s1); util::snapToZero (s2); }

        //==============================================================================
        /** The parameters of the state variable filter. It's up to the caller to ensure
            that these parameters are modified in a thread-safe way. */
        typename Parameters<NumericType>::Ptr parameters;

        //==============================================================================
        template <typename ProcessContext>
        void process (const ProcessContext& context) noexcept
        {
            static_assert (std::is_same<typename ProcessContext::SampleType, SampleType>::value,
                           "The sample-type of the filter must match the sample-type supplied to this process callback");

            if (context.isBypassed)
                processInternal<true, ProcessContext> (context);
            else
                processInternal<false, ProcessContext> (context);
        }

        /** Processes a single sample, without any locking or checking.
            Use this if you need processing of a single value. */
        SampleType JUCE_VECTOR_CALLTYPE processSample (SampleType sample) noexcept
        {
            switch (parameters->type)
            {
                case Parameters<NumericType>::Type::lowPass:  return processLoop<false, Parameters<NumericType>::Type::lowPass>  (sample, *parameters); break;
                case Parameters<NumericType>::Type::bandPass: return processLoop<false, Parameters<NumericType>::Type::bandPass> (sample, *parameters); break;
                case Parameters<NumericType>::Type::highPass: return processLoop<false, Parameters<NumericType>::Type::highPass> (sample, *parameters); break;
                default: jassertfalse;
            }

            return SampleType{0};
        }

    private:
        //==============================================================================
        template <bool isBypassed, typename Parameters<NumericType>::Type type>
        SampleType JUCE_VECTOR_CALLTYPE processLoop (SampleType sample, Parameters<NumericType>& state) noexcept
        {
            y[2] = (sample - s1 * state.R2 - s1 * state.g - s2) * state.h;

            y[1] = y[2] * state.g + s1;
            s1   = y[2] * state.g + y[1];

            y[0] = y[1] * state.g + s2;
            s2   = y[1] * state.g + y[0];

            return isBypassed ? sample : y[static_cast<size_t> (type)];
        }

        template <bool isBypassed, typename Parameters<NumericType>::Type type>
        void processBlock (const SampleType* input, SampleType* output, size_t n) noexcept
        {
            auto state = *parameters;

            for (size_t i = 0 ; i < n; ++i)
                output[i] = processLoop<isBypassed, type> (input[i], state);

            snapToZero();
            *parameters = state;
        }

        template <bool isBypassed, typename ProcessContext>
        void processInternal (const ProcessContext& context) noexcept
        {
            auto&& inputBlock  = context.getInputBlock();
            auto&& outputBlock = context.getOutputBlock();

            // This class can only process mono signals. Use the ProcessorDuplicator class
            // to apply this filter on a multi-channel audio stream.
            jassert (inputBlock.getNumChannels()  == 1);
            jassert (outputBlock.getNumChannels() == 1);

            auto n = inputBlock.getNumSamples();
            auto* src = inputBlock .getChannelPointer (0);
            auto* dst = outputBlock.getChannelPointer (0);

            switch (parameters->type)
            {
                case Parameters<NumericType>::Type::lowPass:  processBlock<isBypassed, Parameters<NumericType>::Type::lowPass>  (src, dst, n); break;
                case Parameters<NumericType>::Type::bandPass: processBlock<isBypassed, Parameters<NumericType>::Type::bandPass> (src, dst, n); break;
                case Parameters<NumericType>::Type::highPass: processBlock<isBypassed, Parameters<NumericType>::Type::highPass> (src, dst, n); break;
                default: jassertfalse;
            }
        }

        //==============================================================================
        std::array<SampleType, 3> y;
        SampleType s1, s2;

        //==============================================================================
        JUCE_LEAK_DETECTOR (Filter)
    };

    //==============================================================================
    /**
        Structure used for the state variable filter parameters.

        @tags{DSP}
    */
    template <typename NumericType>
    struct Parameters  : public ProcessorState
    {
        //==============================================================================
        enum class Type
        {
            lowPass,
            bandPass,
            highPass
        };

        //==============================================================================
        /** The type of the IIR filter */
        Type type = Type::lowPass;

        /** Sets the cutoff frequency and resonance of the IIR filter.

            Note: The bandwidth of the resonance increases with the value of the
            parameter. To have a standard 12 dB/octave filter, the value must be set
            at 1 / sqrt(2).
        */
        void setCutOffFrequency (double sampleRate, NumericType frequency,
                                 NumericType resonance = static_cast<NumericType> (1.0 / MathConstants<double>::sqrt2)) noexcept
        {
            jassert (sampleRate > 0);
            jassert (resonance > NumericType (0));
            jassert (frequency > NumericType (0) && frequency <= NumericType (sampleRate * 0.5));

            g  = static_cast<NumericType> (std::tan (MathConstants<double>::pi * frequency / sampleRate));
            R2 = static_cast<NumericType> (1.0 / resonance);
            h  = static_cast<NumericType> (1.0 / (1.0 + R2 * g + g * g));
        }

        //==============================================================================
        /** The Coefficients structure is ref-counted, so this is a handy type that can be used
            as a pointer to one.
        */
        using Ptr = ReferenceCountedObjectPtr<Parameters>;

        //==============================================================================
        Parameters() = default;
        Parameters (const Parameters& o) : g (o.g), R2 (o.R2), h (o.h) {}
        Parameters& operator= (const Parameters& o) noexcept    { g = o.g; R2 = o.R2; h = o.h; return *this; }

        //==============================================================================
        NumericType g   = static_cast<NumericType> (std::tan (MathConstants<double>::pi * 200.0 / 44100.0));
        NumericType R2  = static_cast<NumericType> (MathConstants<double>::sqrt2);
        NumericType h   = static_cast<NumericType> (1.0 / (1.0 + R2 * g + g * g));
    };
}

} // namespace dsp
} // namespace juce
