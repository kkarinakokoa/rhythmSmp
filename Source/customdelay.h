#pragma once

#include <JuceHeader.h>
#define BUFSIZE 192000

struct delayParameters {
	float delayTime;
	float feedback;
	float wet;
	bool ispingpong;
};

class customdelay {
public:
	customdelay();
	void setSampleRate(double param);
	float getSyncToMillisec(int numerator, int denominator, float bpm);
	void process(float* bufferL, float* bufferR, int numSamples, float delayTime, float feedback, float wet, bool ispingpong);
	void process(float* bufferL, float* bufferR, int numSamples, delayParameters params);
	void clearDelayBuffers();

private:
	double sampleRate;
	int currPos;

	std::vector<float> delaybufL, delaybufR;
	float fbSampleL, fbSampleR;
};