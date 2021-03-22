/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AmpProjectAudioProcessorEditor::AmpProjectAudioProcessorEditor (AmpProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1000, 500);

    addAndMakeVisible(mTitleLabel);
    mTitleLabel.setFont(juce::Font(35.0f, juce::Font::bold));
    mTitleLabel.setText("Archetype Loth", juce::dontSendNotification);
    mTitleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    mTitleLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(mVersionLabel);
    mVersionLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    mVersionLabel.setText("v0.1", juce::dontSendNotification);
    mVersionLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    mVersionLabel.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(mEmailLabel);
    mEmailLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    mEmailLabel.setText("For questions, bug reports or anything else, please contact me at jackjamesloth@gmail.com", juce::dontSendNotification);
    mEmailLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    mEmailLabel.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(mIRLoaded);
    mIRLoaded.setFont(juce::Font(20.0f, juce::Font::bold));
    mIRLoaded.setText("Current IR loaded:\nNone", juce::dontSendNotification);
    mIRLoaded.setColour(juce::Label::textColourId, juce::Colours::black);
    mIRLoaded.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(mCabSimEnabled);
    mCabSimEnabled.setFont(juce::Font(20.0f, juce::Font::bold));
    mCabSimEnabled.setText("Cab sim disabled", juce::dontSendNotification);
    mCabSimEnabled.setColour(juce::Label::textColourId, juce::Colours::black);
    mCabSimEnabled.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(mAmpLoaded);
    mAmpLoaded.setFont(juce::Font(20.0f, juce::Font::bold));
    mAmpLoaded.setText("Current amp loaded:\nNone", juce::dontSendNotification);
    mAmpLoaded.setColour(juce::Label::textColourId, juce::Colours::black);
    mAmpLoaded.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(mToggleCabSim);
    mToggleCabSim.onClick = [this] {
        toggleCabSim();
    };

    addAndMakeVisible(mLoadAmp);
    mLoadAmp.onClick = [this] {
        loadAmp();
    };

    addAndMakeVisible(mLoadIR);
    mLoadIR.onClick = [this] {
        loadIR();
    };
}

AmpProjectAudioProcessorEditor::~AmpProjectAudioProcessorEditor()
{
}

//==============================================================================
void AmpProjectAudioProcessorEditor::paint (Graphics& g)
{
    // fill the whole window white
    g.fillAll (juce::Colours::white);
}

void AmpProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    mTitleLabel.setBounds(0, 0, getWidth(), getHeight()/3);
    mVersionLabel.setBounds(0, 0, getWidth(), 130);
    mEmailLabel.setBounds(0, 0, getWidth(), getHeight()-40);

    mAmpLoaded.setBounds(0, 0, getWidth() / 2 - 50, getHeight() / 2);
    mIRLoaded.setBounds(getWidth() /2 + 25, 0, getWidth()/2 + 25, getHeight()/2);
    mCabSimEnabled.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2-50);

    mLoadAmp.setBounds(getWidth() / 2 - 100, getHeight() / 2 - 100, 100, 50);
    mLoadIR.setBounds(getWidth() / 2, getHeight() / 2 - 100, 100, 50);
    mToggleCabSim.setBounds(getWidth()/2 -100, getHeight()/2-40, 200, 100);

}

void AmpProjectAudioProcessorEditor::toggleCabSim() {
    DBG("toggle cab sim");
    processor.mCabSimEnabled = !processor.mCabSimEnabled;

    if (processor.mCabSimEnabled) {
        mCabSimEnabled.setText("Cab sim enabled", juce::dontSendNotification);
    }
    else {
        mCabSimEnabled.setText("Cab sim disabled", juce::dontSendNotification);
    }
}

void AmpProjectAudioProcessorEditor::loadAmp() {

    FileChooser fc("Select an amp file", {}, "*.json");
    if (fc.browseForFileToOpen()) {
        File ampFile = fc.getResult();
        processor.updateAmpFile(ampFile);

        mAmpLoaded.setText("Current amp loaded:\n" + ampFile.getFileName(), juce::dontSendNotification);
    }

    DBG("load amp");
}

void AmpProjectAudioProcessorEditor::loadIR() {

    FileChooser fc("Select an IR", {}, "*.wav");
    if (fc.browseForFileToOpen()) {
        File irFile = fc.getResult();
        processor.updateIRFile(irFile);
        
        mIRLoaded.setText("Current IR loaded:\n" + irFile.getFileName(), juce::dontSendNotification);
        mCabSimEnabled.setText("Cab sim enabled", juce::dontSendNotification);
    }

    DBG("load IR");
}