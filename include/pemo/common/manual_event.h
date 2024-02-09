//
// Created by poilk on 2024/2/9.
//

#ifndef PEMO_MANUAL_EVENT_H
#define PEMO_MANUAL_EVENT_H

#include <atomic>
#include <mutex>

namespace pemo {
class ManualEvent {
 public:
  ManualEvent() : m_singled(false){};
  void Wait(){
    std::unique_lock<std::mutex> lock(m_mutex);
    if(!m_singled){
      m_cv.wait(lock);
    }
  }
  void Signal(){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_singled = true;
    m_cv.notify_all();
  }

  void Reset(){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_singled = false;
  }

  bool IsSignaled(){
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_singled;
  }

 private:
  std::mutex m_mutex;
  std::condition_variable m_cv;
  std::atomic<bool> m_singled;
};
}  // namespace pemo

#endif  // PEMO_MANUAL_EVENT_H
