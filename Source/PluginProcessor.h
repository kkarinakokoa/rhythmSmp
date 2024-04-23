/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>
#include "customdelay.h"
#include "hassimager.h"
#include "UltimatePhraser.h"

//==============================================================================
/**
*/
class RhythmSmpAudioProcessor : public juce::AudioProcessor,
    public juce::ValueTree::Listener
{
public:
    //==============================================================================
    RhythmSmpAudioProcessor();
    ~RhythmSmpAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void loadFile(const juce::String& path, int samplerOrder, juce::String fileNm, int order);

    juce::AudioBuffer<float>& getWave(int order) { return _wave[order]; }

    void changeADSR();
    void changeRvb();
    void changeDelay();
    void changeImg();
    void changeGain();
    void changeFXOnAndOff();

    bool getOnOrOff(int boxOrder, int fxNum) { return fxSwitchOn[boxOrder][fxNum]; };
    void setFXChain(int boxOrder, int a, int b, int c);
    int* getFXChain(int boxOrder) { return (int *)fxChain[boxOrder]; }

    void delayDelegate1(float* bufL, float* bufR, int numSamples) { if (fxSwitchOn[0][0]) { delay1.process(bufL, bufR, numSamples, delayParams1); } }
    void delayDelegate2(float* bufL, float* bufR, int numSamples) { if (fxSwitchOn[1][0]) { delay2.process(bufL, bufR, numSamples, delayParams2); } }
    void delayDelegate3(float* bufL, float* bufR, int numSamples) { if (fxSwitchOn[2][0]) { delay3.process(bufL, bufR, numSamples, delayParams3); } }
    void imgDelegate1(float* bufL, float* bufR, int numSamples) { if (fxSwitchOn[0][1]) { hass1.process(bufL, bufR, numSamples, hassParams1); } }
    void imgDelegate2(float* bufL, float* bufR, int numSamples) { if (fxSwitchOn[1][1]) { hass2.process(bufL, bufR, numSamples, hassParams2); } }
    void imgDelegate3(float* bufL, float* bufR, int numSamples) { if (fxSwitchOn[2][1]) { hass3.process(bufL, bufR, numSamples, hassParams3); } }
    void rvbDelegate1(float* bufL, float* bufR, int numSamples) { if (fxSwitchOn[0][2]) { rvb1.processStereo(bufL, bufR, numSamples); } }
    void rvbDelegate2(float* bufL, float* bufR, int numSamples) { if (fxSwitchOn[1][2]) { rvb2.processStereo(bufL, bufR, numSamples); } }
    void rvbDelegate3(float* bufL, float* bufR, int numSamples) { if (fxSwitchOn[2][2]) { rvb3.processStereo(bufL, bufR, numSamples); } }

    void setVoices(int boxOrder, int num);
    int getVoices(int boxOrder) { return voiceNum[boxOrder]; }

    void setDownbeatOpt(int boxOrder, bool arg) { downbeatOption[boxOrder] = arg; }
    bool getDownbeatOpt(int boxOrder) { return downbeatOption[boxOrder]; }
    void setTenutoOpt(int boxOrder, bool arg) { tenutoOption[boxOrder] = arg; }
    bool getTenutoOpt(int boxOrder) { return tenutoOption[boxOrder]; }

    void setCreatemode(bool mode);
    std::atomic<bool>& getCreatemode() { return isCreatemode; }

    void setNoteDensities(int boxOrder, int num1, int num2) {
        if (num1 > num2) {
            num1 ^= num2;
            num2 ^= num1;
            num1 ^= num2;
        }
        noteDensityMin[boxOrder] = num1;
        noteDensityMax[boxOrder] = num2;
    }
    int getNoteDensityMin(int boxOrder) { return noteDensityMin[boxOrder]; }
    int getNoteDensityMax(int boxOrder) { return noteDensityMax[boxOrder]; }

    void createNewPhrases(int boxOrder) {
        switch (boxOrder) {
        case 0:
            phraser1.createPhrases(noteDensityMin[0], noteDensityMax[0], downbeatOption[0], tenutoOption[0]);
            restLength[0] = 0;
            isStarted[0] = false;
            break;
        case 1:
            phraser2.createPhrases(noteDensityMin[1], noteDensityMax[1], downbeatOption[1], tenutoOption[1]);
            restLength[1] = 0;
            isStarted[1] = false;
            break;
        case 2:
            phraser3.createPhrases(noteDensityMin[2], noteDensityMax[2], downbeatOption[2], tenutoOption[2]);
            restLength[2] = 0;
            isStarted[2] = false;
            break;
        }
        completeCreated += 40;
    }

    juce::ADSR::Parameters& getADSRParams(int order);
    juce::AudioProcessorValueTreeState& getAPVTS() { return APVTS; }
    std::atomic<bool>& isPlayed(int order) { return isPlaying[order]; }
    std::atomic<int>& getSampleCount(int order) { return sampleNum[order]; }
    juce::String getFileNm(int order) { return fileNames[order]; }

    int getCompleteCreated() { return (int)completeCreated; }
    void decreaseCompleteCreated() { completeCreated--; }

    //=====================================
    juce::Colour bgColors[2]{ juce::Colours::black, juce::Colours::white };
    juce::Colour trackColors[2]{ juce::Colours::black, juce::Colours::magenta };
    juce::Colour fontColors[2]{ juce::Colours::white, juce::Colours::magenta };
    juce::Colour fxfontColors[2]{ juce::Colours::white, juce::Colours::cyan }; //juce::Colour(0xff20006b)
    juce::Colour samplerBgColors1[2]{ juce::Colours::black, juce::Colours::white }; // non playing
    juce::Colour samplerBgColors2[2]{ juce::Colours::black, juce::Colours::white }; // playing
    juce::Colour samplerBars[2]{ juce::Colours::white, juce::Colours::magenta };
    juce::Colour samplerBarsMixed[2]{ juce::Colours::white, juce::Colours::cyan };
    juce::Colour samplerBarsMixed2[2]{ juce::Colours::white, juce::Colours::lightgoldenrodyellow };
    juce::Colour msgColors[2]{ juce::Colours::greenyellow, juce::Colour(0xffbff257) };

    int getTheme() { return theme; }
    void toggleTheme() { theme = (theme + 1) % 2; }

private:
    juce::Synthesiser Sampler, Sampler2, Sampler3;
    juce::AudioBuffer<float> _wave[3];

    juce::ADSR::Parameters ADSRParams1, ADSRParams2, ADSRParams3;

    juce::Reverb rvb;
    juce::Reverb::Parameters rvbParams;

    juce::Reverb rvb1, rvb2, rvb3;
    juce::Reverb::Parameters rvbParams1, rvbParams2, rvbParams3;

    customdelay delay1, delay2, delay3;
    delayParameters delayParams1, delayParams2, delayParams3;

    hassimager hass1, hass2, hass3;
    imagerParameters hassParams1, hassParams2, hassParams3;

    std::atomic<bool> isCreatemode{ false };
    
    bool isStarted[3]{ false, false, false };
    int time{ 0 }, startPos[3], endPos[3];
    int NoteNumTemp[3];
    int testLength, restLength[3];

    float gain1{ 1.0f }, gain2{ 1.0f }, gain3{ 1.0f };
    int velocity1{ 100 }, velocity2{ 100 }, velocity3{ 100 };

    juce::AudioFormatManager formatManager;
    juce::AudioFormatReader* formatReader{ nullptr };

    juce::AudioProcessorValueTreeState APVTS;
    juce::AudioProcessorValueTreeState::ParameterLayout makeParams();
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

    std::atomic<bool> isPlaying[3]{ false, false, false };
    std::atomic<int> sampleNum[3]{ 0, 0, 0 };
    std::atomic<int> currNotesNum[3]{ 0, 0, 0 };

    std::atomic<bool> fxSwitchOn[3][3]{ {false, false, false}, {false, false, false}, {false, false, false} }; // { {Delay, Imager, Reverb}, ... } order
    std::atomic<int> fxChain[3][3]{ {0, 1, 2}, {0, 1, 2}, {0, 1, 2} };
    std::function<void(float*, float*, int)> fxDelegate1[3]{ 
        [this](float* bufL, float* bufR, int num) {
            delayDelegate1(bufL, bufR, num);
            },  
        [this](float* bufL, float* bufR, int num) {
            imgDelegate1(bufL, bufR, num);
            },
        [this](float* bufL, float* bufR, int num) {
            rvbDelegate1(bufL, bufR, num);
            }
    };
    std::function<void(float*, float*, int)> fxDelegate2[3]{
        [this](float* bufL, float* bufR, int num) {
            delayDelegate2(bufL, bufR, num);
            },
        [this](float* bufL, float* bufR, int num) {
            imgDelegate2(bufL, bufR, num);
            },
        [this](float* bufL, float* bufR, int num) {
            rvbDelegate2(bufL, bufR, num);
            }
    };
    std::function<void(float*, float*, int)> fxDelegate3[3]{
        [this](float* bufL, float* bufR, int num) {
            delayDelegate3(bufL, bufR, num);
            },
        [this](float* bufL, float* bufR, int num) {
            imgDelegate3(bufL, bufR, num);
            },
        [this](float* bufL, float* bufR, int num) {
            rvbDelegate3(bufL, bufR, num);
            }
    };

    juce::String fileNames[3]{ "", "", "" };

    UltimatePhraser phraser1, phraser2, phraser3;
    int curr_rhythmIdx[3]{ 0, 0, 0 };
    bool ppp{ false };

    bool downbeatOption[3]{ false, false, false };
    bool tenutoOption[3]{ false, false, false };
    int noteDensityMin[3]{ 3, 3, 3 };
    int noteDensityMax[3]{ 7, 7, 7 };

    juce::AudioPlayHead* playHead;
    juce::AudioPlayHead::CurrentPositionInfo hostInfo;
    float bpm{ 120.0f };

    int voiceNum[3]{ 3, 3, 3 };
    std::atomic<int> completeCreated{ 0 };

    int theme{ 1 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RhythmSmpAudioProcessor)
};
