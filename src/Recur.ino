#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#include <Bounce.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       osc02;  //xy=88,255
AudioInputI2S            input;           //xy=88,329
AudioPlaySdWav           sampler;     //xy=88,442
AudioSynthWaveform       osc01;      //xy=91,202
AudioMixer4              oscMixer; //xy=256,222
AudioMixer4              inputMixer;         //xy=260,342
AudioMixer4              samplerMixer; //xy=270,453
AudioMixer4              mixer;         //xy=419,349
AudioSynthWaveformModulated modL; //xy=733,286
AudioSynthWaveformModulated modR;   //xy=736,425
AudioMixer4              mixerL; //xy=948,293
AudioMixer4              mixerR;         //xy=960.4999771118164,414.49999618530273
AudioEffectDelay         delayR;         //xy=1199,526
AudioEffectDelay         delayL; //xy=1207,187
AudioAmplifier           delayAmpR; //xy=1214,617
AudioAmplifier           delayAmpL;           //xy=1221,96
AudioSynthWaveformDc     dc;            //xy=1447,366
AudioEffectEnvelope      filterEgR; //xy=1611,554
AudioEffectEnvelope      filterEgL;      //xy=1617,194
AudioAmplifier           filterEgRAmp; //xy=1773,553
AudioAmplifier           filterEgLAmp;           //xy=1780,194
AudioFilterStateVariable hpfR; //xy=1958,442
AudioFilterStateVariable lpfR; //xy=1961,503
AudioFilterStateVariable hpfL; //xy=1962,300
AudioFilterStateVariable lpfL;        //xy=1964,237
AudioMixer4              filterRMixer; //xy=2185,462
AudioMixer4              filterLMixer;         //xy=2187,265
AudioEffectEnvelope      ampEgR; //xy=2561,462
AudioEffectEnvelope      ampEgL;      //xy=2564,265
AudioOutputI2S           output;         //xy=2804,360
AudioConnection          patchCord1(osc02, 0, oscMixer, 1);
AudioConnection          patchCord2(input, 0, inputMixer, 0);
AudioConnection          patchCord3(input, 1, inputMixer, 1);
AudioConnection          patchCord4(sampler, 0, samplerMixer, 0);
AudioConnection          patchCord5(sampler, 1, samplerMixer, 1);
AudioConnection          patchCord6(osc01, 0, oscMixer, 0);
AudioConnection          patchCord7(oscMixer, 0, mixer, 0);
AudioConnection          patchCord8(inputMixer, 0, mixer, 1);
AudioConnection          patchCord9(samplerMixer, 0, mixer, 2);
AudioConnection          patchCord10(mixer, 0, mixerR, 0);
AudioConnection          patchCord11(mixer, 0, mixerL, 0);
AudioConnection          patchCord12(mixer, 0, modL, 0);
AudioConnection          patchCord13(mixer, 0, modR, 0);
AudioConnection          patchCord14(modL, 0, mixerL, 1);
AudioConnection          patchCord15(modL, delayL);
AudioConnection          patchCord16(modR, 0, mixerR, 1);
AudioConnection          patchCord17(modR, delayR);
AudioConnection          patchCord18(mixerL, delayL);
AudioConnection          patchCord19(mixerL, 0, lpfL, 0);
AudioConnection          patchCord20(mixerL, 0, hpfL, 0);
AudioConnection          patchCord21(mixerR, 0, lpfR, 0);
AudioConnection          patchCord22(mixerR, 0, hpfR, 0);
AudioConnection          patchCord23(mixerR, delayR);
AudioConnection          patchCord24(delayR, 0, delayAmpR, 0);
AudioConnection          patchCord25(delayL, 0, delayAmpL, 0);
AudioConnection          patchCord26(delayAmpR, 0, mixerR, 2);
AudioConnection          patchCord27(delayAmpR, 0, hpfR, 0);
AudioConnection          patchCord28(delayAmpR, 0, lpfR, 0);
AudioConnection          patchCord29(delayAmpL, 0, mixerL, 2);
AudioConnection          patchCord30(delayAmpL, 0, lpfL, 0);
AudioConnection          patchCord31(delayAmpL, 0, hpfL, 0);
AudioConnection          patchCord32(dc, filterEgL);
AudioConnection          patchCord33(dc, filterEgR);
AudioConnection          patchCord34(filterEgR, filterEgRAmp);
AudioConnection          patchCord35(filterEgL, filterEgLAmp);
AudioConnection          patchCord36(filterEgRAmp, 0, lpfR, 1);
AudioConnection          patchCord37(filterEgRAmp, 0, hpfR, 1);
AudioConnection          patchCord38(filterEgLAmp, 0, lpfL, 1);
AudioConnection          patchCord39(filterEgLAmp, 0, hpfL, 1);
AudioConnection          patchCord40(hpfR, 2, filterRMixer, 1);
AudioConnection          patchCord41(lpfR, 0, filterRMixer, 0);
AudioConnection          patchCord42(hpfL, 2, filterLMixer, 1);
AudioConnection          patchCord43(lpfL, 0, filterLMixer, 0);
AudioConnection          patchCord44(filterRMixer, ampEgR);
AudioConnection          patchCord45(filterLMixer, ampEgL);
AudioConnection          patchCord46(ampEgR, 0, output, 1);
AudioConnection          patchCord47(ampEgL, 0, output, 0);
AudioControlSGTL5000     control;        //xy=83,48
// GUItool: end automatically generated code

// constants
const int AUDIO_MEMORY = 192;

// NOTE: p_varName denotes variables with a corresponding potentiometer
// NOTE: s_varName denotes variables with a corresponding switch (2 or 3-way)

// potentimoeters (+ values)
const int OSC_FREQUENCY_POT = A7;
float p_oscFrequency = 55.0f;
float p_osc01Gain = 0.5f;
float p_osc02Gain = 0.5f;

const char *SAMPLER_FILENAME = "gnd-wrk.wav";

const int MOD_FREQUENCY_POT = A2;
float p_modFrequency = 520.0f;
const int MOD_DEPTH_POT = A7;
float p_modDepth = 0.0f;
const int MOD_PHASE_POT = A6;
float p_modPhase = 0.0f;

const int DELAY_TIME_POT = A3; 
float p_delayTime = 60.0f; 
float previousDelayTime = p_delayTime;
const int DELAY_FEEDBACK_POT = A2;
float p_delayFeedback = 0.0f;

const int FILTER_CUTOFF_POT = A2;
float p_filterEgAttack = 10.0f;
float p_filterEgDecay = 120.0f;
float p_filterEgSustain = 0.4f;
float p_filterEgRelease = 240.0f;
float p_filterCutoff = 20000.0f;
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

int s_modWaveform = 2;

enum MixerMode {
    MIXER_OSC = 0,
    MIXER_LINE_IN = 1,
    MIXER_SAMPLER = 2
};
MixerMode s_mixerMode = MIXER_OSC;

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

    // mod
    modL.begin(p_modDepth, p_modFrequency, s_modWaveform);    
    modL.phaseModulation(0.0f);

    modR.begin(p_modDepth, p_modFrequency, s_modWaveform);
    modR.phaseModulation(0.0f);

    // delay
    delayL.delay(0, p_delayTime);
    delayL.delay(1, 0.25f * p_delayTime);
    delayAmpL.gain(p_delayFeedback);

    delayR.delay(0, p_delayTime);
    delayR.delay(1, 0.25f * p_delayTime);
    delayAmpR.gain(p_delayFeedback);

    // mixing (mod + delay)
    mixerL.gain(0, 0.5f);
    mixerL.gain(1, 0.5f);
    mixerL.gain(2, 0.5f);

    mixerL.gain(0, 0.5f);
    mixerL.gain(1, 0.5f);
    mixerL.gain(2, 0.5f);

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
    processMod(analogRead(MOD_FREQUENCY_POT), analogRead(MOD_DEPTH_POT), analogRead(MOD_PHASE_POT));
    processDelay(analogRead(DELAY_TIME_POT), analogRead(DELAY_FEEDBACK_POT));
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
void processOsc(int frequencyIn) {
    float scaled = frequencyIn / 1023.0f;
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
    Serial.println("Trying to play sample...");
    if(!sampler.isPlaying()) {
        Serial.println("PLAYING");
        sampler.play(SAMPLER_FILENAME);
        delay(1);
    }
}

/*
 *
 * 
 */
void processMod(int frequencyIn, int depthIn, int phaseIn) {
    float freqScaled = frequencyIn / 1023.0f;
    p_modFrequency = 0.00000001f + (10000.0f * freqScaled);
    p_modFrequency = 520.0f;
    modL.frequency(p_modFrequency);
    modR.frequency(p_modFrequency);

    p_modDepth = depthIn / 1023.0f;
    delay(50);
    modL.amplitude(p_modDepth);
    modR.amplitude(p_modDepth);

    float phaseScaled = phaseIn / 1023.0f;
    p_modPhase = 180.0f * phaseScaled;
    modL.phaseModulation(p_modPhase);
    modR.phaseModulation(p_modPhase);
}

/*
 * Update the delay's time, feedback, oscillator frequency, and combine mode.
 * Do NOT change delay settings too quickly. Do NOT make delay
 * times greater than 449.39ms (1 block = 2.9ms).

 * @param timeIn analog reading of DELAY_TIME_POT
 * @param feedbackIn analog reading of DELAY_FEEDBACK_POT
 * @param frequencyIn analog reading of DELAY_OSC_FREQUENCY_POT
 */
void processDelay(int timeIn, int feedbackIn) {
    float timeScaled = timeIn / 1023.0f;
    p_delayTime = 10.0f + (170.0f * timeScaled);
    if(abs(p_delayTime - previousDelayTime) > 0.3f) {
        previousDelayTime = p_delayTime;
        delayL.delay(0, p_delayTime);
        delayL.delay(1, 0.25f * p_delayTime);

        delayR.delay(0, p_delayTime);
        delayR.delay(1, 0.25f * p_delayTime);
    }

    p_delayFeedback = feedbackIn / 1023.0f;
    delayAmpL.gain(p_delayFeedback);
    delayAmpR.gain(p_delayFeedback);
}

/*
 * Update the filter's cutoff frequency, resonance, envelope intensity, and octave control.

 * @param cutoffIn analog reading of FILTER_CUTOFF_POT
 */
void processFilter(int cutoffIn) {
    float scaled = cutoffIn / 1023.0f;
    p_filterCutoff = 20.0f + (20000.0f * scaled);
    
    p_filterCutoff = 1200.0f; // CHANGE LATER

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
