//
// Created by poilk on 2024/2/9.
//

#ifndef PEMO_THREAD_POOL_H
#define PEMO_THREAD_POOL_H

#include <memory>
#include <thread>
#include <vector>

#include "pemo/common/manual_event.h"

namespace pemo {

namespace engine {

class ThreadPool {
 public:
  explicit ThreadPool(int32_t size, const std::function<void()>& func);
  explicit ThreadPool(int32_t size, const std::function<void(std::shared_ptr<ManualEvent> ready)>& func, bool wait);
  ~ThreadPool();

 private:
  void WaitForReady();

 private:
  std::vector<std::thread> m_workerThreads;
  std::vector<std::shared_ptr<ManualEvent>> m_readyEvents;
};

}  // namespace engine
}  // namespace pemo

#endif  // PEMO_THREAD_POOL_H
