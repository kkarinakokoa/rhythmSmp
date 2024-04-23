/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RhythmSmpAudioProcessorEditor::RhythmSmpAudioProcessorEditor(RhythmSmpAudioProcessor& p)
    : AudioProcessorEditor(&p), sampleShape1(p, 0), sampleShape2(p, 1), sampleShape3(p, 2), paramsBox1(p, 0), paramsBox2(p, 1), paramsBox3(p, 2), audioProcessor(p)
{
    int theme = audioProcessor.getTheme();

    modeBox.addItem("Free", 1);
    modeBox.addItem("Random", 2);
    modeBox.addItem("Change theme", 3);

    modeBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
    modeBox.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
    modeBox.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
    modeBox.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);
    
    if (audioProcessor.getCreatemode()) {
        modeBox.setSelectedId(2);
    }
    else {
        modeBox.setSelectedId(1);
    }

    modeLabel.setFont(14.0f);
    modeLabel.setText("Sampler Mode", juce::NotificationType::dontSendNotification);
    modeLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::magenta);
    modeLabel.setJustificationType(juce::Justification::centredTop);
    modeLabel.attachToComponent(&modeBox, false);

    modeBox.onChange = [this] {
        if (modeBox.getSelectedId() == 1) {
            audioProcessor.setCreatemode(false);
        }
        else if (modeBox.getSelectedId() == 2) {
            audioProcessor.setCreatemode(true);
        }
        else if (modeBox.getSelectedId() == 3) {
            audioProcessor.toggleTheme();

            int theme = audioProcessor.getTheme();

            paramsBox1.changeColors();
            paramsBox2.changeColors();
            paramsBox3.changeColors();

            modeBox.setColour(juce::ComboBox::ColourIds::backgroundColourId, audioProcessor.trackColors[theme]);
            modeBox.setColour(juce::ComboBox::ColourIds::textColourId, audioProcessor.fxfontColors[theme]);
            modeBox.setColour(juce::ComboBox::ColourIds::buttonColourId, audioProcessor.trackColors[theme]);
            modeBox.setColour(juce::ComboBox::ColourIds::arrowColourId, audioProcessor.fxfontColors[theme]);

            if (audioProcessor.getCreatemode()) modeBox.setSelectedId(2);
            else modeBox.setSelectedId(1);
        }
        repaint();
        paramsBox1.repaint();
        paramsBox2.repaint();
        paramsBox3.repaint();
    };


    

    addAndMakeVisible(sampleShape1);
    addAndMakeVisible(sampleShape2);
    addAndMakeVisible(sampleShape3);

    addAndMakeVisible(paramsBox1);
    addAndMakeVisible(paramsBox2);
    addAndMakeVisible(paramsBox3);

 
    addAndMakeVisible(modeBox);


    completeMsg.setWidth(getWidth() * 0.2f); completeMsg.setHeight(0.03f * getHeight());
    completeMsg.setX(getWidth() * 0.8f); completeMsg.setY(0.04f * getHeight());


    startTimerHz(20);

    setSize(1300, 800);
    setResizable(true, true);
}

RhythmSmpAudioProcessorEditor::~RhythmSmpAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void RhythmSmpAudioProcessorEditor::paint(juce::Graphics& g)
{
    int theme = audioProcessor.getTheme();
    g.fillAll(audioProcessor.bgColors[theme]);
    if (audioProcessor.getCompleteCreated() > 0) {
        g.setColour(audioProcessor.msgColors[theme]);
        g.setFont(14.0f);
        g.drawFittedText("Load complete", completeMsg, juce::Justification::centred, 1);
    }
}

void RhythmSmpAudioProcessorEditor::timerCallback() {
    repaint();
    paramsBox1.repaint();
    paramsBox2.repaint();
    paramsBox3.repaint();

    if (audioProcessor.getCompleteCreated() > 0) audioProcessor.decreaseCompleteCreated();
}

void RhythmSmpAudioProcessorEditor::resized()
{
    float sampleShapeWidth = 0.4f, sampleShapeHeight = 0.28f;
    float sampleShapeX0 = 0.01f, sampleShapeY0 = 0.1f;
    float paramsBoxWidth = 0.5f;

    modeBox.setBoundsRelative(0.42f, 0.04f, 0.2f, 0.03f);
    completeMsg.setWidth(getWidth() * 0.2f); completeMsg.setHeight(0.03f * getHeight());
    completeMsg.setX(getWidth() * 0.8f); completeMsg.setY(0.04f * getHeight());

    sampleShape1.setBoundsRelative(sampleShapeX0, sampleShapeY0, sampleShapeWidth, sampleShapeHeight);
    sampleShape2.setBoundsRelative(sampleShapeX0, sampleShapeY0 + sampleShapeHeight, sampleShapeWidth, sampleShapeHeight);
    sampleShape3.setBoundsRelative(sampleShapeX0, sampleShapeY0 + 2 * sampleShapeHeight, sampleShapeWidth, sampleShapeHeight);

    paramsBox1.setBoundsRelative(sampleShapeX0 + sampleShapeWidth + 0.01f, sampleShapeY0, paramsBoxWidth, sampleShapeHeight);
    paramsBox2.setBoundsRelative(sampleShapeX0 + sampleShapeWidth + 0.01f, sampleShapeY0 + sampleShapeHeight + 0.02f, paramsBoxWidth, sampleShapeHeight);
    paramsBox3.setBoundsRelative(sampleShapeX0 + sampleShapeWidth + 0.01f, sampleShapeY0 + 2 * (sampleShapeHeight + 0.02f), paramsBoxWidth, sampleShapeHeight);
}
