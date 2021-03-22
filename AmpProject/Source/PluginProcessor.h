/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Model.h"

//==============================================================================
/**
*/
class AmpProjectAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    AmpProjectAudioProcessor();
    ~AmpProjectAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void fillAmpBuffer(const int ampBufferLength, const int bufferLength, int channel, const float* inBuffer);
    void getFromAmpBuffer(AudioBuffer<float>& buffer, const int ampBufferLength, const int bufferLength, int channel, const float* ampModelBufferData);

    float level;

private:
    AmpModel mAmpModel;
    AudioBuffer<float> mAmpBuffer;
    int mReceptiveField{ 2047 };
    int mWritePosition{ 0 };
    const int mNumChannels{ 16 };

    // Cabinet sim
    dsp::Convolution mConvolution1;
    dsp::Convolution mConvolution2;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpProjectAudioProcessor)
        
};
