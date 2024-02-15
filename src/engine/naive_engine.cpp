//
// Created by poilk on 2024/2/10.
//

#include "naive_engine.h"

#include <utility>

namespace pemo {
namespace engine {

void NaiveEngine::WaitForAll() {
}

void NaiveEngine::Process(SyncFunc func) {
  func();
}

void NaiveEngine::Push(AsyncFunc func) {
  func();
}

Engine* CreateNaiveEngine() {
  return new NaiveEngine();
}

}  // namespace engine
}  // namespace pemo
