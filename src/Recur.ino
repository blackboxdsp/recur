#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#include <Bounce.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// OSC
AudioSynthWaveform       osc01;     
AudioSynthWaveform       osc02;  
AudioMixer4              oscMixer; 
AudioInputI2S            input;          
AudioMixer4              inputMixer;        
AudioPlaySdWav           sampler;     
AudioMixer4              samplerMixer;
AudioMixer4              mixer;       

// DELAY
AudioEffectDelay         delayL;         
AudioMixer4              mixerL;         
AudioSynthWaveform       oscL01;        
AudioEffectDigitalCombine combineL01; 
AudioSynthWaveform       oscL02;        
AudioEffectDigitalCombine combineL02;       
AudioEffectDelay         delayR;        
AudioMixer4              mixerR;         
AudioSynthWaveform       oscR01;         
AudioEffectDigitalCombine combineR01;
AudioSynthWaveform       oscR02;         
AudioEffectDigitalCombine combineR02; 

// FILTER
AudioSynthWaveformDc     dc;           
AudioEffectEnvelope      filterEgL;      
AudioFilterStateVariable lpfL;        
AudioFilterStateVariable hpfL; 
AudioEffectEnvelope      ampEgL;   
AudioEffectEnvelope      filterEgR;
AudioFilterStateVariable lpfR;
AudioFilterStateVariable hpfR; 
AudioEffectEnvelope      ampEgR; 

// OUTPUT
AudioOutputI2S           output;        

// PATCHING
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
AudioConnection          patchCord32(filterEgL, 0, lpfL, 1);
AudioConnection          patchCord33(filterEgL, 0, hpfL, 1);
AudioConnection          patchCord34(filterEgR, 0, lpfR, 1);
AudioConnection          patchCord35(filterEgR, 0, hpfR, 1);
AudioConnection          patchCord36(lpfR, 0, ampEgR, 0);
AudioConnection          patchCord37(lpfL, 0, ampEgL, 0);
AudioConnection          patchCord38(ampEgR, 0, output, 1);
AudioConnection          patchCord39(ampEgL, 0, output, 0);

// SGTL5000
AudioControlSGTL5000     control;

void setup() {
    Serial.begin(9600);
    
    initAudio();
    //initSdCard();
    
    delay(1000);
    
    initControls();
    initComponents();
}

void initAudio() {
    AudioMemory(8);
    
    control.enable();
    control.volume(0.75);
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

}

void initComponents() {
  
}

void loop() {
    float amplitude = analogRead(A1) / 1023.0f;
    control.volume(amplitude);
}