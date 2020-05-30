/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once
#include "ADSR.h"
#include "maximilian.h"
#include "../JuceLibraryCode/JuceHeader.h"

// Custom Juce Look and Feel Method
class OtherLookAndFeel : public LookAndFeel_V4
{
public:
	OtherLookAndFeel()
	{
		setColour(Slider::thumbColourId, Colours::black);
	}
	
		
	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		auto radius = jmin(width / 2, height / 2) - 4.0f;
		auto centreX = x + width * 0.5f;
		auto centreY = y + height * 0.5f;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;
		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		// fill
		g.setColour(Colours::orange);
		g.fillEllipse(rx, ry, rw, rw);
		// outline
		g.setColour(Colours::orange);
		g.drawEllipse(rx, ry, rw, rw, 1.0f);

		Path p;
		auto pointerLength = radius * 0.33f;
		auto pointerThickness = 2.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

		// pointer
		g.setColour(Colours::black);
		g.fillPath(p);
	}

private:
	//OtherLookAndFeel otherLookAndFeel; // [2]
	Slider dial1;
	Slider dial2;
	TextButton button1;
	TextButton button2;
};

//==============================================================================
class MainComponent : public AudioAppComponent, public::Slider::Listener, private OSCReceiver,
	private OSCReceiver::ListenerWithOSCAddress<OSCReceiver::MessageLoopCallback>, private Timer
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent();

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	//==============================================================================
	void paint(Graphics& g) override;
	void resized() override;

	// Extra functions:
	void sliderValueChanged(Slider* slider) override;
	void sliderDragEnded(Slider* slider) override;
	void timerCallback() override;
	void oscMessageReceived(const OSCMessage& message);
	void presetChanged();
	void showConnectionErrorMessage(const String& messageText)
	{
		AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
			"Connection error",
			messageText,
			"OK");
	}

	

private:
	//==============================================================================
	// Synth engine:
	// Maximilian:
	maxiOsc op1, op2, op3, op4, op5, op6, saw;
	maxiDistortion limiter;
	// Envelope source code at : http://www.martin-finke.de/blog/articles/audio-plugins-011-envelopes/ 
	EnvelopeGenerator mEnvelopeGenerator;

	// ----- sound variables -------
	double o1, o2, o3, o4, o5, o6, sum, signal;
	float amp1 = 20.0, amp2 = 50.0, amp3 = 100.0, amp4 = 30.0, amp5 = 500.0, amp6 = 700.0;
	int freq1 = 20, freq2 = 100, freq3 = 200, freq4 = 300, freq5 = 1000, freq6 = 1500;

	// OSC:
	OSCSender sender;
	OSCSender maxSender;

	// Presets:
	ComboBox presetBox;
	StringArray presetNames = { "1", "2", "3", "4", "5", "6" };
	vector<vector<float>> presetBank = { {106.0, 1032.0, 150.0, 576.0, 0.0, 0.0, 265.0, 358.0, 0.0, 0.0},
										{250.0, 112.0, 0.0, 120.0, 1424.0, 392.0, 1321.0, 1849.0, 1340.0, 1660.0},
										{50.0, 1032.0, 630.0, 576.0, 1200.0, 1840.0, 1321.0, 358.0, 2000.0, 1585.0 },
										{78.0, 50.0, 0.1, 1248.0, 296.0, 64.0, 562.0, 109.0, 375.0, 406.0},
										{6.0, 568.0, 1567.0, 0.1, 160.0, 640.0, 1390.0, 1868.0, 515.0, 1921.0},
										{0.1, 0.1, 152.0, 1.0, 0.1, 1.3, 0.0, 78.1, 437.5, 109.4} };
										
	
	// --------- Slider and OSC logic variables --------
	bool snap = false;
	bool isSliderDragEnded = false;
	int classifiedPreset;
	int  previousClassification = 0;
	int clusterPresetCounter = 7;
	double whichClass;
	vector<bool> selectedSlider = { false, false, false, false, false, false, false, false, false, false };
	vector<double> enVals = { 0.1, 0.1, 0.5, 1.0 };
	vector<double> sliderVals = { 200., 0., 0., 0., 0., 0., 1., 1., 1., 1. };
	
	// UI
	OwnedArray<Slider> the_sliders;
	OwnedArray<Slider> envelope_sliders;
	Label f1Label, f2Label, f3Label, f4Label, f5Label, f6Label, a1Label, a2Label, a3Label, a4Label, a5Label, a6Label, fuzzLabel, thresholdLabel;
	TextButton sendMessage, sendMessageToMax, snapToPreset, clearCluster, removePreset;
	OtherLookAndFeel rotarySliderDesign;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};