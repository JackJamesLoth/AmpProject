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

    addAndMakeVisible(titleLabel);
    titleLabel.setFont(juce::Font(35.0f, juce::Font::bold));
    titleLabel.setText("Archetype Loth", juce::dontSendNotification);
    titleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    titleLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(versionLabel);
    versionLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    versionLabel.setText("v0.1", juce::dontSendNotification);
    versionLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    versionLabel.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(emailLabel);
    emailLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    emailLabel.setText("For questions, bug reports or anything else, please contact me at jackjamesloth@gmail.com", juce::dontSendNotification);
    emailLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    emailLabel.setJustificationType(juce::Justification::centredBottom);
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

    titleLabel.setBounds(0, 0, getWidth(), getHeight()/3);
    versionLabel.setBounds(0, 0, getWidth(), 130);
    emailLabel.setBounds(0, 0, getWidth(), getHeight()-40);

}

