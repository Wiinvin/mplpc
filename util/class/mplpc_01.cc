// local include files
//
#include "Mplpc.h"

// method: load_parameters
//
// arguments:
//  char* fname: parameter filename (input)
//
// return: a boolean indicating status
//
// This method loads a parameter file.
//
bool Mplpc::load_parameters(char* fname_a) {

  // declare local variables
  //
  bool status;
  char tmp_line[Edf::MAX_LINE_LENGTH];
  char tmp_name[Edf::MAX_SSTR_LENGTH];
  char tmp_value[Edf::MAX_LSTR_LENGTH];
  char tmp_str[Edf::MAX_LSTR_LENGTH];
  long i = 0;
  long j = 0;
  
  // open the file
  //
  strcpy(fname_d, fname_a);
  FILE* fp = fopen(fname_d, "r");
  if (fp == NULL) {
    return false;
  }

  // loop over all lines in the file
  //
  while (fgets(tmp_line, Edf::MAX_LSTR_LENGTH-1, fp) != (char*)NULL) {

    // trim off the trailing line feed
    //
    tmp_line[strlen(tmp_line) - 1] = (char)NULL;

    // ignore a comment character or a blank line
    //
    tmp_str[0] = (char)NULL;
    if (tmp_line[0] == Edf::COMMENT[0]) {
      continue;
    }
    else if (sscanf(tmp_line, "%s", tmp_str) <= 0) {
      continue;
    }

    // split non-blank lines into two pieces
    //
    if (!edf_d.get_nvp(tmp_name, tmp_value, tmp_line)) {
      return false;
    }

    // look up the variable name in the list
    //
    long pos = edf_d.find_vname(tmp_name, (char**)vnames_d);

    if (pos < 0) {
      fprintf(stdout,
	      "**> error in Mplpc::load_parameters(): variable name [%s] "
	      "not found\n",
	      tmp_name);
      return false;
    }

    if (strcmp(tmp_name, vnames_d[pos_montage])== 0){
        long l = strlen(tmp_value);
	montage_d [j] = new char[l+1];
	strcpy(montage_d[j], tmp_value);
	montage_d[j][l] = (char) NULL;
	j++;
    }

    // set the variable
    //
    else {
      if (!edf_d.set_var(pos, tmp_value, (char**)vtypes_d, (void**)vptrs_d)) {
	fprintf(stdout,
		"**> error in Mplpc::load_parameters(): variable [%s] not set\n",
		tmp_name);
	return false;
      }
    }
  }

  num_new_chan_d = i;
  num_montage_d = j;
  
  // convert debug level and verbosity from strings to numeric values
  //
  if(!edf_d.set_montage_chans(num_montage_d)){
    fprintf(stdout,
	    "**> error in Mplpc::load_parameters(): no montage channels\n");
    return false;
  }
    
  // convert enums
  //
  if (!Mplpc::convert_to_enums()) {
    fprintf(stdout,
	    "**> error in Fe::load_parameters(): invalid enumeration\n");
    return false;
  }

  // display debugging information
  //
  if ((verbosity_d >= Vrbl::LEVEL_DETAILED) ||
      (debug_level_d >= Dbgl::LEVEL_FULL)) {
    Mplpc::print_parameters(stdout);
  }

  // make sure this is the correct version
  //
  if (strcmp(vers_d, VERSION) != 0) {
    fprintf(stdout,
	    "**> error in Fe::load_parameters(): incompatible versions "
	    "[c: %s] [f: %s]\n",
	    VERSION, vers_d);
    return false;
  }

  // close the parameter file
  //
  fclose(fp);

  // exit gracefully
  //
  return true;
}

// method: load_montage
//
// arguments:
//  long& nchannels: the number of channels (output)
//  char** channels: the array containing all the channels specified (output)
//  Edf::MATCH_MODE& match_mode: match mode specified by
//                               the parameter file (output) 
//
// return: a boolean indicating status
//
// This method loads the unique montage channels specified by the
// parameter file.
//
// Note also that the match_mode is loaded as well.
//
bool Mplpc::load_montage(long& nchannels_a,
			 char** channels_a,
			 Edf::MATCH_MODE& match_mode_a) {

  // get the channels specified in montage :
  //  it takes montage string, parse it, and load the channels
  //  specified with no duplications.
  // 
  if (!edf_d.get_montage_channels(nchannels_a, channels_a)) {
    fprintf(stdout, "**> load_montage: error parsing the montage string \n");
    return false;
  }

  // return match_mode:
  //  the match_mode is used to check the montage file and
  //  the label of the channels. Hence, the match_mode needs to
  //  be used once the montage will be checked regarding the
  //  match mode selected.
  //
  match_mode_a = match_mode_d;
  
  // exit gracefully
  //
  return true;
}

// method: print_parameters
//
// arguments: 
//  FILE* fp: stream to receive the output (input)
//
// return: a boolean indicating status
//
// This method dumps the internal parameters of the Fe class.
//
bool Mplpc::print_parameters(FILE* fp_a) {

  // display debug information
  //
  if (debug_level_d >= Dbgl::LEVEL_FULL) {
    fprintf(stdout, "\n<--- parameter parsing data structures --->\n");
    long i = 0;
    while (vnames_d[i] != (char*)NULL) {
      fprintf(fp_a, "%ld: [%s] [%s] [%lu]\n",
	      i, vnames_d[i], vtypes_d[i], (long)vptrs_d[i]);
      
      i++;
    }
    fprintf(stdout, "<--- end of parameter parsing data structures --->\n\n");
  }

  // dump the parameter file stuff
  //
  fprintf(fp_a, "\n<--- parameter file data --->\n");
  fprintf(fp_a, "\nsection 1:\n");
  fprintf(fp_a, " version = [%s]\n", vers_d);

  fprintf(fp_a, " parameter filename:: [%s]\n", fname_d);

  // dump the feature filename stuff
  //
  fprintf(fp_a, " debug_level = [%s] [%ld] \n",
	  debug_level_str_d, debug_level_d);
  fprintf(fp_a, " verbosity = [%s] [%ld]\n", verbosity_str_d, verbosity_d);

  // dump the channel processing parameters
  //
  fprintf(fp_a, " cselect_d = [%s]\n", cselect_d);
  fprintf(fp_a, " select_mode_d = [%s] [%ld]\n",
	  select_mode_str_d, (long)select_mode_d);
  fprintf(fp_a, " match_mode_d = [%s] [%ld]\n",
	  match_mode_str_d, (long)select_mode_d);
  for(long i = 0; i < num_montage_d; i++){  
    fprintf(fp_a, " montage_d = [%s]\n", montage_d[i]);
  }

  // dump the signal processing parameters
  //
  fprintf(fp_a, "\nsection 2:\n");  
  fprintf(fp_a, " algo_mode = [%s] [%lu]\n",
	  algo_mode_str_d, (long)algo_mode_d);
  fprintf(fp_a, " frame_mode = [%s] [%lu]\n",
	  frame_mode_str_d, (long)frame_mode_d);

  // dump the signal processing parameters
  //
  fprintf(fp_a, " frame_duration = %f secs\n", frame_duration_d);
  fprintf(fp_a, " window_duration = %f secs\n", window_duration_d);
  fprintf(fp_a, " window_type = [%s] [%lu]\n",
	  win_type_str_d, (long)win_type_d);
  fprintf(fp_a, " window_norm = [%s] [%lu]\n", win_norm_str_d,
	  (long)win_norm_d);
  fprintf(fp_a, " window_alignment = [%s] [%lu]\n",
	  win_align_str_d, (long)win_align_d);
  fprintf(fp_a, " debias_mode = [%s] [%lu]\n",
	  debias_mode_str_d, (long)debias_mode_d);

  // dump the mplpc-related parameters
  //
  fprintf(fp_a, " lp_order = [%lu]\n", lp_order_d);
  fprintf(fp_a, " num_pulses = [%lu]\n", num_pulses_d);

  // dump the output file generation parameters
  //
  fprintf(fp_a, "\nsection 3:\n");  
  fprintf(fp_a, " output_directory = [%s]\n", odir_d);
  fprintf(fp_a, " output_replace = [%s]\n", odir_repl_d);
  fprintf(fp_a, " output_extension = [%s]\n", oext_d);

  // display debug information
  //
  fprintf(fp_a, "\n<--- end of parameter file data --->\n\n");

  // exit gracefully
  //
  return true;
}

// method: convert_to_enum
//
// arguments: none
//
// return: a logical value indicating status
//
// This method converts parameters set as text string to enums so that
// value-checking can be done quickly.
//
bool Mplpc::convert_to_enums() {

  // convert the selection mode
  //
  if (strcmp(cselect_d, Edf::NULL_NAME) == 0) {
    select_mode_d = Edf::DEF_SELECT_MODE;
  }
  else if (strcmp(select_mode_str_d, SELMODE_NAME_00) == 0) {
    select_mode_d = Edf::SELMODE_SELECT;
  }
  else if (strcmp(select_mode_str_d, SELMODE_NAME_01) == 0) {
    select_mode_d = Edf::SELMODE_REMOVE;
  }
   else {
    fprintf(stdout,
	   "**> error in Mplpc::convert_to_enums():invalid selection mode [%s]\n",
	    select_mode_str_d);
    return false;
  }

  // convert the match mode
  //
  if (strcmp(match_mode_str_d, MATMODE_NAME_00) == 0) {
    match_mode_d = Edf::MATMODE_EXACT;
  }
  else if (strcmp(match_mode_str_d, MATMODE_NAME_01) == 0) {
    match_mode_d = Edf::MATMODE_PARTIAL;
  }
  else {
    fprintf(stdout,
	    "**> error in Mplpc::convert_to_enums(): invalid match mode [%s]\n",
	    match_mode_str_d);
    return false;
  }
 
  // convert the algorithm mode
  //
  if (strcmp(algo_mode_str_d, ALGO_MODE_NAME_00) == 0) {
    algo_mode_d = ALGOMODE_SAMP;
  }
  else if (strcmp(algo_mode_str_d, ALGO_MODE_NAME_01) == 0) {
    algo_mode_d = ALGOMODE_FEAT;
  }
  
  else {
    fprintf(stdout,
	    "**> error in Mplpc::convert_to_enums():invalid algorithm mode [%s]\n"
	    , algo_mode_str_d);
    return false;
  }

  // convert the frame mode
  //
  if (strcmp(frame_mode_str_d, FRAME_MODE_NAME_00) == 0) {
    frame_mode_d = FRMMODE_FLOOR;
  }
  else if (strcmp(frame_mode_str_d, FRAME_MODE_NAME_01) == 0) {
    frame_mode_d = FRMMODE_TRUNCATE;
  }
  else {
    fprintf(stdout,
	    "**> error in Mplpc::convert_to_enums(): invalid frame mode [%s]\n",
	    frame_mode_str_d);
    return false;
  }

  // convert the window type
  //
  if (strcmp(win_type_str_d, WINDOW_NAME_00) == 0) {
    win_type_d = WTYP_RECT;
  }
  else if (strcmp(win_type_str_d, WINDOW_NAME_01) == 0) {
    win_type_d = WTYP_HAMMING;
  }
  else {
    fprintf(stdout,
	    "**> error in Mplpc::convert_to_enums(): invalid window type [%s]\n",
	    win_type_str_d);
    return false;
  }

  // convert the window normalization
  //
  if (strcmp(win_norm_str_d, WINDOW_NORM_NAME_00) == 0) {
    win_norm_d = WNRM_NONE;
  }
  else if (strcmp(win_norm_str_d, WINDOW_NORM_NAME_01) == 0) {
    win_norm_d = WNRM_AMPL;
  }
  else {
    fprintf(stdout,
	    "**> error in Mplpc::convert_to_enums(): invalid window normalizationnmode [%s]\n",
	    win_norm_str_d);
    return false;
  }

  // convert the window alignment
  //
  if (strcmp(win_align_str_d, WINDOW_ALIGN_NAME_00) == 0) {
    win_align_d = WAL_CENTER;
  }
  else if (strcmp(win_align_str_d, WINDOW_ALIGN_NAME_01) == 0) {
    win_align_d = WAL_LEFT;
  }
  else if (strcmp(win_align_str_d, WINDOW_ALIGN_NAME_02) == 0) {
    win_align_d = WAL_RIGHT;
  }
  else {
    fprintf(stdout,
	    "**> error in Mplpc::convert_to_enums(): invalid window alignment mode [%s]\n",
	    win_align_str_d);
    return false;
  }

  // convert the debias mode
  //
  if (strcmp(debias_mode_str_d, DEBIAS_MODE_NAME_00) == 0) {
    debias_mode_d = DBS_NONE;
  }
  else if (strcmp(debias_mode_str_d, DEBIAS_MODE_NAME_01) == 0) {
    debias_mode_d = DBS_SIGNAL;
  }
  else if (strcmp(debias_mode_str_d, DEBIAS_MODE_NAME_02) == 0) {
    debias_mode_d = DBS_WINDOW;
  }
  else {
    fprintf(stdout,
	    "**> error in Mplpc::convert_to_enums(): invalid debias mode [%s]\n",
	    debias_mode_str_d);
    return false;
  }

  // exit gracefully
  //
  return true;
}

// method: parse_param
//
// arguments:
//  long& nl: the number of labels (output)
//  char** labels: an array of strings containing the labels (output)
//  char* str: a string containing the label list (input)
//  char* delim: represents the string that is used as the delimiter
// return: a boolean indicating status
//
//
// Note that the character string array labels is assumed to be
// allocated, but each individual element is not defined.
//
bool Mplpc::parse_param(long& nl_a, char** labels_a,
			char* str_a, char* delim) {

  // declare local variables
  //
  char* tmp_ptr = str_a;
  char* str_end = str_a + strlen(str_a);
  
  // iterate over all characters
  //
  nl_a = 0;

  while (tmp_ptr < str_end) {

    // trim off leading whitespace
    //
    while (isspace(*tmp_ptr)) {
      tmp_ptr++;
    }

    // find the next comma
    //
    char* pos_c = strstr(tmp_ptr, delim);
   
    //  note that if one doesn't exist, it is the end of the line. in this
    //  case we go to the end of the line and then step back to the first
    //  non-whitespace.
    //
    if (pos_c == (char*)NULL) {
      pos_c = str_end - 1;
      while (isspace(*pos_c)) {
     	pos_c--;
      }

      // increment by one to be consistent with cases where we are
      // not at the end of the line
      //
      pos_c++;
    }
   
    // compute the length and create space
    //
    long label_len = pos_c - tmp_ptr;
    labels_a[nl_a] = new char [label_len + 1];

    // copy and uppercase the string
    //
    for (long i = 0; i < label_len; i++) {
      labels_a[nl_a][i] = toupper(*tmp_ptr++);
    }
    labels_a[nl_a][label_len] = (char)NULL;
    
    // increment counters
    //
    nl_a++;
    tmp_ptr++;
  }
  
  // exit gracefully
  //
  return true;
}

//
// end of file

