//
// Created by poilk on 2024/2/10.
//

#ifndef PEMO_ENGINE_IMPL_H
#define PEMO_ENGINE_IMPL_H

#include "pemo/engine/engine.h"

namespace pemo {
namespace engine {
struct Opr {
  template <typename T>
  inline T* Cast();
};  // struct Opr

template<typename T>
inline T* Var::Cast(){
  static_assert(std::is_base_of<Var, T>::value, "must inherit `pemo::engine::Var`");
  //todo if debug
  //#if ENGINE_DEBUG
  // return dynamic_cast<T*>(this);
  //#endif
  return static_cast<T*>(this);
}

template<typename T>
inline T* Opr::Cast(){
  static_assert(std::is_base_of<Opr, T>::value, "must inherit `pemo::engine::Opr`");
  //todo if debug
  //#if ENGINE_DEBUG
  // return dynamic_cast<T*>(this);
  //#endif
  return static_cast<T*>(this);
}

Engine *CreateNaiveEngine();

}  // namespace engine
}  // namespace pemo

#endif  // PEMO_ENGINE_IMPL_H
