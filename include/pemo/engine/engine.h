//
// Created by poilk on 2024/2/9.
//

#ifndef PEMO_ENGINE_H
#define PEMO_ENGINE_H

#include <stdlib.h>

#include <functional>

namespace pemo {
class Engine;

namespace engine {

struct Var {
  virtual size_t version() {
    return m_version;
  }
  virtual ~Var() = default;

  template <typename T>
  inline T* Cast();

  size_t m_version;
};

class Opr;

// Variable pointer type, usually hold by user used to specify dependencies
typedef Var* VarHandle;
// Operator pointer type, hold by user
typedef Opr* OprHandle;

}  // namespace engine

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
  typedef std::function<void(RunContext)> SyncFunc;
  typedef std::function<void(RunContext)> AsyncFunc;
  //  typedef engine::VarHandle VarHandle;
  //  typedef engine::OprHandle OprHandle;

 public:
  virtual ~Engine() = default;
  virtual engine::VarHandle NewVariable() = 0;
  virtual engine::OprHandle NewOperator(AsyncFunc func, std::vector<engine::VarHandle> const& constVars,
                                        std::vector<engine::VarHandle> const& mutableVars) = 0;
  virtual void Push(engine::OprHandle op, Context execCtx) = 0;
  virtual void PushSync(SyncFunc, Context execCtx) = 0;
  virtual void PushAsync(AsyncFunc, Context execCtx) = 0;
  virtual void WaitForAll() = 0;
  static Engine* Get();
};

}  // namespace pemo

#endif  // PEMO_ENGINE_H
