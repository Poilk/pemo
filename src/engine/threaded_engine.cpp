//
// Created by poilk on 2024/2/10.
//

#include "threaded_engine.h"

namespace pemo {
namespace engine {

VarHandle ThreadedEngine::NewVariable() {
  return nullptr;
}

OprHandle ThreadedEngine::NewOperator(Engine::AsyncFunc func) {
  auto opr = new ThreadedOpr();
  opr->m_func = func;

  return opr;
}

void ThreadedEngine::Push(OprHandle op, Context execCtx) {
  auto opr = op->Cast<ThreadedOpr>();
  OprBlock oprBlock;
  oprBlock.m_opr = opr;
  oprBlock.m_ctx = execCtx;
  ++m_pending;
  PushToExecute(oprBlock);
}

void ThreadedEngine::PushSync(Engine::SyncFunc func, Context execCtx) {
  // todo impl
  PushAsync(func, execCtx);
}

void ThreadedEngine::PushAsync(Engine::AsyncFunc func, Context execCtx) {
  // todo impl
  auto opr = new ThreadedOpr();
  opr->m_func = func;
  Push(opr, execCtx);
}

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

}  // namespace engine
}  // namespace pemo