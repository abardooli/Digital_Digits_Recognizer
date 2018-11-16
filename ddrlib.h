#ifndef DDRLIB_H
#define DDRLIB_H

using namespace std;

//
// "header file" for digital digit recognition program
//
// history: 2014-02-25 created bmolay (molay@cs.tufts.edu)
//

const int DIG_HGT = 7;                  // dimensions of each digit pattern
const int DIG_WID = 5;
const int DIG_NUM = 10;                 // maximum number of digits

const int IMG_HGT = 16;                 // dimensions of panel to analyze
const int IMG_WID = 76;
const string END_OF_IMAGE = "EOI";      // a sentinel

int  read_patts(string filename, char a[][DIG_HGT][DIG_WID], int maxdigits);
bool open_image(string filename);
bool next_image(char img[IMG_HGT][IMG_WID]);
void close_image(void);
#endif
