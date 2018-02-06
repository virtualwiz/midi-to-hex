#include "MidiFile.h"
#include "Options.h"
#include <ctype.h>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <algorithm> // remove and remove_if
#include "intelhexclass.h"
#include "bprinter/table_printer.h"
#include "NoteListProcessor.h"

using namespace std;
using bprinter::TablePrinter;
using noteListProcessor::NoteListProcessor;

typedef unsigned char uchar;

// user interface variables
Options options;
bool globalDebugFlag = false;

// function declarations:
void convertNoteMaptoRom(map<int, vector<int>> &noteMap, vector<char> &mem);
void convertMemToHexFile(vector<char> &mem, string originalHexFilePath, string targetHexFilePath);
void checkOptions(Options &opts, int argc, char **argv);
void example(void);
void usage(const char *command);
extern "C" {
extern int micronucleus_main(int argc, char **argv);
}

//////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

      map<int, vector<int>> noteMap;
      checkOptions(options, argc, argv);
      vector<char> mem;
      NoteListProcessor np=NoteListProcessor(options.getArg(1));
      np.analyzeNoteMap();
      np.printAnalyzeResult();
      np.transposeTickNoteMap();
      np.generateBin(mem);
      convertMemToHexFile(mem, "./hex-file/mg.hex", "target.hex");

      char argString[][32] = {"micronucleus_mainaaa", "-r", "--fast-mode", "target.hex"};
      char *micronucleus_argv[4];
      for (int i = 0; i < 4; i++)
            micronucleus_argv[i] = argString[i];
      return micronucleus_main(4, micronucleus_argv);
      return 0;
}

void convertMemToHexFile(vector<char> &mem, string originalHexFilePath, string targetHexFilePath)
{
      // Create an input stream
      ifstream intelHexInput;
      // Create an output stream
      ofstream intelHexOutput;

      // Create a variable for the intel hex data
      intelhex ihMusicBoxFirmRom;
      intelHexInput.open(originalHexFilePath, ifstream::in);

      if (!intelHexInput.good())
      {
            std::cerr << "Error: couldn't open " << originalHexFilePath << std::endl;
      }
      /* Decode file                                                            */
      intelHexInput >> ihMusicBoxFirmRom;
      unsigned long addr = 1024 * 2;
      for (auto &byte : mem)
      {
            ihMusicBoxFirmRom.overwriteData(byte, addr++);
      }

      intelHexOutput.open(targetHexFilePath, ofstream::out);

      if (!intelHexOutput.good())
      {
            std::cerr << "Error: couldn't open " << targetHexFilePath << std::endl;
      }

      intelHexOutput << ihMusicBoxFirmRom;
}





void checkOptions(Options &opts, int argc, char *argv[])
{
      opts.define("author=b", "author of program");
      opts.define("version=b", "compilation info");
      opts.define("example=b", "example usages");
      opts.define("h|help=b", "short description");

      opts.define("d|debug=b", "debug mode to find errors in input file");

      opts.process(argc, argv);

      // handle basic options:
      if (opts.getBoolean("author"))
      {
            cout << "Written by Yuan." << endl;
            exit(0);
      }
      else if (opts.getBoolean("version"))
      {
            cout << argv[0] << ", version: 1.0" << endl;
            cout << "compiled: " << __DATE__ << endl;
            exit(0);
      }
      else if (opts.getBoolean("help"))
      {
            usage(opts.getCommand().data());
            exit(0);
      }
      else if (opts.getBoolean("example"))
      {
            example();
            exit(0);
      }
      else if (opts.getBoolean("debug"))
      {
            globalDebugFlag = true;
      }

      if (opts.getArgCount() != 1)
      {
            usage(opts.getCommand().data());
            exit(1);
      }
}

//////////////////////////////
//
// example --
//

void example(void)
{
}

//////////////////////////////
//
// usage --
//

void usage(const char *command)
{
      cout << "Usage: " << command << " midifile" << endl;
}
