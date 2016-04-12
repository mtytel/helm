
namespace juce {

static ThreadLocalValue<AudioProcessor::WrapperType> wrapperTypeBeingCreated;

void JUCE_CALLTYPE AudioProcessor::setTypeOfNextNewPlugin (AudioProcessor::WrapperType type)
{
    wrapperTypeBeingCreated = type;
}

AudioProcessor::AudioProcessor()
    : wrapperType (wrapperTypeBeingCreated.get()),
      playHead (nullptr),
      currentSampleRate (0),
      blockSize (0),
      latencySamples (0),
     #if JUCE_DEBUG
      textRecursionCheck (false),
     #endif
      suspended (false),
      nonRealtime (false),
      processingPrecision (singlePrecision)
{
   #ifdef JucePlugin_PreferredChannelConfigurations
    const short channelConfigs[][2] = { JucePlugin_PreferredChannelConfigurations };
   #else
    const short channelConfigs[][2] = { {2, 2} };
   #endif

 #if ! JucePlugin_IsMidiEffect
   #if ! JucePlugin_IsSynth
    busArrangement.inputBuses.add  (AudioProcessorBus ("Input",  AudioChannelSet::canonicalChannelSet (channelConfigs[0][0])));
   #endif
    busArrangement.outputBuses.add (AudioProcessorBus ("Output", AudioChannelSet::canonicalChannelSet (channelConfigs[0][1])));

  #ifdef JucePlugin_PreferredChannelConfigurations
   #if ! JucePlugin_IsSynth
    AudioProcessor::setPreferredBusArrangement (true,  0, AudioChannelSet::stereo());
   #endif
    AudioProcessor::setPreferredBusArrangement (false, 0, AudioChannelSet::stereo());
  #endif
 #endif
    updateSpeakerFormatStrings();
}

bool AudioProcessor::setPreferredBusArrangement (bool isInput, int busIndex, const AudioChannelSet& preferredSet)
{
    const int oldNumInputs  = getTotalNumInputChannels();
    const int oldNumOutputs = getTotalNumOutputChannels();

    Array<AudioProcessorBus>& buses = isInput ? busArrangement.inputBuses  : busArrangement.outputBuses;

    const int numBuses  = buses.size();

    if (! isPositiveAndBelow (busIndex, numBuses))
        return false;

    AudioProcessorBus& bus = buses.getReference (busIndex);

   #ifdef JucePlugin_PreferredChannelConfigurations
    // the user is using the deprecated way to specify channel configurations
    if (numBuses > 0 && busIndex == 0)
    {
        const short channelConfigs[][2] = { JucePlugin_PreferredChannelConfigurations };
        const int numChannelConfigs = sizeof (channelConfigs) / sizeof (*channelConfigs);

        // we need the main bus in the opposite direction
        Array<AudioProcessorBus>& oppositeBuses = isInput ? busArrangement.outputBuses : busArrangement.inputBuses;
        AudioProcessorBus* oppositeBus = (busIndex < oppositeBuses.size()) ? &oppositeBuses.getReference (0) : nullptr;

        // get the target number of channels
        const int mainBusNumChannels  = preferredSet.size();
        const int mainBusOppositeChannels = (oppositeBus != nullptr) ? oppositeBus->channels.size() : 0;
        const int dir = isInput ? 0 : 1;

        // find a compatible channel configuration on the opposite bus which is the closest match
        // to the current number of channels on that bus
        int distance = std::numeric_limits<int>::max();
        int bestConfiguration = -1;

        for (int i = 0; i < numChannelConfigs; ++i)
        {
            // is the configuration compatible with the preferred set
            if (channelConfigs[i][dir] == mainBusNumChannels)
            {
                const int configChannels = channelConfigs[i][dir^1];
                const int channelDifference = std::abs (configChannels - mainBusOppositeChannels);

                if (channelDifference < distance)
                {
                    distance = channelDifference;
                    bestConfiguration = configChannels;

                    // we can exit if we found a perfect match
                    if (distance == 0)
                        break;
                }
            }
        }

        // unable to find a good configuration
        if (bestConfiguration == -1)
            return false;

        // did the number of channels change on the opposite bus?
        if (mainBusOppositeChannels != bestConfiguration && oppositeBus != nullptr)
        {
            // if the channels on the opposite bus are the same as the preferred set
            // then also copy over the layout information. If not, then assume
            // a cononical channel layout
            if (bestConfiguration == mainBusNumChannels)
                oppositeBus->channels = preferredSet;
            else
                oppositeBus->channels = AudioChannelSet::canonicalChannelSet (bestConfiguration);
        }
    }
   #endif

    bus.channels = preferredSet;

    if (oldNumInputs != getTotalNumInputChannels() || oldNumOutputs != getTotalNumOutputChannels())
    {
        updateSpeakerFormatStrings();
        numChannelsChanged();
    }

    return true;
}

}
