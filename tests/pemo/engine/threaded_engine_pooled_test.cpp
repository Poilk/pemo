//
// Created by poilk on 2024/2/10.
//

#include <gtest/gtest.h>

#include <thread>

#include "engine_impl.h"

using namespace pemo::engine;

void ThreadedEnginePooledBasicTest(Engine *engine, int32_t workerThreadNum) {
  std::atomic<int32_t> count{};
  const int32_t sleepMilliseconds = 1000;
  const SyncFunc foolIncFunc = [&count, &sleepMilliseconds]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMilliseconds));
    count++;
  };
  const SyncFunc foolDecFunc = [&count]() { count--; };

  {
    // test push
    count = 0;
    Context ctx;

    const int32_t nTimesOfWt = 3;
    {
      const auto start = std::chrono::high_resolution_clock::now();

      for (int i = 0; i < workerThreadNum * nTimesOfWt; i++) {
        engine->Push(foolIncFunc);
        engine->Push(foolDecFunc);
      }

      const auto end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double, std::milli> elapsed = end - start;

      // push should be nonblocking;
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

  {
    // test Process Graph
    count = 0;

    const int32_t nTimesOfWt = 3;
    {
      const auto start = std::chrono::high_resolution_clock::now();

      Graph graph;
      for (int i = 0; i < workerThreadNum * nTimesOfWt; i++) {
        graph.AddNode(foolIncFunc, "foolIncFunc");
        graph.AddNode(foolDecFunc, "foolIncFunc");
      }
      engine->Process(graph);

      const auto end = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double, std::milli> elapsed = end - start;

      EXPECT_EQ(count, 0);
      EXPECT_GE(elapsed.count(), nTimesOfWt * sleepMilliseconds - 100);
      EXPECT_LE(elapsed.count(), nTimesOfWt * sleepMilliseconds + 100);
    }
  }
}

TEST(ThreadedEnginePooled, basic) {
  for (int i = 1; i <= 8; i <<= 1) {
    auto engine = pemo::engine::CreateThreadedEnginePooled(i);
    ThreadedEnginePooledBasicTest(engine, i);
    // todo memory leak?
    // delete engine;
  }
}