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
  vptrs_d[i++] = (void*)&(algo_mode_str_d);
  vptrs_d[i++] = (void*)&(frame_mode_str_d);
  vptrs_d[i++] = (void*)&(frame_duration_d);
  vptrs_d[i++] = (void*)&(window_duration_d);
  vptrs_d[i++] = (void*)&(win_type_str_d);
  vptrs_d[i++] = (void*)&(win_norm_str_d);
  vptrs_d[i++] = (void*)&(win_align_str_d);
  vptrs_d[i++] = (void*)&(debias_mode_str_d);

  vptrs_d[i++] = (void*)&(lp_order_d);
  vptrs_d[i++] = (void*)&(num_pulses_d);

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
  strcpy(algo_mode_str_d, DEF_ALGO_MODE_NAME);
  strcpy(frame_mode_str_d, DEF_FRAME_MODE_NAME);
  frame_duration_d = DEF_FRAME_DURATION;
  window_duration_d = DEF_WINDOW_DURATION;
  strcpy(win_type_str_d, DEF_WINDOW_TYPE_NAME);
  strcpy(win_norm_str_d, DEF_WINDOW_NORM_NAME);
  strcpy(win_align_str_d, DEF_WINDOW_ALIGN_NAME);

  strcpy(debias_mode_str_d, DEF_DEBIAS_MODE_NAME);

  lp_order_d = DEF_LP_ORDER;
  num_pulses_d = DEF_NUM_PULSES;

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
  "algorithm_mode",
  "frame_mode",
  "frame_duration",
  "window_duration",
  "window_name",
  "window_norm",
  "window_alignment",
  "debias_mode",

  "lp_order",
  "num_pulses",

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
  "string",		// algorithm name: algo_name_d
  "string",		// frame mode: frame_mode_d
  "double",		// frame duration: frame_duration_d
  "double",		// window duration: window_duration_d
  "string",		// window type: windtype_d
  "string",		// window norm: windnorm_d
  "string",		// window norm: windalign_d
  "string",		// debais mode: debias_mode_d

  "long",		// lp order: lp_order_d
  "long",		// num_pulses: num_pulses_d

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
const char* Mplpc::DEF_TSIG_NAME("");
const char* Mplpc::DEF_ALGO_MODE_NAME(Mplpc::ALGO_MODE_NAME_00);
const char* Mplpc::DEF_FRAME_MODE_NAME(Mplpc::FRAME_MODE_NAME_00);
const char* Mplpc::DEF_DEBIAS_MODE_NAME(Mplpc::DEBIAS_MODE_NAME_00);
const char* Mplpc::DEF_WINDOW_TYPE_NAME(Mplpc::WINDOW_NAME_00);
const char* Mplpc::DEF_WINDOW_NORM_NAME(Mplpc::WINDOW_NORM_NAME_00);
const char* Mplpc::DEF_WINDOW_ALIGN_NAME(Mplpc::WINDOW_ALIGN_NAME_00);

//-----------------------------------------------------------------------------
//
// constants: define the actual choices users have for algo names
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

// constants: algorithm names
//
const char* Mplpc::ALGO_MODE_NAME_00("sampled_data");
const char* Mplpc::ALGO_MODE_NAME_01("features");

// constants: frame modes
//
const char* Mplpc::FRAME_MODE_NAME_00("floor");
const char* Mplpc::FRAME_MODE_NAME_01("truncate");

// constants: window modes
//
const char* Mplpc::WINDOW_NAME_00("rectangular");
const char* Mplpc::WINDOW_NAME_01("hamming");
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

// frame-related constants
//
double Mplpc::DEF_FRAME_DURATION = 1.0;
double Mplpc::DEF_WINDOW_DURATION = 2.0;

// window-related parameters
//
double Mplpc::DEF_WIN_HAMM_ALPHA = 0.54;

// mplpc-related parameters
//
long Mplpc::DEF_LP_ORDER = 16;
long Mplpc::DEF_NUM_PULSES = 2;

//
// end of file
