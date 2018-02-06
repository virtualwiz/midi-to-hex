#ifndef __NOTE_LIST_PROCESSOR__
#define __NOTE_LIST_PROCESSOR__

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
#include <map>
#include "MidiFile.h"
#include "bprinter/table_printer.h"

namespace noteListProcessor
{

class MidiHelper
{
public:
  MidiHelper(string midifilePath);
  ~MidiHelper();
  void getTickNoteMap(map<int, vector<int>> &tickNoteMap);

private:
  MidiFile midifile;

  int tickPerSecond = 120;
};

class NoteListProcessor
{
public:
  int highestPitch;
  int lowestPitch;
  int validHighestPitch=105;
  int validLowestPitch=45;
  int suggestTranpose;
  map<int, int> noteOccurTimesMap;

  NoteListProcessor(string midifilePath);
  ~NoteListProcessor();
  void printAnalyzeResult();
  void analyzeNoteMap();
    void transposeTickNoteMap();
    void generateBin(vector<char> &mem);

private:
  map<int, vector<int>> tickNoteMap;
  map<int, vector<int>> tickNoteMapTransposed;
  string pitchName[129];
  void InitPitchName();

};
}
#endif
