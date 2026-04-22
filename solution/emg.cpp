#include <string>
#include <vector>
#include <fstream>

#include <termviz.h>

#include "emg.h"
#include "debug.h"


// load the EMG data contained in the file specified, and return the
// information 
EMG load_emg (const std::string& emg_filename )
{
  std::ifstream infile (emg_filename);
  if (!infile)
    throw std::runtime_error ("failed to open input file \"" + emg_filename + "\"");

  // First entry should be the marker "EG" as verify this is an ECG file:
  std::string check;
  infile >> check;
  if (check != "EMG")
    throw std::runtime_error ("file \"" + emg_filename + "\" does not contain EMG data in expected format");

    // Next entry should be the sampling interval in milliseconds:
  double sampling_interval;
  infile >> sampling_interval;

    // subsequent entries correspond to the recorded voltage:
  std::vector<double> emg_signal;
  double val;
  while (infile >> val)
    emg_signal.push_back (val);

  if (emg_signal.empty())
    throw std::runtime_error ("no usable data found in input file \"" + emg_filename + "\"");


  // If running in debug/verbose mode, log relevant information, and plot the EMG trace for inspection:
  if (debug::verbose_mode) {
    debug::log (std::format ("EMG traced loaded, sampling rate {} ms, {} samples, total duration {} seconds\n",
          sampling_interval, emg_signal.size(), emg_signal.size() * sampling_interval / 1000.0));
    
          std::vector<double> t (emg_signal.size());
    
    for (int n = 0; n < t.size(); n++)
      t[n] = n*sampling_interval / 1000.0;
    termviz::figure(1024, 256).plot (t, emg_signal);
  }

  return {sampling_interval, emg_signal};
}

// Compute the 3-point moving average filter to smooth the EMG signal
//return the smoothed signal
std::vector<double> filter_moving_average (const std::vector<double>& emg_signal){
if (emg_signal.size() < 3)
    throw std::runtime_error ("vector supplied to filter_moving_average() is too small");

  std::vector<double> filtered (emg_signal.size()-2);
  // We start at index 1 and end at size-1 to avoid out-of-bounds errors
  for (size_t i = 1; i < emg_signal.size()-1; ++i)
    filtered[i-1] = (emg_signal[i-1] + emg_signal[i] + emg_signal[i+1]) / 3.0;

  debug::log ("vector before & after filtering with moving average:");
  if (debug::verbose_mode)
    termviz::figure().plot (emg_signal).plot (filtered);

  return filtered;
}

//functions to compute the statistics (e.g., peak, MAV, RMS) for the signal specified as parameter
peak compute_peak(const std::vector<double>& emg_signal)
{
  if (emg_signal.empty())
    throw std::runtime_error ("compute_peak() called on empty vector");

  double max=fabs(emg_signal[0]);
  int index=0;
  for (int n = 1; n < emg_signal.size(); n++) {
    if (max < fabs(emg_signal[n]))
    { max =  fabs(emg_signal[n]);
      index=n;
    }

  }
  return { max,index };

}

double compute_MAV(const std::vector<double>& emg_signal)
{
  double sumAbs = 0.0;
  for (int n = 0; n < emg_signal.size(); n++) 
    sumAbs+=fabs(emg_signal[n]);

  return sumAbs/emg_signal.size();

}

double compute_RMS(const std::vector<double>& emg_signal)
{
  double sumSq = 0.0;
  for (int n = 0; n < emg_signal.size(); n++) 
    sumSq+=emg_signal[n]*emg_signal[n];

  return sqrt(sumSq/emg_signal.size());

}

// return the fatigue index of the signal specified as parameter:
double fatigue_index (const std::vector<double>& emg_signal)
{
    if (emg_signal.size() < 2)
        throw std::runtime_error ("vector supplied to fatigue_index() is too small");
    int half = emg_signal.size()/ 2;

    std::vector<double> first_half(emg_signal.begin(), emg_signal.begin() + half); 
    std::vector<double> second_half(emg_signal.begin() + half,emg_signal.end()); 

    double rmsFirst  = compute_RMS(first_half);
    double rmsSecond = compute_RMS(second_half);

    return (100.0 * (rmsSecond - rmsFirst) / rmsFirst);


}

// Write out a report to the file specified, based on the computed parameters:
void write_report (const std::string& report_filename, const std::string& emg_filename, const EMG emg)
{
  debug::log ("writing report to file \"" + report_filename + "\"...");
  std::ofstream outfile (report_filename);
  if (!outfile)
    throw std::runtime_error ("failed to open report file \"" + report_filename + "\"");

  std::vector<double> filtered_signal=filter_moving_average(emg.emg_signal);
  peak pk = compute_peak (filtered_signal);

  double MAV = compute_MAV(filtered_signal);
  double RMS = compute_RMS(filtered_signal);

  double FI = fatigue_index(filtered_signal);

  outfile
    << "EMG file: " << emg_filename
    << "\nRaw samples loaded: " << emg.emg_signal.size()
    << "\nSamples available after filtering: " << filtered_signal.size()
    << "\nPeak = " << pk.peak << " happening after " << pk.timestamp*emg.sampling_interval <<" seconds"
    << "\nMAV = " << MAV << " RMS (all) = " << RMS
    << "\nFatigue Index (FI): " << FI <<"%"
    << "\nStatus: ";

  if (FI > 30.0)
    outfile << "Severe Fatigue";
  else if (FI >= 10.0)
    outfile << "Moderate Fatigue";
  else
    outfile << "Severe Fatigue";

  outfile << "\n";

  debug::log ("report writen to file \"" + report_filename + "\" successfully");


}