#include <cstdio>
#include <set>

class Process {
 private:
  int mID;

 public:
  Process(int id) : mID(id) {}
  ~Process() {}
  int getID() const { return this->mID; }
};

class compare_id {
  bool reverse;

 public:
  compare_id(const bool& revparam = false) { reverse = revparam; }
  bool operator()(const Process& lhs, const Process& rhs) const {
    if (reverse)
      return (lhs.getID() > rhs.getID());
    else
      return (lhs.getID() < rhs.getID());
  }
};

int main(int argc, char const* argv[]) {
  std::set<Process, compare_id> processes;

  processes.insert(Process(10));
  processes.insert(Process(1));
  processes.insert(Process(5));
  processes.insert(Process(7));
  processes.insert(Process(3));
  processes.insert(Process(3));
  processes.insert(Process(9));

  for (auto& p : processes) {
    printf("%d ", p.getID());
  }
  printf("\n");

  Process front = *(processes.begin());
  Process back = *(processes.end());

  printf("front: %d\n", front.getID());
  printf("back: %d\n", back.getID());

  // processes.erase(Process(3));

  // for (auto& p : processes) {
  //   printf("%d ", p.getID());
  // }
  // printf("\n");

  // processes.erase(0);
  // for (auto& p : processes) {
  //   printf("%d ", p.getID());
  // }
  // printf("\n");

  //   // processes.erase(processes.cend());
  // for (auto& p : processes) {
  //   printf("%d ", p.getID());
  // }
  // printf("\n");

  return 0;
}
