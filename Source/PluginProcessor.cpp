/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RhythmSmpAudioProcessor::RhythmSmpAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), APVTS(*this, nullptr, "PARAMETERS", makeParams())
#endif
{
    formatManager.registerBasicFormats();
    APVTS.state.addListener(this);

    for (int idx = 0; idx < voiceNum[0]; idx++)
    {
        Sampler.addVoice(new juce::SamplerVoice());
    }
    for (int idx = 0; idx < voiceNum[1]; idx++)
    {
        Sampler2.addVoice(new juce::SamplerVoice());
    }
    for (int idx = 0; idx < voiceNum[2]; idx++)
    {
        Sampler3.addVoice(new juce::SamplerVoice());
    }

}

RhythmSmpAudioProcessor::~RhythmSmpAudioProcessor()
{
    formatReader = nullptr;
}

//==============================================================================
const juce::String RhythmSmpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RhythmSmpAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool RhythmSmpAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool RhythmSmpAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double RhythmSmpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RhythmSmpAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int RhythmSmpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RhythmSmpAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String RhythmSmpAudioProcessor::getProgramName(int index)
{
    return {};
}

void RhythmSmpAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void RhythmSmpAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    Sampler.setCurrentPlaybackSampleRate(sampleRate);
    Sampler2.setCurrentPlaybackSampleRate(sampleRate);
    Sampler3.setCurrentPlaybackSampleRate(sampleRate);

    rvb1.setSampleRate(sampleRate);
    rvb2.setSampleRate(sampleRate);
    rvb3.setSampleRate(sampleRate);

    /*rvb.setSampleRate(sampleRate);
    rvbParams.dryLevel = 0;
    rvbParams.wetLevel = 1;
    rvbParams.roomSize = 0.5f;
    rvbParams.damping = 0.5f;
    rvbParams.width = 1;
    rvbParams.freezeMode = 0.4;
    rvb.setParameters(rvbParams);*/

    delay1.setSampleRate(sampleRate);
    delay2.setSampleRate(sampleRate);
    delay3.setSampleRate(sampleRate);

    /*delayParams1.delayTime = delay1.getSyncToMillisec(3, 16, 120);
    delayParams1.feedback = 0.4;
    delayParams1.ispingpong = true;
    delayParams1.wet = 0.15;*/

    hass1.setSampleRate(sampleRate);
    hass2.setSampleRate(sampleRate);
    hass3.setSampleRate(sampleRate);

    /*hassParams1.dry = 1;
    hassParams1.wet = juce::Decibels::decibelsToGain<float>(3);
    hassParams1.balance = juce::Decibels::decibelsToGain<float>(-3);
    hassParams1.delayTime = 10;*/

    phraser1.setSampleRate(sampleRate);
    phraser2.setSampleRate(sampleRate);
    phraser3.setSampleRate(sampleRate);

    
    if (!ppp) {
        phraser1.createPhrases(noteDensityMin[0], noteDensityMax[0], downbeatOption[0], tenutoOption[0]);
        phraser2.createPhrases(noteDensityMin[1], noteDensityMax[1], downbeatOption[1], tenutoOption[1]);
        phraser3.createPhrases(noteDensityMin[2], noteDensityMax[2], downbeatOption[2], tenutoOption[2]);

        ppp = true;
    }


    changeGain();
    changeADSR();
    changeRvb();
    changeDelay();
    changeImg();
    changeFXOnAndOff();

    //testLength = (int)(sampleRate * (60.0f / 120) * (4.0f / 16.0f)); // 16분음표
    isCreatemode = false;
}

void RhythmSmpAudioProcessor::releaseResources()
{
    currNotesNum[0] = currNotesNum[1] = currNotesNum[2] = 0;
    isPlaying[0] = isPlaying[1] = isPlaying[2] = false;
    
    startPos[0] = startPos[1] = startPos[2] = 0;
    endPos[0] = endPos[1] = endPos[2] = 0;
    restLength[0] = restLength[1] = restLength[2] = 0;

    rvb1.reset();
    rvb2.reset();
    rvb3.reset();

    delay1.clearDelayBuffers();
    delay2.clearDelayBuffers();
    delay3.clearDelayBuffers();

    hass1.clearImagerBuffers();
    hass2.clearImagerBuffers();
    hass3.clearImagerBuffers();

    phraser1.reset();
    phraser2.reset();
    phraser3.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RhythmSmpAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void RhythmSmpAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::AudioBuffer<float> subBuffer1(buffer.getNumChannels(), buffer.getNumSamples()),
        subBuffer2(buffer.getNumChannels(), buffer.getNumSamples()),
        subBuffer3(buffer.getNumChannels(), buffer.getNumSamples());

    for (int idx = totalNumInputChannels; idx < totalNumOutputChannels; idx++) {
        buffer.clear(idx, 0, buffer.getNumSamples());
        subBuffer1.clear(idx, 0, subBuffer1.getNumSamples());
        subBuffer2.clear(idx, 0, subBuffer2.getNumSamples());
        subBuffer3.clear(idx, 0, subBuffer3.getNumSamples());
    }

    juce::MidiBuffer phrasebuffer;

    if (isCreatemode) {
        juce::MidiMessage mp;
        juce::MidiBuffer::Iterator midiItp{ midiMessages };
        int samplePos;
        int offset[3]{ 0, 0, 0 };
        phrasebuffer.clear();

        playHead = this->getPlayHead();
        if (playHead != nullptr) {
            playHead->getCurrentPosition(hostInfo);
            bpm = (float)hostInfo.bpm;
        }
        else {
            bpm = 120;
        }

        while (midiItp.getNextEvent(mp, samplePos)) {
            if (mp.isNoteOn()) {
                int noteOrder = mp.getNoteNumber();
                int rhythmIdx;
                if ((noteOrder >= 48) && (noteOrder < 60)) {
                    rhythmIdx = noteOrder - 48;
                    curr_rhythmIdx[0] = rhythmIdx;

                    offset[0] = startPos[0] = samplePos;
                    NoteNumTemp[0] = 48;
                    if(phraser1.isNote(curr_rhythmIdx[0]))
                        phrasebuffer.addEvent(juce::MidiMessage::noteOn(1, NoteNumTemp[0], phraser1.popNextVelocity(curr_rhythmIdx[0])), startPos[0]);
                    isStarted[0] = true;
                    restLength[0] = phraser1.popNextLength(curr_rhythmIdx[0], bpm);
                }
                else if ((noteOrder >= 60) && (noteOrder < 72)) {
                    rhythmIdx = noteOrder - 60;
                    curr_rhythmIdx[1] = rhythmIdx;

                    offset[1] = startPos[1] = samplePos;
                    NoteNumTemp[1] = 60;
                    if (phraser2.isNote(curr_rhythmIdx[1]))
                        phrasebuffer.addEvent(juce::MidiMessage::noteOn(1, NoteNumTemp[1], phraser2.popNextVelocity(curr_rhythmIdx[1])), startPos[1]);
                    isStarted[1] = true;
                    restLength[1] = phraser2.popNextLength(curr_rhythmIdx[1], bpm);
                }
                else if ((noteOrder >= 72) && (noteOrder < 84)) {
                    rhythmIdx = noteOrder - 72;
                    curr_rhythmIdx[2] = rhythmIdx;

                    offset[2] = startPos[2] = samplePos;
                    NoteNumTemp[2] = 72;
                    if (phraser3.isNote(curr_rhythmIdx[2]))
                        phrasebuffer.addEvent(juce::MidiMessage::noteOn(1, NoteNumTemp[2], phraser3.popNextVelocity(curr_rhythmIdx[2])), startPos[2]);
                    isStarted[2] = true;
                    restLength[2] = phraser3.popNextLength(curr_rhythmIdx[2], bpm);
                }
                
            }
            else if (mp.isNoteOff()) {
                int noteOrder = mp.getNoteNumber();
                int rhythmIdx;
                if ((noteOrder >= 48) && (noteOrder < 60)) {
                    rhythmIdx = noteOrder - 48;

                    endPos[0] = samplePos;
                    restLength[0] = 0;
                    isStarted[0] = false;
                    phrasebuffer.addEvent(juce::MidiMessage::noteOff(1, NoteNumTemp[0]), endPos[0]);
                    phraser1.reset(curr_rhythmIdx[0]);
                }
                else if ((noteOrder >= 60) && (noteOrder < 72)) {
                    rhythmIdx = noteOrder - 60;

                    endPos[1] = samplePos;
                    restLength[1] = 0;
                    isStarted[1] = false;
                    phrasebuffer.addEvent(juce::MidiMessage::noteOff(1, NoteNumTemp[1]), endPos[1]);
                    phraser2.reset(curr_rhythmIdx[1]);
                }
                else if ((noteOrder >= 72) && (noteOrder < 84)) {
                    rhythmIdx = noteOrder - 72;

                    endPos[2] = samplePos;
                    restLength[2] = 0;
                    isStarted[2] = false;
                    phrasebuffer.addEvent(juce::MidiMessage::noteOff(1, NoteNumTemp[2]), endPos[2]);
                    phraser3.reset(curr_rhythmIdx[2]);
                }
            }
        }

        if (isStarted[0]) {
            if (restLength[0] < buffer.getNumSamples()) {
                offset[0] = restLength[0];
                if (phraser1.isNote(curr_rhythmIdx[0])) {
                    phrasebuffer.addEvent(juce::MidiMessage::noteOff(1, NoteNumTemp[0]), offset[0]);
                }
                phraser1.updateNext(curr_rhythmIdx[0]);
                if(phraser1.isNote(curr_rhythmIdx[0])) {
                    phrasebuffer.addEvent(juce::MidiMessage::noteOn(1, NoteNumTemp[0], phraser1.popNextVelocity(curr_rhythmIdx[0])), offset[0]);
                }
                restLength[0] = phraser1.popNextLength(curr_rhythmIdx[0], bpm);
            }
            restLength[0] -= buffer.getNumSamples() - offset[0];
        }
        if (isStarted[1]) {
            if (restLength[1] < buffer.getNumSamples()) {
                offset[1] = restLength[1];
                if (phraser2.isNote(curr_rhythmIdx[1])) {
                    phrasebuffer.addEvent(juce::MidiMessage::noteOff(1, NoteNumTemp[1]), offset[1]);
                }
                phraser2.updateNext(curr_rhythmIdx[1]);
                if (phraser2.isNote(curr_rhythmIdx[1])) {
                    phrasebuffer.addEvent(juce::MidiMessage::noteOn(1, NoteNumTemp[1], phraser2.popNextVelocity(curr_rhythmIdx[1])), offset[1]);
                }
                restLength[1] = phraser2.popNextLength(curr_rhythmIdx[1], bpm);
            }
            restLength[1] -= buffer.getNumSamples() - offset[1];
        }
        if (isStarted[2]) {
            if (restLength[2] < buffer.getNumSamples()) {
                offset[2] = restLength[2];
                if (phraser3.isNote(curr_rhythmIdx[2])) {
                    phrasebuffer.addEvent(juce::MidiMessage::noteOff(1, NoteNumTemp[2]), offset[2]);
                }
                phraser3.updateNext(curr_rhythmIdx[2]);
                if (phraser3.isNote(curr_rhythmIdx[2])) {
                    phrasebuffer.addEvent(juce::MidiMessage::noteOn(1, NoteNumTemp[2], phraser3.popNextVelocity(curr_rhythmIdx[2])), offset[2]);
                }
                restLength[2] = phraser3.popNextLength(curr_rhythmIdx[2], bpm);
            }
            restLength[2] -= buffer.getNumSamples() - offset[2];
        }
        /*else {
            phrasebuffer.addEvent(juce::MidiMessage::noteOff(1, NoteNumTemp), endPos);
        }*/

        

        midiMessages.swapWith(phrasebuffer);
        //return;
    }


    juce::MidiMessage m;
    juce::MidiBuffer::Iterator midiIt{ midiMessages };
    int samplePos;
    //DBG("[[[" << blockIndex << "th block]]]"); // 개빠름 진짜
    //DBG("[[[AudioBuffer Size]]] " << buffer.getNumSamples()); // 480

    while (midiIt.getNextEvent(m, samplePos)) {
        //DBG("[m] " << m.getNoteNumber());
        //DBG("[samplepos] " << samplePos);
        if (m.isNoteOn()) {
            int noteOrder = m.getNoteNumber();
            if ((noteOrder >= 48) && (noteOrder < 60)) {
                if (!isPlaying[0]) isPlaying[0] = true;
                currNotesNum[0]++;
            }
            else if ((noteOrder >= 60) && (noteOrder < 72)) {
                if (!isPlaying[1]) isPlaying[1] = true;
                currNotesNum[1]++;
            }
            else if ((noteOrder >= 72) && (noteOrder < 84)) {
                if (!isPlaying[2]) isPlaying[2] = true;
                currNotesNum[2]++;
            }
        }
        else if (m.isNoteOff()) {
            int noteOrder = m.getNoteNumber();
            if ((noteOrder >= 48) && (noteOrder < 60)) {
                if (currNotesNum[0] > 1) {
                    sampleNum[0] = 0;
                    currNotesNum[0]--;
                }
                else if (currNotesNum[0] == 1) {
                    isPlaying[0] = false;
                    sampleNum[0] = 0;
                    currNotesNum[0]--;
                }
                else {
                    isPlaying[0] = false;
                    sampleNum[0] = 0;
                }
            }
            else if ((noteOrder >= 60) && (noteOrder < 72)) {
                if (currNotesNum[1] > 1) {
                    sampleNum[1] = 0;
                    currNotesNum[1]--;
                }
                else if (currNotesNum[1] == 1) {
                    isPlaying[1] = false;
                    sampleNum[1] = 0;
                    currNotesNum[1]--;
                }
                else {
                    isPlaying[1] = false;
                    sampleNum[1] = 0;
                }
            }
            else if ((noteOrder >= 72) && (noteOrder < 84)) {
                if (currNotesNum[2] > 1) {
                    sampleNum[2] = 0;
                    currNotesNum[2]--;
                }
                else if (currNotesNum[2] == 1) {
                    isPlaying[2] = false;
                    sampleNum[2] = 0;
                    currNotesNum[2]--;
                }
                else {
                    isPlaying[2] = false;
                    sampleNum[2] = 0;
                }
            }
        }
    }

    do {
        if (isPlaying[0])
            sampleNum[0] += buffer.getNumSamples();
        else
            sampleNum[0] = 0;

        if (isPlaying[1])
            sampleNum[1] += buffer.getNumSamples();
        else
            sampleNum[1] = 0;

        if (isPlaying[2])
            sampleNum[2] += buffer.getNumSamples();
        else
            sampleNum[2] = 0;
    } while (false);



    Sampler.renderNextBlock(subBuffer1, midiMessages, 0, subBuffer1.getNumSamples());
    fxDelegate1[fxChain[0][0]](subBuffer1.getWritePointer(0), subBuffer1.getWritePointer(1), subBuffer1.getNumSamples());
    fxDelegate1[fxChain[0][1]](subBuffer1.getWritePointer(0), subBuffer1.getWritePointer(1), subBuffer1.getNumSamples());
    fxDelegate1[fxChain[0][2]](subBuffer1.getWritePointer(0), subBuffer1.getWritePointer(1), subBuffer1.getNumSamples());

    Sampler2.renderNextBlock(subBuffer2, midiMessages, 0, subBuffer2.getNumSamples());
    fxDelegate2[fxChain[1][0]](subBuffer2.getWritePointer(0), subBuffer2.getWritePointer(1), subBuffer2.getNumSamples());
    fxDelegate2[fxChain[1][1]](subBuffer2.getWritePointer(0), subBuffer2.getWritePointer(1), subBuffer2.getNumSamples());
    fxDelegate2[fxChain[1][2]](subBuffer2.getWritePointer(0), subBuffer2.getWritePointer(1), subBuffer2.getNumSamples());

    Sampler3.renderNextBlock(subBuffer3, midiMessages, 0, subBuffer3.getNumSamples());
    fxDelegate3[fxChain[2][0]](subBuffer3.getWritePointer(0), subBuffer3.getWritePointer(1), subBuffer3.getNumSamples());
    fxDelegate3[fxChain[2][1]](subBuffer3.getWritePointer(0), subBuffer3.getWritePointer(1), subBuffer3.getNumSamples());
    fxDelegate3[fxChain[2][2]](subBuffer3.getWritePointer(0), subBuffer3.getWritePointer(1), subBuffer3.getNumSamples());

    for (int channel = 0; channel < totalNumOutputChannels; channel++)
    {
        auto* channelData = buffer.getWritePointer(channel);
        auto* subData1 = subBuffer1.getReadPointer(channel);
        auto* subData2 = subBuffer2.getReadPointer(channel);
        auto* subData3 = subBuffer3.getReadPointer(channel);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            channelData[i] = subData1[i] * gain1;
            channelData[i] += subData2[i] * gain2;
            channelData[i] += subData3[i] * gain3;
        }
    }
   
}

//==============================================================================
bool RhythmSmpAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RhythmSmpAudioProcessor::createEditor()
{
    return new RhythmSmpAudioProcessorEditor(*this);
}

//==============================================================================
void RhythmSmpAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void RhythmSmpAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void RhythmSmpAudioProcessor::loadFile(const juce::String& path, int samplerOrder, juce::String fileNm, int order) // Main loadFile
{
    juce::Synthesiser* currentSampler{};
    
    juce::BigInteger range[3];
    //range.setRange(0, 128, true);
    range[0].setRange(48, 12, true);
    range[1].setRange(60, 12, true);
    range[2].setRange(72, 12, true);
    juce::String name;
    int midinormalpitch{};

    switch (samplerOrder)
    {
    case 0:
        currentSampler = &Sampler;
        name = "Sampler1";
        midinormalpitch = 48;
        break;
    case 1:
        currentSampler = &Sampler2;
        name = "Sampler2";
        midinormalpitch = 60;
        break;
    case 2:
        currentSampler = &Sampler3;
        name = "Sampler3";
        midinormalpitch = 72;
        break;
    default:
        currentSampler = &Sampler;
        name = "Sampler1";
        midinormalpitch = 48;
    }
    currentSampler->clearSounds();

    juce::File f = juce::File(path);
    formatReader = formatManager.createReaderFor(f);

    auto sampleLength = static_cast<int>(formatReader->lengthInSamples);

    _wave[samplerOrder].setSize(1, sampleLength);
    formatReader->read(&_wave[samplerOrder], 0, sampleLength, 0, true, false);

    fileNames[order] = fileNm;

    currentSampler->addSound(new juce::SamplerSound(name, *formatReader, range[samplerOrder], midinormalpitch, 0.1, 0.1, 10.0)); // 60: C3
    //currentSampler->addSound(new juce::SamplerSound("Sampler", *formatReader, range, 60, 0.1, 0.1, 10.0)); // 60: C3

    changeADSR();

}

void RhythmSmpAudioProcessor::changeRvb()
{
    rvbParams1.wetLevel = (float)(APVTS.getRawParameterValue("WET1")->load())/100;
    rvbParams1.dryLevel = 1;// -rvbParams1.wetLevel;
    rvbParams1.roomSize = APVTS.getRawParameterValue("ROOMSIZE1")->load();
    rvbParams1.damping = APVTS.getRawParameterValue("DAMPING1")->load();
    rvbParams1.width = APVTS.getRawParameterValue("WIDTH1")->load();
    rvbParams1.freezeMode = (float)(APVTS.getRawParameterValue("FEEDBACK1")->load()) / 100;
    rvb1.setParameters(rvbParams1);

    rvbParams2.wetLevel = (float)(APVTS.getRawParameterValue("WET2")->load()) / 100;
    rvbParams2.dryLevel = 1;// -rvbParams2.wetLevel;
    rvbParams2.roomSize = APVTS.getRawParameterValue("ROOMSIZE2")->load();
    rvbParams2.damping = APVTS.getRawParameterValue("DAMPING2")->load();
    rvbParams2.width = APVTS.getRawParameterValue("WIDTH2")->load();
    rvbParams2.freezeMode = (float)(APVTS.getRawParameterValue("FEEDBACK2")->load()) / 100;
    rvb2.setParameters(rvbParams2);

    rvbParams3.wetLevel = (float)(APVTS.getRawParameterValue("WET3")->load()) / 100;
    rvbParams3.dryLevel = 1;// -rvbParams3.wetLevel;
    rvbParams3.roomSize = APVTS.getRawParameterValue("ROOMSIZE3")->load();
    rvbParams3.damping = APVTS.getRawParameterValue("DAMPING3")->load();
    rvbParams3.width = APVTS.getRawParameterValue("WIDTH3")->load();
    rvbParams3.freezeMode = (float)(APVTS.getRawParameterValue("FEEDBACK3")->load()) / 100;
    rvb3.setParameters(rvbParams3);
}

void RhythmSmpAudioProcessor::changeDelay()
{
    delayParams1.wet = (float)(APVTS.getRawParameterValue("DELAYWET1")->load()) / 100;
    delayParams2.wet = (float)(APVTS.getRawParameterValue("DELAYWET2")->load()) / 100;
    delayParams3.wet = (float)(APVTS.getRawParameterValue("DELAYWET3")->load()) / 100;

    delayParams1.feedback = (float)(APVTS.getRawParameterValue("DELAYFB1")->load()) / 100;
    delayParams2.feedback = (float)(APVTS.getRawParameterValue("DELAYFB2")->load()) / 100;
    delayParams3.feedback = (float)(APVTS.getRawParameterValue("DELAYFB3")->load()) / 100;
    
    delayParams1.ispingpong = APVTS.getRawParameterValue("PINGPONG1")->load();
    delayParams2.ispingpong = APVTS.getRawParameterValue("PINGPONG2")->load();
    delayParams3.ispingpong = APVTS.getRawParameterValue("PINGPONG3")->load();

    int dt1 = APVTS.getRawParameterValue("DELAYTIME1")->load();
    int dt2 = APVTS.getRawParameterValue("DELAYTIME2")->load();
    int dt3 = APVTS.getRawParameterValue("DELAYTIME3")->load();
    juce::StringArray delayTime3{ "1/1", "3/4", "1/2", "3/8", "1/4", "3/16", "1/8", "3/32", "1/16", "3/64", "1/32" };

    switch (dt1) {
    case 0:
        delayParams1.delayTime = delay1.getSyncToMillisec(1, 1, bpm);
        break;
    case 1:
        delayParams1.delayTime = delay1.getSyncToMillisec(3, 4, bpm);
        break;
    case 2:
        delayParams1.delayTime = delay1.getSyncToMillisec(1, 2, bpm);
        break;
    case 3:
        delayParams1.delayTime = delay1.getSyncToMillisec(3, 8, bpm);
        break;
    case 4:
        delayParams1.delayTime = delay1.getSyncToMillisec(1, 4, bpm);
        break;
    case 5:
        delayParams1.delayTime = delay1.getSyncToMillisec(3, 16, bpm);
        break;
    case 6:
        delayParams1.delayTime = delay1.getSyncToMillisec(1, 8, bpm);
        break;
    case 7:
        delayParams1.delayTime = delay1.getSyncToMillisec(3, 32, bpm);
        break;
    case 8:
        delayParams1.delayTime = delay1.getSyncToMillisec(1, 16, bpm);
        break;
    case 9:
        delayParams1.delayTime = delay1.getSyncToMillisec(3, 64, bpm);
        break;
    case 10:
        delayParams1.delayTime = delay1.getSyncToMillisec(1, 32, bpm);
        break;
    }
    switch (dt2) {
    case 0:
        delayParams2.delayTime = delay2.getSyncToMillisec(1, 1, bpm);
        break;
    case 1:
        delayParams2.delayTime = delay2.getSyncToMillisec(3, 4, bpm);
        break;
    case 2:
        delayParams2.delayTime = delay2.getSyncToMillisec(1, 2, bpm);
        break;
    case 3:
        delayParams2.delayTime = delay2.getSyncToMillisec(3, 8, bpm);
        break;
    case 4:
        delayParams2.delayTime = delay2.getSyncToMillisec(1, 4, bpm);
        break;
    case 5:
        delayParams2.delayTime = delay2.getSyncToMillisec(3, 16, bpm);
        break;
    case 6:
        delayParams2.delayTime = delay2.getSyncToMillisec(1, 8, bpm);
        break;
    case 7:
        delayParams2.delayTime = delay2.getSyncToMillisec(3, 32, bpm);
        break;
    case 8:
        delayParams2.delayTime = delay2.getSyncToMillisec(1, 16, bpm);
        break;
    case 9:
        delayParams2.delayTime = delay2.getSyncToMillisec(3, 64, bpm);
        break;
    case 10:
        delayParams2.delayTime = delay2.getSyncToMillisec(1, 32, bpm);
        break;
    }
    switch (dt3) {
    case 0:
        delayParams3.delayTime = delay3.getSyncToMillisec(1, 1, bpm);
        break;
    case 1:
        delayParams3.delayTime = delay3.getSyncToMillisec(3, 4, bpm);
        break;
    case 2:
        delayParams3.delayTime = delay3.getSyncToMillisec(1, 2, bpm);
        break;
    case 3:
        delayParams3.delayTime = delay3.getSyncToMillisec(3, 8, bpm);
        break;
    case 4:
        delayParams3.delayTime = delay3.getSyncToMillisec(1, 4, bpm);
        break;
    case 5:
        delayParams3.delayTime = delay3.getSyncToMillisec(3, 16, bpm);
        break;
    case 6:
        delayParams3.delayTime = delay3.getSyncToMillisec(1, 8, bpm);
        break;
    case 7:
        delayParams3.delayTime = delay3.getSyncToMillisec(3, 32, bpm);
        break;
    case 8:
        delayParams3.delayTime = delay3.getSyncToMillisec(1, 16, bpm);
        break;
    case 9:
        delayParams3.delayTime = delay3.getSyncToMillisec(3, 64, bpm);
        break;
    case 10:
        delayParams3.delayTime = delay3.getSyncToMillisec(1, 32, bpm);
        break;
    }
    
}

void RhythmSmpAudioProcessor::changeImg()
{
    hassParams1.dry = (float)(APVTS.getRawParameterValue("IMGDRY1")->load()) / 100;
    hassParams2.dry = (float)(APVTS.getRawParameterValue("IMGDRY2")->load()) / 100;
    hassParams3.dry = (float)(APVTS.getRawParameterValue("IMGDRY3")->load()) / 100;

    hassParams1.wet = (float)(APVTS.getRawParameterValue("IMGWET1")->load()) / 100;
    hassParams2.wet = (float)(APVTS.getRawParameterValue("IMGWET2")->load()) / 100;
    hassParams3.wet = (float)(APVTS.getRawParameterValue("IMGWET3")->load()) / 100;

    hassParams1.delayTime = APVTS.getRawParameterValue("IMGDT1")->load();
    hassParams2.delayTime = APVTS.getRawParameterValue("IMGDT2")->load();
    hassParams3.delayTime = APVTS.getRawParameterValue("IMGDT3")->load();

    hassParams1.balance = juce::Decibels::decibelsToGain<float>(APVTS.getRawParameterValue("BAL1")->load());
    hassParams2.balance = juce::Decibels::decibelsToGain<float>(APVTS.getRawParameterValue("BAL2")->load());
    hassParams3.balance = juce::Decibels::decibelsToGain<float>(APVTS.getRawParameterValue("BAL3")->load());
}

void RhythmSmpAudioProcessor::changeGain()
{
    gain1 = juce::Decibels::decibelsToGain<float>(APVTS.getRawParameterValue("GAIN1")->load());
    gain2 = juce::Decibels::decibelsToGain<float>(APVTS.getRawParameterValue("GAIN2")->load());
    gain3 = juce::Decibels::decibelsToGain<float>(APVTS.getRawParameterValue("GAIN3")->load());
}

void RhythmSmpAudioProcessor::changeFXOnAndOff()
{
    fxSwitchOn[0][0] = APVTS.getRawParameterValue("DELAYON1")->load();
    fxSwitchOn[0][1] = APVTS.getRawParameterValue("IMGON1")->load();
    fxSwitchOn[0][2] = APVTS.getRawParameterValue("RVBON1")->load();

    fxSwitchOn[1][0] = APVTS.getRawParameterValue("DELAYON2")->load();
    fxSwitchOn[1][1] = APVTS.getRawParameterValue("IMGON2")->load();
    fxSwitchOn[1][2] = APVTS.getRawParameterValue("RVBON2")->load();

    fxSwitchOn[2][0] = APVTS.getRawParameterValue("DELAYON3")->load();
    fxSwitchOn[2][1] = APVTS.getRawParameterValue("IMGON3")->load();
    fxSwitchOn[2][2] = APVTS.getRawParameterValue("RVBON3")->load();
}

void RhythmSmpAudioProcessor::changeADSR()
{
    ADSRParams1.attack = APVTS.getRawParameterValue("ATTACK1")->load();
    ADSRParams1.decay = APVTS.getRawParameterValue("DECAY1")->load();
    ADSRParams1.sustain = APVTS.getRawParameterValue("SUSTAIN1")->load();
    ADSRParams1.release = APVTS.getRawParameterValue("RELEASE1")->load();

    ADSRParams2.attack = APVTS.getRawParameterValue("ATTACK2")->load();
    ADSRParams2.decay = APVTS.getRawParameterValue("DECAY2")->load();
    ADSRParams2.sustain = APVTS.getRawParameterValue("SUSTAIN2")->load();
    ADSRParams2.release = APVTS.getRawParameterValue("RELEASE2")->load();

    ADSRParams3.attack = APVTS.getRawParameterValue("ATTACK3")->load();
    ADSRParams3.decay = APVTS.getRawParameterValue("DECAY3")->load();
    ADSRParams3.sustain = APVTS.getRawParameterValue("SUSTAIN3")->load();
    ADSRParams3.release = APVTS.getRawParameterValue("RELEASE3")->load();

    for (int idx = 0; idx < Sampler.getNumSounds(); idx++) {
        if (auto sound = dynamic_cast<juce::SamplerSound*>(Sampler.getSound(idx).get()))
            sound->setEnvelopeParameters(ADSRParams1);

        if (auto sound = dynamic_cast<juce::SamplerSound*>(Sampler2.getSound(idx).get()))
            sound->setEnvelopeParameters(ADSRParams2);

        if (auto sound = dynamic_cast<juce::SamplerSound*>(Sampler3.getSound(idx).get()))
            sound->setEnvelopeParameters(ADSRParams3);
    }

}

juce::AudioProcessorValueTreeState::ParameterLayout RhythmSmpAudioProcessor::makeParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> temp;

    // Gain
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN1", "Gain1", -30.0f, 3.0f, 0.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN2", "Gain2", -30.0f, 3.0f, 0.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN3", "Gain3", -30.0f, 3.0f, 0.0f));

    // ADSR
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK1", "Attack1", 0.0f, 5.0f, 0.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY1", "Decay1", 0.0f, 4.0f, 1.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN1", "Sustain1", 0.0f, 1.0f, 1.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE1", "Release1", 0.0f, 10.0f, 2.0f));

    temp.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK2", "Attack2", 0.0f, 5.0f, 0.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY2", "Decay2", 0.0f, 4.0f, 1.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN2", "Sustain2", 0.0f, 1.0f, 1.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE2", "Release2", 0.0f, 10.0f, 2.0f));

    temp.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK3", "Attack3", 0.0f, 5.0f, 0.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY3", "Decay3", 0.0f, 4.0f, 1.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN3", "Sustain3", 0.0f, 1.0f, 1.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE3", "Release3", 0.0f, 10.0f, 2.0f));

    // Reverbs
    temp.push_back(std::make_unique<juce::AudioParameterInt>("WET1", "Wet1", 0, 100, 0));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("WET2", "Wet2", 0, 100, 0));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("WET3", "Wet3", 0, 100, 0));

    temp.push_back(std::make_unique<juce::AudioParameterFloat>("ROOMSIZE1", "Roomsize1", 0.0f, 1.0f, 0.5f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("ROOMSIZE2", "Roomsize2", 0.0f, 1.0f, 0.5f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("ROOMSIZE3", "Roomsize3", 0.0f, 1.0f, 0.5f));

    temp.push_back(std::make_unique<juce::AudioParameterFloat>("DAMPING1", "Damping1", 0.0f, 1.0f, 0.5f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("DAMPING2", "Damping2", 0.0f, 1.0f, 0.5f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("DAMPING3", "Damping3", 0.0f, 1.0f, 0.5f));

    temp.push_back(std::make_unique<juce::AudioParameterFloat>("WIDTH1", "Width1", 0.0f, 1.0f, 0.5f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("WIDTH2", "Width2", 0.0f, 1.0f, 0.5f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("WIDTH3", "Width3", 0.0f, 1.0f, 0.5f));

    temp.push_back(std::make_unique<juce::AudioParameterInt>("FEEDBACK1", "Feedback1", 0, 100, 50));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("FEEDBACK2", "Feedback2", 0, 100, 50));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("FEEDBACK3", "Feedback3", 0, 100, 50));

    // Delays
    temp.push_back(std::make_unique<juce::AudioParameterInt>("DELAYWET1", "DelayWet1", 0, 100, 0));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("DELAYWET2", "DelayWet2", 0, 100, 0));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("DELAYWET3", "DelayWet3", 0, 100, 0));

    temp.push_back(std::make_unique<juce::AudioParameterInt>("DELAYFB1", "DelayFeedback1", 0, 100, 50));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("DELAYFB2", "DelayFeedback2", 0, 100, 50));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("DELAYFB3", "DelayFeedback3", 0, 100, 50));

    temp.push_back(std::make_unique<juce::AudioParameterBool>("PINGPONG1", "PingpongMode1", true));
    temp.push_back(std::make_unique<juce::AudioParameterBool>("PINGPONG2", "PingpongMode2", true));
    temp.push_back(std::make_unique<juce::AudioParameterBool>("PINGPONG3", "PingpongMode3", true));

    juce::StringArray delayTime1{ "1/1", "3/4", "1/2", "3/8", "1/4", "3/16", "1/8", "3/32", "1/16", "3/64", "1/32" };
    juce::StringArray delayTime2{ "1/1", "3/4", "1/2", "3/8", "1/4", "3/16", "1/8", "3/32", "1/16", "3/64", "1/32" };
    juce::StringArray delayTime3{ "1/1", "3/4", "1/2", "3/8", "1/4", "3/16", "1/8", "3/32", "1/16", "3/64", "1/32" };
    temp.push_back(std::make_unique<juce::AudioParameterChoice>("DELAYTIME1", "DelayTime1", delayTime1, 3));
    temp.push_back(std::make_unique<juce::AudioParameterChoice>("DELAYTIME2", "DelayTime2", delayTime2, 3));
    temp.push_back(std::make_unique<juce::AudioParameterChoice>("DELAYTIME3", "DelayTime3", delayTime3, 3));

    // Imagers
    temp.push_back(std::make_unique<juce::AudioParameterInt>("IMGWET1", "ImagerWet1", 0, 100, 0));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("IMGWET2", "ImagerWet2", 0, 100, 0));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("IMGWET3", "ImagerWet3", 0, 100, 0));

    temp.push_back(std::make_unique<juce::AudioParameterInt>("IMGDRY1", "ImagerDry1", 0, 100, 100));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("IMGDRY2", "ImagerDry2", 0, 100, 100));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("IMGDRY3", "ImagerDry3", 0, 100, 100));

    temp.push_back(std::make_unique<juce::AudioParameterInt>("IMGDT1", "ImagerDelayTime1", 10, 30, 20));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("IMGDT2", "ImagerDelayTime2", 10, 30, 20));
    temp.push_back(std::make_unique<juce::AudioParameterInt>("IMGDT3", "ImagerDelayTime3", 10, 30, 20));

    temp.push_back(std::make_unique<juce::AudioParameterFloat>("BAL1", "ImagerGainBalancing1", -30.0f, 3.0f, 0.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("BAL2", "ImagerGainBalancing2", -30.0f, 3.0f, 0.0f));
    temp.push_back(std::make_unique<juce::AudioParameterFloat>("BAL3", "ImagerGainBalancing3", -30.0f, 3.0f, 0.0f));

    // Effect On & Off
    temp.push_back(std::make_unique<juce::AudioParameterBool>("DELAYON1", "DelayOn1", false));
    temp.push_back(std::make_unique<juce::AudioParameterBool>("DELAYON2", "DelayOn2", false));
    temp.push_back(std::make_unique<juce::AudioParameterBool>("DELAYON3", "DelayOn3", false));

    temp.push_back(std::make_unique<juce::AudioParameterBool>("IMGON1", "ImagerOn1", false));
    temp.push_back(std::make_unique<juce::AudioParameterBool>("IMGON2", "ImagerOn2", false));
    temp.push_back(std::make_unique<juce::AudioParameterBool>("IMGON3", "ImagerOn3", false));

    temp.push_back(std::make_unique<juce::AudioParameterBool>("RVBON1", "ReverbOn1", false));
    temp.push_back(std::make_unique<juce::AudioParameterBool>("RVBON2", "ReverbOn2", false));
    temp.push_back(std::make_unique<juce::AudioParameterBool>("RVBON3", "ReverbOn3", false));


    return { temp.begin(), temp.end() };
}

void RhythmSmpAudioProcessor::setFXChain(int boxOrder, int a, int b, int c)
{
    fxChain[boxOrder][0] = a;
    fxChain[boxOrder][1] = b;
    fxChain[boxOrder][2] = c;
}

void RhythmSmpAudioProcessor::setVoices(int boxOrder, int num)
{
    if (num > voiceNum[boxOrder]) {
        if (boxOrder == 0) {
            for (int idx = 0; idx < num - voiceNum[0]; idx++)
            {
                Sampler.addVoice(new juce::SamplerVoice());
            }
        }
        else if (boxOrder == 1) {
            for (int idx = 0; idx < num - voiceNum[0]; idx++)
            {
                Sampler2.addVoice(new juce::SamplerVoice());
            }
        }
        else if (boxOrder == 2) {
            for (int idx = 0; idx < num - voiceNum[0]; idx++)
            {
                Sampler3.addVoice(new juce::SamplerVoice());
            }
        }

        voiceNum[boxOrder] = num;
    }
    else if (num < voiceNum[boxOrder]) {
        if (boxOrder == 0) {
            for (int idx = voiceNum[0]; idx >= 0; idx--)
            {
                Sampler.removeVoice(idx);
            }
            for (int idx = 0; idx < num; idx++)
            {
                Sampler.addVoice(new juce::SamplerVoice());
            }
        }
        else if (boxOrder == 1) {
            for (int idx = voiceNum[1]; idx >= 0; idx--)
            {
                Sampler2.removeVoice(idx);
            }
            for (int idx = 0; idx < num; idx++)
            {
                Sampler2.addVoice(new juce::SamplerVoice());
            }
        }
        else if (boxOrder == 2) {
            for (int idx = voiceNum[2]; idx >= 0; idx--)
            {
                Sampler3.removeVoice(idx);
            }
            for (int idx = 0; idx < num; idx++)
            {
                Sampler3.addVoice(new juce::SamplerVoice());
            }
        }

        voiceNum[boxOrder] = num;
    }
}

void RhythmSmpAudioProcessor::setCreatemode(bool mode)
{   
    bool previousCreatemode = isCreatemode;
    isCreatemode = mode;
    if ((isCreatemode) && (previousCreatemode != isCreatemode)) {
        for (int idx = voiceNum[0]; idx >= 0; idx--)
        {
            Sampler.removeVoice(idx);
        }
        for (int idx = voiceNum[1]; idx >= 0; idx--)
        {
            Sampler2.removeVoice(idx);
        }
        for (int idx = voiceNum[2]; idx >= 0; idx--)
        {
            Sampler3.removeVoice(idx);
        }
        Sampler.addVoice(new juce::SamplerVoice());
        Sampler2.addVoice(new juce::SamplerVoice());
        Sampler3.addVoice(new juce::SamplerVoice());
    }
    else if ((!isCreatemode) && (previousCreatemode != isCreatemode)) {
        Sampler.removeVoice(0);
        Sampler2.removeVoice(0);
        Sampler3.removeVoice(0);
        for (int idx = 0; idx < voiceNum[0]; idx++)
        {
            Sampler.addVoice(new juce::SamplerVoice());
        }
        for (int idx = 0; idx < voiceNum[1]; idx++)
        {
            Sampler2.addVoice(new juce::SamplerVoice());
        }
        for (int idx = 0; idx < voiceNum[2]; idx++)
        {
            Sampler3.addVoice(new juce::SamplerVoice());
        }
    }
}

juce::ADSR::Parameters& RhythmSmpAudioProcessor::getADSRParams(int order) {
    switch (order) {
    case 0:
        return ADSRParams1;
        break;
    case 1:
        return ADSRParams2;
        break;
    case 2:
        return ADSRParams3;
        break;
    }
}

void RhythmSmpAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    changeGain();
    changeADSR();
    changeRvb();
    changeDelay();
    changeImg();
    changeFXOnAndOff();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RhythmSmpAudioProcessor();
}
