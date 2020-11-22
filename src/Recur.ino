#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#include <Bounce.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// oscillator
AudioSynthWaveform       osc01;   
AudioSynthWaveform       osc02;  
AudioMixer4              oscMixer;
AudioInputI2S            input;         
AudioMixer4              inputMixer;
AudioPlaySdWav           sampler;  
AudioMixer4              samplerMixer;
AudioMixer4              mixer;

// delay
AudioEffectDelay         delayL;       
AudioEffectDelay         delayR;        
AudioMixer4              mixerL;       
AudioMixer4              mixerR;     
AudioSynthWaveform       oscL02;        
AudioSynthWaveform       oscL01;      
AudioEffectRectifier     rectifyL; 
AudioEffectMultiply      multiplyL;    
AudioEffectDigitalCombine combineL;     
AudioSynthWaveform       oscR01;        
AudioSynthWaveform       oscR02;       
AudioEffectRectifier     rectifyR;     
AudioEffectMultiply      multiplyR; 
AudioEffectDigitalCombine combineR;

// filter
AudioSynthWaveformDc     dc;          
AudioEffectEnvelope      filterEgL;   
AudioAmplifier           filterEgLAmp;        
AudioFilterStateVariable lpfL;      
AudioFilterStateVariable hpfL;
AudioMixer4              filterLMixer;   
AudioEffectEnvelope      filterEgR; 
AudioAmplifier           filterEgRAmp; 
AudioFilterStateVariable lpfR;
AudioFilterStateVariable hpfR;
AudioMixer4              filterRMixer;

// amp
AudioEffectEnvelope      ampEgL;   
AudioEffectEnvelope      ampEgR; 

// output
AudioOutputI2S           output;      

// patching
AudioConnection          patchCord1(osc02, 0, oscMixer, 1);
AudioConnection          patchCord2(input, 0, inputMixer, 0);
AudioConnection          patchCord3(input, 1, inputMixer, 1);
AudioConnection          patchCord4(sampler, 0, samplerMixer, 0);
AudioConnection          patchCord5(sampler, 1, samplerMixer, 1);
AudioConnection          patchCord6(osc01, 0, oscMixer, 0);
AudioConnection          patchCord7(oscMixer, 0, mixer, 0);
AudioConnection          patchCord8(inputMixer, 0, mixer, 1);
AudioConnection          patchCord9(samplerMixer, 0, mixer, 2);
AudioConnection          patchCord10(mixer, 0, mixerL, 0);
AudioConnection          patchCord11(mixer, 0, mixerR, 0);
AudioConnection          patchCord12(delayL, 0, multiplyL, 1);
AudioConnection          patchCord13(delayL, 1, combineL, 1);
AudioConnection          patchCord14(delayL, 2, rectifyL, 0);
AudioConnection          patchCord15(delayR, 0, multiplyR, 1);
AudioConnection          patchCord16(delayR, 1, combineR, 1);
AudioConnection          patchCord17(delayR, 2, rectifyR, 0);
AudioConnection          patchCord18(mixerL, delayL);
AudioConnection          patchCord19(mixerL, 0, lpfL, 0);
AudioConnection          patchCord20(mixerL, 0, hpfL, 0);
AudioConnection          patchCord21(mixerR, delayR);
AudioConnection          patchCord22(mixerR, 0, lpfR, 0);
AudioConnection          patchCord23(mixerR, 0, hpfR, 0);
AudioConnection          patchCord24(oscL02, 0, combineL, 0);
AudioConnection          patchCord25(oscL01, 0, multiplyL, 0);
AudioConnection          patchCord26(rectifyL, 0, mixerL, 3);
AudioConnection          patchCord27(multiplyL, 0, mixerL, 1);
AudioConnection          patchCord28(combineL, 0, mixerR, 2);
AudioConnection          patchCord29(oscR01, 0, multiplyR, 0);
AudioConnection          patchCord30(oscR02, 0, combineR, 0);
AudioConnection          patchCord31(rectifyR, 0, mixerR, 3);
AudioConnection          patchCord32(multiplyR, 0, mixerR, 1);
AudioConnection          patchCord33(combineR, 0, mixerL, 2);
AudioConnection          patchCord34(dc, filterEgL);
AudioConnection          patchCord35(dc, filterEgR);
AudioConnection          patchCord36(filterEgL, filterEgLAmp);
AudioConnection          patchCord37(filterEgR, filterEgRAmp);
AudioConnection          patchCord38(filterEgLAmp, 0, lpfL, 1);
AudioConnection          patchCord39(filterEgLAmp, 0, hpfL, 1);
AudioConnection          patchCord40(filterEgRAmp, 0, lpfR, 1);
AudioConnection          patchCord41(filterEgRAmp, 0, hpfR, 1);
AudioConnection          patchCord42(lpfR, 0, filterRMixer, 0);
AudioConnection          patchCord43(lpfL, 0, filterLMixer, 0);
AudioConnection          patchCord44(hpfR, 2, filterRMixer, 1);
AudioConnection          patchCord45(hpfL, 2, filterLMixer, 1);
AudioConnection          patchCord46(filterLMixer, ampEgL);
AudioConnection          patchCord47(filterRMixer, ampEgR);
AudioConnection          patchCord48(ampEgR, 0, output, 1);
AudioConnection          patchCord49(ampEgL, 0, output, 0);

// SGTL5000
AudioControlSGTL5000     control;

// constants
const int AUDIO_MEMORY = 204;

// NOTE: p_varName denotes variables with a corresponding potentiometer
// NOTE: s_varName denotes variables with a corresponding switch (2 or 3-way)

// potentimoeters (+ values)
const int OSC_FREQUENCY_POT = A7;
float p_oscFrequency = 55.0f;
float p_osc01Gain = 0.5f;
float p_osc02Gain = 0.5f;

const char *SAMPLER_FILENAME = "gnd-wrk.wav";

const int DELAY_OSC_FREQUENCY_POT = A7;
float p_delayOscFrequency = 220.0f;
const int DELAY_TIME_POT = A6; 
float p_delayTime = 60.0f; 
float previousDelayTime = 60.0f;
const int DELAY_FEEDBACK_POT = A3;
float p_delayFeedback = 0.0f;

const int FILTER_CUTOFF_POT = A2;
float p_filterEgAttack = 10.0f;
float p_filterEgDecay = 120.0f;
float p_filterEgSustain = 0.4f;
float p_filterEgRelease = 240.0f;
float p_filterCutoff = 320.0f;
float p_filterResonance = 1.4f;
float p_filterOctaveControl = 3.6f;
float p_filterEgInt = 0.0f;

float p_ampEgAttack = 10.0f;
float p_ampEgDecay = 120.0f;
float p_ampEgSustain = 0.4f;
float p_ampEgRelease = 240.0f;

const int MIXER_AMP_POT = A1;
float p_mixerAmp = 1.0f;

// switches (+ values)
int s_osc01Waveform = 1;
int s_osc02Waveform = 3;
float s_oscMultiplier = 1.0f;

int s_delayCombineMode = 1;

enum MixerMode {
    MIXER_OSC = 0,
    MIXER_LINE_IN = 1,
    MIXER_SAMPLER = 2
};
MixerMode s_mixerMode = MIXER_SAMPLER;

// buttons
const int TRIG_BUTTON_PIN = 0;
Bounce trigButton = Bounce(TRIG_BUTTON_PIN, 18);

////////////
// SETUP  //
////////////

void setup() {
    Serial.begin(9600);
    
    initAudio();
    initSdCard();
    
    delay(1000);
    
    initControls();
    initComponents();
}

/*
Initialize Teensy board for number of audio blocks and SGTL5000 enabling.
*/
void initAudio() {
    AudioMemory(AUDIO_MEMORY);
    
    control.enable();
    control.volume(0.75f);
}

/*
 * Initialize communications with the SD card.
 */
void initSdCard() {
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) {
        while (1) {
            Serial.println("Unable to access the SD card");
            delay(500);
        }
    }
}

/*
 * Initialize all controls (buttons, pots, switches, etc.) as needed.
 */
void initControls() {
    // potentiometers

    // switches

    // buttons
    pinMode(TRIG_BUTTON_PIN, INPUT_PULLUP);
}

/*
 * Initialize all audio components declared in sketch with default settings.
 */
void initComponents() {
    // oscillator
    osc01.begin(1.0f, p_oscFrequency, s_osc01Waveform);
    osc02.begin(1.0f, p_oscFrequency, s_osc02Waveform);
    oscMixer.gain(0, p_osc01Gain);
    oscMixer.gain(1, p_osc02Gain);

    inputMixer.gain(0, 0.5f);
    inputMixer.gain(1, 0.5f);

    samplerMixer.gain(0, 0.5f);
    samplerMixer.gain(1, 0.5f);

    processMixerInputs((int) s_mixerMode);

    // delay
    delayL.delay(0, p_delayTime);
    delayL.delay(1, p_delayTime);
    delayL.delay(2, p_delayTime);
    oscL01.begin(1.0f, p_delayOscFrequency, WAVEFORM_SINE);
    oscL02.begin(1.0f, p_delayOscFrequency * 0.25f, WAVEFORM_SINE);
    combineL.setCombineMode(s_delayCombineMode);
    mixerL.gain(0, 1.0f);
    mixerL.gain(1, 0.0f);
    mixerL.gain(2, 0.0f);
    mixerL.gain(3, 0.0f);

    delayR.delay(0, p_delayTime);
    delayR.delay(1, p_delayTime);
    delayR.delay(2, p_delayTime);
    oscR01.begin(1.0f, p_delayOscFrequency, WAVEFORM_SINE);
    oscR02.begin(1.0f, p_delayOscFrequency * 0.25f, WAVEFORM_SINE);
    combineR.setCombineMode(s_delayCombineMode);
    mixerR.gain(0, 1.0f);
    mixerR.gain(1, 0.0f);
    mixerR.gain(2, 0.0f);
    mixerR.gain(3, 0.0f);

    // filter
    dc.amplitude(1.0f);
    filterEgL.attack(p_filterEgAttack);
    filterEgL.decay(p_filterEgDecay);
    filterEgL.sustain(p_filterEgSustain);
    filterEgL.release(p_filterEgRelease);
    filterEgLAmp.gain(p_filterEgInt);
    lpfL.frequency(p_filterCutoff);
    lpfL.resonance(p_filterResonance);
    lpfL.octaveControl(p_filterOctaveControl);

    filterEgR.attack(p_filterEgAttack);
    filterEgR.decay(p_filterEgDecay);
    filterEgR.sustain(p_filterEgSustain);
    filterEgR.release(p_filterEgRelease);
    filterEgRAmp.gain(p_filterEgInt);
    lpfR.frequency(p_filterCutoff);
    lpfR.resonance(p_filterResonance);
    lpfR.octaveControl(p_filterOctaveControl);

    filterLMixer.gain(0, 1.0f);
    filterLMixer.gain(1, 0.0f);
    filterRMixer.gain(0, 1.0f);
    filterRMixer.gain(1, 0.0f);

    // amp
    ampEgL.attack(p_ampEgAttack);
    ampEgL.decay(p_ampEgDecay);
    ampEgL.sustain(p_ampEgSustain);
    ampEgL.release(p_ampEgRelease);

    ampEgR.attack(p_ampEgAttack);
    ampEgR.decay(p_ampEgDecay);
    ampEgR.sustain(p_ampEgSustain);
    ampEgR.release(p_ampEgRelease);
}

////////////
//  LOOP  //
////////////

void loop() {
    checkTrigButton();

    processMixer();
    processDelay(analogRead(DELAY_TIME_POT), analogRead(DELAY_FEEDBACK_POT), analogRead(DELAY_OSC_FREQUENCY_POT));
    processFilter(analogRead(FILTER_CUTOFF_POT));
    processAmp(analogRead(MIXER_AMP_POT));
}

/*
 * Checks if trigger button pressed and triggers noteOn / noteOff
 * for the envelope generators accordingly.
 */
void checkTrigButton() {
    trigButton.update();
    if(trigButton.fallingEdge()) {
        filterEgL.noteOn();
        filterEgR.noteOn();
        ampEgL.noteOn();
        ampEgR.noteOn();
    } else if(trigButton.risingEdge()) {
        filterEgL.noteOff();
        filterEgR.noteOff();
        ampEgL.noteOff();
        ampEgR.noteOff();
    }
}

/*
 * Update mixer inputs and sound generators according to the mixer mode.
 */
void processMixer() {
    processMixerInputs((int) s_mixerMode);

    switch(s_mixerMode) {
        default:
        case MIXER_OSC:
            processOsc(analogRead(OSC_FREQUENCY_POT));
            break;
        case MIXER_LINE_IN:
            processSampler();
            break;
        case MIXER_SAMPLER:
            // TODO
            break;

    }
}

/*
 * Updates mixer inputs to open the active channel and close the others.

 * @param activeChannel number (int) of active channel
 */
void processMixerInputs(int activeChannel) {
    for(int i = 0; i < 4; i++) {
        if(i == activeChannel) {
            mixer.gain(i, 1.0f);
        } else {
            mixer.gain(i, 0.0f);
        }
    }
}

/*
 * Update oscillator's frequency, waveform, and octave multiplier.

 * @param freqIn analog reading of OSC_FREQUENCY_POT
 */
void processOsc(int freqIn) {
    float scaled = freqIn / 1023.0f;
    p_oscFrequency = 20.0f + (2000.0f * scaled);

    p_oscFrequency = 55.0f; // DELETE LATER (when more pots)

    p_oscFrequency *= s_oscMultiplier;

    osc01.frequency(p_oscFrequency);
    osc02.frequency(p_oscFrequency);
}

/*
 * Continuously play the specified sample's wav file.
 */
void processSampler() {
    if(!sampler.isPlaying()) {
        Serial.println("PLAYING");
        sampler.play(SAMPLER_FILENAME);
        delay(1);
    }
}

/*
 * Update the delay's time, feedback, oscillator frequency, and combine mode.
 * Do NOT change delay settings too quickly. Do NOT make delay
 * times greater than 449.39ms (1 block = 2.9ms).

 * @param timeIn analog reading of DELAY_TIME_POT
 * @param feedbackIn analog reading of DELAY_FEEDBACK_POT
 * @param frequencyIn analog reading of DELAY_OSC_FREQUENCY_POT
 */
void processDelay(int timeIn, int feedbackIn, int frequencyIn) {
    float timeScaled = timeIn / 1023.0f;
    p_delayTime = 12.0f + (212.0f * timeScaled);
    if(p_delayTime - previousDelayTime > 0.1f) {
        previousDelayTime = p_delayTime;

        delayL.delay(0, p_delayTime);
        delayL.delay(1, p_delayTime);
        delayL.delay(2, p_delayTime);
        delayR.delay(0, p_delayTime);
        delayR.delay(1, p_delayTime);
        delayR.delay(2, p_delayTime);
    }

    p_delayFeedback = feedbackIn / 1023.0f;
    mixerL.gain(0, 1.0f + (0.75f * -p_delayFeedback));
    mixerL.gain(1, 0.25f * p_delayFeedback);
    mixerL.gain(2, 0.25f * p_delayFeedback);
    mixerL.gain(3, 0.25f * p_delayFeedback);
    mixerR.gain(0, 1.0f + (0.75f * -p_delayFeedback));
    mixerR.gain(1, 0.25f * p_delayFeedback);
    mixerR.gain(2, 0.25f * p_delayFeedback);
    mixerR.gain(3, 0.25f * p_delayFeedback);

    float frequencyScaled = frequencyIn / 1023.0f;
    p_delayOscFrequency = 20.0f + (20000.0f * frequencyScaled);
    oscL01.frequency(p_delayOscFrequency);
    oscL02.frequency(p_delayOscFrequency * 0.25f);
    oscR01.frequency(p_delayOscFrequency);
    oscR02.frequency(p_delayOscFrequency * 0.25f);
}

/*
 * Update the filter's cutoff frequency, resonance, envelope intensity, and octave control.

 * @param cutoffIn analog reading of FILTER_CUTOFF_POT
 */
void processFilter(int cutoffIn) {
    float scaled = cutoffIn / 1023.0f;
    p_filterCutoff = 20.0f + (20000.0f * scaled);

    lpfL.frequency(p_filterCutoff);
    lpfR.frequency(p_filterCutoff);
}

/*
 * Update the amplifier's level.

 * @param ampIn analog reading of 
 */
void processAmp(int ampIn) {
    p_mixerAmp = ampIn / 1023.0f;

    control.volume(p_mixerAmp);
}
