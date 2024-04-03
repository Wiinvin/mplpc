// local include files
//
#include "Mplpc.h"

// method: compute_num_frames
//
// arguments:
//  long nsamp: number of samples in the signal (input)
//  long flen: number of samples in a frame
//  long wlen: number of samples in a window
//
// return: a long value containing the number of frames
//
// This method computes the number of frames in a signal by either rounding
// or truncating.
//
long Mplpc::compute_num_frames(long nsamp_a, long flen_a, long wlen_a) {

  // mode: round
  //
  if (frame_mode_d == FRMMODE_FLOOR) {
    return nsamp_a / flen_a;
  }

  // mode: truncate
  //
  //  note that in this implementation, once the forward edge of the
  //  analysis window exceeds the end of the file, the feature
  //  extraction process is terminated. So the number of output frames
  //  is always <= the nsamp/ana_flen because the length of the analysis
  //  window must be considered.
  //
  else if (frame_mode_d == FRMMODE_TRUNCATE) {
    return (double)(nsamp_a - ((wlen_a - flen_a)/2)) / (double)flen_a;
  }

  else {
    fprintf(stdout,
	    "**> error in Fe::compute_num_frames(): invalid frame mode [%s]\n",
	    frame_mode_str_d);
    return -1;
  }
}

// method: create_window
//
// arguments:
//  long wlen: number of points (input)
//
// return: a logical variable indicating status
//
// This method creates a time domain window function. Note that the
// type of window is defined from the protected data.
//
bool Mplpc::create_window(long wlen_a) {

  // declare local variables
  //
  bool status = true;

  // display debug information
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "Mplpc::create_window(): [%s] window [%s] norm [%ld] samples\n",
	    win_type_str_d, win_norm_str_d, wlen_a);
  }

  // compute some constants related to the length of the window
  //
  long N = wlen_a;
  long Nm1 = N - 1;

  // create output space
  //
  edf_d.resize(win_fct_d, N, false);

  // check for the type of window
  //
  if (win_type_d == WTYP_RECT) {

    // set the window
    //
    for (long n = 0; n < N; n++) {
      win_fct_d[n] = 1.0;
    }
  }

  // type: Hamming window
  //
  // this method generates a generalized Hanning window function:
  //
  //  w(n) = { G * (alpha - (1 - alpha) cos (2 * PI * n / (N-1)))  0 <= n < N
  //         { 0                                                   elsewhere
  //
  // the constant alpha can be used to implement a variety of windows,
  // including a Hamming (alpha = 0.54) and Hanning (alpha = 0.50).
  // a good reference for this window can be found at:
  //
  //  L.R. Rabiner and R.W. Schafer, Digital Processing of Speech Signals,
  //  Prentice-Hall, Englewood Cliffs, New Jersey, USA, 1978, pp. 121.
  //
  else if (win_type_d == WTYP_HAMMING) {

    // loop over all elements
    //
    for (long n = 0; n < N; n++) {
      float arg1 = 2.0 * M_PI * (float)n / (float)(Nm1);
      win_fct_d[n] = (float)(DEF_WIN_HAMM_ALPHA -
			     ((float)1.0 - DEF_WIN_HAMM_ALPHA) *
			     (float)cos((double)arg1));
    }
  }

  // normalize the window to have an energy of 1.0
  //
  if (win_norm_d == WNRM_AMPL) {

    // compute the energy of the window shape
    //
    float sum = 0;
    for (long n = 0; n < N; n++) {
      sum += win_fct_d[n] * win_fct_d[n];
    }

    // normalize by energy
    //
    float inv_sum = 1.0 / sqrt(sum);
    for (long n = 0; n < wlen_a; n++) {
      win_fct_d[n] *= inv_sum;
    }
  }

  // display debug information
  //
  if (debug_level_d >= Dbgl::LEVEL_FULL) {
    float sum = 0;
    for (long n = 0; n < N; n++) {
      fprintf(stdout, "\tw[%ld] = %f\n", n, win_fct_d[n]);
      sum += win_fct_d[n] * win_fct_d[n];
    }
    fprintf(stdout, "\tenergy of the window = %f\n", sum);
  }

  // display debug inforation
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "Mplpc::create_window(): done creating window\n");
  }

  // exit gracefully
  //
  return status;
}

// method: debias
// 
// arguments:
//  vector<double>& sig: the signal vector (input/output)
//
// return: a double value containing the average value of the signal
//
// this code computes the average of a signal and subtracts it from
// each sample value.
//
double Mplpc::debias(vector<double>& sig_a) {
  
  // get the number of samples
  //
  long ns = sig_a.size();

  // compute the average
  //
  double sum = 0;
  for (long i = 0; i < ns; i++) {
    sum += sig_a[i];
  }
  double avg = sum / (double)ns;

  // subtract the average
  //
  for (long i = 0; i < ns; i++) {
    sig_a[i] -= avg;
  }

  // exit gracefully
  //
  return avg;
}
