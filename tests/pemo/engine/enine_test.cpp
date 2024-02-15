//
// Created by poilk on 2024/2/10.
//
#include <gtest/gtest.h>

#include <atomic>
#include <functional>

#include "engine_impl.h"
#include "pemo/common/log.h"

using namespace pemo::engine;

void EngineBasicTest(Engine *engine) {
  std::atomic<int32_t> count{};
  const SyncFunc foolIncFunc = [&count]() {
    // PEMO_LOG_INFO("foolIncFunc");
    count++;
  };
  const SyncFunc foolDecFunc = [&count]() {
    // PEMO_LOG_INFO("foolDecFunc");
    count--;
  };

  {
    // test push
    engine->Push(foolIncFunc);
    engine->WaitForAll();
    EXPECT_EQ(count, 1);
    engine->Push(foolIncFunc);
    engine->Push(foolIncFunc);
    engine->WaitForAll();
    EXPECT_EQ(count, 3);
    engine->Push(foolDecFunc);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
    engine->Push(foolDecFunc);
    engine->Push(foolIncFunc);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
  }

  {
    // test process
    count = 0;
    engine->Process(foolIncFunc);
    engine->WaitForAll();
    EXPECT_EQ(count, 1);
    engine->Process(foolIncFunc);
    engine->Process(foolIncFunc);
    engine->WaitForAll();
    EXPECT_EQ(count, 3);
    engine->Process(foolDecFunc);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
    engine->Process(foolDecFunc);
    engine->Process(foolIncFunc);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
  }
}

TEST(Engine, basic) {
  auto naiveEngine = CreateNaiveEngine();
  EngineBasicTest(naiveEngine);
  auto pooledEngine = CreateDefaultThreadedEnginePooled();
  EngineBasicTest(pooledEngine);

  auto pooledEngine2 = CreateThreadedEnginePooled(4);
  EngineBasicTest(pooledEngine2);
}