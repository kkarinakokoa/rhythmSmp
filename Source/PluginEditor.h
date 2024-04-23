/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SampleShape.h"
#include "ParamsBox.h"

//==============================================================================
/**
*/
class RhythmSmpAudioProcessorEditor : public juce::AudioProcessorEditor,
    public juce::Timer
    //public juce::FileDragAndDropTarget,
    //public juce::Slider::Listener
{
public:
    RhythmSmpAudioProcessorEditor(RhythmSmpAudioProcessor&);
    ~RhythmSmpAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:


    SampleShape sampleShape1, sampleShape2, sampleShape3;
    ParamsBox paramsBox1, paramsBox2, paramsBox3;

    juce::ComboBox modeBox;
    juce::Label modeLabel;

    juce::Rectangle<int> completeMsg;
    

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    RhythmSmpAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RhythmSmpAudioProcessorEditor)
};
