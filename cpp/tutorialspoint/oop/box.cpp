#include <iostream>

class Box
{
public:
  double length;
  double breadth;
  double height;

  double getVolume (void);
  void setLength (double l);
  void setBreadth (double b);
  void setHeight (double h);
};

double
Box::getVolume (void)
{
  return length * breadth * height;
}

void
Box::setLength (double l)
{
  length = l;
}

void
Box::setBreadth (double b)
{
  breadth = b;
}

void
Box::setHeight (double h)
{
  height = h;
}

int
main (int argc, char const *argv[])
{
  Box Box;
  Box.setLength (5.0);
  Box.setBreadth (6.0);
  Box.setHeight (7.0);

  double volume = Box.getVolume ();
  std::cout << "Volume of Box : " << volume << std::endl;

  return 0;
}
