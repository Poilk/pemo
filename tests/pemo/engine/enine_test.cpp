//
// Created by poilk on 2024/2/10.
//
#include <gtest/gtest.h>

#include <atomic>
#include <functional>

#include "engine_impl.h"
#include "pemo/common/log.h"

using namespace pemo;

void EngineBasicTest(Engine *engine) {
  std::atomic<int32_t> count{};
  const Engine::SyncFunc foolIncFunc = [&count](Engine::RunContext) {
    // PEMO_LOG_INFO("foolIncFunc");
    count++;
  };
  const Engine::SyncFunc foolDecFunc = [&count](Engine::RunContext) {
    // PEMO_LOG_INFO("foolDecFunc");
    count--;
  };

  {
    // test push sync
    Context ctx;
    engine->PushSync(foolIncFunc, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 1);
    engine->PushSync(foolIncFunc, ctx);
    engine->PushSync(foolIncFunc, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 3);
    engine->PushSync(foolDecFunc, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
    engine->PushSync(foolDecFunc, ctx);
    engine->PushSync(foolIncFunc, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
  }

  {
    // test push sync
    count = 0;
    Context ctx;
    engine->PushAsync(foolIncFunc, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 1);
    engine->PushAsync(foolIncFunc, ctx);
    engine->PushAsync(foolIncFunc, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 3);
    engine->PushAsync(foolDecFunc, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
    engine->PushAsync(foolDecFunc, ctx);
    engine->PushAsync(foolIncFunc, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
  }

  {
    // test push
    count = 0;
    Context ctx;
    engine::OprHandle incOprHandle = engine->NewOperator(foolIncFunc);
    engine::OprHandle decOprHandle = engine->NewOperator(foolDecFunc);
    engine->Push(incOprHandle, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 1);
    engine->Push(incOprHandle, ctx);
    engine->Push(incOprHandle, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 3);
    engine->Push(decOprHandle, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
    engine->Push(incOprHandle, ctx);
    engine->Push(decOprHandle, ctx);
    engine->WaitForAll();
    EXPECT_EQ(count, 2);
  }
}

TEST(Engine, basic) {
  auto naive_engine = engine::CreateNaiveEngine();
  EngineBasicTest(naive_engine);
}