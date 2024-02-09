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

void NaiveEngine::Push(OprHandle op, Context exec_ctx) {
  NaiveOpr* opr = op->Cast<NaiveOpr>();
  opr->m_func(static_cast<RunContext>(exec_ctx));
}

void NaiveEngine::PushSync(Engine::SyncFunc func, Context exec_ctx) {
  func(static_cast<RunContext>(exec_ctx));
}

Engine* CreateNaiveEngine() {
  return new NaiveEngine();
}

}  // namespace engine
}  // namespace pemo
