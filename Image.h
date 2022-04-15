//
// Created by floodd on 22/03/2022.
//

// Created by floodd on 11/02/2022.
//

#ifndef SDLTEST_TGA_FILEHANDLER_H
#define SDLTEST_TGA_FILEHANDLER_H
#include "Colours.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string.h>

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::cout;
using std::cerr;
using std::endl;
using std::clamp;
using std::min;
using std::max;

struct Rgb
{
   Rgb() : r(1), g(1), b(1) {}
   explicit Rgb(unsigned char rr) : r(rr), g(rr), b(rr) {}
   Rgb(unsigned char rr, unsigned char gg, unsigned char bb) : r(rr), g(gg), b(bb) {}
   unsigned char r, g, b;
};

class Image
{
   Image(const unsigned int _w, const unsigned int _h) : w(_w), h(_h), pixels(nullptr)
   {
      pixels = new Rgb[w * h];
   }

   unsigned int w, h;
   Rgb *pixels;

 public:
   Image() : w(300), h(400)
   {
      pixels = new Rgb[w * h];
   }

   enum brightness
   {
      increase,
      decrease
   };

   bool savePPM(const string &filename);
   bool load(const string &filename);
   bool loadRaw(const string &filename);
   void filterRed();
   void filterGreen();
   void filterBlue();
   void greyScale();
   void flipHorizontal();
   void flipVertically();
   void AdditionalFunction1();
   void AdditionalFunction2(const brightness &action);
   void AdditionalFunction3();



   /*Functions used in the GUI - DO NOT MODIFY */
   Rgb *getImage();
   int getWidth();
   int getHeight();
   ~Image()
   {
      if (pixels != nullptr)
         delete[] pixels;
   }
};

#endif // SDLTEST_TGA_FILEHANDLER_H
