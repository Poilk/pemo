//
// Created by poilk on 2024/2/10.
//

#include "threaded_engine.h"

#include <utility>

namespace pemo {
namespace engine {

void ThreadedEngine::WaitForAll() {
  std::unique_lock<std::mutex> lock(m_finishedMtx);
  m_finishedCv.wait(lock, [this]() { return m_pending == 0; });
}

void ThreadedEngine::OnComplete() {
  {
    std::lock_guard<std::mutex> lockGuard(m_finishedMtx);
    m_pending--;
    if (!m_pending) {
      m_finishedCv.notify_all();
    }
  }
}

void ThreadedEngine::OnStart() {
}

void ThreadedEngine::Process(SyncFunc func) {
  // todo use pool to process???
  func();
}

void ThreadedEngine::Push(AsyncFunc func) {
  OprBlock oprBlock;
  oprBlock.m_func = func;
  ++m_pending;
  PushToExecute(oprBlock);
}

void ThreadedEngine::Process(Graph& graph) {
  const auto nodes = graph.GetNodes();
  for (const auto& node : nodes) {
    Push(node.m_func);
  }
  //todo wait for graph????
  WaitForAll();
}

}  // namespace engine
}  // namespace pemo