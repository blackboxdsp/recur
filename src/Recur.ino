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

// potentimoeters
const int OSC_FREQUENCY_POT = A7;
float p_oscFrequency = 110.0f;
float p_osc01Gain = 0.5f;
float p_osc02Gain = 0.5f;

float p_delayOscFrequency = 220.0f;
const int DELAY_TIME_POT = A6; 
float p_delayTime = 60.0f; 
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

// switches
int s_osc01Waveform = 1;
int s_osc02Waveform = 3;
float s_oscMultiplier = 1.0f;

int s_delayCombineMode = 1;

// buttons
const int TRIG_BUTTON_PIN = 0;
Bounce trigButton = Bounce(TRIG_BUTTON_PIN, 18);

////////////
// SETUP  //
////////////

void setup() {
    Serial.begin(9600);
    
    initAudio();
    //initSdCard();
    
    delay(1000);
    
    initControls();
    initComponents();
}

void initAudio() {
    AudioMemory(AUDIO_MEMORY);
    
    control.enable();
    control.volume(0.75f);
}

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

void initControls() {
    // potentiometers

    // switches

    // buttons
    pinMode(TRIG_BUTTON_PIN, INPUT_PULLUP);
}

void initComponents() {
    // oscillator
    osc01.begin(1.0f, p_oscFrequency, s_osc01Waveform);
    osc02.begin(1.0f, p_oscFrequency, s_osc02Waveform);
    oscMixer.gain(0, p_osc01Gain);
    oscMixer.gain(1, p_osc02Gain);
    mixer.gain(0, 1.0f);

    // delay
    delayL.delay(0, p_delayTime);
    delayL.delay(1, p_delayTime * 0.5f);
    delayL.delay(2, p_delayTime * 2.0f);
    oscL01.begin(1.0f, p_delayOscFrequency * 0.98f, WAVEFORM_SINE);
    oscL02.begin(1.0f, p_delayOscFrequency * 1.02f, WAVEFORM_SINE);
    combineL.setCombineMode(s_delayCombineMode);
    mixerL.gain(0, 0.5f);
    mixerL.gain(1, 0.24f);
    mixerL.gain(2, 0.18f);
    mixerL.gain(3, 0.08f);

    delayR.delay(0, p_delayTime);
    delayR.delay(1, p_delayTime * 0.5f);
    delayR.delay(2, p_delayTime * 2.0f);
    oscR01.begin(1.0f, p_delayOscFrequency * 1.02f, WAVEFORM_SINE);
    oscR02.begin(1.0f, p_delayOscFrequency * 0.98f, WAVEFORM_SINE);
    combineR.setCombineMode(s_delayCombineMode);
    mixerR.gain(0, 0.5f);
    mixerR.gain(1, 0.24f);
    mixerR.gain(2, 0.18f);
    mixerR.gain(3, 0.08f);

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

    filterLMixer.gain(0, 0.5f);
    filterLMixer.gain(1, 0.0f);
    filterRMixer.gain(0, 0.5f);
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

    processOsc(analogRead(OSC_FREQUENCY_POT));
    processDelay(analogRead(DELAY_TIME_POT), analogRead(DELAY_FEEDBACK_POT));
    processFilter(analogRead(FILTER_CUTOFF_POT));
    processAmp(analogRead(A1));
}

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

void processOsc(int freqIn) {
    float scaled = freqIn / 1023.0f;
    p_oscFrequency = 20.0f + (scaled * 520.0f);
    p_oscFrequency *= s_oscMultiplier;

    osc01.frequency(p_oscFrequency);
    osc02.frequency(p_oscFrequency);
}

void processDelay(int timeIn, int feedbackIn) {
    // CAUTION: Maximum delay time is at 449.39ms for Teensy 3.2 @ (2.9ms / mem block)
    float timeScaled = timeIn / 1023.0f;
    p_delayTime = (timeScaled * 120.0f) + 12.0f;
    delayL.delay(0, p_delayTime);
    delayL.delay(1, p_delayTime * 0.5f);
    delayL.delay(2, p_delayTime * 2.0f);
    delayR.delay(0, p_delayTime);
    delayR.delay(1, p_delayTime * 0.5f);
    delayR.delay(2, p_delayTime * 2.0f);

    float feedbackScaled = feedbackIn / 1023.0f;
    mixerL.gain(0, 1.0f + (0.5f * -feedbackScaled));
    mixerL.gain(1, 0.24f * feedbackScaled);
    mixerL.gain(2, 0.18f * feedbackScaled);
    mixerL.gain(3, 0.08f * feedbackScaled);
    mixerR.gain(0, 1.0f + (0.5f * -feedbackScaled));
    mixerR.gain(1, 0.24f * feedbackScaled);
    mixerR.gain(2, 0.18f * feedbackScaled);
    mixerR.gain(3, 0.08f * feedbackScaled);
}

void processFilter(int cutoffIn) {
    float scaled = cutoffIn / 1023.0f;
    p_filterCutoff = 20.0f + (scaled * 20000.0f);

    lpfL.frequency(p_filterCutoff);
    lpfR.frequency(p_filterCutoff);
}

void processAmp(int ampIn) {
    float amplitude = ampIn / 1023.0f;

    control.volume(amplitude);
}
