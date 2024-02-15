//
// Created by poilk on 2024/2/10.
//

#ifndef PEMO_THREADED_ENGINE_H
#define PEMO_THREADED_ENGINE_H

#include <condition_variable>
#include <mutex>

#include "engine_impl.h"

namespace pemo {
namespace engine {

struct OprBlock {
  AsyncFunc m_func;
  //Context m_ctx;
};

class ThreadedEngine : public Engine {
 public:
  void Process(SyncFunc func) override;
  void Push(AsyncFunc func) override;
  // todo only single-threaded push is supported, as using multiple threads may cause confusion in the  WaitForAll ???
  void WaitForAll() override;
  void Process(Graph& graph) override;
  virtual void PushToExecute(OprBlock oprBlock) = 0;
  ~ThreadedEngine() override = default;

 protected:
  void OnComplete();
  void OnStart();

 private:
  std::mutex m_finishedMtx;
  std::condition_variable m_finishedCv;
  std::atomic<int32_t> m_pending;
};  // class ThreadedEngine

}  // namespace engine
}  // namespace pemo

#endif  // PEMO_THREADED_ENGINE_H
