//
// Created by poilk on 2024/2/9.
//

#include "thread_pool.h"

#include "pemo/common/log.h"

namespace pemo {
namespace engine {

ThreadPool::ThreadPool(int32_t size, const std::function<void()>& func) {
  assert(size > 0);
  m_workerThreads = std::vector<std::thread>(size);
  for (auto& thread : m_workerThreads) {
    thread = std::thread(func);
  }
}

ThreadPool::ThreadPool(int32_t size, const std::function<void(std::shared_ptr<ManualEvent>)>& func, const bool wait) {
  assert(size > 0);
  m_workerThreads = std::vector<std::thread>(size);
  for (auto& thread : m_workerThreads) {
    std::shared_ptr<ManualEvent> mePtr = std::shared_ptr<ManualEvent>(new ManualEvent());
    m_readyEvents.push_back(mePtr);
    thread = std::thread(func, mePtr);
  }
  if (wait) {
    WaitForReady();
  }
}
void ThreadPool::WaitForReady() {
  for (auto& mePtr : m_readyEvents) {
    mePtr->Wait();
  }
}
ThreadPool::~ThreadPool() {
  for (auto& thread : m_workerThreads) {
    thread.join();
  }
}

}  // namespace engine
}  // namespace pemo