//
// Created by poilk on 2024/2/10.
//

#include "assert.h"
#include "pemo/common/concurrent_queue.h"
#include "thread_pool.h"
#include "threaded_engine.h"

namespace pemo {
namespace engine {

class ThreadedEnginePooled : public ThreadedEngine {
 public:
  void PushToExecute(OprBlock oprBlock) override {
    m_cbq.Push(oprBlock);
  }
  ThreadedEnginePooled(int32_t workerThreadNum = 1);
  void ThreadWorker(std::shared_ptr<ManualEvent> ready);
  ~ThreadedEnginePooled() override = default;

 private:
  ConcurrentBlockingQueue<OprBlock> m_cbq;
  ThreadPool m_threadPool;
};

ThreadedEnginePooled::ThreadedEnginePooled(const int workerThreadNum)
    : m_threadPool(workerThreadNum, [this](std::shared_ptr<ManualEvent> ready) { ThreadWorker(ready); }, true) {
}

void ThreadedEnginePooled::ThreadWorker(std::shared_ptr<ManualEvent> ready) {
  ready->Signal();
  for (;;) {
    OprBlock opr;
    bool ret = m_cbq.Pop(&opr);
    assert(ret == true);
    OnStart();
    opr.m_opr->m_func(opr.m_ctx);
    OnComplete();
  }
}

Engine *CreateDefaultThreadedEnginePooled() {
  return new ThreadedEnginePooled();
}

Engine *CreateThreadedEnginePooled(int32_t workerThreadNum) {
  return new ThreadedEnginePooled(workerThreadNum);
}

}  // namespace engine

}  // namespace pemo