
// local include files
//
#include <Cmdl.h>
#include <Mplpc.h>

// define the help and usage messages
//
#define USAGE_MSG "$VFC/util/cpp/run_mplpc/run_mplpc.usage"
#define HELP_MSG "$VFC/util/cpp/run_mplpc/run_mplpc.help"

// main: driver program
//
// This is a driver program that reads EDF files and generates
// feature files.
//
int main(int argc, const char** argv) {

  // declare local variables
  //
  long status = 0;

  // create a Dbgl object for local debugging
  // (the level of this object is set by the cmdl object during parsing)
  //
  Dbgl dbgl;
  Vrbl vrbl;

  // create an Edf object
  //
  Edf edf;

  // create an MPLPC object
  //
  Mplpc mplpc;

  // initialize a Command Line object
  //
  Cmdl cmdl;
  cmdl.set_usage(USAGE_MSG);
  cmdl.set_help(HELP_MSG);

  // add options
  //
  char pfile[Cmdl::MAX_OPTVAL_SIZE];
  pfile[0] = (char)NULL;
  cmdl.add_option("-parameters", pfile);

  char out_dir[Cmdl::MAX_OPTVAL_SIZE];
  out_dir[0] = (char)NULL;
  cmdl.add_option("-odir", out_dir);

  char repl_dir[Cmdl::MAX_OPTVAL_SIZE];
  repl_dir[0] = (char)NULL;
  cmdl.add_option("-rdir", repl_dir);

  // branch on the status of parsing, checking for usage and help messages
  //
  if ((argc == 1) || (cmdl.parse(argc, argv) == false)) {
    cmdl.display_usage(stdout);
    return (status);
  }
  else if (cmdl.get_help_status() == true) {
    cmdl.display_help(stdout);
    return (status);
  }     
  
  // load the parameter file
  //
  if (!mplpc.load_parameters((char*)pfile)) {
    fprintf(stdout, "**> error parsing the parameter file (%s)\n",
	    (char*)pfile);
    exit(1);
  }
  mplpc.print_parameters(stdout);
  
  // allow the output directory to be overridden
  //
  if(out_dir[0] != (char)NULL){
    mplpc.set_output_directory(out_dir);
  }

  // allow the replace directory to be overridden:
  //  check for "-r" argument
  //
  if(repl_dir[0] != (char)NULL){
    mplpc.set_repl_directory(repl_dir);
  }

  // initialize all the helper function so that they are initialized atleast once
  // and not inlined in the executable..
  //
  
  VectorDouble tmp (10);
  for (int i = 0; i < 10; i++) {
    tmp[i] = i;
  }
  bool tmp_status = mplpc.helper_print_arr(tmp);
  if (!tmp_status) {
    fprintf(stdout, "Error during helper method initialization...");
    return false;
  }
  
  
  // display an informational message
  //
  fprintf(stdout, "beginning argument processing...\n");

  // main processing loop: loop over all input filenames
  //
  long num_files_att = 0;
  long num_files_proc = 0;
  char osig_fname[Edf::MAX_LSTR_LENGTH];

  for (int i=cmdl.get_first_arg_pos(); i<argc; i++) {

    // if it is an edf file, process it
    //
    if (edf.is_edf((char*)argv[i])) {

      // display a status message
      //
      num_files_att++;
      fprintf(stdout, "  %6ld: %s\n", num_files_att, (char*)argv[i]);

      // execute mplpc
      //
      if (mplpc.compute(osig_fname, (char*)argv[i])) {
	fprintf(stdout, "          %s\n", osig_fname);
	num_files_proc++;
      }
      else {
	fprintf(stdout, "  **> run_mplpc: error generating mplpc signal\n");
      }
    }

    // else: treat it as a file list
    //
    else {

      // display an informational message
      //
      fprintf(stdout, " opening list %s...\n", (char*)argv[i]);

      // open the list
      //
      FILE* fp = fopen(argv[i], "r");
      if (fp == (FILE*)NULL) {
	fprintf(stdout, " **> run_mplpc: error opening file list (%s)\n",
		argv[i]);
	exit(1);
      }

      // loop over all files
      //
      char edf_fname[Edf::MAX_LSTR_LENGTH];
      while (fscanf(fp, "%s", edf_fname) == 1) {

	// display a status message
	//
	num_files_att++;
	fprintf(stdout, "  %6ld: %s\n", num_files_att, edf_fname);

	// compute features
	//
	if (mplpc.compute(osig_fname, edf_fname)) {
	  fprintf(stdout, "          %s\n", osig_fname);
	  num_files_proc++;
	}
	else {
	  fprintf(stdout, "  **> run_mplpc: error generating mplpc signal\n");
	}
      }

      // close the list
      //
      fclose(fp);
    }
  }    

  // display the results
  //
  fprintf(stdout, "processed %ld out of %ld files successfully\n",
	  num_files_proc, num_files_att);
  
  // exit gracefully
  //
  exit(status);
}
