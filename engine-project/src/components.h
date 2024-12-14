#include "Reflection/base.h"

class __FLX_API Vec2
{ FLX_REFL_SERIALIZABLE
public:
  float x;
  float y;

  Vec2() : x{ 0 }, y{ 0 } {}
  Vec2(float x, float y) : x{ x }, y{ y } {}

  void Invert()
  {
    x = -x;
    y = -y;
  }

  void Print() const
  {
    std::cout << "Vec2(" << x << ", " << y << ")" << std::endl;
  }
};
