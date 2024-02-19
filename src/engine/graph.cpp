//
// Created by poilk on 2024/2/16.
//

#include "pemo/engine/graph.h"

#include <utility>

#include "pemo/common/log.h"

namespace pemo {
namespace engine {

void engine::Graph::AddNode(AsyncFunc func, std::string name) {
  m_nameToIdxMp[name] = m_nodes.size();
  m_nodes.push_back(Node{std::move(func), std::move(name)});
  m_nodeDependency.push_back({});
}

void Graph::AddDependency(const std::string &srcNodeName, const std::string &dstNodeName) {
  if (!m_nameToIdxMp.count(srcNodeName) || !m_nameToIdxMp.count(dstNodeName)) {
    PEMO_LOG_ERROR("AddDependency node name not in nameMap! srcName:%s, dstName:%s", srcNodeName.c_str(),
                   dstNodeName.c_str());
    return;
  }
  int32_t srcIdx = m_nameToIdxMp.at(srcNodeName);
  int32_t dstIdx = m_nameToIdxMp.at(dstNodeName);
  assert(srcIdx > 0 && srcIdx < m_nameToIdxMp.size());
  m_nodeDependency[srcIdx].push_back(dstIdx);
}

std::vector<Node> &Graph::GetNodes() {
  return m_nodes;
}

std::vector<std::vector<int32_t>> &Graph::GetNodeDependency() {
  return m_nodeDependency;
}

}  // namespace engine
}  // namespace pemo