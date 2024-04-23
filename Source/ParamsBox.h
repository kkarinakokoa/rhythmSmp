#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ParamsBox : public juce::Component
{
public:
    ParamsBox(RhythmSmpAudioProcessor& p, int _boxOrder);
    ~ParamsBox() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    int getOrder() { return boxOrder; }
    void setOrder(int num) { boxOrder = num; }

    void showFX();
    void changeFXChain();

    void changeColors();

private:
    // ADSR
    juce::Label attLabel, decLabel, susLabel, relLabel;
    juce::Slider attSlider, decSlider, susSlider, relSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attAtt, decAtt, susAtt, relAtt;

    // Reverbs
    juce::Label sizeLabel, dampLabel, rvbfbLabel, rvbwidthLabel, rvbwetLabel;
    juce::Slider sizeSlider, dampSlider, rvbfbSlider, rvbwidthSlider, rvbwetSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sizeAtt, dampAtt, rvbfbAtt, rvbwidthAtt, rvbwetAtt;

    // Delays
    juce::Label dtLabel, dfbLabel, dwetLabel, dppLabel;
    juce::Slider dtSlider, dfbSlider, dwetSlider, dppSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dtAtt, dfbAtt, dwetAtt, dppAtt;
    
    // Imagers
    juce::Label idtLabel, idryLabel, iwetLabel, ibalLabel;
    juce::Slider idtSlider, idrySlider, iwetSlider, ibalSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> idtAtt, idryAtt, iwetAtt, ibalAtt;

    // Gains
    float gain;
    juce::Label gainLabel;
    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAtt;

    // On & Off
    juce::Label doLabel, ioLabel, roLabel;
    juce::Slider doSlider, ioSlider, roSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> doAtt, ioAtt, roAtt;

    // Voices ComboBox
    juce::Label voicesLabel;
    juce::ComboBox voicesComboBox;

    // FX Type ComboBox
    juce::Label fxLabel;
    juce::ComboBox fxComboBox;

    // FX Chain ComboBox
    juce::Label fxChainLabel;
    juce::ComboBox fxChain;

    // Random Option ComboBox
    juce::Label roptLabel;
    juce::ComboBox roptBox;

    // Density Option ComboBox
    juce::Label den1Label, den2Label;
    juce::ComboBox den1Box, den2Box;

    // Phrase Create
    juce::Label makepLabel;
    juce::TextButton makepBtn;

    int currFX{ 1 };

    int boxOrder;

    RhythmSmpAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParamsBox)
};