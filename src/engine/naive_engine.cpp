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
  Process(func);
}
void NaiveEngine::Process(Graph& graph) {
  const auto nodes = graph.GetNodes();
  for(const auto &node : nodes){
    Process(node.m_func);
  }
}

//void NaiveEngine::Push(Graph& graph) {
//  const auto nodes = graph.GetNodes();
//  for(const auto &node : nodes){
//    Push(node.m_func);
//  }
//}

Engine* CreateNaiveEngine() {
  return new NaiveEngine();
}

}  // namespace engine
}  // namespace pemo
