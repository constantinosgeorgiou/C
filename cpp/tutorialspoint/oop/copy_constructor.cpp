#include <iostream>

class Line
{
public:
  int getLength (void);
  Line (int len);         // Constructor
  Line (const Line &obj); // Copy constructor
  ~Line (void);           // Destructor
private:
  int *ptr;
};

Line::Line (int len)
{
  std::cout << "Normal constructor allocating ptr." << std::endl;

  ptr = new int;
  *ptr = len;
}

Line::Line (const Line &obj)
{
  std::cout << "Copy constructor allocating ptr." << std::endl;
  ptr = new int;
  *ptr = *obj.ptr; // copy the value
}

Line::~Line (void)
{
  std::cout << "Freeing memory!" << std::endl;
  delete ptr;
}

int
Line::getLength (void)
{
  return *ptr;
}

void
display (Line obj)
{
  std::cout << "Length of line : " << obj.getLength () << std::endl;
}

int
main (int argc, char const *argv[])
{
  Line line1 (10);
  Line line2 = line1;
  display (line1);
  display (line2);
  return 0;
}
