#include <iostream>
#include <exception>

#include "debug.h"
#include "emg.h"

int main(int argc, char* argv[])
{
    debug::set_verbose_mode (argc, argv);

    if (argc != 3) { // <- adjust as required
        std::cerr <<
        "ERROR: invalid number of arguments.\n\n"
        "Command usage:\n\n"
        "   spo2 ppg_input report\n\n";
        return 1;
    }

    try {
    // main processing block:

    EMG data = load_emg (argv[1]);

    write_report (argv[2], argv[1],data);

    // end of main processing block
  }
  catch (std::exception& err) {
    // if any exceptions were thrown, we can handle them here:
    std::cerr << "ERROR: " << err.what() << " - aborting\n";
    return 1;
  }
  catch (...) { // <- the catch-all handler
    std::cerr << "ERROR: exception of unknown type was thrown - aborting\n";
    return 1;
  }

  return 0;
}
