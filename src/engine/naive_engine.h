//
// Created by poilk on 2024/2/10.
//

#ifndef PEMO_NAIVE_ENGINE_H
#define PEMO_NAIVE_ENGINE_H

#include <utility>

#include "pemo/common/base.h"
#include "engine_impl.h"

namespace pemo {
namespace engine {

class NaiveEngine : public Engine {
 public:
  NaiveEngine() = default;
  void Process(SyncFunc func) override;
  void Process(Graph& graph) override;
  void Push(AsyncFunc func) override;
  //void Push(Graph& graph) override;

  void WaitForAll() override;
 private:
  DISALLOW_COPY_AND_ASSIGN(NaiveEngine);
};

}  // namespace engine

}  // namespace pemo

#endif  // PEMO_NAIVE_ENGINE_H
