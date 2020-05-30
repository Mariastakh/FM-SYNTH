// Source code at http://www.martin-finke.de/blog/articles/audio-plugins-011-envelopes/ 
#include <cmath>

class EnvelopeGenerator {
public:
	enum EnvelopeStage {
		ENVELOPE_STAGE_OFF = 0,
		ENVELOPE_STAGE_ATTACK,
		ENVELOPE_STAGE_DECAY,
		ENVELOPE_STAGE_SUSTAIN,
		ENVELOPE_STAGE_RELEASE,
		kNumEnvelopeStages
	};
	void enterStage(EnvelopeStage newStage);
	double nextSample();
	void setSampleRate(double newSampleRate);

	// Extra function to make the envelope parameters adjustible during program runtime:
	void setSettings(double setAttack, double setDecay, double setSustain, double setRelease)
	{
		stageValue[ENVELOPE_STAGE_OFF] = 0.0;
		stageValue[ENVELOPE_STAGE_ATTACK] = setAttack; // time value
		stageValue[ENVELOPE_STAGE_DECAY] = setDecay; // time value
		stageValue[ENVELOPE_STAGE_SUSTAIN] = setSustain; // its a level value, between 1 and 0.1
		stageValue[ENVELOPE_STAGE_RELEASE] = setRelease; // time value
	}
	inline EnvelopeStage getCurrentStage() const { return currentStage; };
	const double minimumLevel;

	EnvelopeGenerator() :
		minimumLevel(0.0001),
		currentStage(ENVELOPE_STAGE_OFF),
		currentLevel(minimumLevel),
		multiplier(1.0),
		sampleRate(44100.0),
		currentSampleIndex(0),
		nextStageSampleIndex(0) {};
private:
	EnvelopeStage currentStage;
	double currentLevel;
	double multiplier;
	double sampleRate;
	double stageValue[kNumEnvelopeStages];
	void calculateMultiplier(double startLevel, double endLevel, unsigned long long lengthInSamples);
	unsigned long long currentSampleIndex;
	unsigned long long nextStageSampleIndex;
};