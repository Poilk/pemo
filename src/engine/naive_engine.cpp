//
// Created by poilk on 2024/2/10.
//

#include "naive_engine.h"

#include <utility>

#include "pemo/common/base.h"

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
  const auto nodeDpd = graph.GetNodeDependency();
  std::vector<int32_t> vis(nodes.size(), 0);
  std::function<void(int32_t)> dfsFunc = [&](int32_t nodeIdx){
    for(int i = 0; i < nodeDpd[nodeIdx].size(); i++){
      if(!vis[nodeDpd[nodeIdx][i]]){
        dfsFunc(i);
      }
    }
    if(!vis[nodeIdx]){
      Process(nodes[nodeIdx].m_func);
      vis[nodeIdx] = true;
    }
  };
  for(int i = 0; i < nodes.size(); i++){
    dfsFunc(i);
  }
}

// void NaiveEngine::Push(Graph& graph) {
//   const auto nodes = graph.GetNodes();
//   for(const auto &node : nodes){
//     Push(node.m_func);
//   }
// }

Engine* CreateNaiveEngine() {
  return new NaiveEngine();
}

}  // namespace engine
}  // namespace pemo
