// This file contains all the helper and plotting functions for the
// debugging, testing and regression analysis
//

#include<fstream>
#include<iostream>

// local include files
//

#include "Mplpc.h"
#include "Fe.h"
#include "Edf.h"

// method: helper_print_arr
//
// arguments:
//  VectorDouble arr: array value that needs to be printed out
//  int st_i: start index of an array to be printed out
//  int end_i: stop index of an array to be printed out
//
// return: a logical variable indicating status
//
// This method simply prints out all the array variables
bool Mplpc::helper_print_arr(VectorDouble& arr, long st_i, long end_i) {

  // if st_i and end_i both are 0 that means, no argument was passed and
  // we need to print out the entire array
  //
  if ((st_i == 0) && (end_i == 0)) {
    st_i = 0;
    end_i = arr.size();
  }
  else if ( (st_i < 0) || (end_i > arr.size()) ) {
    fprintf(stdout, "    Mplpc::helper_print_arr(): Error accessing start/end indices\n");
    fprintf(stdout, "    Start-> %d\n",st_i);
    fprintf(stdout, "    End-> %d\n",end_i);
    return false;
  }

  long ns = end_i;

  // loop through all the elements in the range and print out values
  //
  for (long i = st_i; i < ns; i++) {
    fprintf(stdout, "%d->%f\n", i, arr[i]);
  }

  // return gracefully
  //
  return true;
}

// end of method
//


bool Mplpc::helper_print_arr(VVectorDouble& arr, long st_i, long end_i) {

  // if st_i and end_i both are 0 that means, no argument was passed and
  // we need to print out the entire array
  //
  if ((st_i == 0) && (end_i == 0)) {
    st_i = 0;
    end_i = arr.size();
  }
  else if ( (st_i < 0) || (end_i > arr.size()) ) {
    fprintf(stdout, "    Mplpc::helper_print_arr(): Error accessing start/end indices\n");
    fprintf(stdout, "    Start-> %d\n",st_i);
    fprintf(stdout, "    End-> %d\n",end_i);
    return false;
  }

  long ns = end_i;

  // loop through all the elements in the range and print out values
  //
  for (long i = st_i; i < ns; i++) {
    fprintf(stdout, "%d-> ", i);
    for (long j = 0; j < arr[i].size(); j++) {
      fprintf(stdout, " %f ", arr[i][j]);
    }
    fprintf(stdout, "\n");
  }

  // return gracefully
  //
  return true;
}

// end of method
//




// method: helper_load_dat_tmpf
//
// arguments:
//  fname_str: name of the file in a string format
//  VectorDouble arr: array with values to be saved in the file.
//
// return: a logical variable indicating status
//
// This method simply prints out all the array variables
//
bool Mplpc::helper_load_1d_dat_tmpf(std::string fname_str_a, VectorDouble& arr_a, std::string ext_a,
				 long ind_lim) {

  // create the temporary output file
  // to save the array information in a text file format
  //
  std::string tmp_fname = fname_str_a + "." + ext_a;
  const char *cstr = tmp_fname.c_str();
  fprintf(stdout, "Loading array information in to file-> %s\n", cstr);


  long arrsize = arr_a.size();
  if (ind_lim == 0) {
    ind_lim = arrsize;
  }

  if (ind_lim > arrsize) {
    ind_lim = arrsize;
    fprintf(stdout, "Arg 'indices range' %d is greater than arr length %ld\n",ind_lim, arrsize);
  }
  
  std::ofstream ofile (tmp_fname);
  fprintf(stdout, "index limit is: %ld\n", ind_lim);

  for (int i = 0; i < ind_lim; i++) {
    ofile << std::to_string(i) + " " + std::to_string(arr_a[i]) << std::endl;

  }

  ofile.close();

  // FILE* fp = fopen(cstr, "w");
  // if (fp == (FILE*)NULL) {
  //   fprintf(stdout,
  // 	    "   Mplpc-Helper: load_dat_tmpf(): error opening the tmp output file [%s]\n",
  // 	    cstr);
  //   return false;
  // }

  // for (int i = 0; i < arr_a.size(); i++) {
  //   fwrite(std::to_string(i), sizeof();
  // }
  
  // fclose(fp);

  return true;
}


bool Mplpc::helper_load_2d_dat_tmpf(std::string fname_str_a, VectorDouble& xarr_a, VectorDouble& yarr_a, std::string ext_a, long ind_lim) {

  // create the temporary output file
  // to save the array information in a text file format
  //
  std::string tmp_fname = fname_str_a + "." + ext_a;
  const char *cstr = tmp_fname.c_str();
  fprintf(stdout, "Loading array information in to file-> %s\n", cstr);


  long arrsize = xarr_a.size();
  if (ind_lim == 0) {
    ind_lim = arrsize;
  }

  if (ind_lim > arrsize) {
    ind_lim = arrsize;
    fprintf(stdout, "Arg 'indices range' %d is greater than arr length %ld\n",ind_lim, arrsize);
  }
  
  std::ofstream ofile (tmp_fname);
  fprintf(stdout, "index limit is: %ld\n", ind_lim);

  if (xarr_a.size() != yarr_a.size()) {
    fprintf(stdout, "x and y axis have different lengths %d Vs %s\n", xarr_a.size(), yarr_a.size());
    return false;
  }
  
  for (int i = 0; i < ind_lim; i++) {
    ofile << std::to_string(xarr_a[i]) + " " + std::to_string(yarr_a[i]) << std::endl;

  }

  ofile.close();

  return true;
}


// method: init_fftpow
//
// arguments:
//  long fft_order: the length of the fft - must be a power of 2 (input)
//
// return: a logical value indicating status
//
// creates lookup tables for the radix-4 transform.
//
bool Mplpc::helper_init_fftpow(long fft_order_a) {

  Edf edf_d;
  
  // set the order
  //
  fft_n_d = fft_order_a;

  // compute the base
  //
  long i = 1;
  fft_m_d = 0;

  while (i < fft_n_d) {
    i = (long)(2 * i);
    fft_m_d = (long)(fft_m_d + 1);
  }

  // compute constants
  //
  fft_q_d = 2.0 * M_PI / (double)fft_n_d;
  
  // allocate space for lookup tables
  //
  edf_d.resize(fft_wd_real_d, fft_n_d, false);
  edf_d.resize(fft_wd_imag_d, fft_n_d, false);
  
  // compute the lookup table entries
  //
  for (long i = 0; i < fft_n_d; i++) {
    double t = fft_q_d * (double)i;
    fft_wd_real_d[i] = cos(t);
    fft_wd_imag_d[i] = sin(t);
  }
  
  // allocate memory for the temporary workspace used
  //
  edf_d.resize(fft_tempd_d, fft_n_d, false);
  edf_d.resize(fft_tempc_d, 2*fft_n_d, false);

  // exit gracefully
  //
  return true;
}

// method: convert_to_fftpow
// 
// arguments:
//  vector<double>& output: the feature vector (output)
//  vector<double>& input: the signal data (input)
//
// return: a logical value indicating status
//
// This method computes a Radix-2 FFT and returns the magnitude squared
// of the spectrum, which is technically the power spectrum.
//
// The code follows the exact structure of that in the book :
//
//  C.S. Burrus and T.W. Parks, "DFT/FFT and Convolution Algorithms - Theory
//  and Implementation", 2nd Edition, John Wiley and Sons, New York, NY, 1985.
// 
// Note that the data is zero-stuffed if the data is less than the
// order of the fft.
//
// Note also the order is defined in the header file, and this routine
// outputs samples from [0,N/2] since the input signal is real.
//
bool Mplpc::helper_convert_to_fftpow(vector<double>& output_a, vector<double>& input_a) {
  
  // declare local variables
  //
  bool status = true;
  long ana_wlen = input_a.size();

  // declare some scratch variables for the fft
  //
  long i, j, k, l, m, i1, i2, i3;
  long n1, n2, N_d_1, N_d_2;
  long a, b, c;
  long r1, r2, r3, r4, s1, s2, s3, s4, co1, co2, co3, si1, si2, si3, temp;
  double real_twid, imag_twid;
  double tempr, tempi;

  // copy at most fft_n_d samples into the vector
  //
  vector<double> freq_zsig(fft_n_d);
  long N = ana_wlen;
  if (N > fft_n_d) {
    N = fft_n_d;
  }
  for (i = 0; i < N; i++) {
    freq_zsig[i] = input_a[i];
  }
  for (i = N; i < fft_n_d; i++) {
    freq_zsig[i] = 0.0;
  }

  // copy input data to temporary memory and zero the output so that
  // the input data is retained after calculations.
  //
  for (k = 0; k < fft_n_d; k++) {
    fft_tempc_d[k] = freq_zsig[k];
    fft_tempd_d[k] = 0.0;
  }

  // actual dif radix-2 computation:
  //  main computation loop; looping through all the stages; there are
  //  log2(order_d) stages.
  //

  // looping  through the first  stage
  //
  n2 = fft_n_d;
  n1 = n2;
  n2 = n2 >> 1;
  i1 = 0;
  i2 = (long)((fft_n_d) / n1);
  
  for (j = 0; j < n2; ++j) {
    
    // loop through each butterfly
    //
    real_twid = fft_wd_real_d[i1];
    imag_twid = fft_wd_imag_d[i1];
    i1 = i1 + i2;
    
    for (k = j; k < fft_n_d; k += n1) {
	
      // perform the butterfly computation
      //
      l = k + n2;
      tempr = fft_tempc_d[k] - fft_tempc_d[l];
      fft_tempc_d[k] = fft_tempc_d[k] + fft_tempc_d[l];
      fft_tempc_d[l] = (real_twid * tempr);
      fft_tempd_d[l] = -(imag_twid * tempr);
    }
  }
  
  for (i = 1; i < fft_m_d; ++i) {
    
    // looping  through each of the remaining stages
    //
    n1 = n2;
    n2 = n2 >> 1;
    i1 = 0;
    i2 = (long)(fft_n_d / n1);
    
    for (j = 0; j < n2; ++j) {

      // loop through each butterfly
      //
      real_twid = fft_wd_real_d[i1];
      imag_twid = fft_wd_imag_d[i1];
      i1 = i1 + i2;
      
      for (k = j; k < fft_n_d; k += n1) {

	// perform the butterfly computation; computation within square braces
	// in eqn (9.3.34) multiplied by the twiddle factor
	//
	l = k + n2;
        tempr = fft_tempc_d[k] - fft_tempc_d[l];
        fft_tempc_d[k] = fft_tempc_d[k] + fft_tempc_d[l];
        tempi = fft_tempd_d[k] - fft_tempd_d[l];
        fft_tempd_d[k] = fft_tempd_d[k] + fft_tempd_d[l];
        fft_tempc_d[l] = (real_twid * tempr) + (imag_twid * tempi);
        fft_tempd_d[l] =  (real_twid * tempi) - (imag_twid * tempr);
      }
    }
  }
  
  // procedure for bit reversal
  //
  j = 0;
  n1 = fft_n_d - 1;
  n2 = fft_n_d >> 1;
  
  for (i = 0; i < n1 ; ++i) {
    if (i < j) {
      tempr = fft_tempc_d[j];
      fft_tempc_d[j] = fft_tempc_d[i];
      fft_tempc_d[i] = tempr;
      tempr = fft_tempd_d[j];
      fft_tempd_d[j] = fft_tempd_d[i];
      fft_tempd_d[i] = tempr;
    }
    k = n2;
    while (k <= j) {
      j -= k;
      k = k >> 1;
    }
    j += k;
  }

  // interlace the output and store in the fft_tempc_d array
  //
  for (k = fft_n_d - 1; k >= 0; k--) {
    fft_tempc_d[k * 2 + 1] = fft_tempd_d[k];
    fft_tempc_d[k * 2] = fft_tempc_d[k];    
  }

  // allocate output space
  //
  long num_pts_out = n2 + 1;
  edf_d.resize(output_a, num_pts_out, false);
		   
  // convert the complex spectrum to a log magnitude spectrum
  //
  for (k = 0; k < num_pts_out; k++) {

    // compute the magnitude squared
    //
    long kk = 2 * k;
    long kk1 = kk + 1;
    double val_r = fft_tempc_d[kk];
    double val_i = fft_tempc_d[kk1];
    double mag_sq = val_r*val_r + val_i*val_i;
    output_a[k] = mag_sq;
  }

  // exit gracefully
  //
  return status;
}


// method: helper_normalize_fftpow
// 
// arguments:
//
// return: a logical value indicating status
//
// normalizes the FFT array for both y and x axis
// 
bool Mplpc::helper_normalize_fftpow(VectorDouble& xfft_vec, VectorDouble& yfft_vec,
				    VectorDouble& fft_arr_a, int siglen_a, int fft_order,
				    double samprate_a) {
  
  fprintf(stdout, "fft array length is: %d\n", fft_arr_a.size());

  // allocate memory for the output arrays
  //
  long outsize = fft_arr_a.size();
  
  edf_d.resize(xfft_vec, outsize, false);
  edf_d.resize(yfft_vec, outsize, false);
  
  for (long i = 0; i < fft_arr_a.size(); i++) {

    xfft_vec[i] = (double) i * samprate_a / (double) fft_order;
    yfft_vec[i] = fft_arr_a[i] / (double) siglen_a;
  }

  return true;
}


// method: helper_est_roots
// 
// arguments:
//
// return: a logical value indicating status
//
// this method estimates the roots of the quadratic equation
// 
bool Mplpc::helper_est_roots(VectorDouble& roots_a, VectorDouble& pc_a) {

  fprintf(stdout, "helper_est_roots is called...");
  return true;

}


// method: helper_eigen_analysis
//
// arguments:
//
//
// return: a logical value indicating the status
//
//   This method generates the eigenvalues and eigenvectors of a matrix
//
//bool Mplpc::helper_eigen_analysis(Eigen::EigenSolve<Eigen::Matrix<double>> eigensolv_obj, Matrix<double>& mat) {
