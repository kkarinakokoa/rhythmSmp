#include <JuceHeader.h>
#include "ParamsBox.h"

//==============================================================================
ParamsBox::ParamsBox(RhythmSmpAudioProcessor& p, int _boxOrder) : audioProcessor(p), boxOrder(_boxOrder)
{
    int theme = audioProcessor.getTheme();

    /*   ADSR   */
    {
        // Sliders
        attSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar); // LinearBarVertical 도 간지남
        decSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        susSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        relSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);

        attSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
        decSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
        susSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
        relSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);

        /*attSlider.setRange(0.0f, 5.0f, 0.01f);
        decSlider.setRange(0.0f, 5.0f, 0.01f);
        susSlider.setRange(0.0f, 1.0f, 0.01f);
        relSlider.setRange(0.0f, 5.0f, 0.01f);*/

        attSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
        decSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
        susSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
        relSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);

        attSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
        decSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
        susSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
        relSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

        attSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
        decSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
        susSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
        relSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

        /*attSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        decSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        susSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        relSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::transparentBlack);*/

        /*attSlider.addListener(this);
        decSlider.addListener(this);
        susSlider.addListener(this);
        relSlider.addListener(this);*/
        if (boxOrder == 0) {
            attAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "ATTACK1", attSlider);
            decAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "DECAY1", decSlider);
            susAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "SUSTAIN1", susSlider);
            relAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "RELEASE1", relSlider);
            //DBG("[0 connected]");
        }
        else if (boxOrder == 1) {
            attAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "ATTACK2", attSlider);
            decAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "DECAY2", decSlider);
            susAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "SUSTAIN2", susSlider);
            relAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "RELEASE2", relSlider);
            //DBG("[1 connected]");
        }
        else if (boxOrder == 2) {
            attAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "ATTACK3", attSlider);
            decAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "DECAY3", decSlider);
            susAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "SUSTAIN3", susSlider);
            relAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "RELEASE3", relSlider);
            //DBG("[2 connected]");
        }
        else {
            attAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "ATTACK1", attSlider);
            decAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "DECAY1", decSlider);
            susAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "SUSTAIN1", susSlider);
            relAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "RELEASE1", relSlider);
            //DBG("[Non-connected]");
        }

        // Labels
        attLabel.setFont(14.0f);
        attLabel.setText("Attack", juce::NotificationType::dontSendNotification);
        attLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
        attLabel.setJustificationType(juce::Justification::centredTop);
        attLabel.attachToComponent(&attSlider, false);

        decLabel.setFont(14.0f);
        decLabel.setText("Decay", juce::NotificationType::dontSendNotification);
        decLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
        decLabel.setJustificationType(juce::Justification::centredTop);
        decLabel.attachToComponent(&decSlider, false);

        susLabel.setFont(14.0f);
        susLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
        susLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
        susLabel.setJustificationType(juce::Justification::centredTop);
        susLabel.attachToComponent(&susSlider, false);

        relLabel.setFont(14.0f);
        relLabel.setText("Release", juce::NotificationType::dontSendNotification);
        relLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
        relLabel.setJustificationType(juce::Justification::centredTop);
        relLabel.attachToComponent(&relSlider, false);
    }


    /*   Reverbs  */
    {
        // Sliders
        sizeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        dampSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        rvbfbSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        rvbwidthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        rvbwetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);

        sizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
        dampSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
        rvbfbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
        rvbwidthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
        rvbwetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);

        sizeSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
        dampSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
        rvbfbSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
        rvbwidthSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
        rvbwetSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);

        sizeSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
        dampSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
        rvbfbSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
        rvbwidthSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
        rvbwetSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

        sizeSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
        dampSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
        rvbfbSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
        rvbwidthSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
        rvbwetSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

        rvbfbSlider.setTextValueSuffix("%");
        rvbwetSlider.setTextValueSuffix("%");

        if (boxOrder == 0) {
            sizeAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "ROOMSIZE1", sizeSlider);
            dampAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "DAMPING1", dampSlider);
            rvbfbAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "FEEDBACK1", rvbfbSlider);
            rvbwidthAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "WIDTH1", rvbwidthSlider);
            rvbwetAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "WET1", rvbwetSlider);
            //DBG("[rvb 0 connected]");
        }
        else if (boxOrder == 1) {
            sizeAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "ROOMSIZE2", sizeSlider);
            dampAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "DAMPING2", dampSlider);
            rvbfbAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "FEEDBACK2", rvbfbSlider);
            rvbwidthAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "WIDTH2", rvbwidthSlider);
            rvbwetAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "WET2", rvbwetSlider);
            //DBG("[rvb 1 connected]");
        }
        else if (boxOrder == 2) {
            sizeAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "ROOMSIZE3", sizeSlider);
            dampAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "DAMPING3", dampSlider);
            rvbfbAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "FEEDBACK3", rvbfbSlider);
            rvbwidthAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "WIDTH3", rvbwidthSlider);
            rvbwetAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "WET3", rvbwetSlider);
            //DBG("[rvb 2 connected]");
        }
        else {
            sizeAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "ROOMSIZE1", sizeSlider);
            dampAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "DAMPING1", dampSlider);
            rvbfbAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "FEEDBACK1", rvbfbSlider);
            rvbwidthAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "WIDTH1", rvbwidthSlider);
            rvbwetAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "WET1", rvbwetSlider);
            //DBG("[Non-connected]");
        }

        // Labels
        sizeLabel.setFont(14.0f);
        sizeLabel.setText("Reverb Size", juce::NotificationType::dontSendNotification);
        sizeLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
        sizeLabel.setJustificationType(juce::Justification::centredTop);
        sizeLabel.attachToComponent(&sizeSlider, false);

        dampLabel.setFont(14.0f);
        dampLabel.setText("Damping", juce::NotificationType::dontSendNotification);
        dampLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
        dampLabel.setJustificationType(juce::Justification::centredTop);
        dampLabel.attachToComponent(&dampSlider, false);

        rvbfbLabel.setFont(14.0f);
        rvbfbLabel.setText("Feedback", juce::NotificationType::dontSendNotification);
        rvbfbLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
        rvbfbLabel.setJustificationType(juce::Justification::centredTop);
        rvbfbLabel.attachToComponent(&rvbfbSlider, false);

        rvbwidthLabel.setFont(14.0f);
        rvbwidthLabel.setText("Width", juce::NotificationType::dontSendNotification);
        rvbwidthLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
        rvbwidthLabel.setJustificationType(juce::Justification::centredTop);
        rvbwidthLabel.attachToComponent(&rvbwidthSlider, false);

        rvbwetLabel.setFont(14.0f);
        rvbwetLabel.setText("Mix", juce::NotificationType::dontSendNotification);
        rvbwetLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
        rvbwetLabel.setJustificationType(juce::Justification::centredTop);
        rvbwetLabel.attachToComponent(&rvbwetSlider, false);
    }

    /*    Delays     */
       
    // Sliders
    dtSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    dfbSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    dwetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    dppSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);

    dtSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    dfbSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    dwetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    dppSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);

    dtSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
    dfbSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
    dwetSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
    dppSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);

    dtSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    dfbSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    dwetSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    dppSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

    dtSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    dfbSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    dwetSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    dppSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

    dfbSlider.setTextValueSuffix("%");
    dwetSlider.setTextValueSuffix("%");
    
    if (boxOrder == 0) {
        dtAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYTIME1", dtSlider);
        dfbAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYFB1", dfbSlider);
        dwetAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYWET1", dwetSlider);
        dppAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "PINGPONG1", dppSlider);
    }
    else if (boxOrder == 1) {
        dtAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYTIME2", dtSlider);
        dfbAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYFB2", dfbSlider);
        dwetAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYWET2", dwetSlider);
        dppAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "PINGPONG2", dppSlider);
    }
    else if (boxOrder == 2) {
        dtAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYTIME3", dtSlider);
        dfbAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYFB3", dfbSlider);
        dwetAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYWET3", dwetSlider);
        dppAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "PINGPONG3", dppSlider);
    }
    else {
        dtAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYTIME1", dtSlider);
        dfbAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYFB1", dfbSlider);
        dwetAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYWET1", dwetSlider);
        dppAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "PINGPONG1", dppSlider);
    }
    // Labels
    dtLabel.setFont(14.0f);
    dtLabel.setText("Delay Time", juce::NotificationType::dontSendNotification);
    dtLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    dtLabel.setJustificationType(juce::Justification::centredTop);
    dtLabel.attachToComponent(&dtSlider, false);

    dfbLabel.setFont(14.0f);
    dfbLabel.setText("Feedback", juce::NotificationType::dontSendNotification);
    dfbLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    dfbLabel.setJustificationType(juce::Justification::centredTop);
    dfbLabel.attachToComponent(&dfbSlider, false);

    dwetLabel.setFont(14.0f);
    dwetLabel.setText("Mix", juce::NotificationType::dontSendNotification);
    dwetLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    dwetLabel.setJustificationType(juce::Justification::centredTop);
    dwetLabel.attachToComponent(&dwetSlider, false);

    dppLabel.setFont(14.0f);
    dppLabel.setText("Pingpong Mode", juce::NotificationType::dontSendNotification);
    dppLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    dppLabel.setJustificationType(juce::Justification::centredTop);
    dppLabel.attachToComponent(&dppSlider, false);

    /*    Imagers    */
    // Sliders
    idtSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    idrySlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    iwetSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ibalSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);

    idtSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    idrySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    iwetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    ibalSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);

    idtSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
    idrySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
    iwetSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
    ibalSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);

    idtSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    idrySlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    iwetSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    ibalSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

    idtSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    idrySlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    iwetSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    ibalSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

    idtSlider.setTextValueSuffix("ms");
    idrySlider.setTextValueSuffix("%");
    iwetSlider.setTextValueSuffix("%");
    ibalSlider.setTextValueSuffix("dB");

    if (boxOrder == 0) {
        idtAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGDT1", idtSlider);
        idryAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGDRY1", idrySlider);
        iwetAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGWET1", iwetSlider);
        ibalAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "BAL1", ibalSlider);
    }
    else if (boxOrder == 1) {
        idtAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGDT2", idtSlider);
        idryAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGDRY2", idrySlider);
        iwetAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGWET2", iwetSlider);
        ibalAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "BAL2", ibalSlider);
    }
    else if (boxOrder == 2) {
        idtAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGDT3", idtSlider);
        idryAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGDRY3", idrySlider);
        iwetAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGWET3", iwetSlider);
        ibalAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "BAL3", ibalSlider);
    }
    else {
        idtAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGDT1", idtSlider);
        idryAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGDRY1", idrySlider);
        iwetAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGWET1", iwetSlider);
        ibalAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "BAL1", ibalSlider);
    }
    // Labels
    idtLabel.setFont(14.0f);
    idtLabel.setText("Delay Time", juce::NotificationType::dontSendNotification);
    idtLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    idtLabel.setJustificationType(juce::Justification::centredTop);
    idtLabel.attachToComponent(&idtSlider, false);

    idryLabel.setFont(14.0f);
    idryLabel.setText("Dry", juce::NotificationType::dontSendNotification);
    idryLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    idryLabel.setJustificationType(juce::Justification::centredTop);
    idryLabel.attachToComponent(&idrySlider, false);

    iwetLabel.setFont(14.0f);
    iwetLabel.setText("Wet", juce::NotificationType::dontSendNotification);
    iwetLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    iwetLabel.setJustificationType(juce::Justification::centredTop);
    iwetLabel.attachToComponent(&iwetSlider, false);

    ibalLabel.setFont(14.0f);
    ibalLabel.setText("Gain Balance", juce::NotificationType::dontSendNotification);
    ibalLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    ibalLabel.setJustificationType(juce::Justification::centredTop);
    ibalLabel.attachToComponent(&ibalSlider, false);


    /* Gain */

    {
        gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
        gainSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
        gainSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
        
        gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 30, 15);
        gainSlider.setTextValueSuffix("dB");

        if (boxOrder == 0) {
            gainAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "GAIN1", gainSlider);

            gainLabel.setFont(14.0f);
            gainLabel.setText("Gain 1", juce::NotificationType::dontSendNotification);
            gainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
            gainLabel.setJustificationType(juce::Justification::centredTop);
            gainLabel.attachToComponent(&gainSlider, false);
        }
        else if (boxOrder == 1) {
            gainAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "GAIN2", gainSlider);

            gainLabel.setFont(14.0f);
            gainLabel.setText("Gain 2", juce::NotificationType::dontSendNotification);
            gainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
            gainLabel.setJustificationType(juce::Justification::centredTop);
            gainLabel.attachToComponent(&gainSlider, false);
        }
        else if (boxOrder == 2) {
            gainAtt
                = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                (audioProcessor.getAPVTS(), "GAIN3", gainSlider);

            gainLabel.setFont(14.0f);
            gainLabel.setText("Gain 3", juce::NotificationType::dontSendNotification);
            gainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
            gainLabel.setJustificationType(juce::Justification::centredTop);
            gainLabel.attachToComponent(&gainSlider, false);
        }
    }

    /*    FX On OFF   */
    // Sliders
    doSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    ioSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    roSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);

    doSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    ioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    roSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);

    doSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
    ioSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);
    roSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::magenta);

    doSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    ioSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    roSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

    doSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    ioSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    roSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

    doSlider.onValueChange = [this] { repaint(); };
    ioSlider.onValueChange = [this] { repaint(); };
    roSlider.onValueChange = [this] { repaint(); };

    if (boxOrder == 0) {
        doAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYON1", doSlider);
        ioAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGON1", ioSlider);
        roAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "RVBON1", roSlider);
    }
    else if (boxOrder == 1) {
        doAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYON2", doSlider);
        ioAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGON2", ioSlider);
        roAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "RVBON2", roSlider);
    }
    else if (boxOrder == 2) {
        doAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYON3", doSlider);
        ioAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGON3", ioSlider);
        roAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "RVBON3", roSlider);
    }
    else {
        doAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "DELAYON1", doSlider);
        ioAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "IMGON1", ioSlider);
        roAtt
            = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
            (audioProcessor.getAPVTS(), "RVBON1", roSlider);
    }
    // Labels
    doLabel.setFont(14.0f);
    doLabel.setText("FX On/Off", juce::NotificationType::dontSendNotification);
    doLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    doLabel.setJustificationType(juce::Justification::centredTop);
    doLabel.attachToComponent(&doSlider, false);

    ioLabel.setFont(14.0f);
    ioLabel.setText("FX On/Off", juce::NotificationType::dontSendNotification);
    ioLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    ioLabel.setJustificationType(juce::Justification::centredTop);
    ioLabel.attachToComponent(&ioSlider, false);

    roLabel.setFont(14.0f);
    roLabel.setText("FX On/Off", juce::NotificationType::dontSendNotification);
    roLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    roLabel.setJustificationType(juce::Justification::centredTop);
    roLabel.attachToComponent(&roSlider, false);


    /*    FX ComboBox        */

    fxComboBox.addItem("Delay", 1);
    fxComboBox.addItem("Imager", 2);
    fxComboBox.addItem("Reverb", 3);

    fxComboBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    fxComboBox.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    fxComboBox.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    fxComboBox.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    fxComboBox.setSelectedId(1);

    fxLabel.setFont(14.0f);
    fxLabel.setText("Effect Type", juce::NotificationType::dontSendNotification);
    fxLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    fxLabel.setJustificationType(juce::Justification::centredTop);
    fxLabel.attachToComponent(&fxComboBox, false);

    fxComboBox.onChange = [this] { showFX(); };

    /*    FX Chain ComboBox    */
    
    fxChain.addItem("D-I-R", 1);
    fxChain.addItem("D-R-I", 2);
    fxChain.addItem("I-D-R", 3);
    fxChain.addItem("I-R-D", 4);
    fxChain.addItem("R-D-I", 5);
    fxChain.addItem("R-I-D", 6);

    fxChain.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    fxChain.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    fxChain.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    fxChain.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    int* _chain = audioProcessor.getFXChain(boxOrder);
    if ((_chain[0] == 0) && (_chain[1] == 1) && (_chain[2] == 2))
        fxChain.setSelectedId(1);
    else if ((_chain[0] == 0) && (_chain[1] == 2) && (_chain[2] == 1))
        fxChain.setSelectedId(2);
    else if ((_chain[0] == 1) && (_chain[1] == 0) && (_chain[2] == 2))
        fxChain.setSelectedId(3);
    else if ((_chain[0] == 1) && (_chain[1] == 2) && (_chain[2] == 0))
        fxChain.setSelectedId(4);
    else if ((_chain[0] == 2) && (_chain[1] == 0) && (_chain[2] == 1))
        fxChain.setSelectedId(5);
    else if ((_chain[0] == 2) && (_chain[1] == 1) && (_chain[2] == 0))
        fxChain.setSelectedId(6);


    fxChainLabel.setFont(14.0f);
    fxChainLabel.setText("Effect Chain", juce::NotificationType::dontSendNotification);
    fxChainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    fxChainLabel.setJustificationType(juce::Justification::centredTop);
    fxChainLabel.attachToComponent(&fxChain, false);

    fxChain.onChange = [this] { changeFXChain(); };


    /*     Voices Combobox     */

    voicesComboBox.addItem("1", 1);
    voicesComboBox.addItem("2", 2);
    voicesComboBox.addItem("3", 3);
    voicesComboBox.addItem("4", 4);
    voicesComboBox.addItem("5", 5);

    voicesComboBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    voicesComboBox.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    voicesComboBox.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    voicesComboBox.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    voicesComboBox.setSelectedId(audioProcessor.getVoices(boxOrder));

    voicesLabel.setFont(14.0f);
    voicesLabel.setText("Sampler Voices", juce::NotificationType::dontSendNotification);
    voicesLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    voicesLabel.setJustificationType(juce::Justification::centredTop);
    voicesLabel.attachToComponent(&voicesComboBox, false);

    voicesComboBox.onChange = [this] { audioProcessor.setVoices(boxOrder, voicesComboBox.getSelectedId()); repaint(); };


    /*     Random Option Combobox     */

    roptBox.addItem("No option", 1);
    roptBox.addItem("Always Start On Downbeat", 2);
    roptBox.addItem("Tenuto mode", 3);
    roptBox.addItem("Downbeat & Tenuto", 4);

    roptBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    roptBox.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    roptBox.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    roptBox.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    bool downbeatMode = audioProcessor.getDownbeatOpt(boxOrder), tenutoMode = audioProcessor.getTenutoOpt(boxOrder);
    if (!downbeatMode && !tenutoMode) roptBox.setSelectedId(1);
    else if (downbeatMode && !tenutoMode) roptBox.setSelectedId(2);
    else if (!downbeatMode && tenutoMode) roptBox.setSelectedId(3);
    else if (downbeatMode && tenutoMode) roptBox.setSelectedId(4);

    roptLabel.setFont(14.0f);
    roptLabel.setText("Phrase Option", juce::NotificationType::dontSendNotification);
    roptLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    roptLabel.setJustificationType(juce::Justification::centredTop);
    roptLabel.attachToComponent(&roptBox, false);

    /*roptBox.onChange = [this] {
        if (roptBox.getSelectedId() == 1) {
            audioProcessor.setDownbeatOpt(boxOrder, false);
            audioProcessor.setTenutoOpt(boxOrder, false);
        }
        else if (roptBox.getSelectedId() == 2) {
            audioProcessor.setDownbeatOpt(boxOrder, true);
            audioProcessor.setTenutoOpt(boxOrder, false);
        }
        else if (roptBox.getSelectedId() == 3) {
            audioProcessor.setDownbeatOpt(boxOrder, false);
            audioProcessor.setTenutoOpt(boxOrder, true);
        }
        else if (roptBox.getSelectedId() == 4) {
            audioProcessor.setDownbeatOpt(boxOrder, true);
            audioProcessor.setTenutoOpt(boxOrder, true);
        }
        repaint();
    };*/

    /*    Number Density Comboboxes   */

    den1Box.addItem("2", 1); den1Box.addItem("3", 2); den1Box.addItem("4", 3); den1Box.addItem("5", 4); den1Box.addItem("6", 5); 
    den1Box.addItem("7", 6); den1Box.addItem("8", 7); den1Box.addItem("9", 8); den1Box.addItem("10", 9); den1Box.addItem("11", 10); 
    den1Box.addItem("12", 11); den1Box.addItem("13", 12); den1Box.addItem("14", 13); den1Box.addItem("15", 14); den1Box.addItem("16", 15);

    den2Box.addItem("2", 1); den2Box.addItem("3", 2); den2Box.addItem("4", 3); den2Box.addItem("5", 4); den2Box.addItem("6", 5);
    den2Box.addItem("7", 6); den2Box.addItem("8", 7); den2Box.addItem("9", 8); den2Box.addItem("10", 9); den2Box.addItem("11", 10);
    den2Box.addItem("12", 11); den2Box.addItem("13", 12); den2Box.addItem("14", 13); den2Box.addItem("15", 14); den2Box.addItem("16", 15);

    den1Box.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    den1Box.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    den1Box.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    den1Box.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);


    den2Box.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    den2Box.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    den2Box.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    den2Box.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    den1Box.setSelectedId(audioProcessor.getNoteDensityMin(boxOrder) - 1);
    den2Box.setSelectedId(audioProcessor.getNoteDensityMax(boxOrder) - 1);

    den1Label.setFont(14.0f);
    den1Label.setText("Minimum Notes #", juce::NotificationType::dontSendNotification);
    den1Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    den1Label.setJustificationType(juce::Justification::centredTop);
    den1Label.attachToComponent(&den1Box, false);

    den2Label.setFont(14.0f);
    den2Label.setText("Maximum Notes #", juce::NotificationType::dontSendNotification);
    den2Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    den2Label.setJustificationType(juce::Justification::centredTop);
    den2Label.attachToComponent(&den2Box, false);

    /*den1Box.onChange = [this] {
        audioProcessor.setNoteDensities(boxOrder, den1Box.getSelectedId() + 1, den2Box.getSelectedId() + 1);
        repaint();
    };
    den2Box.onChange = [this] {
        audioProcessor.setNoteDensities(boxOrder, den1Box.getSelectedId() + 1, den2Box.getSelectedId() + 1);
        repaint();
    };*/
    
    /*   Phrase Maker   */

    makepBtn.setColour(juce::TextButton::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    makepBtn.setColour(juce::TextButton::ColourIds::textColourOffId, audioProcessor.fxfontColors[theme]);
    makepBtn.setColour(juce::TextButton::ColourIds::textColourOnId, audioProcessor.fxfontColors[theme]);
    makepBtn.setColour(juce::TextButton::ColourIds::textColourOffId, audioProcessor.fxfontColors[theme]);
    makepBtn.setButtonText("Reload");

    makepBtn.onClick = [this] {
        if (roptBox.getSelectedId() == 1) {
            audioProcessor.setDownbeatOpt(boxOrder, false);
            audioProcessor.setTenutoOpt(boxOrder, false);
        }
        else if (roptBox.getSelectedId() == 2) {
            audioProcessor.setDownbeatOpt(boxOrder, true);
            audioProcessor.setTenutoOpt(boxOrder, false);
        }
        else if (roptBox.getSelectedId() == 3) {
            audioProcessor.setDownbeatOpt(boxOrder, false);
            audioProcessor.setTenutoOpt(boxOrder, true);
        }
        else if (roptBox.getSelectedId() == 4) {
            audioProcessor.setDownbeatOpt(boxOrder, true);
            audioProcessor.setTenutoOpt(boxOrder, true);
        }
        audioProcessor.setNoteDensities(boxOrder, den1Box.getSelectedId() + 1, den2Box.getSelectedId() + 1);
        audioProcessor.createNewPhrases(boxOrder);

        den1Box.setSelectedId(audioProcessor.getNoteDensityMin(boxOrder) - 1);
        den2Box.setSelectedId(audioProcessor.getNoteDensityMax(boxOrder) - 1);

        repaint();
    };

    makepLabel.setFont(14.0f);
    makepLabel.setText("New Phrases", juce::NotificationType::dontSendNotification);
    makepLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    makepLabel.setJustificationType(juce::Justification::centredTop);
    makepLabel.attachToComponent(&makepBtn, false);

    // Make em visible

    addAndMakeVisible(attSlider);
    addAndMakeVisible(decSlider);
    addAndMakeVisible(susSlider);
    addAndMakeVisible(relSlider);

    addAndMakeVisible(dtSlider);
    addAndMakeVisible(dfbSlider);
    addAndMakeVisible(dwetSlider);
    addAndMakeVisible(dppSlider);

    addAndMakeVisible(sizeSlider);
    addAndMakeVisible(dampSlider);
    addAndMakeVisible(rvbfbSlider);
    addAndMakeVisible(rvbwidthSlider);
    addAndMakeVisible(rvbwetSlider);

    addAndMakeVisible(idtSlider);
    addAndMakeVisible(idrySlider);
    addAndMakeVisible(iwetSlider);
    addAndMakeVisible(ibalSlider);

    addAndMakeVisible(gainSlider);
    
    addAndMakeVisible(doSlider);
    addAndMakeVisible(ioSlider);
    addAndMakeVisible(roSlider);

    addAndMakeVisible(voicesComboBox);
    addAndMakeVisible(roptBox);
    addAndMakeVisible(den1Box);
    addAndMakeVisible(den2Box);
    addAndMakeVisible(makepBtn);

    addAndMakeVisible(fxComboBox);
    addAndMakeVisible(fxChain);

    

}

ParamsBox::~ParamsBox()
{
}

void ParamsBox::paint(juce::Graphics& g)
{
    // Gain

    float gainX0 = 0, gainY0 = 0.1f, gainWidth = 0.1f * 0.8, gainHeight = 0.85f;
    gainSlider.setBoundsRelative(gainX0, gainY0, gainWidth, gainHeight);

    // ADSR

    float adsrX0 = gainX0 + gainWidth + 0.04f * 0.8, adsrY0 = 0.1f, adsrWidth = 0.16f * 0.8, adsrHeight = 0.1f;

    attSlider.setBoundsRelative(adsrX0, adsrY0, adsrWidth, adsrHeight);
    decSlider.setBoundsRelative(adsrX0, adsrY0 + 0.25, adsrWidth, adsrHeight);
    susSlider.setBoundsRelative(adsrX0, adsrY0 + 0.5, adsrWidth, adsrHeight);
    relSlider.setBoundsRelative(adsrX0, adsrY0 + 0.75, adsrWidth, adsrHeight);


    // fx type ComboBox

    float fxX0 = adsrX0 + adsrWidth + 0.04f * 0.8, fxY0 = 0.1f, fxWidth = 0.2f * 0.8, fxHeight = 0.1f;
    fxComboBox.setBoundsRelative(fxX0, fxY0, fxWidth, fxHeight);

    // fxChain ComboBox

    float fxChainX0 = fxX0 + fxWidth + 0.04f * 0.8, fxChainY0 = 0.1f, fxChainWidth = 0.2f * 0.8, fxChainHeight = 0.1f;
    fxChain.setBoundsRelative(fxChainX0, fxChainY0, fxChainWidth, fxChainHeight);


    // fxs
    float X0 = adsrX0 + adsrWidth + 0.04f * 0.8, Y0 = 0.1f, _Width = 0.2f * 0.8, _Height = 0.1f;
    
    // Delay
    dtSlider.setBoundsRelative(X0, Y0 + 0.25f, _Width, _Height);
    dfbSlider.setBoundsRelative(X0, Y0 + 0.5f, _Width, _Height);
    dppSlider.setBoundsRelative(X0 + (_Width + 0.04f * 0.8), Y0 + 0.25f, _Width, _Height);
    dwetSlider.setBoundsRelative(X0 + (_Width + 0.04f * 0.8), Y0 + 0.5f, _Width, _Height);
    doSlider.setBoundsRelative(X0 + (_Width + 0.04f * 0.8), Y0 + 0.75f, _Width, _Height);

    // RVB
    sizeSlider.setBoundsRelative(X0, Y0 + 0.25f, _Width, _Height);
    dampSlider.setBoundsRelative(X0, Y0 + 0.5f, _Width, _Height);
    rvbfbSlider.setBoundsRelative(X0, Y0 + 0.75f, _Width, _Height);
    rvbwidthSlider.setBoundsRelative(X0 + (_Width + 0.04f * 0.8), Y0 + 0.25f, _Width, _Height);
    rvbwetSlider.setBoundsRelative(X0 + (_Width + 0.04f * 0.8), Y0 + 0.5f, _Width, _Height);
    roSlider.setBoundsRelative(X0 + (_Width + 0.04f * 0.8), Y0 + 0.75f, _Width, _Height);

    // Imager
    idtSlider.setBoundsRelative(X0, Y0 + 0.25f, _Width, _Height);
    idrySlider.setBoundsRelative(X0, Y0 + 0.5f, _Width, _Height);
    ibalSlider.setBoundsRelative(X0 + (_Width + 0.04f * 0.8), Y0 + 0.25f, _Width, _Height);
    iwetSlider.setBoundsRelative(X0 + (_Width + 0.04f * 0.8), Y0 + 0.5f, _Width, _Height);
    ioSlider.setBoundsRelative(X0 + (_Width + 0.04f * 0.8), Y0 + 0.75f, _Width, _Height);

    if (currFX == 1) {
        dtSlider.setVisible(true);
        dfbSlider.setVisible(true);
        dppSlider.setVisible(true);
        dwetSlider.setVisible(true);
        doSlider.setVisible(true);

        sizeSlider.setVisible(false);
        dampSlider.setVisible(false);
        rvbfbSlider.setVisible(false);
        rvbwidthSlider.setVisible(false);
        rvbwetSlider.setVisible(false);
        roSlider.setVisible(false);

        idtSlider.setVisible(false);
        idrySlider.setVisible(false);
        iwetSlider.setVisible(false);
        ibalSlider.setVisible(false);
        ioSlider.setVisible(false);
    }
    else if (currFX == 2) {
        dtSlider.setVisible(false);
        dfbSlider.setVisible(false);
        dppSlider.setVisible(false);
        dwetSlider.setVisible(false);
        doSlider.setVisible(false);

        sizeSlider.setVisible(false);
        dampSlider.setVisible(false);
        rvbfbSlider.setVisible(false);
        rvbwidthSlider.setVisible(false);
        rvbwetSlider.setVisible(false);
        roSlider.setVisible(false);

        idtSlider.setVisible(true);
        idrySlider.setVisible(true);
        iwetSlider.setVisible(true);
        ibalSlider.setVisible(true);
        ioSlider.setVisible(true);
    }
    else if (currFX == 3) {
        dtSlider.setVisible(false);
        dfbSlider.setVisible(false);
        dppSlider.setVisible(false);
        dwetSlider.setVisible(false);
        doSlider.setVisible(false);

        sizeSlider.setVisible(true);
        dampSlider.setVisible(true);
        rvbfbSlider.setVisible(true);
        rvbwidthSlider.setVisible(true);
        rvbwetSlider.setVisible(true);
        roSlider.setVisible(true);

        idtSlider.setVisible(false);
        idrySlider.setVisible(false);
        iwetSlider.setVisible(false);
        ibalSlider.setVisible(false);
        ioSlider.setVisible(false);
    }

    // Enable or Disable
    if (audioProcessor.getOnOrOff(boxOrder, 0)) {
        dtSlider.setEnabled(true);
        dfbSlider.setEnabled(true);
        dppSlider.setEnabled(true);
        dwetSlider.setEnabled(true);
    }
    else {
        dtSlider.setEnabled(false);
        dfbSlider.setEnabled(false);
        dppSlider.setEnabled(false);
        dwetSlider.setEnabled(false);
    }
    if (audioProcessor.getOnOrOff(boxOrder, 1)) {
        idtSlider.setEnabled(true);
        idrySlider.setEnabled(true);
        iwetSlider.setEnabled(true);
        ibalSlider.setEnabled(true);
    }
    else {
        idtSlider.setEnabled(false);
        idrySlider.setEnabled(false);
        iwetSlider.setEnabled(false);
        ibalSlider.setEnabled(false);
    }
    if (audioProcessor.getOnOrOff(boxOrder, 2)) {
        sizeSlider.setEnabled(true);
        dampSlider.setEnabled(true);
        rvbfbSlider.setEnabled(true);
        rvbwidthSlider.setEnabled(true);
        rvbwetSlider.setEnabled(true);
    }
    else {
        sizeSlider.setEnabled(false);
        dampSlider.setEnabled(false);
        rvbfbSlider.setEnabled(false);
        rvbwidthSlider.setEnabled(false);
        rvbwetSlider.setEnabled(false);
    }

    // Voices ComboBox
    float vX0 = X0 + (_Width + 0.04f * 0.8) + _Width + 0.1f * 0.8, vY0 = 0.5f, vW = 0.2f * 0.8, vH = 0.1f;
    voicesComboBox.setBoundsRelative(vX0, vY0, vW, vH);

    // Random Option Combobox & Number Density boxes & Phrase Maker
    float rX0 = X0 + (_Width + 0.04f * 0.8) + _Width + 0.1f * 0.8, rY0 = 0.1f, rW = 0.2f * 0.8, rH = 0.1f;
    roptBox.setBoundsRelative(rX0, rY0, rW, rH);
    den1Box.setBoundsRelative(rX0, rY0 + 0.25f, rW, rH);
    den2Box.setBoundsRelative(rX0, rY0 + 0.5f, rW, rH);
    makepBtn.setBoundsRelative(rX0, rY0 + 0.75f, rW, rH);

    /*den1Box.setSelectedId(audioProcessor.getNoteDensityMin(boxOrder) - 1);
    den2Box.setSelectedId(audioProcessor.getNoteDensityMax(boxOrder) - 1);*/

    if (audioProcessor.getCreatemode()) {
        voicesComboBox.setVisible(false);
        roptBox.setVisible(true);
        den1Box.setVisible(true);
        den2Box.setVisible(true);
        makepBtn.setVisible(true);
    }
    else {
        voicesComboBox.setVisible(true);
        roptBox.setVisible(false);
        den1Box.setVisible(false);
        den2Box.setVisible(false);
        makepBtn.setVisible(false);

    }
}

void ParamsBox::resized()
{
    repaint();    
}

void ParamsBox::showFX()
{
    currFX = fxComboBox.getSelectedId();
    repaint();
}

void ParamsBox::changeFXChain()
{
    /*fxChain.addItem("D-I-R", 1);
    fxChain.addItem("D-R-I", 2);
    fxChain.addItem("I-D-R", 3);
    fxChain.addItem("I-R-D", 4);
    fxChain.addItem("R-D-I", 5);
    fxChain.addItem("R-I-D", 6);*/

    int idx = fxChain.getSelectedId();

    switch (idx) {
    case 1:
        audioProcessor.setFXChain(boxOrder, 0, 1, 2);
        break;
    case 2:
        audioProcessor.setFXChain(boxOrder, 0, 2, 1);
        break;
    case 3:
        audioProcessor.setFXChain(boxOrder, 1, 0, 2);
        break;
    case 4:
        audioProcessor.setFXChain(boxOrder, 1, 2, 0);
        break;
    case 5:
        audioProcessor.setFXChain(boxOrder, 2, 0, 1);
        break;
    case 6:
        audioProcessor.setFXChain(boxOrder, 2, 1, 0);
        break;
    }
}

void ParamsBox::changeColors()
{
    int theme = audioProcessor.getTheme();

    attSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    decSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    susSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    relSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

    attSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    decSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    susSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    relSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

    sizeSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    dampSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    rvbfbSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    rvbwidthSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    rvbwetSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

    sizeSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    dampSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    rvbfbSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    rvbwidthSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    rvbwetSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

    dtSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    dfbSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    dwetSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    dppSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

    dtSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    dfbSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    dwetSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    dppSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

    idtSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    idrySlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    iwetSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    ibalSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

    idtSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    idrySlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    iwetSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    ibalSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

    gainSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    gainSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

    doSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    ioSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);
    roSlider.setColour(juce::Slider::ColourIds::textBoxTextColourId, audioProcessor.fxfontColors[theme]);

    doSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    ioSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);
    roSlider.setColour(juce::Slider::ColourIds::trackColourId, audioProcessor.trackColors[theme]);

    fxComboBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    fxComboBox.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    fxComboBox.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    fxComboBox.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    fxChain.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    fxChain.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    fxChain.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    fxChain.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    voicesComboBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    voicesComboBox.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    voicesComboBox.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    voicesComboBox.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    roptBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    roptBox.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    roptBox.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    roptBox.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    den1Box.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    den1Box.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    den1Box.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    den1Box.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);


    den2Box.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    den2Box.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    den2Box.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    den2Box.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

    makepBtn.setColour(juce::TextButton::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    makepBtn.setColour(juce::TextButton::ColourIds::textColourOffId, audioProcessor.fxfontColors[theme]);
    makepBtn.setColour(juce::TextButton::ColourIds::textColourOnId, audioProcessor.fxfontColors[theme]);
    makepBtn.setColour(juce::TextButton::ColourIds::textColourOffId, audioProcessor.fxfontColors[theme]);
}
