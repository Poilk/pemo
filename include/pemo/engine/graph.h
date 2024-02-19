//
// Created by poilk on 2024/2/15.
//

#ifndef PEMO_GRAPH_H
#define PEMO_GRAPH_H

#include <map>
#include <string>

#include "pemo/engine/engine_common.h"

namespace pemo {
namespace engine {

struct Node {
  // int32_t m_idx;
  AsyncFunc m_func;
  std::string m_name;
};

class Graph {
 public:
  void AddNode(AsyncFunc func, std::string name);
  void AddDependency(const std::string &srcNodeName, const std::string &dstNodeName);
  std::vector<Node> &GetNodes();
  std::vector<std::vector<int32_t>> &GetNodeDependency();

 private:
  std::vector<std::vector<int32_t>> m_nodeDependency;
  std::vector<Node> m_nodes;
  std::map<std::string, int32_t> m_nameToIdxMp;
  // int64_t m_version;
};

}  // namespace engine
}  // namespace pemo

#endif  // PEMO_GRAPH_H
