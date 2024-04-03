// make sure definitions are only made once
//
#ifndef RUN_MPLPC
#define RUN_MPLPC

// local include files
//
#ifndef RUN_EDF
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
  enum ALGO_MODE {ALGOMODE_SAMP = 0, ALGOMODE_FEAT,
		  DEF_ALGO_MODE = ALGOMODE_FEAT};

  enum FRAME_MODE {FRMMODE_FLOOR = 0, FRMMODE_TRUNCATE,
		   DEF_FRAME_MODE = FRMMODE_FLOOR};

  enum WINDOW_TYPE {WTYP_RECT = 0, WTYP_HAMMING,
		    DEF_WINDOW_TYPE = WTYP_RECT};

  enum WINDOW_NORM {WNRM_NONE = 0, WNRM_AMPL,
		    DEF_WINDOW_NORM = WNRM_NONE};

  enum WINDOW_ALIGN {WAL_CENTER = 0, WAL_LEFT, WAL_RIGHT,
		     DEF_WINDOW_ALIGN = WAL_CENTER};

  enum DEBIAS_MODE {DBS_NONE = 0, DBS_SIGNAL, DBS_WINDOW,
		    DEF_DEBIAS_MODE = DBS_NONE};

  //###########################################################################
  //
  // protected default values
  //
  //###########################################################################
protected:

  // algorithm name-related parameters
  //
  static const char* DEF_SELMODE_NAME;
  static const char* DEF_MATMODE_NAME;
  static const char* DEF_TSIG_NAME;
  static const char* DEF_ALGO_MODE_NAME;
  static const char* DEF_FRAME_MODE_NAME;
  static const char* DEF_WINDOW_TYPE_NAME;
  static const char* DEF_WINDOW_NORM_NAME;
  static const char* DEF_WINDOW_ALIGN_NAME;
  static const char* DEF_DEBIAS_MODE_NAME;

  // frame-related constants
  //
  static double DEF_FRAME_DURATION;
  static double DEF_WINDOW_DURATION;

  // window-related parameters
  //
  static double DEF_WIN_HAMM_ALPHA;

  // mplpc-related parameters
  //
  static long DEF_LP_ORDER;
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

  // algorithm name-related parameters
  //
  static const char* ALGO_MODE_NAME_00;
  static const char* ALGO_MODE_NAME_01;

  // frame mode name-related parameters
  //
  static const char* FRAME_MODE_NAME_00;
  static const char* FRAME_MODE_NAME_01;

  // window-related parameters
  //
  static const char* WINDOW_NAME_00;
  static const char* WINDOW_NAME_01;
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

  // define channel processing parameters
  //
  char cselect_d[Edf::MAX_LSTR_LENGTH];          // channel selection string
  char select_mode_str_d[Edf::MAX_LSTR_LENGTH];  // selection mode string
  char match_mode_str_d[Edf::MAX_SSTR_LENGTH];   // matching mode string
  char* montage_d[Edf::MAX_LSTR_LENGTH];         // montage processing
  static const long pos_montage = 6;             // montage string in vnames
  long num_montage_d;                            // number of montage channels

  //----------------------------------------
  //
  // section 2: variables related to signal processing
  //
  //----------------------------------------

  // define algorithm name-related parameters
  //
  char algo_mode_str_d[Edf::MAX_MSTR_LENGTH];   // algorithm mode

  // define interpolation name-related parameters
  //
  long num_new_chan_d;

  // define algorithm-related parameters
  //
  char frame_mode_str_d[Edf::MAX_MSTR_LENGTH];  // frame mode (e.g., truncate)
  double frame_duration_d;                      // frame duration in secs

  double window_duration_d;                     // window duration in secs
  char win_type_str_d[Edf::MAX_SSTR_LENGTH];    // window type
  char win_norm_str_d[Edf::MAX_SSTR_LENGTH];    // window normalization
  char win_align_str_d[Edf::MAX_SSTR_LENGTH];   // window alignment
  char debias_mode_str_d[Edf::MAX_SSTR_LENGTH]; // debias mode

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
  ALGO_MODE algo_mode_d;                        // algorithm mode
  Edf::SELECT_MODE select_mode_d;               // selection mode
  Edf::MATCH_MODE match_mode_d;                 // matching mode

  // define parameters related to handling EDF headers
  //
  Edf edf_d;

  // define additional buffers for signals and features
  //
  VectorDouble win_fct_d;

  //----------------------------------------
  //
  // section 2: variables related to signal processing
  //
  //----------------------------------------

  // define additional parameters and buffers for signal processing
  //
  long num_chan_file_d;                       // numbers of channels in file
  long num_chan_proc_d;                       // numbers of channels processed
  double sample_freq_d;                       // signal sample frequency

  // define additional frame-related parameters
  //
  FRAME_MODE frame_mode_d;		      // frame mode

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
  long lp_order_d;	                      // linear prediction order
  long num_pulses_d;                          // number of pulses per frame

  //----------------------------------------
  //
  // section 3: variables related to feature file format generation
  //
  //----------------------------------------

  // define feature file format-related parameters
  //
  Edf::FFMT ffmt_d;                           // feature file format

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

  // check montage method (mplpc_01)
  //
  bool load_montage(long& nchannels,
		    char** channels,
		    Edf::MATCH_MODE& matmode);

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
  bool compute_mplpc(VVectorDouble& osig, VVectorDouble& isig);
  bool compute_mplpc(VectorDouble& osig, VectorDouble& isig);
  
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
  long compute_num_frames(long nsamp, long flen, long wlen);
  bool create_window(long wlen);
  double debias(VectorDouble& sig);

  // signal processing functions (mplpc_02): algorithms
  //
  bool compute_00_select(VVectorDouble& sig, char* iname);
  bool compute_00_remove(VVectorDouble& sig, char* iname);

  bool compute_01_frame(VVVectorDouble& feat, char* iname);

  bool compute_02_frame(VVVectorDouble& feat);
  bool compute_03_frame(VVVectorDouble& feat);
  bool compute_04_frame(VVVectorDouble& feat, long feat_num);

  // signal procssing functions (mplpc_03): initialization
  //

  // signal procssing functions (mplpc_04): computation
  //
  // bool convert_to_dct(VectorDouble& out, VectorDouble& in);

  //----------------------------------------
  //
  // section 4: addtional general purpose methods (mplpc_05)
  //
  //----------------------------------------

  // math and vector functions (mplpc_05): general
  //
  bool trim_zeroes(long& i1, long& i2, VectorDouble& val);

  //
  // end of class
};

// end of include file
//
#endif
