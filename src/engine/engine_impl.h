//
// Created by poilk on 2024/2/10.
//

#ifndef PEMO_ENGINE_IMPL_H
#define PEMO_ENGINE_IMPL_H

#include "pemo/engine/engine.h"

namespace pemo {
namespace engine {

Engine *CreateNaiveEngine();
Engine *CreateThreadedEnginePooled(int32_t workerThreadNum);
Engine *CreateDefaultThreadedEnginePooled();

}  // namespace engine
}  // namespace pemo

#endif  // PEMO_ENGINE_IMPL_H
