//
// Created by poilk on 2024/2/10.
//

#include <gtest/gtest.h>

#include <thread>

#include "engine_impl.h"

using namespace pemo;

void ThreadedEnginePooledBasicTest(Engine *engine, int32_t workerThreadNum) {
  std::atomic<int32_t> count{};
  const int32_t sleepMilliseconds = 1000;
  const Engine::SyncFunc foolIncFunc = [&count, &sleepMilliseconds](Engine::RunContext) {
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMilliseconds));
    count++;
  };
  const Engine::SyncFunc foolDecFunc = [&count](Engine::RunContext) { count--; };

  {
    // test push
    count = 0;
    Context ctx;
    engine::OprHandle incOprHandle = engine->NewOperator(foolIncFunc, {}, {});
    engine::OprHandle decOprHandle = engine->NewOperator(foolDecFunc, {}, {});

    const int32_t nTimesOfWt = 3;
    {
      const auto start = std::chrono::high_resolution_clock::now();

      for (int i = 0; i < workerThreadNum * nTimesOfWt; i++) {
        engine->Push(incOprHandle, ctx);
        engine->Push(decOprHandle, ctx);
      }

      const auto end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double, std::milli> elapsed = end - start;

      //push should be nonblocking;
      EXPECT_LE(elapsed.count(), 100);
    }

    {
      const auto start = std::chrono::high_resolution_clock::now();

      engine->WaitForAll();

      const auto end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double, std::milli> elapsed = end - start;

      EXPECT_GE(elapsed.count(), nTimesOfWt * sleepMilliseconds - 100);
      EXPECT_LE(elapsed.count(), nTimesOfWt * sleepMilliseconds + 100);

    }
    EXPECT_EQ(count, 0);
  }
}

TEST(ThreadedEnginePooled, basic) {
  for(int i = 1; i <= 8; i<<=1){
    auto engine = pemo::engine::CreateThreadedEnginePooled(i);
    ThreadedEnginePooledBasicTest(engine, i);
    //todo memory leak?
    //delete engine;
  }
}