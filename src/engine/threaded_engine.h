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

class ThreadedVar final : public Var {};  // struct Threaded

struct ThreadedOpr final : public Opr {
  Engine::AsyncFunc m_func;
};  // struct ThreadedOpr

struct OprBlock {
  ThreadedOpr* m_opr{nullptr};
  Context m_ctx;
};

class ThreadedEngine : public Engine {
 public:
  VarHandle NewVariable() override;
  OprHandle NewOperator(AsyncFunc func) override;
  void Push(OprHandle op, Context execCtx) override;
  void PushSync(SyncFunc func, Context execCtx) override;
  void PushAsync(AsyncFunc func, Context execCtx) override;
  // todo only single-threaded push is supported, as using multiple threads may cause confusion in the  WaitForAll ???
  void WaitForAll() override;
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
