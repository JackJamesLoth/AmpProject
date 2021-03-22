/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AmpProjectAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    AmpProjectAudioProcessorEditor (AmpProjectAudioProcessor&);
    ~AmpProjectAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    Label titleLabel;
    Label versionLabel;
    Label emailLabel;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AmpProjectAudioProcessor& processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpProjectAudioProcessorEditor)
};
