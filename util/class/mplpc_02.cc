// This file contains the compute methods.
//

// include the global modules
//

// local include files
//
#include "Mplpc.h"
#include "Fe.h"

// method: compute
//
// arguments:
//  char* oname: mplpc filename (output)
//  char* iname: EDF filename (input)
//
// return: a boolean indicating status
//
// This method converts an EDF file to a mplpc excitation file. Its main
// purpose is to invoke a lower-level compute function and write the
// result to a file. Currently we process raw sampled data signals,
// but this needs to be changed to EDF signals.
//
bool Mplpc::compute(char* oname_a, char* iname_a) {
  
  // declare local variables
  //
  bool status;
  VVVectorDouble sig;
  
  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "Mplpc::compute(): begin mplpc analysis (sampled data mode)\n");
  }

  // initalize the window function
  //
  status = create_window();

  // do the actual mplpc analysis
  //

  // case 1: when file is edf
  //
  if (edf_d.is_edf((char*) iname_a)) {

    // mplpc analysis
    //
    status = Mplpc::compute_00_edf(sig, iname_a);

  }
  // case 2: when file is not edf
  //
  else {

    // mplpc analysis
    //
    status = Mplpc::compute_00(sig, iname_a);
   
  }
  

  // save the sampled data
  //
  edf_d.create_filename(oname_a, iname_a, odir_d, oext_d, odir_repl_d);

  // create the output file
  //
  FILE* fp = fopen(oname_a, "w");
  if (fp == (FILE*)NULL) {
    fprintf(stdout,
	    "   Mplpc::compute(): error opening output file [%s]\n",
	    oname_a);
    return false;
  }


  if (strcmp(oext_d, DEF_FEAT_TYPE_NAME) == 0) {
  // write the data
  //
  short int val;
  long ch_end = sig[0].size();
  long feat_end = sig[0][0].size();

  // iterate each channel
  //
  for (long j = 0; j < sig.size(); j++) {
    // iterate each frame
    //
    for (long k = 0; k < ch_end; k++) {
      // iterate over feature dimensions
      //
      for (long l = 0; l < feat_end; l++) {
	val = Mplpc::clip_value(sig[j][k][l]);

	if (fwrite(&val, sizeof(short int), 1, fp) != 1) {
	  fprintf(stdout, "   Mplpc::compute(): error writing data\n");
	  return false;
	}
      }
    }
  }

  // close the output file
  //
  fclose(fp);
  }

  else if (strcmp(oext_d, Edf::FFMT_NAME_01) == 0) {
    edf_d.write_features_raw(sig, oname_a);
  }



  // exit gracefully
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "Mplpc::compute(): end of mplpc analysis\n");
  }
  return true;
}

// method: compute_00
//
// arguments:
//  VVVectorDouble& sig: signal data (output)
//  char* iname: EDF filename (input)
//
// return: a boolean indicating status
//
// This method opens a binary file, retrieves the signal, processes it
// through the mplpc algorithm, and returns the new data. It outputs
// a multichannel signal.
//
bool Mplpc::compute_00(VVVectorDouble& sig_a, char* iname_a) {

  // declare local variables
  //
  VVectorDouble sig_t;
  bool status = false;

  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00(): begin sampled data processing\n");
  }
  
  // open the file
  //
  FILE* fp = fopen(iname_a, "r");
  if (fp == (FILE*)NULL) {
    fprintf(stdout, "   Mplpc::compute_00(): error opening file (%s)\n",
	    iname_a);
  }
  else if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00(): file is opened (%s)\n", iname_a);
  }

  // get the number of samples from the file
  //
  fseek(fp, 0L, SEEK_END);
  long num_samples = ftell(fp) / sizeof(short int);
  rewind(fp);

  // retrieve the signal:
  //  we use a temporary buffer so we can delete it and free memory
  //
  short int *buf = new short int[num_samples];
  long num_samples_read = fread(buf, sizeof(short int), num_samples, fp);
  if (num_samples_read != num_samples) {
    fprintf(stdout, "   Mplpc::compute_00(): error reading file (%ld %ld)\n",
	    num_samples, num_samples_read);
    fclose(fp);
    return false;
  }
  else if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "   Mplpc::compute_00(): processing %ld samples\n",
	    num_samples);
  }

  // resize the VectorDouble signal to be one channel x num_samples (temporary)
  //
  edf_d.resize(sig_t, (long)1); // set up size for channels
  edf_d.resize(sig_t[0], num_samples); // set up size for the elements of the vector

  // transfer the data and delete buf
  //
  for (long i = 0; i < num_samples; i++) {
    sig_t[0][i] = buf[i];
  }
  delete [] buf;

  // close the file
  //
  fclose(fp);
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00(): file is closed (%s)\n", iname_a);
  }

  // do an mplpc analysis
  //
  status = Mplpc::compute_mplpc(sig_a, sig_t);

  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "   Mplpc::compute_00(): end sampled data processing\n");
  }
  
  // exit gracefully
  //
  return status;
}

// method: compute_00_edf
// may need to revise
// arguments:
//  VVVectorDouble& sig: signal data (output)
//  char* iname: EDF filename (input)
//
// return: a boolean indicating status
//
// This method opens a binary file, retrieves the signal, processes it
// through the mplpc algorithm, and returns the new data. It outputs
// a multichannel signal.
//
bool Mplpc::compute_00_edf(VVVectorDouble& sig_a, char* iname_a) {

  // declare local variables
  //
  VVectorDouble sig_t;
  VVectorDouble sig_s;
  VVectorDouble sig_f;
  bool status = false;

  // display a debug message
  
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
  	    "   Mplpc::compute_00_edf(): begin edf data processing\n");
  }
  
  // read the EDF file
  //
  if (!(status = edf_d.read_edf(sig_t, iname_a, true, true))) {
    return status;
  }
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_01_edf(): signal loaded from the EDF file\n");
  }

  // select the channels
  //
  if (!(status = edf_d.select(sig_s, sig_t, cselect_d, match_mode_d))) {
    return status;
  }
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Fe::compute_00_select(): channel selection completed\n");
  }


  // pick up the sample frequency and number of channels
  //
  sample_freq_d = edf_d.get_sample_frequency();
  num_chan_file_d = edf_d.get_num_channels_file();
  num_chan_proc_d = edf_d.get_num_channels_proc();

  // apply the montage
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_select(): applying the montage\n");
  }
  if (!(status = edf_d.apply_montage(sig_f, sig_s, montage_d, match_mode_d))) {
    return status;
  }

  // do an mplpc analysis
  //
  status = Mplpc::compute_mplpc(sig_a, sig_f);

  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "   Mplpc::compute_00(): end sampled data processing\n");
  }
  
  // exit gracefully
  //
  return status;
}


// method: compute_mplpc
//
// arguments:
//  VVVectorDouble& osig: signal data (output)
//  VVectorDouble& isig: signal data (input)
//
// return: a boolean indicating status
//
// This method processes a multichannel signal by iterating over its
// channels one channel at a time.
//
bool Mplpc::compute_mplpc(VVVectorDouble& osig_a, VVectorDouble& isig_a) {

  // declare local variables
  //
  bool status = true;

  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_mplpc(): begin channel processing\n");
  }


  // resize the output signal
  //
  edf_d.resize(osig_a, isig_a.size());

  // resize the osig_a based on the number of frames, channels and out-dimensions/features
  //
  
  // loop over all the channels
  //
  for (long i = 0; i < isig_a.size(); i++) {

    if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
      fprintf(stdout,
	      "   Mplpc::compute_mplpc(): processing channel %ld\n", i);
    }
    status = Mplpc::compute_mplpc(osig_a[i], isig_a[i]);
    if (!status) {
      fprintf(stdout,
	      "   Mplpc::compute_mplpc(): error processing channel %d\n", i);
      return status;
    }	
  }

  // exit gracefully
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_mplpc(): end of channel processing\n");
  }

  return status;
}

// method: compute_mplpc
//
// arguments:
//  VVectorDouble& osig: signal data (output)
//  VectorDouble& isig: signal data (input)
//
// return: a boolean indicating status
//
// This method processes a single channel through the algorithm known
// as multipulse linear prediction. This is main processing function
// that does all the heavy lifting.
//
bool Mplpc::compute_mplpc(VVectorDouble& osig_a, VectorDouble& isig_a) {

  // make sure the right analysis parameters are set. to keep things simple,
  // we currently only support these configurations:
  //    window duration >= frame duration
  //    right-aligned windows
  //
  if (win_align_d != Mplpc::WNAL_RIGHT) {
    return false;
  }
  else if (frame_duration_d > window_duration_d) {
    return false;
  }
  else if (debias_mode_d == Mplpc::DBS_WINDOW) {
    return false;
  }

  // declare local variables
  //
  bool status = true;

  // convert parameters from time (secs) to integers (samples):
  //  use a truncation approach right now
  //
  long nsamps = isig_a.size();
  long n_fdur = round(frame_duration_d * sample_freq_d);
  long n_wdur = round(window_duration_d * sample_freq_d);
  long num_frames = nsamps / n_fdur;
  long n_impres = round(impres_dur_d * sample_freq_d);

  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
  fprintf(stdout, "   Mplpc::compute_mplpc(): nsamps = %ld, n_fdur = %ld, n_wdur = %ld, num_frames = %ld\n", nsamps, n_fdur, n_wdur, num_frames);
  }
 
  // resize the output signal to make room for the output data
  //
  edf_d.resize(osig_a, isig_a.size());

  // calculate output feature dimensions from info collected from
  // parameter file
  //
  long out_dim = 1;
  
  // initialize the output signal array so that pulses
  // at various indices can be loaded at any time in the loop
  //
  for (int i = 0; i < osig_a.size(); i++) {
    edf_d.resize(osig_a[i], out_dim);
  }
  
  // step 1: debias the data
  //
  if (debias_mode_d == Mplpc::DBS_SIGNAL) {
    float avg = Mplpc::debias(isig_a);
  }

  // step 2: preemphasize the signal "in place" to save memory
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "   Mplpc::compute_mplpc(): preemphasis\n");
  }
  float pre = 0.0;
  float cur = 0.0;

  // fprintf(stdout, "preemphsize begin...");
  for (long i = 0; i < nsamps; i++) {
    cur = isig_a[i];
    isig_a[i] = cur + preemphasis_d * pre;
    pre = cur;
    // fprintf(stdout, "isig updated at %d -> %f\n", i, isig_a[i]);
  }
  
  // declare a buffer to use for windowing and clear it
  //
  VectorDouble sig_pbuf(n_wdur, (double)0);
  
  // loop over the signal by frames
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "   Mplpc::compute_mplpc(): looping over frames\n");
  }
  for (long i = 0; i < num_frames; i++) {

    // resize the output signal to make room for the output data
    //
    //edf_d.resize(osig_a[i], out_dim);
    
    // compute the start and end indexes of the frame and window in the signal
    //
    long n_offset = n_wdur - n_fdur;
    long i_frame_beg = i * n_fdur;
   
    // copy the newest data to the front of the window
    //
    if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
      fprintf(stdout, "   Mplpc::compute_mplpc(): copying data\n");
    }
    long n = n_offset;
    long m = i_frame_beg;
    for (long j = 0; j < n_fdur; j++) {
      sig_pbuf[n] = isig_a[m];
       n++;
       m++;
     }

    // step 3: Hamming window the data
    //
    if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
      fprintf(stdout, "   Mplpc::compute_mplpc(): windowing data\n");
    }
    VectorDouble sig_wbuf(n_wdur, (double)0.0);
    for (long j = 0; j < n_wdur; j++) {
      sig_wbuf[j] = sig_pbuf[j] * win_fct_d[j];
    }

    // step 4: autocorrelation computation
    //
    if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
      fprintf(stdout, "   Mplpc::compute_mplpc(): autocorrelation\n");
    }
    VectorDouble autocor;
    status = compute_autocor(autocor, sig_wbuf, lp_order_d);

    // step 5: linear prediction computation
    //
    if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
      fprintf(stdout, "   Mplpc::compute_mplpc(): linear prediction\n");
    }
    VectorDouble rc;
    VectorDouble pc;
    status = compute_lpc(rc, pc, autocor, lp_order_d);
      
    // step 6: compute impulse response and the energy of the impulse response
    //
    if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
      fprintf(stdout, "   Mplpc::compute_mplpc(): impulse response\n");
    }

    VectorDouble impres;
    status = compute_impulse_response(impres, pc, n_impres);
    float impres_egy = 0;
    for (long j = 0; j < n_impres; j++) {
      impres_egy += impres[j] * impres[j];
    }

    // step 7: transfer a chunk of the signal into a temporary buffer
    //         so we can work on it without corrupting future frames.
    //         this buffer needs to be larger than a frame so pulses
    //         at the edge of a frame can be accurately loaded. we are
    //         going to find a pulse, subtract out its effect from this
    //         tmp signal, and find the next pulse.
    //
    if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
      fprintf(stdout, "   Mplpc::compute_mplpc(): copying signal\n");
    }

    long sig_tmp_len = n_fdur + n_impres;
    long sig_tmp_off = i_frame_beg;        
    VectorDouble sig_tmp(sig_tmp_len, (double)0.0);

    // check if the loop will exceed the nsamples
    // if the sig_tmp_off exceeds nsamps we set the
    // value of sig_tmp_len to the nfdur
    //
    if ((sig_tmp_off + sig_tmp_len)> nsamps){
      sig_tmp_len = n_fdur;
    }

    for (long j = 0; j < sig_tmp_len; j++) {
      // if (sig_tmp_off < nsamps) {
	sig_tmp[j] = isig_a[sig_tmp_off];
	sig_tmp_off++;
	//      }
    }

    // step 8: loop over all the pulses
    //
    if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
      fprintf(stdout, "   Mplpc::compute_mplpc(): finding pulses\n");
    }

    for (long j = 0; j < num_pulses_d; j++) {

      // step 8a: find the maximum in the crosscorrelation function
      //
      long max_loc = (long)0;
      float max_val = (float)0.0;

      for (long k = 0; k < n_fdur; k++) {

	// compute the crosscorrelation: make sure we don't go over the end
	// of the signal buffer
	//
	float sum = (float)0.0;
	long cc_off = k;
	
	for (long l = 0; l < n_impres; l++) {
	  if (cc_off < sig_tmp.size()) {
	    sum += sig_tmp[cc_off] * impres[l];
	    cc_off++;
	  }
	}
      
	// find the maximum: note we must preserve the sign, so we
	// have to take the absolute value
	//
	if (fabs(sum) > fabs(max_val)) {
	  max_val = sum;
	  max_loc = k;
	}
      }

      // compute the gain of the pulse: crosscorrelation / energy
      //
      float gain = max_val / impres_egy;
      // fprintf(stdout, " max_value and impulse resp energy is: %f and %f\n", max_val, impres_egy);

      // subtract off the effects of the pulse
      //
      long m = max_loc;

      for (long k = 0; k < n_impres; k++) {
	  sig_tmp[m] -= gain * impres[k];
	m++;
      }

      // output the pulse location and amplitude
      //
      if (debug_level_d > Dbgl::LEVEL_BRIEF) {
	fprintf(stdout, "frame no: %d, pulse no. %d, loc/amp = (%d, %f)\n",
		i, j, i_frame_beg + max_loc, max_val);
      }
      // fprintf(stdout, "osig index is %d\n", i);
      // fprintf(stdout, "index for osig is: %d\n", (int) i_frame_beg + max_loc); 
      osig_a[i_frame_beg + max_loc][0] += gain;
      // osig_a[i_frame_beg + max_loc][0] += max_val;
      
      // fprintf(stdout, "osig value at index %d is: %f", i, osig_a[i_frame_beg + max_loc][0]); 
    }

    // shift the window:
    //  we should really do a block move here
    //
    if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
      fprintf(stdout, "   Mplpc::compute_mplpc(): shifting the signal\n");
    }
    long num_to_shift = n_wdur - n_fdur;
    long k = n_fdur - 1;
    for (long j = 0; j < num_to_shift; j++) {
      sig_pbuf[j] = sig_pbuf[k++];
    }
  }

  // exit gracefully
  //
  return status;
}

//
// end of file
