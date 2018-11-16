#include <iostream>
#include <cstdlib>
#include "ddrlib.h"
using namespace std;

// ddr.cpp: digital digit recognizer

//  purpose: read in plates of bitmaps and find the digits in those images
//     uses: a file of digit bitmaps
//    input: a sequence of images from cin
//   output: for each image, a list of the digits and their locations
//   format: 2 @ 2,1 3 @ 2,12 6 @ 3,20 ... -1
//  details: each digit is listed as the digit, a space, an @ sign,
//           a space, then the row, col with a comma (and no space)
//           between them.
//           The end of the list has a -1 sentinel value
//   errors: any character it cannot recognize is reported as
//           ? @ row,col   where row,col are the actual coordinates of
//           the upper left pixel in the character
//
//     Code by: Ahmed Bardooli
//        Date: 03/08/2014
//       Title: Digital Digits Detector
// Description: The program goes through the entire plate and compares the cells
//              to a pre-made reference that has the entire digits the program
//              looks for. It outputs a list of digits along with the position
//              of the top left corner of each digit preceeded with "@", and
//              each list ends with the SENTINEL "-1"
//

const int SENTINEL = -1;
struct Coordinates {
  int col;
  int row;
};

void print_match( Coordinates pos,int num, bool is_match);
Coordinates Find_dig( char plate[IMG_HGT][IMG_WID]);
bool compare(char plate[IMG_HGT][IMG_WID],
             char digs[DIG_NUM][DIG_HGT][DIG_WID],
             Coordinates pos,
             int num);

void analyze(char [IMG_HGT][IMG_WID], char [DIG_NUM][DIG_HGT][DIG_WID]);

int main(int ac, char *av[])
{
        char    digits[DIG_NUM][DIG_HGT][DIG_WID];
        char    image[IMG_HGT][IMG_WID];

        if ( ac == 1 ) {
                cerr << "usage: " << av[0] << " digit_bitmap_file\n";
                exit(1);
        }
        if ( read_patts( string(av[1]), digits, DIG_NUM ) != DIG_NUM ) {
                cerr << av[0] << ": error - too few digit bitmaps\n";
                exit(1);
        }
        open_image("");
        while ( next_image(image) ) {
                analyze(image, digits);
        }
        return 0;
}

//
// analyze
//   purpose: Analyzes the entire plate (every row and column) for digits
//            from 0 to 9
// arguments: Array of characters from plate, an array of characters from digs
//   returns: Nothing.
//     Notes: Uses three functions: find, compare, and print_match to accomplish
//            the given task
//


void analyze(char plate[IMG_HGT][IMG_WID],char digs[DIG_NUM][DIG_HGT][DIG_WID])
{
 int num = 0;
  struct Coordinates pos;
  bool is_match = false;
  pos = Find_dig(plate);

   for(pos.col = 0;pos.col < IMG_WID ; pos.col++){
     for(pos.row = 0; pos.row < IMG_HGT ; pos.row++){
       for ( num = 0; num < DIG_NUM ; num++){   // checks numbers from 0 to 9
         is_match = compare(plate,digs,pos,num);
         print_match(pos, num, is_match);
       }
     }
   }
   cout << SENTINEL << endl;// endl separates different set or plates and "-1"
                        // is printed at the end of each plate
}

//
// print_match
//   purpose: Outputs the coordinates of the top left corner whenever
//            a match is found
// arguments: coordinates of starting position (top left corner),
//            an integer of the number being compared,a boolean is_match is
//            true if a number detected.
//   returns: Nothing.
//

void print_match( Coordinates pos, int num, bool is_match)
{
    if(is_match == true){
      cout << num << " @ " << pos.row << "," << pos.col << " " ;
    }
}

//
// compare
//   purpose: Looks at the selected 5 by 7 rectangle, and compares
//            selected space to the reference digs
// arguments: Array of characters from plate, an array of characters from
//            digs, an integer
//   returns: True if a match is found, False if no match is found
//     Notes: the integer num keeps track of what digit number being compared
//

bool compare(char plate[IMG_HGT][IMG_WID],
             char digs[DIG_NUM][DIG_HGT][DIG_WID],
             Coordinates pos, int num)
{
   for (int row = 0 ; row < DIG_HGT; row++){
      for (int col = 0; col < DIG_WID; col++){

        if(pos.row != 0){  // To avoid looking outside the plate when row is 0

            if(plate[pos.row-1][pos.col] == '#'){// checks if this is a new
                   return false;                //  digit or part of old one,
          }
        }
        if(pos.col != 0){
            if (plate[pos.row][pos.col-1]=='#'){
                   return false;
            }
            if(plate[pos.row+3][pos.col-1]== '#'){
                   return false;  // Specifically deals with computing the end
            }                    // part of 4 as 1. (Note: row can be 0 here)
        }
        if (plate[row + pos.row][col + pos.col] != digs[num][row][col]){
          return false;
        }
        else if (plate[row + pos.row][col + pos.col] ==
                 digs[num][row][col]){}
      }
   }
 return true;
 }

//
// Find_dig
//   Purpose: Finds the row and column coordinates of the first
//            '#' in the image (or plate).
// arguments: Array of characters from plate
//   returns: Coordinates of the first '#' the function finds
//     Notes: The loop checks top to bottom, and left to right.
//            It is only used for the first digit, it makes the program
//            run more efficiently. It is a disposable function though.
//

Coordinates Find_dig( char plate[IMG_HGT][IMG_WID])
{
  Coordinates pos;

      for(int col = 0;col < IMG_WID ; col++){
          for(int row = 0; row < IMG_HGT; row++){
            if(plate[row][col]=='#') {
              pos =(Coordinates){col,row};
              return pos;
            }
          }
      }

      return pos;
}
