//
// Created by poilk on 2024/2/9.
//

#ifndef PEMO_ENGINE_H
#define PEMO_ENGINE_H

#include <stdlib.h>

#include <functional>

#include "pemo/engine/engine_common.h"
#include "pemo/engine/graph.h"

namespace pemo {

namespace engine {

struct Context {
  enum DeviceType { kCPU, KGPU };
  DeviceType m_deviceType;
  int32_t m_deviceId;
  Context() : m_deviceType(kCPU), m_deviceId(0) {
  }
  inline bool operator==(const Context& b) const {
    return m_deviceType == b.m_deviceType && m_deviceId == b.m_deviceId;
  }
};

class Engine {
 public:
  typedef Context RunContext;
  //  typedef engine::VarHandle VarHandle;
  //  typedef engine::OprHandle OprHandle;

 public:
  virtual ~Engine() = default;
  virtual void Process(SyncFunc func) = 0;
  virtual void Process(Graph &graph) = 0;
  virtual void Push(AsyncFunc) = 0;
  //virtual void Push(Graph& graph) = 0;
  // virtual void Push(SyncFunc, Context execCtx) = 0;
  //   virtual void Process(, Context execCtx) = 0;
  // virtual void PushAsync(AsyncFunc, Context execCtx) = 0;
  virtual void WaitForAll() = 0;
  //static Engine* Get();
};

}  // namespace engine
}  // namespace pemo

#endif  // PEMO_ENGINE_H
