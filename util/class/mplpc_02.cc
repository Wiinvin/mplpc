// local include files
//
#include "Mplpc.h"

// method: compute
//
// arguments:
//  char* oname: mplpc filename (output)
//  char* iname: EDF filename (input)
//
// return: a boolean indicating status
//
// This method converts an EDF file to a mplpc excitation file.
//
bool Mplpc::compute(char* oname_a, char* iname_a) {
  
  // declare local variables
  //
  bool status = false;
  VVectorDouble sig;
  
  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "Mplpc::compute(): begin mplpc analysis (%u)\n",
	    algo_mode_d);
  }

  // branch on the selection mode
  //
  if (select_mode_d == Edf::SELMODE_SELECT) {
    status = compute_00_select(sig, iname_a);
  }
  else if (select_mode_d == Edf::SELMODE_REMOVE) {
    status = compute_00_remove(sig, iname_a);
  }
  status = true;

  // save the sampled data
  //
  if (status == true) {

    // create a filename
    //
    edf_d.create_filename(oname_a, iname_a, odir_d, oext_d, odir_repl_d);

    // write the data
    //
    if ((status = edf_d.write_edf(sig, oname_a)) == false) {
      fprintf(stdout, "%s [%s]\n",
	      "**> error in Mplpc::compute(): error writing",
	      oname_a);
      return status;
    }
  }

  // case 3: unknown
  //
  else {
    fprintf(stdout,
	    "**> error in Mplpc::compute(): unknown mode [%s] [%s]\n",
	    algo_mode_str_d, select_mode_str_d);
  }

  // exit gracefully
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "Mplpc::compute(): end of feature computation\n");
  }
  return true;
}

// method: compute_00_select
//
// arguments:
//  VVectorDouble& sig: signal data (output)
//  char* iname: EDF filename (input)
//
// return: a boolean indicating status
//
// This method converts an EDF signal to another EDF signal using
// selection mode.
//
bool Mplpc::compute_00_select(VVectorDouble& sig_a, char* iname_a) {

  // declare local variables
  //
  VVectorDouble sig_t;
  bool status = false;

  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_select(): begin sampled data selection\n");
  }
  
  // check for an EDF file
  //
  if (!(status = edf_d.is_edf(iname_a))) {
    return status;
  }

  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "   Mplpc::compute_00_select(): processing an EDF file\n");
  }
  
  // read the EDF file
  //
  if (!(status = edf_d.read_edf(sig_t, iname_a, true, true))) {
    return status;
  }
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_select(): signal loaded from the EDF file\n");
  }

  // select the channels
  //
  VVectorDouble sig_s;

  if (!(status = edf_d.select(sig_s, sig_t, cselect_d, match_mode_d))) {
    return status;
  }
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_select(): channel selection completed\n");
  }
  
  // pick up the sample frequency and number of channels
  //
  sample_freq_d = edf_d.get_sample_frequency();
  num_chan_file_d = edf_d.get_num_channels_file();
  num_chan_proc_d = edf_d.get_num_channels_proc();

  // apply the montage
  //
  VVectorDouble sig_m;

  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_select(): applying the montage\n");
  }

  if (!(status = edf_d.apply_montage(sig_m, sig_s, montage_d, match_mode_d))) {
    return status;
  }

  // do the mplpc analysis
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_select(): computing the mplpc excitation\n");
  }

  if (!(status = compute_mplpc(sig_a, sig_m))) {
    return status;
  }

  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_select(): end sampled data selection\n");
  }
  
  // exit gracefully
  //
  return status;
}

// method: compute_00_remove
//
// arguments:
//  VVectorDouble& sig: signal data (output)
//  char* iname: EDF filename (input)
//
// return: a boolean indicating status
//
// This method converts an EDF signal to another EDF signal using
// remove mode. Note that this method does not support signal generation
// or montage processing. Use mode "select" for these modes.
//
bool Mplpc::compute_00_remove(VVectorDouble& sig_a, char* iname_a) {

  // declare local variables
  //
  VVectorDouble sig_t;
  bool status = false;

  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_remove(): begin remove channel(s) [%s]\n",
	    cselect_d);
  }
  
  // check for an EDF file
  //
  if (!(status = edf_d.is_edf(iname_a))) {
    return status;
  }
  
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "   Mplpc::compute_00_remove(): processing an EDF file\n");
  }
  
  // read the EDF file
  //
  if (!(status = edf_d.read_edf(sig_t, iname_a, true, true))) {
    return status;
  }
  
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_remove(): signal loaded from the EDF file\n");
  }
  
  // remove the channels
  //
  if (!(status = edf_d.remove(sig_a, sig_t, cselect_d, match_mode_d))) {
    return status;
  }
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_00_remove(): channel selection completed\n");
  }
  
  // pick up the sample frequency and number of channels
  //
  sample_freq_d = edf_d.get_sample_frequency();
  num_chan_file_d = edf_d.get_num_channels_file();
  num_chan_proc_d = edf_d.get_num_channels_proc();

  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "   Mplpc::compute_00_remove(): end remove channel(s)\n");
  }
  
  // exit gracefully
  //
  return status;
}

// method: compute_mplpc
//
// arguments:
//  VVectorDouble& osig: signal data (output)
//  VVectorDouble& isig: signal data (input)
//
// return: a boolean indicating status
//
// This method computes the mplpc excitation signal.
//
bool Mplpc::compute_mplpc(VVectorDouble& osig_a, VVectorDouble& isig_a) { 

  // declare local variables
  //
  bool status = true;

  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_mplpc(): begin mplpc analysis\n");
  }
  
  // create the output signal container
  //
  long num_channels = isig_a.size();
  osig_a.resize(num_channels);

  // loop over all channels
  //
  for (long i = 0; i < num_channels; i++) {
    compute_mplpc(osig_a[i], isig_a[i]);
  }

  // display a debug message
  //
  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout,
	    "   Mplpc::compute_mplpc(): end mplpc analysis\n");
  }
  
  // exit gracefully
  //
  return status;
}

// method: compute_mplpc
//
// arguments:
//  VectorDouble& osig: signal data (output)
//  VectorDouble& isig: signal data (input)
//
// return: a boolean indicating status
//
// This method computes the mplpc excitation signal one channel at a time.
//
bool Mplpc::compute_mplpc(VectorDouble& osig_a, VectorDouble& isig_a) { 

  // declare local variables
  //
  long nsamp = isig_a.size();
  bool status = true;

  // resize the output
  //
  osig_a.resize(nsamp);

  // convert the frame and window duration
  //
  long ana_flen = frame_duration_d * sample_freq_d;
  long ana_wlen = window_duration_d * sample_freq_d;

  // compute the number of frames
  //
  long ana_nf = Mplpc::compute_num_frames(nsamp, ana_flen, ana_wlen);

  if (debug_level_d >= Dbgl::LEVEL_DETAILED) {
    fprintf(stdout, "%s = [%ld, %ld, %ld]\n",
	    "Mplpc::compute_mplpc(): frame/window/num frames",
	    ana_flen, ana_wlen, ana_nf);
  }

  // create a window function and a buffer
  //
  if (!Mplpc::create_window(ana_wlen)) {
    return false;
  }
  VectorDouble sig_wbuf(ana_wlen);

  // loop over all samples in the input
  //
  for (long i = 0; i < ana_nf; i++) {

    if ((debug_level_d >= Dbgl::LEVEL_MEDIUM) && (i < Edf::DEF_DBG_NF)) {
      fprintf(stdout, "%s = [%ld] [%ld, %ld, %ld]\n",
	      "Mplpc::compute_mplpc(): (frame) (start/stop/nsamp)",
	      i, i * ana_flen, (i+1) * ana_flen, nsamp);
    }
      
    // compute the center sample and the start of the analysis window:
    //  n_ptr is the index of the current sample to be transfered
    //  from the signal buffer to the window
    //
    long n_center = i * ana_flen + ana_flen / 2;
    long n_ptr, n_right;

    if (win_align_d == WAL_CENTER) {
      n_ptr = n_center - ana_wlen / 2;
      n_right = n_ptr + ana_wlen / 2;
    }
    else if (win_align_d == WAL_LEFT) {
      n_ptr = i * ana_flen;
      n_right = n_ptr + ana_wlen;
    }
    else if (win_align_d == WAL_RIGHT) {
      n_ptr = (i + 1) * ana_flen - ana_wlen;
      n_right = (i + 1) * ana_flen;
    }
      
    // make sure sig_wbuf is cleared
    //
    if ((n_ptr < 0) || (n_right >= nsamp)) {
      for (long j = 0; j < ana_wlen; j++) {
	sig_wbuf[j] = 0.0;
      }
    }
      
    // transfer the data paying attention to file boundaries
    //
    for (long j = 0; j < ana_wlen; j++) {
      if ((n_ptr >= 0) && (n_ptr < nsamp)) {
	sig_wbuf[j] = isig_a[n_ptr];
      }
	
      // increment counter
      //
      n_ptr++;
    }

    // debias the data
    //
    if (debias_mode_d == DBS_WINDOW) {
      double window_avg = Mplpc::debias(sig_wbuf);
      if ((debug_level_d >= Dbgl::LEVEL_DETAILED) && (i < Edf::DEF_DBG_NF)) {
    	fprintf(stdout, "%s = [%ld, %f]\n",
    		"Mplpc::compute_mplpc(): (frame/window_avg)",
    		n_center, window_avg);
      }
    }

    // window the data
    //
    if (win_type_d != WTYP_RECT) {
      for (long j = 0; j < ana_wlen; j++) {
    	sig_wbuf[j] = win_fct_d[j] * sig_wbuf[j];
      }
    }
  }

  // exit gracefully
  //
  return status;
}

//
// end of file
