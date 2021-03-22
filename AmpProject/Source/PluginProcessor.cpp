/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Model.h"
#include "ConvLayer.h"


//==============================================================================
AmpProjectAudioProcessor::AmpProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{

}

AmpProjectAudioProcessor::~AmpProjectAudioProcessor()
{
}

//==============================================================================
const String AmpProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmpProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AmpProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AmpProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AmpProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmpProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AmpProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmpProjectAudioProcessor::setCurrentProgram (int index)
{
}

const String AmpProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void AmpProjectAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AmpProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    // 2047 is extra samples that should be padded using 10 conv layers
    const int ampBufferSize = samplesPerBlock + mReceptiveField; 
    
    // Initialize amp buffer
    mAmpBuffer.setSize(mNumChannels, ampBufferSize);

    // Initialize amp model
    mAmpModel.initModel(10, 16, ampBufferSize, samplesPerBlock);
    
    /*
    * 
    * 
    * ***NOTE*** The samples in the buffer default to -4.31602e+08
    * It would be ideal for this to be 0, however, I will leave it as it is for now at least
    * It might sound weird at the beginning...
    * 
    * 
    */
}

void AmpProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AmpProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AmpProjectAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    // Get numer of samples in the buffer
    const int bufferLength = buffer.getNumSamples();
    const int ampBufferLength = mAmpBuffer.getNumSamples();

    // Can use auto keyword here isntead of float
    const float* inBuffer = buffer.getReadPointer(0);
    float* outBuffer = buffer.getWritePointer(0);

    mAmpModel.forward(buffer, bufferLength, 0, mWritePosition);

    // Get pointer to amp buffer
    //const float* ampModelBufferData = mAmpBuffer.getReadPointer(0);

    // Copy data to amp buffer
    //fillAmpBuffer(ampBufferLength, bufferLength, 0, inBuffer);
    // ****NOTE**** This may be too much copying
    //              If it is not working well, try switching to circular buffer instead (already implemented!)
    //mAmpBuffer.copyFrom(0, 0, ampModelBufferData + (ampBufferLength - mReceptiveField), mReceptiveField);
    //mAmpBuffer.copyFrom(0, mReceptiveField, inBuffer, bufferLength);
    
    // Send the samples through the model
    //mAmpModel.forward(mAmpBuffer, ampBufferLength, 0, mWritePosition);
    


    // Copy amp buffer samples to output buffer
    //getFromAmpBuffer(buffer, ampBufferLength, bufferLength, 0, ampModelBufferData);



    //buffer.copyFrom(0, 0, ampModelBufferData + mReceptiveField, bufferLength);




    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // Can use auto keyword here isntead of float
        const float* inBuffer = buffer.getReadPointer(channel);
        float* outBuffer = buffer.getWritePointer(channel);

        // Get pointer to amp buffer
        const float* ampModelBufferData = mAmpBuffer.getReadPointer(channel);

        // Copy data to amp buffer
        //fillAmpBuffer(ampBufferLength, bufferLength, channel, inBuffer);
        // ****NOTE**** This may be too much copying
        //              If it is not working well, try switching to circular buffer instead (already implemented!)
        mAmpBuffer.copyFrom(channel, 0, ampModelBufferData + (ampBufferLength - mReceptiveField), mReceptiveField);
        mAmpBuffer.copyFrom(channel, mReceptiveField, inBuffer, bufferLength);

        // Send the samples through the model
        mAmpModel.forward(mAmpBuffer, bufferLength, channel, mWritePosition);

        // Copy amp buffer samples to output buffer
        //getFromAmpBuffer(buffer, ampBufferLength, bufferLength, channel, ampModelBufferData);
        buffer.copyFrom(channel, 0, ampModelBufferData + mReceptiveField, bufferLength);
    }
    */

    mWritePosition += bufferLength;
    mWritePosition %= ampBufferLength;

}

void AmpProjectAudioProcessor::fillAmpBuffer(const int ampBufferLength, const int bufferLength, int channel, const float* inBuffer) {
    if (ampBufferLength > mWritePosition + bufferLength) {
        mAmpBuffer.copyFrom(channel, mWritePosition, inBuffer, bufferLength);
    }
    else {
        const int remaining = ampBufferLength - mWritePosition;
        mAmpBuffer.copyFrom(channel, mWritePosition, inBuffer, remaining);
        mAmpBuffer.copyFrom(channel, 0, inBuffer + remaining, bufferLength - remaining);
    }
}

void AmpProjectAudioProcessor::getFromAmpBuffer(AudioBuffer<float>& buffer, const int ampBufferLength, const int bufferLength, int channel, const float* ampModelBufferData) {
    if (ampBufferLength > mWritePosition + bufferLength) {
        buffer.copyFrom(channel, 0, ampModelBufferData + mWritePosition, bufferLength);
    }
    else {
        int remaining = ampBufferLength - mWritePosition;
        buffer.copyFrom(channel, 0, ampModelBufferData + mWritePosition, remaining);
        buffer.copyFrom(channel, remaining, ampModelBufferData, bufferLength - remaining);
    }
}

//==============================================================================
bool AmpProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AmpProjectAudioProcessor::createEditor()
{
    return new AmpProjectAudioProcessorEditor (*this);
}

//==============================================================================
void AmpProjectAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AmpProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmpProjectAudioProcessor();
}
