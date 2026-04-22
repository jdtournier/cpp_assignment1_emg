#pragma once

#include <string>
#include <vector>

struct EMG{
    double sampling_interval;
    std::vector<double> emg_signal;
};

struct peak {
    double peak;
    int timestamp;
};

// load the EMG data contained in the file specified, and return the
// information 
EMG load_emg (const std::string& emg_filename );

// Compute the 3-point moving average filter to smooth the EMG signal
//return the smoothed signal
std::vector<double> filter_moving_average (const std::vector<double>& emg_signal);

//functions to compute the statistics (e.g., peak, MAV, RMS) for the signal specified as parameter
peak compute_peak(const EMG emg);
double compute_MAV(const std::vector<double>& emg_signal);
double compute_RMS(const std::vector<double>& emg_signal);

// return the fatigue index of the signal specified as parameter:
double fatigue_index (const std::vector<double>& emg_signal);

// Write out a report to the file specified, based on the computed parameters:
void write_report (const std::string& report_filename, const std::string& emg_filename,const EMG emg);

