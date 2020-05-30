/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/
#include "maximilian.h"
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{

	// This timer will trigger the timerCallback method
	// to send OSC messages Max every 5 seconds:
	startTimer(5000); 

	// Envelope setup methods:
	mEnvelopeGenerator.setSampleRate(44100);
	mEnvelopeGenerator.setSettings(2.0, 0.5, 0.3, 0.5);
	
	// -----------------------
	// LISTENER PORT:
	if (!connect(12000))                   
		showConnectionErrorMessage("Error: could not connect to UDP port 9001.");
	// tell the component to listen for OSC messages matching this address:
	addListener(this, "/wek/outputs"); 
	addListener(this, "/max/outputs");

	// WEKINATOR SENDER PORT:
	if (!sender.connect("127.0.0.1", 6448)) 
		showConnectionErrorMessage("Error: could not connect to UDP port 9001.");

	// MAX SENDER PORT:
	if (!maxSender.connect("127.0.0.1", 6440)) 
		showConnectionErrorMessage("Error: could not connect to UDP port for Max.");

	// -----------------------
	// Button to send Source OSC messages to Max, in order to receive cluster results:
	addAndMakeVisible(sendMessage);
	sendMessageToMax.setButtonText("Get Presets");
	sendMessageToMax.setColour(TextButton::buttonColourId, Colours::black);
	sendMessageToMax.onClick = [this] {maxSender.send("/max/cluster", (int)1);  };

	// Button to clear the Max cluster and reset:
	addAndMakeVisible(clearCluster);
	clearCluster.setButtonText("Clear Cluster");
	clearCluster.setColour(TextButton::buttonColourId, Colours::black);
	clearCluster.onClick = [this] { maxSender.send("/max/clear", (int)1);  };

	// Button to send Target OSC messages to Wekinator for training:
	addAndMakeVisible(sendMessageToMax);
	sendMessage.setButtonText("Send Source");
	sendMessage.setColour(TextButton::buttonColourId, Colours::black);
	sendMessage.onClick = [this] {
		sender.send("/wek/inputs", (float)sliderVals[0], (float)sliderVals[1],
			(float)sliderVals[2], (float)sliderVals[3], (float)sliderVals[4],
			(float)sliderVals[5], (float)sliderVals[6], (float)sliderVals[7],
			(float)sliderVals[8], (float)sliderVals[9]);
	};

	// Button to enable preset snapping:
    addAndMakeVisible(snapToPreset);
    snapToPreset.setButtonText("Snap to Preset");
    snapToPreset.setColour(TextButton::buttonColourId, Colours::black);
	snapToPreset.onClick = [this] {snap = !snap; };
	
	//----------
	// Preset drop-down menu:
	addAndMakeVisible(presetBox);
	presetBox.setTextWhenNoChoicesAvailable("Preset");
	presetBox.setColour(ComboBox::backgroundColourId, Colours::black);
	presetBox.addItemList(presetNames, 1); //string array
	presetBox.onChange = [this]() { presetChanged();
	};
    
    //----------
	// Frequency and Amplitude sliders:
    for (int i = 0; i < 10; i++) 
	{
		Slider* t = new Slider();
		if(i < 6)
    	    {
				t->setSliderStyle(Slider::RotaryVerticalDrag);
    	        t->setRange(0.1, 2000.0, 0.1);
				t->setLookAndFeel(&rotarySliderDesign);
    	    } else { 
			t->setSliderStyle(Slider::LinearBarVertical);
			t->setRange(0.0, 2000.0, 0.1); }

		t->setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
		t->setColour(Slider::textBoxTextColourId, Colours::black);
		t->setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
		t->setValue(1);
    	t->setValue(sliderVals[i]);
		addAndMakeVisible(the_sliders.add(t));
		the_sliders[i]->addListener(this);
	}
    
	// ---------------------
	// Envelope sliders:
	for (int i = 0; i < 4; i++)
	{
		Slider* t = new Slider();
		t->setSliderStyle(Slider::LinearVertical);

		// Set the sustain slider to a range between 0.1 and 1.0:
		if (i == 2)
		{
			t->setRange(0.01, 1.0);
		} else { t->setRange(0.01, 5); }
		
		t->setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
		t->setColour(Slider::textBoxTextColourId, Colours::black);
		t->setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
		t->setValue(0.1);
		addAndMakeVisible(envelope_sliders.add(t));
		envelope_sliders[i]->addListener(this);

	}

	// ---------------------
	// Labels:
	f1Label.setText("OP 1", dontSendNotification);
	f1Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(f1Label);

	f2Label.setText("Freq 2", dontSendNotification);
	f2Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(f2Label);

	f3Label.setText("OP 2", dontSendNotification);
	f3Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(f3Label);

	f4Label.setText("Freq 4", dontSendNotification);
	f4Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(f4Label);

	f5Label.setText("Freq 5", dontSendNotification);
	f5Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(f5Label);

	f6Label.setText("Freq 6", dontSendNotification);
	f6Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(f6Label);

	//---

	a1Label.setText("Amp 1", dontSendNotification);
	a1Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(a1Label);

	a2Label.setText("Amp 2", dontSendNotification);
	a2Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(a2Label);

	a3Label.setText("Amp 3", dontSendNotification);
	a3Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(a3Label);

	a4Label.setText("Amp 4", dontSendNotification);
	a4Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(a4Label);

	a5Label.setText("Amp 5", dontSendNotification);
	a5Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(a5Label);

	a6Label.setText("Amp 6", dontSendNotification);
	a6Label.setColour(Label::textColourId, Colours::black);
	addAndMakeVisible(a6Label);

    setSize (800, 600);
    setAudioChannels (0, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	
	// Access the left and right audio channel buffers:
	auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

	for (int sample = 0; sample < bufferToFill.numSamples; sample++)
	{
		// Carrier operator o3 is modulated by o4, which is modulated by o5, which is modulated by o6:
		o6 = op6.sinewave(sliderVals[5]);
		o5 = op5.sinewave(sliderVals[4] + (o6 * sliderVals[9])); // amp6
		o4 = op4.sinewave(sliderVals[3] + (o5 * sliderVals[8])); // amp5
		o3 = op3.sinewave(sliderVals[2] + (o4 * sliderVals[7])); // amp4

		// Carrier operator o1 is modulated by o2:
		o2 = op2.sinewave(sliderVals[1]);
		o1 = op1.sinewave(sliderVals[0] + (o2 * sliderVals[6]));

		// Sum the Carriers:
		 sum = (o1 + o3)*0.5;

		 // Set the envelope settings
		 mEnvelopeGenerator.setSettings(enVals[0], enVals[1], enVals[2], enVals[3]);

		// The envelope will be set off immediately upon finishing, 
		// so if the envelope has reached 0 (stage_off), then set it back to 
		// the beginning (stage_attach):
		if (mEnvelopeGenerator.getCurrentStage() == EnvelopeGenerator::ENVELOPE_STAGE_OFF) {
			mEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_ATTACK);
		}
		// If the envelope has reached its sustain level, let the release stage begin 
		// Otherwise the envelope will get stuck at the sustain stage indefinitely
		if (mEnvelopeGenerator.getCurrentStage() == EnvelopeGenerator::ENVELOPE_STAGE_SUSTAIN) {
			mEnvelopeGenerator.enterStage(EnvelopeGenerator::ENVELOPE_STAGE_RELEASE);
		}

		// Multiply the sum of the operators by the envelope:
		signal = sum * mEnvelopeGenerator.nextSample();
		// Apply atan function to distort the signal, this adds a bit of warmth and character:
		signal = limiter.atanDist(signal, 1.25);
		// Output:
		leftBuffer[sample] = signal;
		rightBuffer[sample] = leftBuffer[sample];

	}
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
	// UI colours:
	g.fillAll(Colours::darkgrey);
	g.setColour(Colours::dimgrey);
	g.fillRoundedRectangle(100, 90, 150, 300, 50.5);
	g.fillEllipse(5, 405, 130, 130); // F 4
	g.drawLine(75, 415, 125, 350, 20);
	g.fillEllipse(195, 405, 130, 130); // F 5
	g.drawLine(230, 415, 185, 350, 20);
	g.fillEllipse(285, 285, 130, 130); // F 6
	g.drawLine(240, 325, 390, 330, 20);
	g.fillEllipse(225, -3, 130, 130); // F 2
	g.drawLine(210, 130, 240, 100, 20);
}

void MainComponent::resized()
{
	// Position the frequency sliders:
	the_sliders[0]->setBounds(100, 100, 150, 150); // Freq 1 Slider
	the_sliders[1]->setBounds(230, 10, 120, 120); // Freq 2 Slider
	the_sliders[2]->setBounds(100, 250, 150, 150); // Freq 3 Slider
	the_sliders[3]->setBounds(10, 420, 120, 120); // Freq 4 Slider
	the_sliders[4]->setBounds(200, 420, 120, 120); // Freq 5 Slider
	the_sliders[5]->setBounds(290, 300, 120, 120); // Freq 6 Slider
	// Labels:
	f1Label.setBounds(150, 150, 50, 50); // Freq 1 Label
	f2Label.setBounds(270, 40, 100, 50); // Freq 2 Label
	f3Label.setBounds(150, 280, 100, 50); // Freq 3 Label
	f4Label.setBounds(50, 450, 70, 30); // Freq 4 Label
	f5Label.setBounds(240, 450, 70, 30); // Freq 5 Label
	f6Label.setBounds(325, 330, 70, 30);


	// Position the amplitude sliders:
	the_sliders[6]->setBounds(370, 20, 30, 130); // Amp2 - Freq 2 Gain
	the_sliders[7]->setBounds(140, 440, 30, 130); // Amp 4
	the_sliders[8]->setBounds(330, 440, 30, 130); // Amp 5
	the_sliders[9]->setBounds(430, 310, 30, 130); // Amp 6
	// Labels:
	a2Label.setBounds(370, 150, 100, 50); // Amp2 Label
	a4Label.setBounds(140, 560, 100, 50); // Amp 4
	a5Label.setBounds(330, 560, 100, 50); // Amp 5
	a6Label.setBounds(430, 430, 100, 50); // Amp 6
	
	// Position the envelope sliders:
	for (int i = 0; i < 4; i++)
	{
		envelope_sliders[i]->setBounds(550+(i*50), 210, 50, 150);
	}

	// Buttons :
	//sendMessage.setBounds(640, 450, 100, 40); // only used for training
	sendMessageToMax.setBounds(10, 10, 70, 30);
	snapToPreset.setBounds(500, 10, 100, 30);
	clearCluster.setBounds(90, 10, 60, 30);

	// Preset comboBox:
	presetBox.setBounds(620, 10, 100, 30);
}

void MainComponent::sliderValueChanged(Slider* slider)
{
	// Define behaviours for when slider values are changed

	// Frequency and Amplitude sliders:
    for (int i = 0; i < 10; i++)
	{
		if (slider == the_sliders[i])
		{
			isSliderDragEnded = false;
			sliderVals[i] = the_sliders[i]->getValue();
		}
	}

	// Envelope sliders:
	for (int i = 0; i < 4; i++)
	{
		if (slider == envelope_sliders[i])
		{
			enVals[i] = envelope_sliders[i]->getValue();
		}
	}
	
	// Send values to Wekinator, this is for the envelope adjustment functionality:
	for (int i = 0; i < 10; i++)
	{
		if (slider == the_sliders[i])
		{
			selectedSlider[i] = true;
			// send out all the values
			sender.send("/wek/inputs", (float)sliderVals[0], (float)sliderVals[1],
				(float)sliderVals[2], (float)sliderVals[3], (float)sliderVals[4],
				(float)sliderVals[5], (float)sliderVals[6],
				(float)sliderVals[7], (float)sliderVals[8],
				(float)sliderVals[9]);
		}
		else {
			selectedSlider[i] = false;
		}
	}
	
}

//=================================
// This is the function which receives data from Max and Wekinator 
// and subsequently sets the synth parameters to the new values:
void MainComponent::oscMessageReceived(const OSCMessage& message)
{
	// CHECKING FOR MESSAGES FROM WEKINATOR
	if (message.getAddressPattern() == "/wek/outputs")
	{

		if (message.size() == 5 && message[0].isFloat32())
		{
			// Set Envelope values to the new values defined by the regression model:
			double attack = message[1].getFloat32();
			double decay = message[2].getFloat32();
			double sustain = message[3].getFloat32();
			double release = message[4].getFloat32();
			// Store envelope values in a vector:
			vector<double> envelope = {attack, decay, sustain, release};
			// Set the new envelope values:
			for (int i = 0; i < 4; i++)
			{
				envelope_sliders[i]->setValue(envelope[i]);
			}

			// If Preset Snapping is enabled and the user has finished dragging:
			if (snap == true && isSliderDragEnded == true)
			{
				// Store the result of the classifier:
				whichClass = message[0].getFloat32();
				// Set the corresponding preset:
				classifiedPreset = whichClass - 1;

				// If the designated preset isn't already the previously assigned one: 
				if (classifiedPreset != previousClassification)
				{
					// Snap all sliders to the nearest preset - apart from the currently dragged slider:
					for (int i = 0; i < 10; i++)
					{
						if (selectedSlider[i] != true)
						{
							the_sliders[i]->setValue(presetBank[classifiedPreset][i]);
						}

					}
					// Store the index of the current preset:
					previousClassification = classifiedPreset;
				}
			}
		}
	}

	// CHECKING FOR MESSAGES FROM MAX:
	if (message.getAddressPattern() == "/max/outputs")
	{
		// the index of every message will be the cluster index
		// the following will be the values of the cluster. So
		// we ignore index 0. 

		// Store the preset in a temporary vector:
		vector<float> thePreset = { message[1].getFloat32() ,message[2].getFloat32() ,
		message[3].getFloat32() , message[4].getFloat32() ,
		message[5].getFloat32() , message[6].getFloat32() , 
		message[7].getFloat32() , message[8].getFloat32(),
		message[9].getFloat32() , message[10].getFloat32() };
		
		// Save the preset to our bank:
		presetBank.push_back(thePreset);;
		// Add a new index to the preset menu:
		presetBox.addItem(std::to_string(clusterPresetCounter) + " Custom preset", clusterPresetCounter);
		// Increment the index counter:
		clusterPresetCounter++;

	}

}



void MainComponent::timerCallback()
{
	// send value to Max Clustering Path
	maxSender.send("/max/inputs", (float)sliderVals[0], (float)sliderVals[1],
		(float)sliderVals[2],
		(float)sliderVals[3], (float)sliderVals[4],
		(float)sliderVals[5], (float)sliderVals[6],
	    (float)sliderVals[7], (float)sliderVals[8],
		(float)sliderVals[9]);
}


//---------------------------------------------
void MainComponent::presetChanged()
{
	// Define the behaviour for the preset selection Combo Box:
	// Set the slider values to the values in the preset array
	// which is at the selected comboBox index.
	for (int i = 0; i < 10; i++)
	{
		the_sliders[i]->setValue(presetBank[presetBox.getSelectedItemIndex()][i]);
	}
}

//------------------------------

void MainComponent::sliderDragEnded(Slider* slider)
{
	// Send values to Wekinator when a user has finished dragging a slider
	// this is for the preset snapping functionality:
	if (snap == true)
	{
		for (int i = 0; i < 10; i++)
		{
			if (slider == the_sliders[i])
			{
				isSliderDragEnded = true;
				selectedSlider[i] = true;
				// send out all the values
				sender.send("/wek/inputs", (float)sliderVals[0], (float)sliderVals[1],
					(float)sliderVals[2], (float)sliderVals[3], (float)sliderVals[4],
					(float)sliderVals[5], (float)sliderVals[6],
					(float)sliderVals[7], (float)sliderVals[8],
					(float)sliderVals[9]);
			}
			else {
				selectedSlider[i] = false;
			}
		}
	}
}