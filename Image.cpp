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
      for (int i = 0; i < this->w * this->h; i++)
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
   return true;
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
         pix[0] = static_cast<unsigned char>(this->pixels[i].r);
         pix[1] = static_cast<unsigned char>(this->pixels[i].g);
         pix[2] = static_cast<unsigned char>(this->pixels[i].b);
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
   for (int i = 0; i < this->w * this->h; i++)
   {
      this->pixels[i].g = 0;
      this->pixels[i].b = 0;
   }
}

void Image::filterGreen()
{
   for (int i = 0; i < this->w * this->h; i++)
   {
      this->pixels[i].r = 0;
      this->pixels[i].b = 0;
   }
}

void Image::filterBlue()
{
   for (int i = 0; i < this->w * this->h; i++)
   {
      this->pixels[i].r = 0;
      this->pixels[i].g = 0;
   }
}
void Image::greyScale()
{
   for (int i = 0; i < this->w * this->h; i++)
   {
      int avg = (this->pixels[i].r + this->pixels[i].g + this->pixels[i].b) / 3;
      this->pixels[i].r = avg;
      this->pixels[i].g = avg;
      this->pixels[i].b = avg;
   }
}

void Image::flipHorizontal()
{
   for (int i = 0; i < this->h; i++)
   {
      for (int j = 0; j < this->w / 2; j++)
      {
         int tempIdx = i * this->w + j;
         int targetIdx = i * this->w + (this->w - j - 1);
         
         Rgb temp = this->pixels[tempIdx];
         this->pixels[tempIdx] = this->pixels[targetIdx];
         this->pixels[targetIdx] = temp;
      }
   }
}

void Image::flipVertically()
{
   int imgPixelCount = this->w * this->h;
   int pixelsArrLen = imgPixelCount - 1;

   for (int i = 0; i < imgPixelCount / 2; i++)
   {
      Rgb temp = this->pixels[i];
      this->pixels[i] = this->pixels[pixelsArrLen - i];
      this->pixels[pixelsArrLen - i] = temp;
   }
}

void Image::AdditionalFunction1()
{
   // negative
   for (int i = 0; i < this->w * this->h; i++)
   {
      this->pixels[i].r = 255 - this->pixels[i].r;
      this->pixels[i].g = 255 - this->pixels[i].g;
      this->pixels[i].b = 255 - this->pixels[i].b;
   }
}

void Image::AdditionalFunction2()
{
}

void Image::AdditionalFunction3()
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
