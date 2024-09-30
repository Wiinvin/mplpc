// This file contains basic methods such as constructors and destructors.

// local include files
//
#include "Mplpc.h"

//-----------------------------------------------------------------------------
//
// basic required methods
//
//-----------------------------------------------------------------------------

// method: default constructor
//
// arguments: none
//  
// return: none
//
// This method implements the detaul constructor for the Mplpc class.
//
Mplpc::Mplpc() {

  // display debugging information
  //
  if (debug_level_d >= Dbgl::LEVEL_FULL) {
    fprintf(stdout, "Mplpc(): initalizing an Mplpc object\n");
  }

  // declare local variables
  //
  long i = 0;

  //---------------------------------------------------------------------------
  //
  // define variable pointers:
  //  note that the order here is very important since every parameter
  //  that appears in a parameter file must have a variable here,
  //  and these variables must be listed in EXACTLY the same order
  //  as the names and types that appear at the end of this file.
  //
  //---------------------------------------------------------------------------

  // section 1: filename processing
  //
  vptrs_d[i++] = (void*)&(vers_d);
  vptrs_d[i++] = (void*)&(cselect_d);
  vptrs_d[i++] = (void*)&(select_mode_str_d);
  vptrs_d[i++] = (void*)&(match_mode_str_d);
  vptrs_d[i++] = (void*)&(montage_d);

  // section 2: signal processing
  //
  vptrs_d[i++] = (void*)&(sample_freq_d);
  vptrs_d[i++] = (void*)&(frame_duration_d);
  vptrs_d[i++] = (void*)&(window_duration_d);
  vptrs_d[i++] = (void*)&(win_type_str_d);
  vptrs_d[i++] = (void*)&(win_norm_str_d);
  vptrs_d[i++] = (void*)&(win_align_str_d);
  vptrs_d[i++] = (void*)&(debias_mode_str_d);

  vptrs_d[i++] = (void*)&(preemphasis_d);
  vptrs_d[i++] = (void*)&(lp_order_d);
  vptrs_d[i++] = (void*)&(impres_dur_d);
  vptrs_d[i++] = (void*)&(num_pulses_d);
  vptrs_d[i++] = (void*)&(feat_type_d);

  //vptrs_d[i++] = (void*)&(algo_mode_str_d);

  // section 3: feature file generation
  //
  vptrs_d[i++] = (void*)&(ffmt_str_d);
  vptrs_d[i++] = (void*)&(odir_d);
  vptrs_d[i++] = (void*)&(odir_repl_d);
  vptrs_d[i++] = (void*)&(oext_d);

  //---------------------------------------------------------------------------
  //
  // initialize parameters used for parameter file-related parameters
  //
  //---------------------------------------------------------------------------

  // section 1: filename processing
  //
  vers_d[0] = (char)NULL;

  fname_d[0] = (char)NULL;

  cselect_d[0] = (char)NULL;  
  select_mode_d = Edf::DEF_SELECT_MODE;
  match_mode_d = Edf::DEF_MATCH_MODE;
  montage_d[0] = (char*)NULL;

  // section 2: signal processing
  //
  sample_freq_d = DEF_SAMPLE_FREQUENCY;
  frame_duration_d = DEF_FRAME_DURATION;
  window_duration_d = DEF_WINDOW_DURATION;
  strcpy(win_type_str_d, DEF_WINDOW_TYPE_NAME);
  strcpy(win_norm_str_d, DEF_WINDOW_NORM_NAME);
  strcpy(win_align_str_d, DEF_WINDOW_ALIGN_NAME);

  strcpy(debias_mode_str_d, DEF_DEBIAS_MODE_NAME);

  preemphasis_d = DEF_PREEMPHASIS;
  lp_order_d = DEF_LP_ORDER;
  num_pulses_d = DEF_NUM_PULSES;
  feat_type_d = DEF_FEAT_TYPE;
  
  // section 3: feature file generation
  //
  ffmt_str_d[0] = (char)NULL;
  odir_d[0] = (char)NULL;
  odir_repl_d[0] = (char)NULL;
  oext_d[0] = (char)NULL;

  //---------------------------------------------------------------------------
  //
  // initialize additional parameters
  //
  //---------------------------------------------------------------------------

  // section 1: filename processing
  //
  num_chan_file_d = -1;
  num_chan_proc_d = -1;
  sample_freq_d = -1;

  // section 2: signal processing
  //

  // section 3: output file generation
  //
  strcpy(ffmt_str_d, Edf::DEF_FFMT_NAME);

  //---------------------------------------------------------------------------
  //
  // exit gracefully
  //
  //---------------------------------------------------------------------------

  if (debug_level_d >= Dbgl::LEVEL_FULL) {
    fprintf(stdout, "Mplpc(): done initalizing an Mplpc object\n");
  }
};

// method: destructor
//
// arguments: none
//
// return: none
//
// This method implements the destructor.
//
Mplpc::~Mplpc() {

  // display debugging information
  //
  if (debug_level_d >= Dbgl::LEVEL_FULL) {
    fprintf(stdout, "~Mplpc(): destroying the object\n");
  }
}

//#############################################################################
//
// we define non-integral constants in the default constructor
//
//#############################################################################

// constants: class name and version
//
const char* Mplpc::CLASS_NAME("Mplpc");
const float Mplpc::MAX_VALUE = (float)32767.0;
const char* Mplpc::VERSION("1.0");

// constants: variable names appearing in the parameter file:
//  note this order must match vptrs above
//
const char* Mplpc::vnames_d[Mplpc::MAX_NVN] = {

  // section 1: filename processing
  //
  "version",

  "channel_selection",
  "select_mode",
  "match_mode",
  "montage",

  // section 2: signal processing
  //
  "sample_frequency",
  "frame_duration",
  "window_duration",
  "window_type",
  "window_norm",
  "window_alignment",
  "debias_mode",

  "preemphasis",
  "lp_order",
  "impulse_response_duration",
  "num_pulses",
  "feat_type",
  
  // section 3: output file generation
  //
  "output_format",
  "output_directory",
  "output_replace",
  "output_extension",
};

// constants: variable types for variables appearing in the parameter file:
//  note this order must match vptrs and vnames above
//
const char* Mplpc::vtypes_d[Mplpc::MAX_NVN] = {

  // section 1: filename processing
  //
  "string",		// version: vers_d

  "string",		// channel selection: cselect_d
  "string",		// select mode: select_mode_d
  "string",		// match mode: match_mode_d
  "string",		// montage: montage_d

  // section 2: signal processing
  //
  "float",		// sample frequency: sample_freq_d
  "float",		// frame duration: frame_duration_d
  "float",		// window duration: window_duration_d
  "string",		// window type: win_type_d
  "string",		// window norm: win_norm_d
  "string",		// window norm: win_align_d
  "string",		// debais mode: debias_mode_d

  "float",		// premphasis: preemphasis_d
  "long",		// lp order: lp_order_d
  "float",		// impulse response duration: impres_dur_d
  "long",		// num_pulses: num_pulses_d
  "string",             // feat_type: excitation, pc, rc
  
  // section 5: feature file generation
  //
  "string",		// output format: offmt_d
  "string",		// output directory: odir_d
  "string",             // output directory replace: odir_repl_d 
  "string",		// output extension: oext_d
};

//-----------------------------------------------------------------------------
//
// constants: default names for various algorithms
//
// These are names that define default values for variables associated with
// decoding the parameter file symbolic names
// that control signal processing specifications (section 2).
//
//-----------------------------------------------------------------------------

const char* Mplpc::DEF_SELMODE_NAME(Mplpc::SELMODE_NAME_00);
const char* Mplpc::DEF_MATMODE_NAME(Mplpc::MATMODE_NAME_00);
const char* Mplpc::DEF_DEBIAS_MODE_NAME(Mplpc::DEBIAS_MODE_NAME_00);
const char* Mplpc::DEF_FEAT_TYPE_NAME(Mplpc::FEAT_TYPE_NAME_00);
const char* Mplpc::DEF_WINDOW_TYPE_NAME(Mplpc::WINDOW_TYPE_NAME_00);
const char* Mplpc::DEF_WINDOW_NORM_NAME(Mplpc::WINDOW_NORM_NAME_00);
const char* Mplpc::DEF_WINDOW_ALIGN_NAME(Mplpc::WINDOW_ALIGN_NAME_00);

//-----------------------------------------------------------------------------
//
// constants: define the default values for key parameters
//
//-----------------------------------------------------------------------------

// constants: selection mode names
//
const char* Mplpc::SELMODE_NAME_00("select");
const char* Mplpc::SELMODE_NAME_01("remove");

// constants: match mode names
//
const char* Mplpc::MATMODE_NAME_00("exact");
const char* Mplpc::MATMODE_NAME_01("partial");

// frame-related constants
//
float Mplpc::DEF_SAMPLE_FREQUENCY = 8000.0;
float Mplpc::DEF_FRAME_DURATION = 1.0;
float Mplpc::DEF_WINDOW_DURATION = 2.0;

// constants: window modes
//
const char* Mplpc::WINDOW_TYPE_NAME_00("rectangular");
const char* Mplpc::WINDOW_TYPE_NAME_01("hamming");
const char* Mplpc::WINDOW_NORM_NAME_00("none");
const char* Mplpc::WINDOW_NORM_NAME_01("energy");
const char* Mplpc::WINDOW_ALIGN_NAME_00("center");
const char* Mplpc::WINDOW_ALIGN_NAME_01("left");
const char* Mplpc::WINDOW_ALIGN_NAME_02("right");

// constants: debias names
//
const char* Mplpc::DEBIAS_MODE_NAME_00("none");
const char* Mplpc::DEBIAS_MODE_NAME_01("signal");
const char* Mplpc::DEBIAS_MODE_NAME_02("window"); 

// constants: feature types
//
const char* Mplpc::FEAT_TYPE_NAME_00("mplpc");
const char* Mplpc::FEAT_TYPE_NAME_01("pc");
const char* Mplpc::FEAT_TYPE_NAME_02("rc");

// mplpc-related parameters
//
float Mplpc::DEF_PREEMPHASIS = 0.95;
float Mplpc::DEF_WIN_HAMM_ALPHA = 0.54;
long Mplpc::DEF_LP_ORDER = 16;
float Mplpc::DEF_IMPRES_DURATION = 0.01;
long Mplpc::DEF_NUM_PULSES = 2;

//
// end of file
