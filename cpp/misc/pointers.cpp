#include <cstdio>
#include <memory>
#include <vector>

class B {
 private:
  int mNumber = -1;

 public:
  B(int number) : mNumber(number) {}
  ~B() {}
  int getNumber() const { return this->mNumber; }
  void setNumber(int number) { this->mNumber = number; }
};

class A {
 private:
  int mData = -1;
  B& mB;

 public:
  A(int data, B b) : mData(data), mB(b) {}
  ~A() {}
  int getData() const { return this->mData; }
  void setData(int data) { this->mData = data; }
  B getB() const { return this->mB; }
};

int main(int argc, char const* argv[]) {
  std::vector<A> as;
  as.push_back(A(100, B(1010)));
  as.push_back(A(200, B(2020)));

  for (auto&& i : as) {
    printf("%d ", i.getData());
  }
  printf("\n");

  A* ptr;

  ptr = &as.front();

  printf("ptr data: %d\n", ptr->getData());

  as.front().setData(-99);
  as.front().getB().setNumber(1111);

  printf("ptr data: %d\tnumber: %d\n", ptr->getData(), ptr->getB().getNumber());
  return 0;
}
