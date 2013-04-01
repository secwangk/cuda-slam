//
// This Program is provided by Duke University and the authors as a service to the
// research community. It is provided without cost or restrictions, except for the
// User's acknowledgement that the Program is provided on an "As Is" basis and User
// understands that Duke University and the authors make no express or implied
// warranty of any kind.  Duke University and the authors specifically disclaim any
// implied warranty or merchantability or fitness for a particular purpose, and make
// no representations or warranties that the Program will not infringe the
// intellectual property rights of others. The User agrees to indemnify and hold
// harmless Duke University and the authors from and against any and all liability
// arising out of User's use of the Program.
//
// high.h
// Copyright 2005 Austin Eliazar, Ronald Parr, Duke University
//
// See low.h for comments (it is mostly identical)
//
#ifndef _HIGH_H_
#define _HIGH_H_
#include "highMap.h"

void InitHighSlam();
void CloseHighSlam();
void HighSlam(TPath *path, TSenseLog *obs);
extern unsigned char h_map[H_MAP_WIDTH][H_MAP_HEIGHT];

#endif