//
// Created by poilk on 2024/2/10.
//

#include "naive_engine.h"

namespace pemo {
namespace engine {

VarHandle NaiveEngine::NewVariable() {
  return new NaiveVar();
}

OprHandle NaiveEngine::NewOperator(AsyncFunc func) {
  auto opr = new NaiveOpr();
  opr->m_func = func;
  return opr;
}

void NaiveEngine::Push(OprHandle op, Context execCtx) {
  NaiveOpr* opr = op->Cast<NaiveOpr>();
  opr->m_func(static_cast<RunContext>(execCtx));
}

void NaiveEngine::PushSync(Engine::SyncFunc func, Context execCtx) {
  auto opr = new NaiveOpr();
  opr->m_func = func;
  Push(opr, execCtx);
}
void NaiveEngine::PushAsync(Engine::AsyncFunc func, Context execCtx) {
  PushSync(func, execCtx);
}
void NaiveEngine::WaitForAll() {
}

Engine* CreateNaiveEngine() {
  return new NaiveEngine();
}

}  // namespace engine
}  // namespace pemo
