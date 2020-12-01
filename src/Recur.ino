#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#include <assert.h>
#include <Bounce.h>
#include <MIDI.h>

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

// NOTE: m_varName denotes variables with corresponding MIDI data
// NOTE: p_varName denotes variables with a corresponding potentiometer
// NOTE: s_varName denotes variables with a corresponding switch (2 or 3-way)

// constants
const int BAUD_RATE = 9600;
const int AUDIO_MEMORY = 192;
const int MSEC_TIMER = 15;
elapsedMillis msec = 0;

// multiplexers
const int MUX_DELAY_TIME = 40;
const int MUX_01_S0 = 0;
const int MUX_01_Z = A2;
const int MUX_02_S0 = 3;
const int MUX_02_Z = A3;

// MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI_2)
int m_note;
int m_velocity;

// potentimoeters (+ values)
float p_oscFrequency = 55.0f;
float p_osc01Gain = 0.5f;
float p_osc02Gain = 0.5f;

const char *SAMPLER_FILENAME = "gnd-wrk.wav";

const int MOD_FREQ_POT = 0;
float p_modFrequency = 520.0f;
const int MOD_DEPTH_POT = 2;
float p_modDepth = 0.0f;
const int MOD_PHASE_POT = 1;
float p_modPhase = 0.0f;

const int DLY_TIME_POT = 3;
float p_delayTime = 60.0f; 
float previousDelayTime = p_delayTime;
const int DLY_FBCK_POT = 4;
float p_delayFeedback = 0.0f;

const int FLTR_EG_ATK_POT = 4;
float p_filterEgAttack = 10.0f;
const int FLTR_EG_DEC_POT = 5;
float p_filterEgDecay = 120.0f;
const int FLTR_EG_SUS_POT = 6;
float p_filterEgSustain = 0.4f;
const int FLTR_EG_REL_POT = 7;
float p_filterEgRelease = 240.0f;
const int FLTR_CUTOFF_POT = 5;
float p_filterCutoff = 20000.0f;
const int FLTR_RES_POT = 6;
float p_filterResonance = 1.4f;
float p_filterOctaveControl = 3.6f;
float p_filterEgInt = 0.2f;

const int AMP_EG_ATK_POT = 0;
float p_ampEgAttack = 10.0f;
const int AMP_EG_DEC_POT = 1;
float p_ampEgDecay = 120.0f;
const int AMP_EG_SUS_POT = 2;
float p_ampEgSustain = 0.4f;
const int AMP_EG_REL_POT = 3;
float p_ampEgRelease = 240.0f;

const int MIXER_AMP_POT = 7;
float p_mixerAmp = 1.0f;

// switches (+ values)
int s_osc01Waveform = 1;
int s_osc02Waveform = 3;
float s_oscMultiplier = 1.0f;

int s_modWaveform = 2;

enum FilterMode {
    LOW_PASS_FILTER = 0,
    HIGH_PASS_FILTER = 1
};
FilterMode s_filterMode = LOW_PASS_FILTER;

enum MixerMode {
    OSC = 0,
    LINE_IN = 1,
    SAMPLER = 2
};
MixerMode s_mixerMode = OSC;

// buttons
const int TRIG_BUTTON_PIN = 8;
Bounce trigButton = Bounce(TRIG_BUTTON_PIN, 18);

////////////
// SETUP  //
////////////

void setup() {
    Serial.begin(BAUD_RATE);

    initAudio();
    //initMIDI();
    //initSdCard();
    initMultiplexer(MUX_01_S0, MUX_01_Z);
    initMultiplexer(MUX_02_S0, MUX_02_Z);
    
    delay(1000);
    
    initControls();
    initComponents();
}

/*
 * Initialize Teensy board for number of audio blocks and SGTL5000 enabling.
 */
void initAudio() {
    AudioMemory(AUDIO_MEMORY);
    
    control.enable();
    control.volume(0.75f);
}

/*
 *
 */
void initMIDI() {
    MIDI_2.begin(MIDI_CHANNEL_OMNI);
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
 *
 */
void initMultiplexer(uint8_t muxS0, uint8_t muxZ) {
    for(int i = 0; i < 3; i++) {
        pinMode(muxS0 + i, OUTPUT);
        digitalWrite(muxS0 + i, LOW);
        delayMicroseconds(MUX_DELAY_TIME);
    }
}

/*
 * Initialize all controls (buttons, pots, switches, etc.) as needed.
 */
void initControls() {
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
    if(msec < MSEC_TIMER) return;
    msec = 0;

    checkTrigButton();

    processMIDI();

    processMixer();
    processMod();
    processDelay();
    processFilter();
    processAmp();
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

void processMIDI() {
    int type, note, velocity, channel, data1, data2;

    if(MIDI_2.read()) {
        type = MIDI_2.getType();
        switch(type) {
            case midi::NoteOn:
                channel = MIDI_2.getChannel();
                note = MIDI_2.getData1();
                velocity = MIDI_2.getData2();

                m_note = note;
                m_velocity = velocity;

                if(velocity > 0) {
                    printMIDIData("Note On", channel, note, velocity);
                } else {
                    printMIDIData("Note Off", channel, note, velocity);
                }

                MIDI_2.sendNoteOn(note, velocity, channel);

                break;

            case midi::NoteOff:
                note = MIDI_2.getData1();
                velocity = MIDI_2.getData2();
                channel = MIDI_2.getChannel();

                m_note = note;
                m_velocity = velocity;

                printMIDIData("Note Off", channel, note, velocity);

                MIDI_2.sendNoteOff(note, velocity, channel);

                break;

            default: 
                data1 = MIDI_2.getData1();
                data2 = MIDI_2.getData2();

                printMIDIData(data1, data2);

                break;
        }
    }
}

float noteToFreq(int note) {
    float a = 440.0f;
    return (a / 32.0f) * pow(2, ((note - 9) / 12.0f));
}

/* 
 * Print non-musical MIDI data to Serial console
 * 
 * @param data1 MIDI data1 from method call
 * @param data2 MIDI data2 from method call
 */
void printMIDIData(int data1, int data2) {
    Serial.print("Message: data1=");
    Serial.print(data1);
    Serial.print(", data2=");
    Serial.println(data2);
}

/* 
 * Print musical MIDI data to Serial console
 * 
 * @param start starting string to print
 * @param channel the MIDI channel
 * @param note the MIDI note number
 * @param velocity the MIDI note velocity
 */
void printMIDIData(String start, int channel, int note, int velocity) {
    Serial.print(start);
    Serial.print(": ch=");
    Serial.print(channel);
    Serial.print(", note=");
    Serial.print(note);
    Serial.print(", velocity=");
    Serial.println(velocity);
}

/*
 * Update mixer inputs and sound generators according to the mixer mode.
 */
void processMixer() {
    processMixerInputs((int) s_mixerMode);

    switch(s_mixerMode) {
        default:
        case OSC:
            processOsc();
            break;
        case LINE_IN:
            processSampler();
            break;
        case SAMPLER:
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
 */
void processOsc() {
    float scaled = 0.2f; // CHANGE
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
void processMod() {
    float freq = readMuxZPin(MOD_FREQ_POT, MUX_01_S0, MUX_01_Z);
    p_modFrequency = 0.00000001f + (5200.0f * freq);
    modL.frequency(p_modFrequency);
    modR.frequency(p_modFrequency);

    p_modDepth = readMuxZPin(MOD_DEPTH_POT, MUX_01_S0, MUX_01_Z);
    modL.amplitude(p_modDepth);
    modR.amplitude(p_modDepth);

    float phase = readMuxZPin(MOD_PHASE_POT, MUX_01_S0, MUX_01_Z);
    p_modPhase = 180.0f * phase;
    modL.phaseModulation(p_modPhase);
    modR.phaseModulation(p_modPhase);
}

/*
 * Update the delay's time, feedback, oscillator frequency, and combine mode.
 * Do NOT change delay settings too quickly. Do NOT make delay
 * times greater than 449.39ms (1 block = 2.9ms).
 */
void processDelay() {
    float time = readMuxZPin(DLY_TIME_POT, MUX_01_S0, MUX_01_Z);
    p_delayTime = 10.0f + (170.0f * time);
    if(abs(p_delayTime - previousDelayTime) > 0.3f) {
        previousDelayTime = p_delayTime;
        delayL.delay(0, p_delayTime);
        delayL.delay(1, 0.25f * p_delayTime);

        delayR.delay(0, p_delayTime);
        delayR.delay(1, 0.25f * p_delayTime);
    }

    p_delayFeedback = readMuxZPin(DLY_FBCK_POT, MUX_01_S0, MUX_01_Z);
    delayAmpL.gain(p_delayFeedback);
    delayAmpR.gain(p_delayFeedback);
}

/*
 * Update the filter's cutoff frequency, resonance, envelope intensity, and octave control.
 */
void processFilter() {
    filterLMixer.gain(s_filterMode, 1.0f);
    filterLMixer.gain(!s_filterMode, 0.0f);

    filterRMixer.gain(s_filterMode, 1.0f);
    filterRMixer.gain(!s_filterMode, 0.0f);

    float cutoff = readMuxZPin(FLTR_CUTOFF_POT, MUX_01_S0, MUX_01_Z);
    p_filterCutoff = 20.0f + (20000.0f * cutoff);

    float resonance = readMuxZPin(FLTR_RES_POT, MUX_01_S0, MUX_01_Z);
    p_filterResonance = 0.8f + (4.0f * resonance);

    float attack = 1000.0f * readMuxZPin(FLTR_EG_ATK_POT, MUX_02_S0, MUX_02_Z);
    if(attack != p_filterEgAttack) {
        p_filterEgAttack = attack;
        filterEgL.attack(p_filterEgAttack);
        filterEgR.attack(p_filterEgAttack);
    }
    float decay = 1000.0f * readMuxZPin(FLTR_EG_DEC_POT, MUX_02_S0, MUX_02_Z);
    if(decay != p_filterEgDecay) {
        p_filterEgDecay = decay;
        filterEgL.decay(p_filterEgDecay);
        filterEgR.decay(p_filterEgDecay);
    }
    float sustain = readMuxZPin(FLTR_EG_SUS_POT, MUX_02_S0, MUX_02_Z);
    if(sustain != p_filterEgSustain) {
        p_filterEgSustain = sustain;
        filterEgL.sustain(p_filterEgSustain);
        filterEgR.sustain(p_filterEgSustain);
    }
    float release = 2000.0f * readMuxZPin(FLTR_EG_REL_POT, MUX_02_S0, MUX_02_Z);
    if(release != p_filterEgRelease) {
        p_filterEgRelease = release;
        filterEgL.release(p_filterEgRelease);
        filterEgR.release(p_filterEgRelease);
    }

    switch(s_filterMode) {
        default:
        case LOW_PASS_FILTER:
            lpfL.frequency(p_filterCutoff);
            lpfR.frequency(p_filterCutoff);

            lpfL.resonance(p_filterResonance);
            lpfR.resonance(p_filterResonance);
            break;

        case HIGH_PASS_FILTER:
            hpfL.frequency(p_filterCutoff);
            hpfR.frequency(p_filterCutoff);

            hpfL.resonance(p_filterResonance);
            hpfR.resonance(p_filterResonance);
            break;
    }
}

/*
 * Update the amplifier's level.
 */
void processAmp() {

    float attackScaled = readMuxZPin(AMP_EG_ATK_POT, MUX_02_S0, MUX_02_Z);
    ampEgL.attack(attackScaled * 1000.0f);
    ampEgR.attack(attackScaled * 1000.0f);
    float decayScaled = readMuxZPin(AMP_EG_DEC_POT, MUX_02_S0, MUX_02_Z);
    ampEgL.decay(decayScaled * 1000.0f);
    ampEgR.decay(decayScaled * 1000.0f);
    float sustainScaled = readMuxZPin(AMP_EG_SUS_POT, MUX_02_S0, MUX_02_Z);
    ampEgL.sustain(sustainScaled);
    ampEgR.sustain(sustainScaled);
    float releaseScaled = readMuxZPin(AMP_EG_REL_POT, MUX_02_S0, MUX_02_Z);
    ampEgL.release(releaseScaled * 2000.0f);
    ampEgR.release(releaseScaled * 2000.0f);

    p_mixerAmp = readMuxZPin(MIXER_AMP_POT, MUX_01_S0, MUX_01_Z);

    control.volume(p_mixerAmp);
}

/* 
 * Return the multiplexer's z value given a corresponding y pin (after
 * setting its' digital pins accordingly).
 * 
 * @param muxY analog pin to read
 * @param muxS0 starting digital pin of where the multiplexer is plugged in (assumes they are sequential)
 * @param muxZ analog pin receiving the multiplexer's z-output
 */
float readMuxZPin(int muxY, int muxS0, int muxZ) {
    setMuxSPins(muxY, muxS0, muxZ);

    return analogRead(muxZ) / 1023.0f;
}

/* 
 * Set the digital s pins of the given multiplexer according to the specified 
 * y pin.
 * 
 * @param muxY analog pin to read
 * @param muxS0 starting digital pin of where the multiplexer is plugged in (assumes they are sequential)
 * @param muxZ analog pin receiving the multiplexer's z-output
 */
void setMuxSPins(int muxY, int muxS0, int muxZ) {
    for(int i = 0; i < 3; i++) {
        bool expr = muxY & (1 << i);
        digitalWrite(muxS0 + i, expr ? HIGH : LOW);
    }

    delayMicroseconds(MUX_DELAY_TIME);
}
