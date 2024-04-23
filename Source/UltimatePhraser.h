#pragma once

#include <JuceHeader.h>

class UltimatePhraser {
public:
	struct cell {
		int startPos;
		int len;
		bool isNote;
		juce::uint8 velocity;
	};

	UltimatePhraser();
	void setSampleRate(double param);
	std::vector<struct cell> createPhrase(int noteNum, bool downbeat, bool tenuto);
	void createPhrases(int min, int max, bool downbeat, bool tenuto);
	void reset();
	void reset(int index);
	int popNextLength(int index, float bpm);
	bool isNote(int index) { return phrases[index][idxNextPop[index]].isNote; }
	juce::uint8 popNextVelocity(int index) { return phrases[index][idxNextPop[index]].velocity; }
	void updateNext(int index);

private:
	double sampleRate;
	int minNoteNum, maxNoteNum;

	juce::Random divining;
	std::vector<struct cell> phrases[12];
	std::vector<int> idxNextPop{ 0,0,0,0,0,0,0,0,0,0,0,0 };
};