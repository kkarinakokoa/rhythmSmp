#pragma once

#include <JuceHeader.h>
#define BUFSIZE 192000

struct imagerParameters {
	float dry;
	float wet;
	float balance;
	float delayTime;
};

class hassimager {
public:
	hassimager();
	void setSampleRate(double param);
	void process(float* bufferL, float* bufferR, int numSamples, float dry, float wet, float balance, float delayTime);
	void process(float* bufferL, float* bufferR, int numSamples, imagerParameters params);
	void clearImagerBuffers();

private:
	double sampleRate;
	int currPos;

	std::vector<float> delaybufR;
	float fbSampleL, fbSampleR;
};