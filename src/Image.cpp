//
// Created by floodd on 23/03/2022.
//
#include "Image.h"

bool Image::load(const string &filename)
{
   std::ifstream ifs;
   ifs.open(filename, std::ios::binary);

   try
   {
      if (ifs.fail())
         throw "Could not open file";

      string header;
      int b;

      ifs >> header >> this->w >> this->h >> b;

      if (strcmp(header.c_str(), "P6") != 0 || b != 255)
         throw "Not a PPM file";

      if (this->w <= 0 || this->h <= 0)
         throw "Invalid image size";

      this->pixels = new Rgb[this->w * this->h];

      ifs.ignore(256, '\n');

      unsigned char pix[3];

      for (int i = 0; i < w * h; i++)
      {
         ifs.read((char *)pix, 3);
         this->pixels[i].r = pix[0];
         this->pixels[i].g = pix[1];
         this->pixels[i].b = pix[2];
      }
      ifs.close();
   }
   catch (const char* err)
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
   ifs.open(filename, std::ios::in);

   try
   {
      if (ifs.fail())
         throw "Could not open file";

      ifs >> this->w >> this->h;

      if (this->w <= 0 || this->h <= 0)
         throw "Invalid image size";

      this->pixels = new Rgb[this->w * this->h];

      ifs.ignore(256, '\n');

      double pix[3];

      for (int i = 0; i < this->w * this->h; i++)
      {
         ifs >> pix[0] >> pix[1] >> pix[2];
         this->pixels[i].r = (int)std::clamp(pix[0] * 255, 0.0, 255.0);
         this->pixels[i].g = (int)std::clamp(pix[1] * 255, 0.0, 255.0);
         this->pixels[i].b = (int)std::clamp(pix[2] * 255, 0.0, 255.0);
      }
      ifs.close();
   }
   catch (const char* err)
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
         ofs.write((char *)pix, 3);
      }

      ofs.close();
   }
   catch (const char* err)
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
      this->pixels[i] = Rgb(avg, avg, avg);
   }
}

void Image::flipHorizontal()
{
   for (int i = 0; i < this->w / 2; i++)
   {
      for (int j = 0; j < this->h; j++)
      {
         int l_idx = i + j * this->w;
         int r_idx = (this->w - i - 1) + j * this->w;

         Rgb temp = this->pixels[l_idx];
         this->pixels[l_idx] = this->pixels[r_idx];
         this->pixels[r_idx] = temp;
      }
   }
}

void Image::flipVertically()
{
   int len = this->w * this->h;

   for (int i = 0; i < len / 2; i++)
   {
      Rgb tmp = this->pixels[i];
      this->pixels[i] = this->pixels[len - i - 1];
      this->pixels[len - i - 1] = tmp;
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

            hsv_t pixelH = Colours::rgb2hsv(pixelR);
            pixelH.v = std::min((double)1, pixelH.v + steps);

            rgb_t pixelR2 = Colours::hsv2rgb(pixelH);

            this->pixels[i].r = (int)std::clamp(pixelR2.r * 255, 0.0, 255.0);
            this->pixels[i].g = (int)std::clamp(pixelR2.g * 255, 0.0, 255.0);
            this->pixels[i].b = (int)std::clamp(pixelR2.b * 255, 0.0, 255.0);
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

            hsv_t pixelH = Colours::rgb2hsv(pixelR);
            pixelH.v = std::max((double)0, pixelH.v - steps);

            rgb_t pixelR2 = Colours::hsv2rgb(pixelH);

            this->pixels[i].r = (int)std::clamp(pixelR2.r * 255, 0.0, 255.0);
            this->pixels[i].g = (int)std::clamp(pixelR2.g * 255, 0.0, 255.0);
            this->pixels[i].b = (int)std::clamp(pixelR2.b * 255, 0.0, 255.0);
         }
         break;
      }
   }
}

void Image::AdditionalFunction3(const blur &type)
{
   // gaussian + mean blur

   // This is a very basic implementation of a gaussian + mean blur.
   // references:
   //    - https://youtu.be/C_zFhWdM4ic
   //    - https://medium.com/swlh/how-image-blurring-works-652051aee2d1#:~:text=Roughly%20speaking%2C%20blurring%20an%20image,kernel%2C%20to%20the%20image
   //    - https://en.wikipedia.org/wiki/Gaussian_blur
   //    - https://en.wikipedia.org/wiki/Kernel_(image_processing)

   std::vector<std::vector<int>> kernel;

   switch (type)
   {
      case blur::mean:
         kernel = {
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, 1}
         };
         break;
      case blur::gaussian:
         kernel = {
            {1, 2, 1},
            {2, 4, 2},
            {1, 2, 1}
         };
         break;
   }

   int kernelSize = 3, kernelSum = 0;

   // temporary pixel storage to store the new pixel value
   Rgb *tmp = new Rgb[this->w * this->h];

   // loop through each pixel
   for (int imgX = 0; imgX < this->w; imgX++)
   {
      for (int imgY = 0; imgY < this->h; imgY++)
      {
         int R = 0,
             G = 0,
             B = 0;

         kernelSum = 0;

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

                  R += pixel.r * kernelValue;
                  G += pixel.g * kernelValue;
                  B += pixel.b * kernelValue;
                  kernelSum += kernelValue;
               }
            }
         }
         // Assign the new pixel RGB value to the temporary pixel storage
         // The kernelSum is the sum of all the kernel values, in this case 16 for gaussian and 9 for mean blur.
         tmp[imgY * this->w + imgX] = Rgb(R / kernelSum, G / kernelSum, B / kernelSum);
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
