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
    Label mTitleLabel;
    Label mVersionLabel;
    Label mEmailLabel;

    Label mAmpLoaded;
    Label mIRLoaded;
    Label mCabSimEnabled;

    TextButton mToggleCabSim{ "Enable/disable cabinet simulation" };
    TextButton mLoadAmp{ "Load amp" };
    TextButton mLoadIR{ "Load IR" };

    void toggleCabSim();
    void loadAmp();
    void loadIR();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AmpProjectAudioProcessor& processor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmpProjectAudioProcessorEditor)
};
