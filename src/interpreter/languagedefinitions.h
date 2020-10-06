#ifndef LANGUAGEDEFINITIONS_H
#define LANGUAGEDEFINITIONS_H

#include <string>

namespace CS {
/**
 * This namespace holds names for constants and functions
 * defined in the language
 */
namespace Names {

const std::string c1 = "N_C1";
const std::string d1 = "N_D1";
const std::string e1 = "N_E1";
const std::string f1 = "N_F1";
const std::string g1 = "N_G1";
const std::string a1 = "N_A1";
const std::string b1 = "N_B1";
const std::string c2 = "N_C2";
const std::string d2 = "N_D2";
const std::string e2 = "N_E2";
const std::string f2 = "N_F2";
const std::string g2 = "N_G2";
const std::string a2 = "N_A2";
const std::string b2 = "N_B2";
const std::string c3 = "N_C3";
const std::string d3 = "N_D3";
const std::string e3 = "N_E3";
const std::string f3 = "N_F3";
const std::string g3 = "N_G3";
const std::string a3 = "N_A3";
const std::string b3 = "N_B3";
const std::string c4 = "N_C4";
const std::string d4 = "N_D4";
const std::string e4 = "N_E4";
const std::string f4 = "N_F4";
const std::string g4 = "N_G4";
const std::string a4 = "N_A4";
const std::string b4 = "N_B4";
const std::string c5 = "N_C5";
const std::string d5 = "N_D5";
const std::string e5 = "N_E5";
const std::string f5 = "N_F5";
const std::string g5 = "N_G5";
const std::string a5 = "N_A5";
const std::string b5 = "N_B5";

const std::string sinSound      = "S_SIN";
const std::string squareSound   = "S_SQUARE";

const std::string sampleFunction            = "SAMPLE";
const std::string stopFunction              = "STOP";
const std::string startRecordingFunction    = "START_RECORDING";
const std::string stopRecordingFunction     = "STOP_RECORDING";
const std::string startServerFunction       = "START_SERVER";
const std::string restartServerFunction     = "RESTART_SERVER";
const std::string killServerFunction        = "KILL_SERVER";

}

/**
 * Definition for internal functions related with the language
 */
namespace Functions {

const std::string freqParam     = "P_FREQ";
const std::string durationParam = "P_DURATION";

double sin(double);
double square(double);

void sample();

void stop();
void startRecording();
void stopRecording();
void startServer();
void restartServer();
void killServer();

}

}

#endif // LANGUAGEDEFINITIONS_H
