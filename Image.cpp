//
// Created by floodd on 23/03/2022.
//
#include "Image.h"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

Colours *col = new Colours();

bool Image::load(const string &filename)
{
   std::ifstream ifs;
   ifs.open(filename, std::ios::binary);

   try
   {
      if (ifs.fail())
         throw "Could not open file";

      string header;
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

      string header;
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
   catch (const string err)
   {
      fprintf(stderr, "Error: %s\n", err);
      ifs.close();
      return false;
   }
   return true;
}

bool Image::savePPM(const string &filename)
{
   std::ofstream ofs;
   try
   {
      if (this->w == 0 || this->h == 0)
         throw "Can't an save empty image";

      ofs.open(filename, std::ios::binary);

      if (ofs.fail())
         throw "Can't open output file";

      ofs << "P6\n" << this->w << " " << this->h << "\n255\n";
      unsigned char pix[3];

      for (int i = 0; i < this->w * this->h; i++)
      {
         pix[0] = (unsigned char)this->pixels[i].r;
         pix[1] = (unsigned char)this->pixels[i].g;
         pix[2] = (unsigned char)this->pixels[i].b;
         ofs.write(reinterpret_cast<char *>(pix), 3);
      }

      ofs.close();
   }
   catch (const string err)
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

void Image::AdditionalFunction2(const brightness &action)
{
   // brightness

   // To increase and decrease brightness, the colour value of each pixel is converted from rgb to hsv.
   // This to allow the brightness to be increased or decreased by a constant value.
   // Increasing the rgb value directly can work, but it is not as accurate as the hsv value.
   // The hsv value is converted back to rgb and the new rgb value is assigned to the pixel.
   // The used for algorithms the conversion are not mine and are copied from this stackoverflow post:
   // https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

   double steps = 0.1;

   switch (action)
   {
      case brightness::increase:
      {
         for (int i = 0; i < this->w * this->h; i++)
         {
            rgb_t pixelR;
            pixelR.r = (double)this->pixels[i].r / 255;
            pixelR.g = (double)this->pixels[i].g / 255;
            pixelR.b = (double)this->pixels[i].b / 255;

            hsv_t pixelH = col->rgb2hsv(pixelR);
            pixelH.v = std::min((double)1, pixelH.v + steps);

            rgb_t pixelR2 = col->hsv2rgb(pixelH);

            this->pixels[i].r = std::clamp(pixelR2.r * 255, (double)0, (double)255);
            this->pixels[i].g = std::clamp(pixelR2.g * 255, (double)0, (double)255);
            this->pixels[i].b = std::clamp(pixelR2.b * 255, (double)0, (double)255);
         }
         break;
      }
      case brightness::decrease:
      {
         for (int i = 0; i < this->w * this->h; i++)
         {
            rgb_t pixelR;
            pixelR.r = (double)this->pixels[i].r / 255;
            pixelR.g = (double)this->pixels[i].g / 255;
            pixelR.b = (double)this->pixels[i].b / 255;

            hsv_t pixelH = col->rgb2hsv(pixelR);
            pixelH.v = std::max((double)0, pixelH.v - steps);

            rgb_t pixelR2 = col->hsv2rgb(pixelH);

            this->pixels[i].r = std::clamp(pixelR2.r * 255, (double)0, (double)255);
            this->pixels[i].g = std::clamp(pixelR2.g * 255, (double)0, (double)255);
            this->pixels[i].b = std::clamp(pixelR2.b * 255, (double)0, (double)255);
         }
         break;
      }
   }
}

void Image::AdditionalFunction3()
{
   // gaussian blur

   // This is a very basic implementation of a gaussian blur.
   // references:
   //    - https://youtu.be/C_zFhWdM4ic
   //    - https://medium.com/swlh/how-image-blurring-works-652051aee2d1#:~:text=Roughly%20speaking%2C%20blurring%20an%20image,kernel%2C%20to%20the%20image
   //    - https://en.wikipedia.org/wiki/Gaussian_blur
   //    - https://en.wikipedia.org/wiki/Kernel_(image_processing)
   int kernelSize = 3;
   int kernel[3][3] = {
      {1, 2, 1},
      {2, 4, 2},
      {1, 2, 1}
   };
   int kernelSum = 16;

   // temporary pixel storage to store the new pixel value
   Rgb *tmp = new Rgb[this->w * this->h];

   // loop through each pixel
   for (int imgX = 0; imgX < this->w; imgX++)
   {
      for (int imgY = 0; imgY < this->h; imgY++)
      {
         int kernelR = 0,
             kernelG = 0,
             kernelB = 0;

         // loop through kernel (convolution of kernel matrix)
         for (int kernelX = 0; kernelX < kernelSize; kernelX++)
         {
            for (int kernelY = 0; kernelY < kernelSize; kernelY++)
            {
               // the 'grid coordinates' of the pixel that is to be used by the kernel
               int targetX = imgX + kernelX - 1,
                   targetY = imgY + kernelY - 1;

               // avoid using edge pixels
               if (targetX >= 0 && targetX < this->w && targetY >= 0 && targetY < this->h)
               {
                  // The selection of the pixels is easy to understand if the image is visualized as a grid.
                  // I get all the pixels around the target pixel (imgX * imgY) and I multiply the kernel value
                  // with the pixel value.
                  // This operation is done for each of the RGB values of 9 pixels (since a 3x3 matrix was used)
                  // around the target pixel.
                  Rgb pixel = this->pixels[targetY * this->w + targetX];
                  int kernelValue = kernel[kernelX][kernelY];

                  kernelR += pixel.r * kernelValue;
                  kernelG += pixel.g * kernelValue;
                  kernelB += pixel.b * kernelValue;
               }
            }
         }
         // Assign the new pixel RGB value to the temporary pixel storage
         // The kernelSum is the sum of all the kernel values, in this case 16.
         tmp[imgY * this->w + imgX] = Rgb(kernelR / kernelSum, kernelG / kernelSum, kernelB / kernelSum);
      }
   }
   // the new pixel values are copied to the original image
   std::swap(this->pixels, tmp);
   delete[] tmp;
   tmp = nullptr;
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
