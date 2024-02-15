//
// Created by poilk on 2024/2/10.
//

#ifndef PEMO_NAIVE_ENGINE_H
#define PEMO_NAIVE_ENGINE_H

#include <utility>

#include "engine_impl.h"

namespace pemo {
namespace engine {

class NaiveEngine : public Engine {
 public:
  void Process(SyncFunc func) override;
  void Process(Graph& graph) override;
  void Push(AsyncFunc func) override;
  //void Push(Graph& graph) override;

  void WaitForAll() override;
};

}  // namespace engine

}  // namespace pemo

#endif  // PEMO_NAIVE_ENGINE_H
