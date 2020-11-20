# TI-F28379D-SPEECH-RECOGNITION

## DSP Speech Recognition

### Notes

- Planning on using KNN for the classification and MFCC feature extraction for the audio files
- MFCC uses FFT feature extraction
- Extracting from wav files and a test set provided by me using the microphone on the codec
- Not sure how many test files will be needed, planning on doing 20-30 per word
- Planning on using the TensorFlow Command Speech data set for training
- Another possiblity is to run an FFT on the data and test its pitch (yes high pitch, no low pitch)
- Python will handle the Pre-computation of the ML model's parameters

### Functionality

The dsp will be continuously sampling the microphone. When audio is detected on the microphone (threshold is surpassed), it will begin to record the sampled data into the external SRAM. After recording the data into the SRAM, it will process the data in the background (ML processing, MFCC to KNN, or FFT). Responses from the DSP will be displayed through UART and a serial console.

Plans for the DSP to have a simple conversation with the user (with responses that illustrate the response ie. If answer no, DSP respond with "why not")

*Yes* and *No* will be the words initially featured. If the DSP can handle more, will add *Hello* and *Goodbye*

Conversation theme is about the DSP taking over the world. Answer improperly and flash the RED LEDs or answer correctly and flash the BLUE LEDs

Using the 8 bit LEDs for debugging the state.

**Logical states:** 

- Idle (listening and waiting for a signal or a response on the microphone)
- Recording (Storing the sampling data into the SRAM)
- Processing (Running ML processing or FFT(During this point either save the next signal response or don't sample))

After the processing is complete, trigger the DSPs response and send it through UART