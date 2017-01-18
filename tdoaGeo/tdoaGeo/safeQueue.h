#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class SafeQueue
{
public:
  SafeQueue(void)
    : q()
    , m()
    , c()
  {}

  ~SafeQueue(void)
  {}

  void push(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  void pop(void)
  {
	std::lock_guard<std::mutex> lock(m);
    q.pop();
  }

  T front(void)
  {
	  std::unique_lock<std::mutex> lock(m);
	  while (q.empty())
	  {
		  c.wait(lock);
	  }
	  T val = q.front();
	  return val;
  }

  size_t size(void)
  {
	  std::lock_guard<std::mutex> lock(m);
	  return q.size();
  }

  void clear(void)
  {
	  std::lock_guard<std::mutex> lock(m);
	  while (!q.empty())
	  {
		  q.pop();
	  }
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};
#endif