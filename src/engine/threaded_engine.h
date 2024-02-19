//
// Created by poilk on 2024/2/10.
//

#ifndef PEMO_THREADED_ENGINE_H
#define PEMO_THREADED_ENGINE_H

#include <condition_variable>
#include <list>
#include <mutex>

#include "engine_impl.h"
#include "pemo/common/base.h"

namespace pemo {
namespace engine {

struct OprBlock {
  AsyncFunc m_func;
  std::string m_name;
  void* threadNodePtr = nullptr;
  // Context m_ctx;
};

enum ThreadNodeInsState {
  TNI_STATE_UNKNOWN = 0,
  TNI_STATE_WAIT_DEP = 1,
  TNI_STATE_PENDING_PROCESSING = 2,
  TNI_STATE_DONE = 3,
};

struct ThreadNodeIns {
  int32_t m_pendingNode{};
  ThreadNodeInsState m_state;
  OprBlock m_oprBlock;
  std::vector<ThreadNodeIns*> m_nextThreadNodePtrList;
};

struct ThreadGraphIns {
  std::vector<std::shared_ptr<ThreadNodeIns>> m_threadNodePtrList;
};

class ThreadedEngine : public Engine {
 public:
  ThreadedEngine() = default;
  void Process(SyncFunc func) override;
  void Process(Graph& graph) override;
  void Push(AsyncFunc func) override;
  // todo only single-threaded push is supported, as using multiple threads may cause confusion in the  WaitForAll ???
  void WaitForAll() override;
  virtual void PushToExecute(OprBlock oprBlock) = 0;
  ~ThreadedEngine() override = default;

 protected:
  void OnComplete(void* threadNode);
  void OnStart(void* threadNode);

 private:
  std::mutex m_mtx;
  std::condition_variable m_cv;
  std::atomic<int32_t> m_pending;

  std::list<ThreadGraphIns> m_graphInsList;

 private:
  DISALLOW_COPY_AND_ASSIGN(ThreadedEngine);
};  // class ThreadedEngine

}  // namespace engine
}  // namespace pemo

#endif  // PEMO_THREADED_ENGINE_H
