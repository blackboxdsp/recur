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
AudioAmplifier           ampEgRAmp; //xy=2618,463
AudioAmplifier           ampEgLAmp;           //xy=2623,266
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
AudioConnection          patchCord46(filterRMixer, ampEgR);
AudioConnection          patchCord47(filterLMixer, ampEgL);
AudioConnection          patchCord48(ampEgL, ampEgLAmp);
AudioConnection          patchCord49(ampEgR, ampEgRAmp);
AudioConnection          patchCord50(ampEgRAmp, 0, output, 1);
AudioConnection          patchCord51(ampEgLAmp, 0, output, 0);
AudioControlSGTL5000     control;
// GUItool: end automatically generated code

// NOTE: m_varName denotes variables with corresponding MIDI data
// NOTE: p_varName denotes variables with a corresponding potentiometer
// NOTE: s_varName denotes variables with a corresponding switch (2 or 3-way)

// constants
const int BAUD_RATE = 9600;
const int AUDIO_MEMORY = 1024;
const int MSEC_TIME = 15;
elapsedMillis msec = 0;

// multiplexers
const int MUX_DELAY_TIME = 40;
const int MUX_S0 = 32;
const int MUX_Z = A2;

// MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial6, MIDI_6)
int m_note;
int m_velocity;

// potentimoeters (+ values)
const int OSC_MULT_SWITCH_01 = 0;
const int OSC_MULT_SWITCH_02 = 1;
const int OSC_01_FREQ_POT = 15;
const int OSC_01_WAVEFORM_SWITCH_01 = 33;
const int OSC_01_WAVEFORM_SWITCH_02 = 34;
const int OSC_02_FREQ_POT = A17;
const int OSC_02_WAVEFORM_SWITCH_01 = 35;
const int OSC_02_WAVEFORM_SWITCH_02 = 36;
float p_osc01Frequency = 55.0f;
float p_osc02Frequency = 55.0f;

const char *SAMPLER_FILENAME = "gnd-wrk.wav";

const int MOD_WAVEFORM_SWITCH_01 = 2;
const int MOD_WAVEFORM_SWITCH_02 = 3;
const int MOD_FREQ_POT = A16;
float p_modFrequency = 520.0f;
const int MOD_DEPTH_POT = A15;
float p_modDepth = 0.0f;
const int MOD_PHASE_POT = 14;
float p_modPhase = 0.0f;

const int DLY_TIME_POT = 3;
float p_delayTime = 60.0f; 
const int DLY_FBCK_POT = A14;
float p_delayFeedback = 0.0f;

const int FLTR_EG_ATK_POT = 10;
float p_filterEgAttack = 10.0f;
const int FLTR_EG_DEC_POT = 8;
float p_filterEgDecay = 120.0f;
const int FLTR_EG_SUS_POT = 5;
float p_filterEgSustain = 0.4f;
const int FLTR_EG_REL_POT = 2;
float p_filterEgRelease = 240.0f;
const int FLTR_CUTOFF_POT = 14;
float p_filterCutoff = 20000.0f;
const int FLTR_RES_POT = 12;
float p_filterResonance = 1.4f;
const int FLTR_OCT_CTRL_POT = 11;
float p_filterOctaveControl = 3.6f;
const int FLTR_EG_INT_POT = 13;
float p_filterEgInt = 0.2f;

const int AMP_EG_ATK_POT = 9;
float p_ampEgAttack = 10.0f;
const int AMP_EG_DEC_POT = 7;
float p_ampEgDecay = 120.0f;
const int AMP_EG_SUS_POT = 4;
float p_ampEgSustain = 0.4f;
const int AMP_EG_REL_POT = 1;
float p_ampEgRelease = 240.0f;

const int MIXER_AMP_POT = 0;
float p_mixerAmp = 1.0f;

// switches (+ values)
float s_oscMultiplier = 1.0f;
int s_osc01Waveform = WAVEFORM_SINE;
int s_osc02Waveform = WAVEFORM_TRIANGLE;

int s_modWaveform = 2;

enum FilterMode {
    LOW_PASS_FILTER = 0,
    HIGH_PASS_FILTER = 1
};
FilterMode s_filterMode = LOW_PASS_FILTER;
const int FLTR_MODE_SWITCH = 4;

enum MixerMode {
    OSC = 0,
    LINE_IN = 1,
    SAMPLER = 2
};
MixerMode s_mixerMode = OSC;
const int MXR_MODE_SWITCH_01 = 5;
const int MXR_MODE_SWITCH_02 = 9; 

// buttons
const int TRIG_BUTTON_PIN = 37;
Bounce trigButton = Bounce(TRIG_BUTTON_PIN, 18);

////////////
// SETUP  //
////////////

void setup() {
    Serial.begin(BAUD_RATE);

    initAudio();
    // initMIDI();
    initSdCard();
    initMultiplexer(MUX_S0, MUX_Z);
    
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
    MIDI_6.begin(MIDI_CHANNEL_OMNI);
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
    for(int i = 0; i < 4; i++) {
        pinMode(muxS0 - i, OUTPUT);
        digitalWrite(muxS0 - i, LOW);
        delayMicroseconds(MUX_DELAY_TIME);
    }
}

/*
 * Initialize all controls (buttons, pots, switches, etc.) as needed.
 */
void initControls() {
    pinMode(OSC_01_WAVEFORM_SWITCH_01, INPUT_PULLUP);
    pinMode(OSC_01_WAVEFORM_SWITCH_02, INPUT_PULLUP);
    pinMode(OSC_02_WAVEFORM_SWITCH_01, INPUT_PULLUP);
    pinMode(OSC_02_WAVEFORM_SWITCH_02, INPUT_PULLUP);
    pinMode(OSC_MULT_SWITCH_01, INPUT_PULLUP);
    pinMode(OSC_MULT_SWITCH_02, INPUT_PULLUP);

    pinMode(MOD_WAVEFORM_SWITCH_01, INPUT_PULLUP);
    pinMode(MOD_WAVEFORM_SWITCH_02, INPUT_PULLUP);

    pinMode(FLTR_MODE_SWITCH, INPUT_PULLUP);

    pinMode(MXR_MODE_SWITCH_01, INPUT_PULLUP);
    pinMode(MXR_MODE_SWITCH_02, INPUT_PULLUP);

    pinMode(TRIG_BUTTON_PIN, INPUT_PULLUP);
}

/*
 * Initialize all audio components declared in sketch with default settings.
 */
void initComponents() {
    // oscillator
    osc01.begin(1.0f, p_osc01Frequency, s_osc01Waveform);
    osc02.begin(1.0f, p_osc01Frequency, s_osc02Waveform);
    oscMixer.gain(0, 0.5f);
    oscMixer.gain(1, 0.5f);

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
    hpfL.frequency(p_filterCutoff);
    hpfL.resonance(p_filterResonance);
    hpfL.octaveControl(p_filterOctaveControl);

    filterEgR.attack(p_filterEgAttack);
    filterEgR.decay(p_filterEgDecay);
    filterEgR.sustain(p_filterEgSustain);
    filterEgR.release(p_filterEgRelease);
    filterEgRAmp.gain(p_filterEgInt);
    lpfR.frequency(p_filterCutoff);
    lpfR.resonance(p_filterResonance);
    lpfR.octaveControl(p_filterOctaveControl);
    hpfR.frequency(p_filterCutoff);
    hpfR.resonance(p_filterResonance);
    hpfR.octaveControl(p_filterOctaveControl);

    filterLMixer.gain(0, 1.0f);
    filterLMixer.gain(1, 0.0f);
    filterRMixer.gain(0, 1.0f);
    filterRMixer.gain(1, 0.0f);

    // amp
    ampEgL.attack(p_ampEgAttack);
    ampEgL.decay(p_ampEgDecay);
    ampEgL.sustain(p_ampEgSustain);
    ampEgL.release(p_ampEgRelease);
    ampEgLAmp.gain(1.0f);

    ampEgR.attack(p_ampEgAttack);
    ampEgR.decay(p_ampEgDecay);
    ampEgR.sustain(p_ampEgSustain);
    ampEgR.release(p_ampEgRelease);
    ampEgRAmp.gain(1.0f);
}

////////////
//  LOOP  //
////////////

void loop() {
    if(msec < MSEC_TIME) return;
    msec = 0;

    checkTrigButton();
    // processMIDI();

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
        triggerNoteOn();
    } else if(trigButton.risingEdge()) {
        triggerNoteOff();
    }
}

void triggerNoteOn() {
    filterEgL.noteOn();
    filterEgR.noteOn();
    ampEgL.noteOn();
    ampEgR.noteOn();
}

void triggerNoteOff() {
    filterEgL.noteOff();
    filterEgR.noteOff();
    ampEgL.noteOff();
    ampEgR.noteOff();
}

void processMIDI() {
    int type, note, velocity, channel, data1, data2;

    if(MIDI_6.read()) {
        type = MIDI_6.getType();
        switch(type) {
            case midi::NoteOn:
                channel = MIDI_6.getChannel();
                note = MIDI_6.getData1();
                velocity = MIDI_6.getData2();

                //MIDI_6.sendNoteOn(note, velocity, channel);

                m_note = note;
                m_velocity = velocity;

                processOsc(noteToFreq(m_note));
                processAmpEg(m_velocity / 127.0f);

                triggerNoteOn();

                if(velocity > 0) {
                    printMIDIData("Note On", channel, m_note, m_velocity);
                } else {
                    printMIDIData("Note Off", channel, m_note, m_velocity);
                }

                break;

            case midi::NoteOff:
                note = MIDI_6.getData1();
                velocity = MIDI_6.getData2();
                channel = MIDI_6.getChannel();

                //MIDI_6.sendNoteOff(note, velocity, channel);

                m_note = note;
                m_velocity = velocity;

                triggerNoteOff();

                printMIDIData("Note Off", channel, m_note, m_velocity);

                break;

            default: 
                data1 = MIDI_6.getData1();
                data2 = MIDI_6.getData2();

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
    int pin1 = digitalRead(MXR_MODE_SWITCH_01);
    int pin2 = digitalRead(MXR_MODE_SWITCH_02);
    if(pin1) {
        if(pin2) {
            s_mixerMode = LINE_IN;
        } else {
            s_mixerMode = OSC;
        }
    } else {
        s_mixerMode = SAMPLER;
    }

    processMixerInputs((int) s_mixerMode);

    switch(s_mixerMode) {
        default:
        case OSC:
            processOsc();
            break;
        case LINE_IN:
            // TODO
            break;
        case SAMPLER:
            processSampler();
            break;
    }
}

/*
 * Updates mixer inputs to open the active channel and close the others.

 * @param activeChannel number (int) of active channel
 */
void processMixerInputs(int activeChannel) {
    for(int i = 0; i < 4; i++) {
        mixer.gain(i, i == activeChannel ? 1.0f : 0.0f);
    }
}

/*
 * Update oscillator's frequency, waveform, and octave multiplier.
 */
void processOsc() {
    int osc01Pin1 = digitalRead(OSC_01_WAVEFORM_SWITCH_01);
    int osc01Pin2 = digitalRead(OSC_01_WAVEFORM_SWITCH_02);
    int osc01Result;
    if(osc01Pin1) {
        if(osc01Pin2) {
            osc01Result = WAVEFORM_SAWTOOTH;
        } else {
            osc01Result = WAVEFORM_SQUARE;
        }
    } else {
        osc01Result = WAVEFORM_SINE;
    }
    if(osc01Result != s_osc01Waveform) {
        s_osc01Waveform = osc01Result;
        osc01.begin(s_osc01Waveform);
        osc02.begin(s_osc02Waveform);
    }

    int osc02Pin1 = digitalRead(OSC_02_WAVEFORM_SWITCH_01);
    int osc02Pin2 = digitalRead(OSC_02_WAVEFORM_SWITCH_02);
    int osc02Result;
    if(osc02Pin1) {
        if(osc02Pin2) {
            osc02Result = WAVEFORM_SAWTOOTH;
        } else {
            osc02Result = WAVEFORM_SQUARE;
        }
    } else {
        osc02Result = WAVEFORM_TRIANGLE;
    }
    if(osc02Result != s_osc02Waveform) {
        s_osc02Waveform = osc02Result;
        osc02.begin(s_osc02Waveform);
        osc02.begin(s_osc02Waveform);
    }

    int multPin1 = digitalRead(OSC_MULT_SWITCH_01);
    int multPin2 = digitalRead(OSC_MULT_SWITCH_02);
    if(multPin1) {
        if(multPin2) {
            s_oscMultiplier = 1.0f;
        } else {
            s_oscMultiplier = 0.5f;
        }
    } else {
        s_oscMultiplier = 2.0f;
    }

    float osc01Frequency = 20.0f + (2000.0f * readMuxZPin(OSC_01_FREQ_POT, MUX_S0, MUX_Z));
    p_osc01Frequency = osc01Frequency * s_oscMultiplier;
    float osc02Frequency = 20.0f + (2000.0f * analogRead(OSC_02_FREQ_POT) / 1023.0f);
    p_osc02Frequency = osc02Frequency * s_oscMultiplier;

    osc01.frequency(p_osc01Frequency);
    osc02.frequency(p_osc02Frequency);
}

/*
 * Update oscillator's frequency, waveform, and octave multiplier.
 * 
 * @param freq a specified frequency for the oscillators
 */
void processOsc(float freq) {
    p_osc01Frequency = freq;
    p_osc01Frequency *= s_oscMultiplier;

    osc01.frequency(p_osc01Frequency);
    osc02.frequency(p_osc01Frequency);
}

/*
 * Continuously play the specified sample's wav file.
 */
void processSampler() {
    if(!sampler.isPlaying()) {
        sampler.play(SAMPLER_FILENAME);
        delay(1);
    }
}

/*
 *
 * 
 */
void processMod() {
    int pin1 = digitalRead(MOD_WAVEFORM_SWITCH_01);
    int pin2 = digitalRead(MOD_WAVEFORM_SWITCH_02);
    int result;
    if(pin1) {
        if(pin2) {
            result = WAVEFORM_SAWTOOTH;
        } else {
            result = WAVEFORM_SQUARE;
        }
    } else {
        result = WAVEFORM_SINE;
    }
    if(result != s_modWaveform) {
        s_modWaveform = result;
        modL.begin(s_modWaveform);
        modR.begin(s_modWaveform);
    }

    float freq = analogRead(MOD_FREQ_POT) / 1023.0f;
    p_modFrequency = 0.00000001f + (5200.0f * freq);
    modL.frequency(p_modFrequency);
    modR.frequency(p_modFrequency);

    p_modDepth = analogRead(MOD_DEPTH_POT) / 1023.0f;
    modL.amplitude(p_modDepth);
    modR.amplitude(p_modDepth);

    float phase = readMuxZPin(MOD_PHASE_POT, MUX_S0, MUX_Z);
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
    float time = 10.0f + (1240.0f * readMuxZPin(DLY_TIME_POT, MUX_S0, MUX_Z));
    if(time != p_delayTime) {
        p_delayTime = time;
        delayL.delay(0, p_delayTime);
        delayL.delay(1, 0.25f * p_delayTime);

        delayR.delay(0, p_delayTime);
        delayR.delay(1, 0.25f * p_delayTime);
    }

    p_delayFeedback = analogRead(DLY_FBCK_POT) / 1023.0f;
    delayAmpL.gain(p_delayFeedback);
    delayAmpR.gain(p_delayFeedback);
}

/*
 * Update the filter's cutoff frequency, resonance, envelope intensity, and octave control.
 */
void processFilter() {
    s_filterMode = (FilterMode) !digitalRead(FLTR_MODE_SWITCH);
    filterLMixer.gain(s_filterMode, 1.0f);
    filterLMixer.gain(!s_filterMode, 0.0f);
    filterRMixer.gain(s_filterMode, 1.0f);
    filterRMixer.gain(!s_filterMode, 0.0f);

    p_filterCutoff = 20000.0f * readMuxZPin(FLTR_CUTOFF_POT, MUX_S0, MUX_Z);

    float resonance = readMuxZPin(FLTR_RES_POT, MUX_S0, MUX_Z);
    p_filterResonance = 0.8f + (4.0f * resonance);

    float octaveControl = readMuxZPin(FLTR_OCT_CTRL_POT, MUX_S0, MUX_Z);
    p_filterOctaveControl = octaveControl * 4.0f;    

    p_filterEgInt = readMuxZPin(FLTR_EG_INT_POT, MUX_S0, MUX_Z);
    filterEgLAmp.gain(p_filterEgInt);
    filterEgRAmp.gain(p_filterEgInt);

    float attack = 1000.0f * readMuxZPin(FLTR_EG_ATK_POT, MUX_S0, MUX_Z);
    if(attack != p_filterEgAttack) {
        p_filterEgAttack = attack;
        filterEgL.attack(p_filterEgAttack);
        filterEgR.attack(p_filterEgAttack);
    }
    float decay = 1000.0f * readMuxZPin(FLTR_EG_DEC_POT, MUX_S0, MUX_Z);
    if(decay != p_filterEgDecay) {
        p_filterEgDecay = decay;
        filterEgL.decay(p_filterEgDecay);
        filterEgR.decay(p_filterEgDecay);
    }
    float sustain = readMuxZPin(FLTR_EG_SUS_POT, MUX_S0, MUX_Z);
    if(sustain != p_filterEgSustain) {
        p_filterEgSustain = sustain;
        filterEgL.sustain(p_filterEgSustain);
        filterEgR.sustain(p_filterEgSustain);
    }
    float release = 2000.0f * readMuxZPin(FLTR_EG_REL_POT, MUX_S0, MUX_Z);
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

            lpfL.octaveControl(p_filterOctaveControl);
            lpfR.octaveControl(p_filterOctaveControl);
            break;

        case HIGH_PASS_FILTER:
            hpfL.frequency(p_filterCutoff);
            hpfR.frequency(p_filterCutoff);

            hpfL.resonance(p_filterResonance);
            hpfR.resonance(p_filterResonance);

            hpfL.octaveControl(p_filterOctaveControl);
            hpfR.octaveControl(p_filterOctaveControl);
            break;
    }
}

/*
 * Update the amplifier's level.
 */
void processAmp() {
    float attack = 1000.0f * readMuxZPin(AMP_EG_ATK_POT, MUX_S0, MUX_Z);
    if(attack != p_ampEgAttack) {
        p_ampEgAttack = attack;
        ampEgL.attack(p_ampEgAttack);
        ampEgR.attack(p_ampEgAttack);
    }
    float decay = 1000.0f * readMuxZPin(AMP_EG_DEC_POT, MUX_S0, MUX_Z);
    if(decay != p_ampEgDecay) {
        p_ampEgDecay = p_ampEgDecay;
        ampEgL.decay(p_ampEgDecay);
        ampEgR.decay(p_ampEgDecay);
    }
    float sustain = readMuxZPin(AMP_EG_SUS_POT, MUX_S0, MUX_Z);
    if(sustain != p_ampEgSustain) {
        p_ampEgSustain = sustain;
        ampEgL.sustain(p_ampEgSustain);
        ampEgR.sustain(p_ampEgSustain);
    }
    float release = 2000.0f * readMuxZPin(AMP_EG_REL_POT, MUX_S0, MUX_Z);
    if(release != p_ampEgRelease) {
        p_ampEgRelease = release;
        ampEgL.release(p_ampEgRelease);
        ampEgR.release(p_ampEgRelease);
    }

    p_mixerAmp = readMuxZPin(MIXER_AMP_POT, MUX_S0, MUX_Z);

    control.volume(p_mixerAmp);
}

void processAmpEg(float amp) {
    ampEgLAmp.gain(amp);
    ampEgRAmp.gain(amp);
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
    for(int i = 0; i < 4; i++) {
        bool expr = muxY & (1 << i);
        digitalWrite(muxS0 - i, expr ? HIGH : LOW);
    }

    delayMicroseconds(MUX_DELAY_TIME);
}
