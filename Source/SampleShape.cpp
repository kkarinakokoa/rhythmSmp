#include <JuceHeader.h>
#include <math.h>
#include "SampleShape.h"

//==============================================================================
SampleShape::SampleShape(RhythmSmpAudioProcessor& p, int order) : audioProcessor(p), samplerOrder(order)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

SampleShape::~SampleShape()
{
}

void SampleShape::paint(juce::Graphics& g)
{
    //g.fillAll(juce::Colours::magenta.darker());
    
    int theme = audioProcessor.getTheme();
    juce::AudioBuffer<float> _wave = audioProcessor.getWave(samplerOrder);

    if (_wave.getNumSamples() > 0)
    {
        g.fillAll(audioProcessor.samplerBgColors2[theme]);

        juce::Path p, p_rest;
        audioData.clear();

        auto _wave = audioProcessor.getWave(samplerOrder);
        auto ratio = _wave.getNumSamples() / getWidth();
        auto buffer = _wave.getReadPointer(0);

        for (int sample = 0; sample < _wave.getNumSamples(); sample += ratio)
        {
            audioData.push_back(buffer[sample]);
        }

        if (audioProcessor.isPlayed(samplerOrder)) {

            const auto area = getLocalBounds();

            const juce::Colour startColor = audioProcessor.samplerBgColors2[theme];
            const juce::Colour endColor = audioProcessor.samplerBgColors2[theme];
            auto playHeadPosition = juce::jmap<int>(audioProcessor.getSampleCount(samplerOrder), 0, audioProcessor.getWave(samplerOrder).getNumSamples(),
                0, getWidth());

            auto maxFreq = 20;
            auto freq = maxFreq - juce::jmap<int>(audioProcessor.getSampleCount(samplerOrder), 0, audioProcessor.getWave(samplerOrder).getNumSamples(),
                0, 20);

            float offset = (std::cos((samplerOrder + 1.5f) * (float)audioProcessor.getSampleCount(samplerOrder) / 37013)) / 8;

            float ampX0 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) / 500)) / 17;
            float ampdX0 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);

            juce::Point<float> startP0 = { ((float)area.getWidth() / 7.0f) * (1 + 6 * ampX0 + 6 * offset - 0.3f) - ((float)area.getWidth() / 130.0f) * ampdX0, (float)area.getHeight() / 2.0f };
            juce::Point<float> endP0 = { ((float)area.getWidth() / 7.0f) * (1 + 6 * ampX0 + 6 * offset - 0.3f) + ((float)area.getWidth() / 130.0f) * ampdX0, (float)area.getHeight() / 2.0f };

            float ampX1 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) / 300)) / 30;
            float ampdX1 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startP1 = { ((float)area.getWidth() / 4.0f) * (1 + 6 * ampX1 + 6 * offset) - ((float)area.getWidth() / 120.0f) * ampdX1, (float)area.getHeight() / 2.0f };
            juce::Point<float> endP1 = { ((float)area.getWidth() / 4.0f) * (1 + 6 * ampX1 + 6 * offset) + ((float)area.getWidth() / 120.0f) * ampdX1, (float)area.getHeight() / 2.0f };

            float ampX2 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) / 500)) / 50;
            float ampdX2 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startP2 = { ((float)area.getWidth() / 2.5f) * (1 + 6 * ampX2 + 6 * offset) - ((float)area.getWidth() / 160.0f) * ampdX2, (float)area.getHeight() / 2.0f };
            juce::Point<float> endP2 = { ((float)area.getWidth() / 2.5f) * (1 + 6 * ampX2 + 6 * offset) + ((float)area.getWidth() / 160.0f) * ampdX2, (float)area.getHeight() / 2.0f };

            float ampX3 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) / 500)) / 45;
            float ampdX3 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startP3 = { ((float)area.getWidth() / 1.8f) * (1 + 6 * ampX3 + 6 * offset) - ((float)area.getWidth() / 160.0f) * ampdX3, (float)area.getHeight() / 2.0f };
            juce::Point<float> endP3 = { ((float)area.getWidth() / 1.8f) * (1 + 6 * ampX3 + 6 * offset) + ((float)area.getWidth() / 160.0f) * ampdX3, (float)area.getHeight() / 2.0f };

            float ampX4 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) / 300)) / 30;
            float ampdX4 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startP4 = { ((float)area.getWidth() / 1.4f) * (1 + 6 * ampX4 + 6 * offset) - ((float)area.getWidth() / 120.0f) * ampdX4, (float)area.getHeight() / 2.0f };
            juce::Point<float> endP4 = { ((float)area.getWidth() / 1.4f) * (1 + 6 * ampX4 + 6 * offset) + ((float)area.getWidth() / 120.0f) * ampdX4, (float)area.getHeight() / 2.0f };

            float ampX5 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) / 170)) / 20;
            float ampdX5 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startP5 = { ((float)area.getWidth() / 1.2f) * (1 + 6 * ampX5 + 6 * offset) - ((float)area.getWidth() / 130.0f) * ampdX5, (float)area.getHeight() / 2.0f };
            juce::Point<float> endP5 = { ((float)area.getWidth() / 1.2f) * (1 + 6 * ampX5 + 6 * offset) + ((float)area.getWidth() / 130.0f) * ampdX5, (float)area.getHeight() / 2.0f };


            float offsetp = (std::cos((samplerOrder + 4.0f) * (float)audioProcessor.getSampleCount(samplerOrder) / 38917)) / 7;

            float ampXp0 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) / 500)) / 17;
            float ampdXp0 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startPp0 = { ((float)area.getWidth() / 5.0f) * (1 + 6 * ampX0 + 6 * offset - 0.3f) - ((float)area.getWidth() / 180.0f) * ampdX0, (float)area.getHeight() / 2.0f };
            juce::Point<float> endPp0 = { ((float)area.getWidth() / 5.0f) * (1 + 6 * ampX0 + 6 * offset - 0.3f) + ((float)area.getWidth() / 180.0f) * ampdX0, (float)area.getHeight() / 2.0f };

            float ampXp1 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) / 300)) / 30;
            float ampdXp1 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startPp1 = { ((float)area.getWidth() / 3.0f) * (1 + 6 * ampXp1 + 6 * offsetp) - ((float)area.getWidth() / 180.0f) * ampdXp1, (float)area.getHeight() / 2.0f };
            juce::Point<float> endPp1 = { ((float)area.getWidth() / 3.0f) * (1 + 6 * ampXp1 + 6 * offsetp) + ((float)area.getWidth() / 180.0f) * ampdXp1, (float)area.getHeight() / 2.0f };

            float ampXp2 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) / 500)) / 50;
            float ampdXp2 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startPp2 = { ((float)area.getWidth() / 2.0f) * (1 + 6 * ampXp2 + 6 * offsetp) - ((float)area.getWidth() / 160.0f) * ampdXp2, (float)area.getHeight() / 2.0f };
            juce::Point<float> endPp2 = { ((float)area.getWidth() / 2.0f) * (1 + 6 * ampXp2 + 6 * offsetp) + ((float)area.getWidth() / 160.0f) * ampdXp2, (float)area.getHeight() / 2.0f };

            float ampXp3 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) / 500)) / 45;
            float ampdXp3 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startPp3 = { ((float)area.getWidth() / 1.6f) * (1 + 6 * ampXp3 + 6 * offsetp) - ((float)area.getWidth() / 180.0f) * ampdXp3, (float)area.getHeight() / 2.0f };
            juce::Point<float> endPp3 = { ((float)area.getWidth() / 1.6f) * (1 + 6 * ampXp3 + 6 * offsetp) + ((float)area.getWidth() / 180.0f) * ampdXp3, (float)area.getHeight() / 2.0f };

            float ampXp4 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) / 300)) / 30;
            float ampdXp4 = (float)(std::cos((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startPp4 = { ((float)area.getWidth() / 1.1f) * (1 + 6 * ampXp4 + 6 * offsetp) - ((float)area.getWidth() / 180.0f) * ampdXp4, (float)area.getHeight() / 2.0f };
            juce::Point<float> endPp4 = { ((float)area.getWidth() / 1.1f) * (1 + 6 * ampXp4 + 6 * offsetp) + ((float)area.getWidth() / 180.0f) * ampdXp4, (float)area.getHeight() / 2.0f };

            float ampXp5 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) / 170)) / 20;
            float ampdXp5 = (float)(std::sin((double)audioProcessor.getSampleCount(samplerOrder) * freq) + 1);
            juce::Point<float> startPp5 = { ((float)area.getWidth() / 1.05f) * (1 + 6 * ampXp5 + 6 * offsetp) - ((float)area.getWidth() / 170.0f) * ampdXp5, (float)area.getHeight() / 2.0f };
            juce::Point<float> endPp5 = { ((float)area.getWidth() / 1.05f) * (1 + 6 * ampXp5 + 6 * offsetp) + ((float)area.getWidth() / 170.0f) * ampdXp5, (float)area.getHeight() / 2.0f };

            if (startP0.x < 0) startP0.x = 0;
            if (endP0.x >= (float)area.getWidth()) endP0.x = (float)area.getWidth() - 1;

            if (startP1.x < 0) startP1.x = 0;
            if (endP1.x >= (float)area.getWidth()) endP1.x = (float)area.getWidth() - 1;

            if (startP2.x < 0) startP2.x = 0;
            if (endP2.x >= (float)area.getWidth()) endP2.x = (float)area.getWidth() - 1;

            if (startP3.x < 0) startP3.x = 0;
            if (endP3.x >= (float)area.getWidth()) endP3.x = (float)area.getWidth() - 1;

            if (startP4.x < 0) startP4.x = 0;
            if (endP4.x >= (float)area.getWidth()) endP4.x = (float)area.getWidth() - 1;

            if (startP5.x < 0) startP5.x = 0;
            if (endP5.x >= (float)area.getWidth()) endP5.x = (float)area.getWidth() - 1;


            if (startPp0.x < 0) startPp0.x = 0;
            if (endPp0.x >= (float)area.getWidth()) endPp0.x = (float)area.getWidth() - 1;

            if (startPp1.x < 0) startPp1.x = 0;
            if (endPp1.x >= (float)area.getWidth()) endPp1.x = (float)area.getWidth() - 1;

            if (startPp2.x < 0) startPp2.x = 0;
            if (endPp2.x >= (float)area.getWidth()) endPp2.x = (float)area.getWidth() - 1;

            if (startPp3.x < 0) startPp3.x = 0;
            if (endPp3.x >= (float)area.getWidth()) endPp3.x = (float)area.getWidth() - 1;

            if (startPp4.x < 0) startPp4.x = 0;
            if (endPp4.x >= (float)area.getWidth()) endPp4.x = (float)area.getWidth() - 1;

            if (startPp5.x < 0) startPp5.x = 0;
            if (endPp5.x >= (float)area.getWidth()) endPp5.x = (float)area.getWidth() - 1;

            juce::Rectangle rect0(startP0.x, 0.0f, endP0.x - startP0.x, (float)getHeight() - 1),
                rect1(startP1.x, 0.0f, endP1.x - startP1.x, (float)getHeight() - 1),
                rect2(startP2.x, 0.0f, endP2.x - startP2.x, (float)getHeight() - 1),
                rect3(startP3.x, 0.0f, endP3.x - startP3.x, (float)getHeight() - 1),
                rect4(startP4.x, 0.0f, endP4.x - startP4.x, (float)getHeight() - 1),
                rect5(startP5.x, 0.0f, endP5.x - startP5.x, (float)getHeight() - 1);

            juce::Rectangle rectp0(startPp0.x, 0.0f, endPp0.x - startPp0.x, (float)getHeight() - 1),
                rectp1(startPp1.x, 0.0f, endPp1.x - startPp1.x, (float)getHeight() - 1),
                rectp2(startPp2.x, 0.0f, endPp2.x - startPp2.x, (float)getHeight() - 1),
                rectp3(startPp3.x, 0.0f, endPp3.x - startPp3.x, (float)getHeight() - 1),
                rectp4(startPp4.x, 0.0f, endPp4.x - startPp4.x, (float)getHeight() - 1),
                rectp5(startPp5.x, 0.0f, endPp5.x - startPp5.x, (float)getHeight() - 1);

            juce::Path path0, path1, path2, path3, path4, path5;
            path0.addRectangle(rect0);
            path1.addRectangle(rect1);
            path2.addRectangle(rect2);
            path3.addRectangle(rect3);
            path4.addRectangle(rect4);
            path5.addRectangle(rect5);

            juce::Path pathp0, pathp1, pathp2, pathp3, pathp4, pathp5;
            pathp0.addRectangle(rectp0);
            pathp1.addRectangle(rectp1);
            pathp2.addRectangle(rectp2);
            pathp3.addRectangle(rectp3);
            pathp4.addRectangle(rectp4);
            pathp5.addRectangle(rectp5);

            juce::ColourGradient gradient0{ startColor, startP0, endColor, endP0, false };
            juce::ColourGradient gradient1{ startColor, startP1, endColor, endP1, false };
            juce::ColourGradient gradient2{ startColor, startP2, endColor, endP2, false };
            juce::ColourGradient gradient3{ startColor, startP3, endColor, endP3, false };
            juce::ColourGradient gradient4{ startColor, startP4, endColor, endP4, false };
            juce::ColourGradient gradient5{ startColor, startP5, endColor, endP5, false };

            juce::ColourGradient gradientp0{ startColor, startPp0, endColor, endPp0, false };
            juce::ColourGradient gradientp1{ startColor, startPp1, endColor, endPp1, false };
            juce::ColourGradient gradientp2{ startColor, startPp2, endColor, endPp2, false };
            juce::ColourGradient gradientp3{ startColor, startPp3, endColor, endPp3, false };
            juce::ColourGradient gradientp4{ startColor, startPp4, endColor, endPp4, false };
            juce::ColourGradient gradientp5{ startColor, startPp5, endColor, endPp5, false };

            gradient0.addColour(0.5, audioProcessor.samplerBars[theme]);
            gradient1.addColour(0.5, audioProcessor.samplerBars[theme]);
            gradient2.addColour(0.5, audioProcessor.samplerBars[theme]);
            gradient3.addColour(0.5, audioProcessor.samplerBars[theme]);
            gradient4.addColour(0.5, audioProcessor.samplerBars[theme]);
            gradient5.addColour(0.5, audioProcessor.samplerBars[theme]);

            gradientp0.addColour(0.5, audioProcessor.samplerBarsMixed[theme]);
            gradientp1.addColour(0.5, audioProcessor.samplerBarsMixed2[theme]);
            gradientp2.addColour(0.5, audioProcessor.samplerBarsMixed[theme]);
            gradientp3.addColour(0.5, audioProcessor.samplerBarsMixed2[theme]);
            gradientp4.addColour(0.5, audioProcessor.samplerBarsMixed[theme]);
            gradientp5.addColour(0.5, audioProcessor.samplerBarsMixed2[theme]);

            g.setGradientFill(gradient0);
            g.fillPath(path0);
            g.setGradientFill(gradient1);
            g.fillPath(path1);
            g.setGradientFill(gradient2);
            g.fillPath(path2);
            g.setGradientFill(gradient3);
            g.fillPath(path3);
            g.setGradientFill(gradient4);
            g.fillPath(path4);
            g.setGradientFill(gradient5);
            g.fillPath(path5);             

            g.setGradientFill(gradientp0);
            g.fillPath(pathp0);
            g.setGradientFill(gradientp1);
            g.fillPath(pathp1);
            g.setGradientFill(gradientp2);
            g.fillPath(pathp2);
            g.setGradientFill(gradientp3);
            g.fillPath(pathp3);
            g.setGradientFill(gradientp4);
            g.fillPath(pathp4);
            g.setGradientFill(gradientp5);
            g.fillPath(pathp5);

            g.setColour(audioProcessor.samplerBarsMixed[theme]);
            g.drawRect(area, 3);
        }
        else {
            
            g.fillAll(audioProcessor.samplerBgColors1[theme]);
            g.setColour(audioProcessor.fontColors[theme]);
            p.startNewSubPath(0, getHeight() / 2);

            for (int sample = 0; sample < audioData.size(); sample++)
            {
                auto point = juce::jmap<float>(audioData[sample], -1.0f, 1.0f, getHeight(), 0);

                p.lineTo(sample, point);
            }

            g.strokePath(p, juce::PathStrokeType(1));

            g.setColour(audioProcessor.fontColors[theme]);
            g.setFont(15.0f);

            auto textBounds = getLocalBounds().reduced(10, 10);
            g.drawFittedText(audioProcessor.getFileNm(samplerOrder), textBounds, juce::Justification::topLeft, 1);
            
        }

    }
    else
    {
        g.fillAll(audioProcessor.samplerBgColors1[theme]);
        g.setColour(audioProcessor.fontColors[theme]);

        juce::Path p;
        float w = (float)getWidth(), h = (float)getHeight();
        p.startNewSubPath(w / 3 + w * 2.0f / 9, h / 3 + h / 12);
        p.lineTo(w / 3 + w * 2.0f / 9, h / 3);
        p.lineTo(w / 3, h / 3);
        p.lineTo(w / 3, h * 3.0f / 4);
        p.lineTo(w / 3 + w / 15, h / 3 + h / 12);
        p.lineTo(w * 2.0f / 3, h / 3 + h / 12);
        p.lineTo(w * 2.0f / 3 - w / 15, h * 3.0f / 4 + 2);
        p.lineTo(w / 3, h * 3.0f / 4 + 2);

        g.strokePath(p, juce::PathStrokeType(2));
    }
}

void SampleShape::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void SampleShape::mouseEnter(const juce::MouseEvent& event) {
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void SampleShape::mouseExit(const juce::MouseEvent& event) {
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void SampleShape::mouseDown(const juce::MouseEvent& event) {
    juce::FileChooser chooser{ "Please load a file" };

    if (chooser.browseForFileToOpen()) {
        juce::File loadedFile = chooser.getResult();

        juce::String fileExt = loadedFile.getFileExtension().toLowerCase();
        if (fileExt.contains(".wav") || fileExt.contains(".aif") || fileExt.contains(".mp3")) {
            juce::String fileNm = loadedFile.getFileNameWithoutExtension();
            audioProcessor.loadFile(loadedFile.getFullPathName(), this->getOrder(), fileNm, samplerOrder);
        }
    }
    repaint();
}

bool SampleShape::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (juce::String f : files)
        if (f.toLowerCase().contains(".wav") || f.toLowerCase().contains(".aif") || f.toLowerCase().contains(".mp3"))
            return true;

    return false;
}

void SampleShape::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (juce::String f : files) {
        if (isInterestedInFileDrag(f)) {
            std::unique_ptr<juce::File> loadedFile = std::make_unique<juce::File>(f);
            juce::String fileNm = loadedFile->getFileNameWithoutExtension();

            audioProcessor.loadFile(f, this->getOrder(), fileNm, samplerOrder);
        }
    }
    repaint();
}
