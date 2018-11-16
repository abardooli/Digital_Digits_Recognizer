#include <iostream>
#include <fstream>
#include "ddrlib.h"

using namespace std;

// -------- Digital Digit Recognition Reader Functions -----------------
//
// These functions can be used as 'library functions' to read in
// data to help the ddr code .
//
// There are these functions
//    read_patts  -- read digit patterns from a file
//    open_image  -- open a new image file (or cin)
//    next_image  -- read in next image to analyze
//    close_image -- close the connection
//
// To use these functions, YOUR program has to have at the top:
//
//  #include "ddrlib.h"
//
// history: 2014-02-26 added handling for cin
// history: 2014-02-25 created bmolay (molay@cs.tufts.edu)
//

//
// purp: Read in a set of character patterns from a file and
//       store those patterns in an array of 2D arrays.
// args: the name of the file, the array of 2D arrays, the array len
// rets: the number of patterns read in OR
//       -1 for file error (e.g. no such file)
//       -2 for data error (e.g. dimensions exceed allowed size)
//          or char is not a legal digit
//
int read_patts(string filename,char digs[][DIG_HGT][DIG_WID], int numpatts)
{
        string   line;
        ifstream input;
        int      digit, width, height, digits_read = 0;

        input.open( filename.c_str() );         // open
        if ( !input.is_open() )                 // any problems?
                return -1;                      //   Y: ret -1

        while( input >> digit )                 // start of block
        {
                if ( digit < 0 || digit >= numpatts )
                        return -2;
                                                // now read in ...
                input >> height >> width ;              // dimensions
                if ( width > DIG_WID || height > DIG_HGT )
                        return -2;

                for (int row=0; row<height; row++)      // data
                {
                        input >> line;                  // read rows
                        if ( input.eof() )
                                return -2;
                        for (int col=0; col<width; col++)  // and store them
                        {
                                digs[digit][row][col] = line[col];
                        }
                }
                digits_read++;
        }
        input.close();                          // close file
        return digits_read;                     // and go!
}

static ifstream image_stream;                   // connection to image file
static bool use_cin = false;                    // unless we use this

//
// open a file for input
// if string is "" then use cin as the ifstream
//
// args: name of file as string (or "" or "-" for cin)
// rets: true if worked, false if not
// does: closes file stream if already open
//
bool open_image(string filename)
{
        // close image in case it is open
        close_image();

        if ( filename == "" || filename == "-" ){
                use_cin = true;
                return true;
        }
        else {
                // try new one
                image_stream.open( filename.c_str() );
                use_cin = false;
                return image_stream.is_open();
        }
}

void close_image()
{
        if ( !use_cin && image_stream.is_open() )
                image_stream.close();
        use_cin = false;
}

//
// next_line -- read next string from current image stream
//   rets: true if it got one, false at EOF
//
bool next_line(string& line)
{
        if ( use_cin ){
                getline(cin, line);
                return !(cin.eof());
        }
        else {
                getline( image_stream, line );
                return !(image_stream.eof());
        }
}
//
// load next image from stream
//   args: an array to store the image map
//   rets: true if worked, false if not
//   errs: stream not open, too few lines
//   note: ends at sentinel line or EOF
//         in which case it fills the rest across and down
//   note: skips blank lines and lines starting with --
//
bool next_image(char img[IMG_HGT][IMG_WID])
{
        string  line;
        int     row, col, len;
        bool    saw_end_of_image = false;
        bool    next_line(string&);

        if ( use_cin == false && !image_stream.is_open() )
                return false;

        for(row=0; row<IMG_HGT; row++)
        {
                if ( !saw_end_of_image ){
                        if ( !next_line( line ) || line == END_OF_IMAGE ){
                                if ( row == 0 )
                                        return false;
                                saw_end_of_image = true;
                                len = 0;
                        }
                        else
                                len = line.length();
                        if ( line == "" || (line[0] == '-' && line[1] == '-')){
                                row--;
                                continue;
                        }
                }
                for(col=0; col<IMG_WID; col++){
                        if ( col<len )
                                img[row][col] = line[col];
                        else
                                img[row][col] = '.';
                }
        }
        return true;
}
