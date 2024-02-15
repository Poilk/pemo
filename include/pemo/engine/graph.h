//
// Created by poilk on 2024/2/15.
//

#ifndef PEMO_GRAPH_H
#define PEMO_GRAPH_H

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
  // void AddDependency(std::string srcNodeName, std::string dstNodeName);
  std::vector<Node> &GetNodes();

 private:
  std::vector<Node> m_nodes;
  // int64_t m_version;
};

}  // namespace engine
}  // namespace pemo

#endif  // PEMO_GRAPH_H
