#include <limits>

// Reference: https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

typedef struct
{
   double r; // a fraction between 0 and 1
   double g; // a fraction between 0 and 1
   double b; // a fraction between 0 and 1
} rgb_t;

typedef struct
{
   double h; // angle in degrees
   double s; // a fraction between 0 and 1
   double v; // a fraction between 0 and 1
} hsv_t;

class Colours
{
 public:
   Colours();
   static hsv_t rgb2hsv(rgb_t in);
   static rgb_t hsv2rgb(hsv_t in);
};