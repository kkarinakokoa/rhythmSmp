#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class SampleShape : public juce::Component,
    public juce::FileDragAndDropTarget,
    public juce::MouseListener
{
public:
    SampleShape(RhythmSmpAudioProcessor& p, int order);
    ~SampleShape() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    void mouseDown(const juce::MouseEvent& event) override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    int getOrder() { return samplerOrder; }
    void setOrder(int num) { samplerOrder = num; }

private:
    std::vector<float> audioData;
    int samplerOrder;

    RhythmSmpAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleShape)
};

