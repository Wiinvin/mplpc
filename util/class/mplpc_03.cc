// This file contains basic signal processing and initialization methods.
//

// local include files
//
#include "Mplpc.h"

// method: clip_value
//
// arguments:
//  float val: value to be checked (input)
//
// return: a short integer that is clipped
//
// This method limits the value of a float to a 16-bit range.
//
short int Mplpc::clip_value(float val_a) {

  // check for saturation
  //
  if (val_a > Mplpc::MAX_VALUE) {
    return (short int)Mplpc::MAX_VALUE;
  }
  else if (val_a < -Mplpc::MAX_VALUE) {
    return (short int)-Mplpc::MAX_VALUE;
  }

  // else: return a rounded value
  //
  else {
    return (short int)round(val_a);
  }
}

// method: create_window
//
// arguments: none
//
// return: a logical variable indicating status
//
// This method creates a time domain window function. Note that the
// type of window is defined from the protected data.
//
bool Mplpc::create_window() {

  // declare local variables
  //
  bool status = true;

  // compute some constants related to the length of the window
  //
  long N = round(window_duration_d * sample_freq_d);
  long Nm1 = N - 1;

  // display debug information
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "Mplpc::create_window(): [%s] window [%s] norm [%ld] samples\n",
	    win_type_str_d, win_norm_str_d, N);
  }

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

  // else: unkwown window type
  //
  else {
    fprintf(stdout,
	    "   Mplpc::create_window(): unknown window type [%d]\n",
	    (long)win_type_d);
    return false;
  }

  // normalize the window to have an energy of 1.0
  //
  if (win_norm_d == WNRM_EGY) {

    // compute the energy of the window shape
    //
    float sum = 0;
    for (long n = 0; n < N; n++) {
      sum += win_fct_d[n] * win_fct_d[n];
    }

    // normalize by energy
    //
    float inv_sum = 1.0 / sqrt(sum);
    for (long n = 0; n < N; n++) {
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
//  VectorDouble& sig: the signal vector (input/output)
//
// return: a double value containing the average value of the signal
//
// this code computes the average of a signal and subtracts it from
// each sample value.
//
double Mplpc::debias(VectorDouble& sig_a) {
  
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
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "   Mplpc::debias(): average value = %f\n", avg);
  }

  // subtract the average
  //
  for (long i = 0; i < ns; i++) {
    sig_a[i] -= avg;
  }

  // exit gracefully
  //
  return avg;
}

// method: compute_autocor
//
// arguments:
//  VectorDouble& autocor: autocorrelation function (output)
//  VectorDouble& sig: signal vector (input)
//  long lp_order: the order of the autocorrelation analysis (input)
//
// return: a logical variable indicating status
//
// This method computes an autocorrelation function. Note that it
// uses the length of the signal vector for the number of samples.
// It outputs (lp_order + 1) values since it includes R[0].
//
bool Mplpc::compute_autocor(VectorDouble& autocor_a, VectorDouble& sig_a,
			    long lp_order_a) {

  // declare local variables
  //
  long status = true;
  long N = sig_a.size();
  float N_1 = 1.0 / (float)N;

  // create output space
  //
  autocor_a.resize(lp_order_a + 1);

  // loop over the order
  //
  for (long i = 0; i <= lp_order_a; i++) {

    float sum = 0.0;
    for (long j = 0; j < N - i; j++) {
      sum += sig_a[j] * sig_a[j + i];
    }
    autocor_a[i] = sum * N_1;
  }

  // exit gracefully
  //
  return status;
}

// method: compute_lpc
//
// arguments:
//  VectorDouble& rc: reflection coefficients (output)
//  VectorDouble& pc: predictor coefficients (output)
//  VectorDouble& autocor: autocorrelation function (input)
//  long lp_order: the order of the lpc analysis (input)
//
// return: a logical variable indicating status
//
// This method converts an autocorrelation function to a linear prediction
// model using the step down procedure.
//
bool Mplpc::compute_lpc(VectorDouble& rc_a, VectorDouble& pc_a,
			VectorDouble& autocor_a, long lp_order_a) {


  long status = true;
  long pc_order = lp_order_a + 1;
  VectorDouble tmp(pc_order, (double)0.0);

  rc_a.resize(lp_order_a, (double)0.0);
  pc_a.resize(pc_order+1, (double)0.0);
  fprintf(stdout, "in compute method...");
  float err_egy = autocor_a[0];  
  //  rc_a[0] = autocor_a[0];// PROBABLY THIS SHOULDB'T BE HERE....
  pc_a[0] = 1.0;

  for (long i = 1; i <= lp_order_a; i++) {
    float acc = 0.0;
    for (long j = 1; j <= i; j++) {
      acc -= pc_a[i-j] * autocor_a[j];
    }
    pc_a[i] = acc / err_egy;
    rc_a[i-1] = pc_a[i];

    float pci = 0.0;
    float pcki = 0.0;
    for (long k = 1; k <= i/2; k++) {
      pci = pc_a[k] + pc_a[i] * pc_a[i - k];
      pcki = pc_a[i - k] + pc_a[i] * pc_a[k];
      pc_a[k] = pci;
      pc_a[i - k] = pcki;
    }
       
    err_egy = err_egy * (1.0 - pc_a[i]*pc_a[i]);
  }
  fprintf(stdout, "compute method finished...");
  return status;
}

// method: compute_residual
//
// arguments:
//  VectorDouble& osig: residual signal (output)
//  VectorDouble& isig: original signal (input)
//  VectorDouble& pc: predictor coefficients (input)
//  long idx: starting index in the signal
//  long n_fdur: frame duration in samples
//
// return: a logical variable indicating status
//
// This method computes the LPC residual.
//
bool Mplpc::compute_residual(VectorDouble& osig_a, VectorDouble& isig_a,
			     VectorDouble& pc_a, long idx_a, long n_fdur_a) {

  // declare local variables
  //
  long i_end = idx_a + n_fdur_a;
  bool status = true;

  // filter the input signal
  //
  for (long i = idx_a; i < i_end; i++) {

    // compute the dot product between the filter and the signal
    //
    float sum = isig_a[i];
    for (long j = 1; j < pc_a.size(); j++) {
      if ((i - j) >= 0) {
	sum -= isig_a[i - j] * pc_a[j];
      }
    }
    osig_a[i] = sum;
  }

  // exit gracefully
  //
  return status;
}

// method: compute_impulse_response
//
// arguments:
//  VectorDouble& hres: impulse response (output)
//  VectorDouble& pc: predictor coefficients (input)
//  long num_samples: number of samples to generate (input)
//
// return: a logical variable indicating status
//
// This method computes the impulse response of the LPC filter.
//
bool Mplpc::compute_impulse_response(VectorDouble& hres_a,
				     VectorDouble& pc_a, long num_samples_a) {

  // declare local variables
  //
  long flen = pc_a.size();
  VectorDouble filt(flen, (double)0.0);
  bool status = true;

  // create space
  //
  hres_a.resize(num_samples_a);

  // initialize the impulse response and the filter memory:
  //  note only the first sample of the input is non-zero
  //
  hres_a[0] = 1.0 * pc_a[0];
  filt[0] = hres_a[0];

  // loop over all the output samples
  //
  for (long i = 1; i < num_samples_a; i++) {

    // shift the filter memory
    //
    long j = flen - 1;
    while (j > 0) {
      filt[j] = filt[j-1];
      j--;
    }

    // compute the dot product
    //
    float sum = 0.0;
    for (long j = 1; j < flen; j++) {
      sum += pc_a[j] * filt[j];
    }
    hres_a[i] = sum;
    filt[0] = sum;
  }

  // exit gracefully
  //
  return status;
}

//
// end of file
