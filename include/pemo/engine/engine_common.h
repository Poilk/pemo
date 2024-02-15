//
// Created by poilk on 2024/2/15.
//

#ifndef PEMO_ENGINE_COMMON_H
#define PEMO_ENGINE_COMMON_H

#include <functional>

namespace pemo {
namespace engine {

typedef std::function<void()> SyncFunc;
typedef std::function<void()> AsyncFunc;

}  // namespace engine
}  // namespace pemo

#endif  // PEMO_ENGINE_COMMON_H
