
// make sure definitions are only made once
//
#ifndef AUTOEEG_MPLPC
#define AUTOEEG_MPLPC

// local include files
//
#ifndef AUTOEEG_EDF
#include <Edf.h>
#endif

// Mplpc: a class that performs multipulse linear predictive coding (MPLPC)
// analysis.
//
class Mplpc {

  //###########################################################################
  //
  // public constants
  //
  //###########################################################################
public:

  // define the class name and version
  //
  static const char* CLASS_NAME;

  // define the max signal value
  //
  static const float MAX_VALUE;

  // define a debug level and verbosity:
  //
  Dbgl debug_level_d;
  Vrbl verbosity_d;

  //###########################################################################
  //
  // protected enumerations
  //
  //###########################################################################
protected:

  // enumerations related to parameter name parsing
  //
  enum WINDOW_TYPE {WTYP_RECT = 0, WTYP_HAMMING,
		    DEF_WINDOW_TYPE = WTYP_RECT};

  enum WINDOW_NORM {WNRM_NONE = 0, WNRM_EGY,
		    DEF_WINDOW_NORM = WNRM_NONE};

  enum WINDOW_ALIGN {WNAL_CENTER = 0, WNAL_LEFT, WNAL_RIGHT,
		     DEF_WINDOW_ALIGN = WNAL_CENTER};

  enum DEBIAS_MODE {DBS_NONE = 0, DBS_SIGNAL, DBS_WINDOW,
		    DEF_DEBIAS_MODE = DBS_NONE};

  enum FEAT_TYPE {FEAT_EXCIT = 0, FEAT_PC, FEAT_RC,
		  DEF_FEAT_TYPE = FEAT_EXCIT};
  
  //###########################################################################
  //
  // protected default values
  //
  //###########################################################################
protected:
  // edf file processing related parameters
  //
  static const char* DEF_SELMODE_NAME;
  static const char* DEF_MATMODE_NAME;

  // algorithm name-related parameters
  //
  static const char* DEF_WINDOW_TYPE_NAME;
  static const char* DEF_WINDOW_NORM_NAME;
  static const char* DEF_WINDOW_ALIGN_NAME;
  static const char* DEF_DEBIAS_MODE_NAME;
  static const char* DEF_FEAT_TYPE_NAME;

  // frame-related constants
  //
  static float DEF_SAMPLE_FREQUENCY;
  static float DEF_FRAME_DURATION;
  static float DEF_WINDOW_DURATION;

  // mplpc-related parameters
  //
  static float DEF_PREEMPHASIS;
  static float DEF_WIN_HAMM_ALPHA;
  static long DEF_LP_ORDER;
  static float DEF_IMPRES_DURATION;
  static long DEF_NUM_PULSES;

  
  //###########################################################################
  //
  // protected constants: constants related to externally visible
  //                      parameter names (e.g., parameter file parsing)
  //
  //###########################################################################
protected:

  //----------------------------------------
  //
  // section 1: parameters related to filename parsing
  //
  //----------------------------------------

  // versioning information
  //
  static const char* VERSION;

  // selection mode name-related parameters
  //
  static const char* SELMODE_NAME_00;
  static const char* SELMODE_NAME_01;

  // match mode name-related parameters
  //
  static const char* MATMODE_NAME_00;
  static const char* MATMODE_NAME_01;

  //----------------------------------------
  //
  // section 2: parameters related to signal processing
  //
  //----------------------------------------

  // window-related parameters
  //
  static const char* WINDOW_TYPE_NAME_00;
  static const char* WINDOW_TYPE_NAME_01;
  static const char* WINDOW_NORM_NAME_00;
  static const char* WINDOW_NORM_NAME_01;
  static const char* WINDOW_ALIGN_NAME_00;
  static const char* WINDOW_ALIGN_NAME_01;
  static const char* WINDOW_ALIGN_NAME_02;
  
  // debias-related parameters
  //
  static const char* DEBIAS_MODE_NAME_00;
  static const char* DEBIAS_MODE_NAME_01;
  static const char* DEBIAS_MODE_NAME_02;  

  //----------------------------------------
  //
  // section 3: parameters related to feature file generation
  //
  //----------------------------------------
  static const char* FEAT_TYPE_NAME_00;
  static const char* FEAT_TYPE_NAME_01;
  static const char* FEAT_TYPE_NAME_02;  

  //----------------------------------------
  //
  // section 4: addtional constants
  //
  //----------------------------------------

  //###########################################################################
  //
  // protected data: parameters that appear in the parameter file
  //
  //###########################################################################
protected:

  //----------------------------------------
  //
  // section 1: variables related to file processing
  //
  //----------------------------------------

  // parameter file information
  //
  char fname_d[Edf::MAX_LSTR_LENGTH];           // parameter file name
  char vers_d[Edf::MAX_SSTR_LENGTH];            // param file version number
  char debug_str_d[Edf::MAX_SSTR_LENGTH]; 	// debug level as a string
  char verbosity_str_d[Edf::MAX_SSTR_LENGTH]; 	// verbosity level as a strig


  // define channel processing parameters
  //
  char cselect_d[Edf::MAX_LSTR_LENGTH];          // channel selection string
  char select_mode_str_d[Edf::MAX_LSTR_LENGTH];  // selection mode string
  char match_mode_str_d[Edf::MAX_SSTR_LENGTH];   // matching mode string
  char* montage_d[Edf::MAX_LSTR_LENGTH];         // montage processing
  static const long pos_montage = 4;             // montage string in vnames // check if this is a valid way to write this. editedV
  long num_montage_d;                            // number of montage channels

  //----------------------------------------
  //
  // section 2: variables related to signal processing
  //
  //----------------------------------------

  // define algorithm-related parameters
  //
  char win_type_str_d[Edf::MAX_SSTR_LENGTH];    // window type
  char win_norm_str_d[Edf::MAX_SSTR_LENGTH];    // window normalization
  char win_align_str_d[Edf::MAX_SSTR_LENGTH];   // window alignment
  char debias_mode_str_d[Edf::MAX_SSTR_LENGTH]; // debias mode
  char feat_type_str_d[Edf::MAX_SSTR_LENGTH]; // feature type

  //----------------------------------------
  //
  // section 3: output file parameters
  //
  //----------------------------------------

  // output filename/directory parameters
  //
  char ffmt_str_d[Edf::MAX_SSTR_LENGTH];        // file format
  char odir_d[Edf::MAX_LSTR_LENGTH];            // feat file output directory
  char odir_repl_d[Edf::MAX_LSTR_LENGTH];       // feat file replace directory
  char oext_d[Edf::MAX_SSTR_LENGTH];            // feat file output extension

  //###########################################################################
  //
  // protected data: additional variables used by methods to implement
  //                 various algorithms and features
  //
  //###########################################################################
protected:

  //----------------------------------------
  //
  // section 1: variables related to file processing
  //
  //----------------------------------------

  // define an array to hold variable nams, types and points:
  //  note that vptrs cannot be static so that multiple versions
  //  of the class can be instantiated.
  //
  static const long MAX_NVN = 99;               // max no. of parameter names
  static const char* vnames_d[MAX_NVN];         // exposed parameter names
  static const char* vtypes_d[MAX_NVN];         // exposed parameter types
  void* vptrs_d[MAX_NVN];                       // exposed parameter pointers

  // define additional parameters for signal and channel selection modes
  //
  Edf::SELECT_MODE select_mode_d;               // selection mode
  Edf::MATCH_MODE match_mode_d;                 // matching mode

  // define parameters related to handling EDF headers
  //
  Edf edf_d;

  //----------------------------------------
  //
  // section 2: variables related to signal processing
  //
  //----------------------------------------

  // define additional parameters and buffers for signal processing
  //
  long num_chan_file_d;                       // numbers of channels in file
  long num_chan_proc_d;                       // numbers of channels processed
  float sample_freq_d;                         // signal sample frequency

  float frame_duration_d;                      // frame duration in secs
  float window_duration_d;                     // window duration in secs

  // define additional window-related parameters
  //
  WINDOW_TYPE win_type_d;		      // window type
  WINDOW_NORM win_norm_d;		      // window normalization
  WINDOW_ALIGN win_align_d;                   // window alignment

  // define debias-related parameters
  //
  DEBIAS_MODE debias_mode_d;                  // debias mode

  // define additional parameters for mplpc
  //
  float preemphasis_d;			      // preemphasis
  long lp_order_d;	                      // linear prediction order
  float impres_dur_d;			      // lpc impulse response duration
  long num_pulses_d;                          // number of pulses per frame
  FEAT_TYPE feat_type_d;                      // type of features to generate
  
  // editedV
  // variables for fft plotting (for method stolen from Fe class)
  long fft_n_d;
  long fft_m_d;
  double fft_q_d;
  VectorDouble fft_wd_real_d;
  VectorDouble fft_wd_imag_d;
  VectorDouble fft_tempd_d;
  VectorDouble fft_tempc_d;

  // end of editedV
  //
  
  //----------------------------------------
  //
  // section 3: variables related to output file format generation
  //
  //----------------------------------------

  //----------------------------------------
  //
  // section 4: internal variables used in signal processing
  //
  //----------------------------------------

  // define a vector for the window function
  //
  VectorDouble win_fct_d;

  //###########################################################################
  //
  // required public methods (mplpc_00)
  //
  //###########################################################################
public:

  // method: name
  //
  static const char* name() {
    return CLASS_NAME;
  }

  // method: destructor
  //
  ~Mplpc();

  // method: default constructor
  //
  Mplpc();

  //---------------------------------------------------------------------------
  //
  // other public methods
  //
  //---------------------------------------------------------------------------
public:

  // parameter file methods (mplpc_01)
  //
  bool load_parameters(char* fname);
  bool print_parameters(FILE* fp = stdout);

  // parameter file methods (mplpc_01)
  //
  bool set_output_directory(char* arg) {
    strcpy(odir_d, arg);
    return true;
  }

  // parameter file methods (mplpc_01)
  //
  bool set_repl_directory(char* arg) {
    strcpy(odir_repl_d, arg);
    return true;
  }

  // computational methods (mplpc_02)
  //
  bool compute(char* oname, char* iname);
  bool compute_00(VVVectorDouble& isig, char* iname);

  // might need to revise
  //
  bool compute_00_edf(VVVectorDouble& isig, char* iname);

  bool compute_mplpc(VVVectorDouble& osig, VVectorDouble& isig);
  bool compute_mplpc(VVectorDouble& osig, VectorDouble& isig);



  //----------------------------------------
  //
  // section 2: helper functions for regression analysis, unit tests and debugging
  //
  //----------------------------------------

  // editedV
  // prints out boost arrays
  bool helper_print_arr(VectorDouble& arr, long st_i = 0, long end_i = 0); 
  bool helper_print_arr(VVectorDouble& arr, long st_i = 0, long end_i = 0);
  bool helper_print_arr(VVVectorDouble& arr, long st_i = 0, long end_i = 0);
  bool helper_load_1d_dat_tmpf(std::string fname_str_a, VectorDouble& arr_a, std::string ext_a, long ind_lim = 0);
  bool helper_load_2d_dat_tmpf(std::string fname_str_a, VectorDouble& xarr_a, VectorDouble& yarr_a, std::string ext_a, long ind_lim = 0);
  bool helper_convert_to_fftpow(VectorDouble& osig, VectorDouble& isig);
  bool helper_normalize_fftpow(VectorDouble& xfft_vec, VectorDouble& yfft_vec, VectorDouble& fft_arr_a, int siglen_a, int fft_order, double samprate_a = double(1.0));
  bool helper_init_fftpow(long fft_order_a);
  bool helper_est_roots(VectorDouble& roots, VectorDouble& pc);
  std::string& helper_char2str(const char* s); // gdb can't create strings from
  // the char arr pointers interactively. Using this method strings can be passed
  // to the methods


  
  //###########################################################################
  //
  // private methods
  //
  //###########################################################################
private:

  //----------------------------------------
  //
  // section 1: methods related to file processing
  //
  //----------------------------------------

  bool convert_to_enums();
  bool parse_param(long& nl_a, char** labels_a, char* str_a, char* delim);

  //----------------------------------------
  //
  // section 2: methods related to signal processing
  //
  //----------------------------------------

  // signal processing functions (mplpc_03): framing, windowing, debiasing
  //
  short int clip_value(float value);
  bool create_window();
  double debias(VectorDouble& sig);
  bool compute_autocor(VectorDouble& autocor, VectorDouble& sig,
		       long lp_order);
  bool compute_lpc(VectorDouble& rc, VectorDouble& pc,
		   VectorDouble& autocor, long lp_order);
  bool compute_residual(VectorDouble& osig, VectorDouble& isig,
			VectorDouble& pc, long idx, long n_fdur);
  bool compute_impulse_response(VectorDouble& h, VectorDouble& pc,
				long num_samples);
  
  //
  // end of class
};

// end of include file
//
#endif
