[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/jdtournier/cpp_assignment1_spo2?quickstart=1)


# BEng Biomedical Engineering
### Object-Oriented Programming (5CCYB041) &ndash; Assessment 1


## Objective

To demonstrate proficiency in C++ by developing a program to perform a defined computational task.


# Assessment criteria

Your submission will be marked according to the following criteria:

**40%: Performance**
- Does the program compile?
- Does the program run successfully?
- Is the output correct?
- Does the program meet all requirements?
- Is the code for each individual aspect of the task correct?
- Are command-line arguments handled correctly?

**30%: Design**
- Is the code logically organised?
- Does it use appropriate C++ features?
- Does it make good use of control and data structures?
- Does it make good use of functions?
- Does the code include error checking and appropriate handling?

**30%: Maintainability**
- Is the code easily understandable?
- Is the code designed for modularity and re-use?
- Is the code organised across multiple files?
- Are functions and variables given interpretable names?
- Does the code follow established naming conventions?
- Does the code include comments where appropriate?
- Is code indentation correct throughout?




---

# Introduction 

Electromyography (EMG) is a technique used to evaluate and record the electrical activity produced by skeletal muscles. Surface EMG (sEMG) is the non-invasive variant, where electrodes are placed on the skin overlying the muscle of interest. It is widely used in clinical diagnostics, rehabilitation engineering, and sports science.

During a muscle contraction, motor neurons transmit electrical signals that cause muscle fibres to activate. The EMG signal captures these Motor Unit Action Potentials (MUAPs) as a time-varying voltage measured at the skin surface. The amplitude and frequency content of the EMG signal carry important information about muscle effort and fatigue.

By analysing the amplitude characteristics of the signal, we can assess the level of muscular effort and detect signs of fatigue.


# Instructions 

Your task in this assignment is to write a C++ program to read raw sEMG sensor data from a file, process and analyse the signals, and write a diagnostic report to an output file.

Your program should perform the following main steps:
1. Load the raw sEMG data from a text file specified as a command-line argument.
2. Apply a 3-point moving average filter to smooth the EMG signal.
3. Extract features from the filtered signal: 
   - Peak amplitude and timestamp
   - Mean Absolute Value (MAV)
   - Root Mean Square (RMS)
4. Calculate the Fatigue Index (FI) by comparing the RMS of the first half of the signal to the second half.
5. Classify the muscle fatigue status based on the Fatigue Index thresholds.
6. Write a summary report of the analysis to an output text file (specified as a command-line argument).


# Program requirements

## Command-line interface 


Your program should accept at least 2 arguments: the filename containing the raw sEMG recording, and the filename to use for the output report.

## Loading the input data 

You are provided with three data files (`emg1.txt`, `emg2.txt` & `emg3.txt`) containing synthetic sEMG recordings. The data in each file is formatted as illustrated below:
```
EMG
5
0.0512	
0.0891	
0.1345	
-0.0723	
...
```

In more detail:
- the first entry is the special identifier EMG: this is used to verify that the file is indeed an EMG file in the expected format. Your program should not accept input files that don't contain this identifier as the very first item in the file.
- the second entry is the sampling interval, in milliseconds. This is the time between measurements in this EMG recording.
- subsequent entries consist of the recordings themselves, in units of millivolts, in the order they were recorded.
- Your program should be capable of loading any file provided in this format, regardless of the number of data rows.


## Computing a moving average filter for noise reduction

Raw EMG signals contain high-frequency noise and sensor artefacts. To smooth the data, apply a 3-point moving average filter to the EMG signal.

For every element $i$ in your data (excluding the first and last elements), compute:

$$ d_\textrm{filtered} = \frac{d[i-1] + d[i] + d[i+1]}{3} $$

The first and last elements of the raw data are excluded from the filtered output. Therefore, if the raw signal has N samples, the filtered signal will have N−2 samples.

## Feature Extraction

For the filtered signal, your program should compute the following:
- **Peak Amplitude and timestamp**: The maximum absolute value in the filtered signal and the time at which this peak occurs, assuming the first sample is acquired at $t = 0$
- **Mean Absolute Value (MAV)**: The average of the *absolute* values of all samples in the filtered signal.

$$ \mbox{MAV} = \frac{1}{N} \sum_{i=1}^N \left| x_i \right| $$

- **Root Mean Square (RMS)**: A measure of signal power, computed as:

$$ \mbox{RMS} = \sqrt{ \frac{1}{N} \sum_{i=1}^N x_i^2 } $$

## Fatigue Index Calculation

As fatigue develops, the RMS amplitude tends to increase due to additional motor unit recruitment.
- Divide the filtered signal into two equal halves
- Compute $\mbox{RMS}_\mbox{first}$ using the first half of the filtered samples.
- Compute $\mbox{RMS}_\mbox{second}$ using the second half of the filtered samples.
- Calculate the Fatigue Index (FI) as:

$$ \mbox{FI} = 100 \times \frac{\mbox{RMS}_\mbox{second} - \mbox{RMS}_\mbox{first} }{ \mbox{RMS}_\mbox{first} } $$

A positive Fatigue Index indicates that signal amplitude increased over time, which is a hallmark of muscle fatigue. 


## Classifying the Muscle Fatigue Status

Your program should use the computed Fatigue Index to classify the muscle’s status as follows:

| FI Range | Classification | Interpretation |
|---|---|:---|
| < 10%	| No Fatigue | Muscle performance is stable |
| 10% – 30%	| Moderate Fatigue | Early signs of muscle fatigue |
| > 30%	| Severe Fatigue | Significant fatigue &ndash; risk of injury |

## Writing the result to file

Your program should write its final report to the text file specified as the second command-line argument. This should include:
- The name of the input sEMG data file.
- The number of raw samples loaded.
- The number of filtered samples.
- The Peak Amplitude and the time at which this peak occurs.
- MAV, and RMS.
- The RMS of the first half and second half.
- The computed Fatigue Index (FI).
- The classification: ‘No Fatigue’, ‘Moderate Fatigue’, or ‘Severe Fatigue’.

An example report is shown below:
```
EMG file: emg3.txt
SRaw samples loaded: 500
Samples available after filtering: 498
Peak = 0.4727 happening after 2325 seconds
MAV = 0.111557 RMS (all) = 0.146715
Fatigue Index (FI): 33.7034%
Status: Severe Fatigue
```
