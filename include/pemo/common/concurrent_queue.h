//
// Created by poilk on 2024/2/10.
//

#ifndef PEMO_CONCURRENT_QUEUE_H
#define PEMO_CONCURRENT_QUEUE_H

#include <mutex>
#include <queue>

#include "pemo/common/base.h"

namespace pemo {

template <typename T>
class ConcurrentBlockingQueue {
 public:
  ConcurrentBlockingQueue();
  ~ConcurrentBlockingQueue() = default;
  template <class E>
  void Push(E&& e);
  bool Pop(T* rv);
  size_t Size();

 private:
  std::mutex m_mutex;
  std::condition_variable m_cv;
  int32_t m_nwaitConsumer{};
  std::queue<T> m_fifo_queue;

  DISALLOW_COPY_AND_ASSIGN(ConcurrentBlockingQueue);
};

template <typename T>
ConcurrentBlockingQueue<T>::ConcurrentBlockingQueue() = default;

template <typename T>
template <class E>
void ConcurrentBlockingQueue<T>::Push(E&& e) {
  static_assert(std::is_same<typename std::remove_cv<typename std::remove_reference<E>::type>::type, T>::value,
                "Types must match.");
  bool notify = false;
  {
    std::lock_guard<std::mutex> lockGuard(m_mutex);
    m_fifo_queue.push(std::forward<T>(e));
    notify = m_nwaitConsumer != 0;
  }
  if (notify) {
    m_cv.notify_one();
  }
}
template <typename T>
bool ConcurrentBlockingQueue<T>::Pop(T* rv) {
  std::unique_lock<std::mutex> lock(m_mutex);
  {
    ++m_nwaitConsumer;
    m_cv.wait(lock, [this]() { return !m_fifo_queue.empty(); });
    --m_nwaitConsumer;
    *rv = std::move(m_fifo_queue.front());
    m_fifo_queue.pop();
    return true;
  }
  return false;
}

template <typename T>
size_t ConcurrentBlockingQueue<T>::Size() {
  std::lock_guard<std::mutex> lockGuard(m_mutex);
  return m_fifo_queue.size();
}

}  // namespace pemo

#endif  // PEMO_CONCURRENT_QUEUE_H
