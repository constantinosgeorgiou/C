#include <cstdio>
#include <queue>

class Process {
 private:
  int mPid = -1;
  int mPriority = -1;

 public:
  Process(int pid, int priority) : mPid(pid), mPriority(priority) {}
  ~Process() {}
  int getPid() const { return this->mPid; }
  int getPriority() const { return this->mPriority; }
};

class compare_priorities {
  bool reverse;

 public:
  compare_priorities(const bool& revparam = false) { reverse = revparam; }
  bool operator()(const Process& lhs, const Process& rhs) const {
    if (reverse)
      return (lhs.getPriority() < rhs.getPriority());
    else
      return (lhs.getPriority() > rhs.getPriority());
  }
};

std::priority_queue<Process, std::vector<Process>, compare_priorities>
priority_queue_remove(std::priority_queue<Process, std::vector<Process>,
                                          compare_priorities>& queue,
                      Process& process) {
  std::priority_queue<Process, std::vector<Process>, compare_priorities> pq;

  while (!queue.empty()) {
    Process p = queue.top();

    if (p.getPid() != process.getPid()) {
      pq.push(p);
    }

    queue.pop();
  }

  return pq;
}

void display(std::priority_queue<Process, std::vector<Process>,
                                 compare_priorities>& pq) {
  std::priority_queue<Process, std::vector<Process>, compare_priorities> q = pq;

  while (!q.empty()) {
    Process p = q.top();
    printf("%d (%d)\n", p.getPid(), p.getPriority());
    q.pop();
  }
}

int main(int argc, char const* argv[]) {
  std::priority_queue<Process, std::vector<Process>, compare_priorities> pq;

  pq.push(Process(0, 3));
  pq.push(Process(1, 7));
  pq.push(Process(2, 2));
  pq.push(Process(3, 1));
  pq.push(Process(4, 1));
  pq.push(Process(5, 3));
  pq.push(Process(6, 2));
  pq.push(Process(7, 7));

  display(pq);

  Process p = Process(1, 7);
  pq = priority_queue_remove(pq, p);

  printf("after\n");
  display(pq);

  return 0;
}
