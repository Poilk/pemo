//
// Created by poilk on 2024/2/10.
//

#ifndef PEMO_NAIVE_ENGINE_H
#define PEMO_NAIVE_ENGINE_H

#include <utility>

#include "engine_impl.h"

namespace pemo {
namespace engine {

class NaiveVar final : public Var {
 public:
  inline static NaiveVar *CastFromBase(Var *ptr) {
    return ptr->Cast<NaiveVar>();
  }
};

class NaiveEngine : public Engine {
 public:
  struct NaiveOpr : public Opr {
    //  explicit NaiveOpr(AsyncFunc func): m_func(std::move(func)){
    //  }
    AsyncFunc m_func;
    std::vector<VarHandle> m_constVars;
    std::vector<VarHandle> m_mutableVars;
  };

  VarHandle NewVariable() override;
  OprHandle NewOperator(AsyncFunc func, std::vector<VarHandle> const &constVars,
                        std::vector<VarHandle> const &mutableVars) override;
  void Push(OprHandle op, Context execCtx) override;
  void PushAsync(AsyncFunc func, Context execCtx) override;
  void PushSync(SyncFunc func, Context execCtx) override;
  void WaitForAll() override;
};

}  // namespace engine

}  // namespace pemo

#endif  // PEMO_NAIVE_ENGINE_H
