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
AudioMixer4              mixerL;
AudioSynthWaveform       oscL01;
AudioSynthWaveform       oscL02;
AudioEffectDigitalCombine combineL01; 
AudioEffectDigitalCombine combineL02;      
AudioEffectDelay         delayR;
AudioMixer4              mixerR;
AudioSynthWaveform       oscR01;
AudioSynthWaveform       oscR02;
AudioEffectDigitalCombine combineR01; 
AudioEffectDigitalCombine combineR02; 

// filter
AudioSynthWaveformDc     dc;          
AudioEffectEnvelope      filterEgL;   
AudioAmplifier           filterEgLAmp;        
AudioMixer4              filterEgLAmpMixer;   
AudioFilterStateVariable lpfL;      
AudioFilterStateVariable hpfL;
AudioMixer4              filterLMixer;   
AudioEffectEnvelope      filterEgR; 
AudioAmplifier           filterEgRAmp; 
AudioMixer4              filterEgRAmpMixer; 
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
AudioConnection          patchCord12(delayL, 0, combineL01, 1);
AudioConnection          patchCord13(delayL, 1, combineL02, 1);
AudioConnection          patchCord14(delayR, 0, combineR01, 1);
AudioConnection          patchCord15(delayR, 1, combineR02, 1);
AudioConnection          patchCord16(mixerL, delayL);
AudioConnection          patchCord17(mixerL, 0, lpfL, 0);
AudioConnection          patchCord18(mixerL, 0, hpfL, 0);
AudioConnection          patchCord19(mixerR, delayR);
AudioConnection          patchCord20(mixerR, 0, lpfR, 0);
AudioConnection          patchCord21(mixerR, 0, hpfR, 0);
AudioConnection          patchCord22(oscL02, 0, combineL02, 0);
AudioConnection          patchCord23(oscL01, 0, combineL01, 0);
AudioConnection          patchCord24(oscR02, 0, combineR02, 0);
AudioConnection          patchCord25(oscR01, 0, combineR01, 0);
AudioConnection          patchCord26(combineL02, 0, mixerR, 2);
AudioConnection          patchCord27(combineL01, 0, mixerL, 1);
AudioConnection          patchCord28(combineR02, 0, mixerL, 2);
AudioConnection          patchCord29(combineR01, 0, mixerR, 1);
AudioConnection          patchCord30(dc, filterEgL);
AudioConnection          patchCord31(dc, filterEgR);
AudioConnection          patchCord32(filterEgL, filterEgLAmp);
AudioConnection          patchCord33(filterEgL, 0, filterEgLAmpMixer, 0);
AudioConnection          patchCord34(filterEgR, filterEgRAmp);
AudioConnection          patchCord35(filterEgR, 0, filterEgRAmpMixer, 0);
AudioConnection          patchCord36(filterEgRAmp, 0, filterEgRAmpMixer, 1);
AudioConnection          patchCord37(filterEgLAmp, 0, filterEgLAmpMixer, 1);
AudioConnection          patchCord38(filterEgRAmpMixer, 0, lpfR, 1);
AudioConnection          patchCord39(filterEgRAmpMixer, 0, hpfR, 1);
AudioConnection          patchCord40(filterEgLAmpMixer, 0, lpfL, 1);
AudioConnection          patchCord41(filterEgLAmpMixer, 0, hpfL, 1);
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

// potentimoeters
const int OSC_FREQUENCY_POT = A7;
float p_oscFrequency = 110.0f;
float p_osc01Gain = 0.5f;
float p_osc02Gain = 0.5f;

const int DELAY_OSC_FREQUENCY = A6;
const int DELAY_TIME = A3;

const int FILTER_CUTOFF = A2;
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
int s_oscWaveform = 1;
float s_oscMultiplier = 1.0f;

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
    AudioMemory(32);
    
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
    osc01.begin(1.0f, p_oscFrequency, s_oscWaveform);
    osc02.begin(1.0f, p_oscFrequency, s_oscWaveform);
    oscMixer.gain(0, p_osc01Gain);
    oscMixer.gain(1, p_osc02Gain);
    mixer.gain(0, 1.0f);

    // delay
    mixerL.gain(0, 1.0f);
    mixerR.gain(0, 1.0f);

    // filter
    dc.amplitude(1.0f);
    filterEgL.attack(p_filterEgAttack);
    filterEgL.decay(p_filterEgDecay);
    filterEgL.sustain(p_filterEgSustain);
    filterEgL.release(p_filterEgRelease);
    filterEgLAmp.gain(1.0f);
    filterEgLAmpMixer.gain(0, 1.0f - p_filterEgInt);
    filterEgLAmpMixer.gain(0, p_filterEgInt);
    lpfL.frequency(p_filterCutoff);
    lpfL.resonance(p_filterResonance);
    lpfL.octaveControl(p_filterOctaveControl);

    filterEgR.attack(p_filterEgAttack);
    filterEgR.decay(p_filterEgDecay);
    filterEgR.sustain(p_filterEgSustain);
    filterEgR.release(p_filterEgRelease);
    filterEgRAmp.gain(1.0f);
    filterEgRAmpMixer.gain(0, 1.0f - p_filterEgInt);
    filterEgRAmpMixer.gain(0, p_filterEgInt);
    lpfR.frequency(p_filterCutoff);
    lpfR.resonance(p_filterResonance);
    lpfR.octaveControl(p_filterOctaveControl);

    filterLMixer.gain(0, 0.5f);
    filterRMixer.gain(1, 0.5f);

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

    float amplitude = analogRead(A1) / 1023.0f;
    control.volume(amplitude);
}