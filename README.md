# EMG-Based-Muscle-Signal-Acquisition-and-Gesture-Classification-System
Electromyography (EMG) is a biomedical technique used to record the electrical activity produced by muscles during contraction. These signals are extremely small, typically between 0.1 mV and 5 mV, making them highly susceptible to noise and interference. they require precise amplification, filtering, and digital processing 



1.	Introduction:
Electromyography (EMG) is a biomedical technique used to record the electrical activity produced by skeletal muscles during contraction. These signals are extremely small, typically between 0.1 mV and 5 mV, making them highly susceptible to noise and interference. Therefore, they require precise amplification, analog filtering, and digital processing to extract meaningful features.
This project presents a full EMG acquisition and classification system that includes:
•	A custom PCB for sensing weak muscle signals. 
•	An instrumentation amplifier–based analog front-end (AFE) with HPF and LPF stages.
•	An ESP32 C3 microcontroller for real time digital processing.
•	Gesture classification algorithms based on EMG peak strength and duration.
•	Visualization and wireless data streaming through BLE/WiFi.
The system is designed to be flexible, low cost, and suitable for biomedical students, robotics developers, and rehabilitation applications.

2. System Overview:
The full signal chain consists of several essential stages:
1.	EMG Electrodes – Capture differential biopotentials from muscle tissue.
2.	Measurement PCB (Green Board) – Buffers and transfers the raw, micro volt EMG signal.
3.	Analog Front-End (Brown Board):
o	Instrumentation amplifier for high gain amplification.
o	High pass filter to remove drift and movement artifacts.
o	Low pass filter to suppress high frequency noise.
4.	ESP32 C3 Microcontroller: 
o	12 bit ADC reading. 
o	Low pass digital filter.
o	Moving average smoothing.
o	Peak detection.
o	Gesture classification.
o	Real time data logging via serial, WiFi or BLE.

5.	Output Stages:
o	LED indicators.
o	Motors or actuators.
o	GUI dashboard.
o	Serial plotter.
This pipeline ensures accurate and stable EMG measurements suitable for gesture recognition.

3. EMG Signal Characteristics:
EMG signals originate from the depolarization of motor units during muscle activation. Their properties include:
•	Amplitude: Typically 0.1–5 mV before amplification.
•	Frequency Range: 20–450 Hz.
•	Nature: Non stationary, highly variable, and noisy.
•	Common Noise Sources:
o	Motion artifacts.
o	Power-line interference (50/60 Hz).
o	Electrode placement variability.
o	Skin impedance.
Due to these factors, both analog and digital signal conditioning are essential.

4. Hardware Design:
4.1 Measurement PCB (Green Board)
The green PCB handles:
•	Acquisition of extremely low amplitude muscle potentials using differential electrodes.
•	Reduction of common mode noise using short electrode leads.
•	Maintaining high input impedance to avoid loading the signal.
•	Ensuring minimal distortion before amplification.
Proper electrode placement and skin preparation significantly improve the signal quality.

4.2 Analog Front-End (Brown Board)
The analog front-end (AFE) prepares the EMG signal for ADC conversion.
Instrumentation Amplifier (IA)
•	Provides high, stable gain (up to ×1000).
•	High CMRR reduces interference such as 50 Hz noise.
•	Differential input ensures accuracy in low voltage measurements.
Commonly used ICs include: INA128, INA333, AD620.


High Pass Filter (HPF)
•	Removes low frequency drift caused by movement and electrode shifts.
•	Typical cutoff frequency: 20–25 Hz.
Low Pass Filter (LPF)
•	Removes high frequency noise and smooths the EMG envelope.
•	Typical cutoff frequency: 450–500 Hz.
Together, these filters create a clean, stable signal ready for digital processing.

5. ESP32 C3 Microcontroller Processing:
The ESP32 C3 mini handles both acquisition and classification.
5.1 ADC Sampling
•	12 bit ADC resolution provides values between 0 and 4095.
•	The threshold varies depending on each user's muscle density, fat levels, and electrode positioning.
•	Example user threshold range: 15 → 1920.
5.2 Advantages of Using ESP32 C3
•	Built in WiFi and BLE for wireless streaming.
•	Low power consumption.
•	Fast processing for real time EMG decoding.
•	Suitable for wearable biomedical applications.
6. Digital Signal Processing (DSP):
To ensure stable analysis, the project applies two main filters.
6.1 Low Pass Filter (IIR Smoothing)
A first order digital low pass filter is applied:
filtered = alpha * raw + (1 - alpha) * previous;
Removes rapid spikes.Smooths sawtooth like noise into sinusoidal like waveform.Alpha controls responsiveness.
6.2 Moving Average Filter (MAF)
A window size of 10 samples provides:
Reduced momentary fluctuations. A more stable baseline for peak detection. Less jitter in classification.
This combination ensures clarity in distinguishing muscle contractions.
7. Peak Detection Algorithm:
Peak detection identifies muscle contractions by monitoring when the smoothed signal crosses the threshold.
Parameters Tracked:
•	Peak Amplitude (maximum value during contraction).
•	Peak Duration (how long the signal stays above threshold).
Logic:
1.	When the signal exceeds the threshold → Peak starts.
2.	Algorithm records amplitude and time.
3.	Once signal falls below threshold → Peak ends.
4.	Data is passed to the classification stage.
This enables extraction of meaningful features for gesture recognition.
8. Gesture Classification:
Gestures are classified using both peak magnitude and time duration.
Implemented Mappings:
•	UP: peak > 1800 → strong contraction.
•	RIGHT: peak > 1200 & duration > 200 ms.
•	LEFT: peak > 1200 & duration ≤ 200 ms.
•	DOWN: peak > threshold.
•	NONE: no detectable contraction.
This simple rule based classifier is computationally efficient and suitable for real time embedded systems.
Possible Extensions:
RMS based classification. Machine learning models. Ulti channel EMG gesture control. Adaptive thresholding per user.
9. Real Time Data Visualization:
The ESP32 outputs comma separated data in this format: 
raw, filtered, smooth, threshold, activeFlag
This can be:
Displayed in the Arduino Serial Plotter. Streamed over WiFi to a dashboard. Sent via BLE for mobile apps.
Visualization is useful for:
Debugging the sensor. Understanding EMG patterns, Adjusting thresholds per user.
10. Applications:
This EMG based system can be used in many fields:
Biomedical Applications:
Prosthetic limb control.Muscle rehabilitation.Physiotherapy monitoring.Stress analysis via muscle tension.
Robotics and Engineering:
Gesture controlled robots.Robotic exoskeletons.Human machine interfaces.
11. Conclusion:
This project demonstrates a complete and practical EMG signal acquisition and interpretation system. By combining a custom hardware front end, powerful filtering techniques, and a lightweight gesture classifier, the system provides reliable recognition of muscle activity in real time.
With further development—such as machine learning classification or multi channel expansion—the system could be adapted for professional prosthetics, advanced rehabilitation tools, or intelligent wearable devices.
12. References: 
1.	Electromyography fundamentals – biomedical engineering textbooks.
2.	INA128/AD620 instrumentation amplifier datasheets.
3.	ESP32 C3 technical reference manual.
4.	Research papers on EMG processing and gesture decoding.
