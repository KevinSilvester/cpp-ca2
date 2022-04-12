//
// Created by floodd on 23/03/2022.
//
#include "Image.h"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

bool Image::load(const string &filename)
{
   std::ifstream ifs;
   ifs.open(filename, std::ios::binary);

   try
   {
      if (ifs.fail())
         throw "Could not open file";

      std::string header;
      int w, h, b;

      ifs >> header >> w >> h >> b;

      cout << "Header: " << header << endl;
      cout << "Width: " << w << endl;
      cout << "Height: " << h << endl;
      cout << "Bits per pixel: " << b << endl;

      if (strcmp(header.c_str(), "P6") != 0)
         throw "Not a PPM file";

      this->w = w;
      this->h = h;
      this->pixels = new Rgb[w * h];
      ifs.ignore(256, '\n');
      unsigned char pix[3];
      for (int i = 0; i < w * h; i++)
      {
         ifs.read(reinterpret_cast<char *>(pix), 3);
         this->pixels[i].r = pix[0];
         this->pixels[i].g = pix[1];
         this->pixels[i].b = pix[2];
      }
      ifs.close();
   }
   catch (const char *err)
   {
      fprintf(stderr, "Error: %s\n", err);
      ifs.close();
      return false;
   }
   return true;
}
bool Image::loadRaw(const string &filename)
{
   std::ifstream ifs;
   ifs.open(filename, std::ios::binary);

   try
   {
      if (ifs.fail())
         throw "Could not open file";

      std::string header;
      int w, h, b;

      ifs >> header >> w >> h >> b;

      if (strcmp(header.c_str(), "P6") != 0)
         throw "Not a PPM file";

      this->w = w;
      this->h = h;
      this->pixels = new Rgb[w * h];
      ifs.ignore(256, '\n');
      unsigned char pix[3];
      for (int i = 0; i < w * h; i++)
      {
         ifs.read(reinterpret_cast<char *>(pix), 3);
         this->pixels[i].r = pix[0];
         this->pixels[i].g = pix[1];
         this->pixels[i].b = pix[2];
      }
      ifs.close();
   }
   catch (const char *err)
   {
      std::cerr << err << std::endl;
      ifs.close();
      return false;
   }
   return false;
}

bool Image::savePPM(string filename)
{
   if (this->w == 0 || this->h == 0)
   {
      fprintf(stderr, "Error: Can't save empty image\n");
      return false;
   }
   std::ofstream ofs;
   try {
      ofs.open(filename, std::ios::binary);

      if (ofs.fail())
         throw "Could not open file";

      ofs << "P6\n" << this->w << " " << this->h << "\n255\n";
      unsigned char pix[3];

      for (int i = 0; i < this->w * this->h; i++)
      {
         pix[0] = static_cast<unsigned char>( this->pixels[i].r);
         pix[1] = static_cast<unsigned char>( this->pixels[i].g);
         pix[2] = static_cast<unsigned char>( this->pixels[i].b);
         ofs.write(reinterpret_cast<char *>(pix), 3);
      }

      ofs.close();
   }
   catch (const char *err)
   {
      fprintf(stderr, "Error: %s\n", err);
      ofs.close();
      return false;
   }
   return true;
}

void Image::filterRed()
{
}
void Image::filterGreen()
{
}
void Image::filterBlue()
{
}
void Image::greyScale()
{
}
void Image::flipHorizontal()
{
}
void Image::flipVertically()
{
}
void Image::AdditionalFunction2()
{
}
void Image::AdditionalFunction3()
{
}
void Image::AdditionalFunction1()
{
}

/* Functions used by the GUI - DO NOT MODIFY */
int Image::getWidth()
{
   return w;
}

int Image::getHeight()
{
   return h;
}

Rgb *Image::getImage()
{
   return pixels;
}
