//
// Created by poilk on 2024/2/16.
//

#include "pemo/engine/graph.h"

#include <utility>

namespace pemo {
namespace engine {

void engine::Graph::AddNode(AsyncFunc func, std::string name) {
  m_nodes.push_back(Node{std::move(func), std::move(name)});
}
std::vector<Node> &Graph::GetNodes() {
  return m_nodes;
}

}  // namespace engine
}  // namespace pemo