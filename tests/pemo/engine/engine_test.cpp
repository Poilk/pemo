//
// Created by poilk on 2024/2/10.
//
#include <gtest/gtest.h>

#include <atomic>
#include <functional>

#include "engine_impl.h"
#include "pemo/common/log.h"
#include "pemo/engine/graph.h"

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

  {
    // test Process Graph
    count = 0;
    Graph graph;
    graph.AddNode(foolIncFunc, "foolIncFunc");
    graph.AddNode(foolIncFunc, "foolIncFunc");
    engine->Process(graph);
    EXPECT_EQ(count, 2);
    graph.AddNode(foolDecFunc, "foolDecFunc");
    engine->Process(graph);
    EXPECT_EQ(count, 3);
  }

  {
    // test Process Dependency 1
    count = 0;
    Graph graph;
    auto fa = [&count]() {
      EXPECT_EQ(count, 0);
      count++;
    };
    auto fb = [&count]() {
      EXPECT_EQ(count, 1);
      count++;
    };
    auto fc = [&count]() {
      EXPECT_EQ(count, 2);
      count++;
    };
    auto fd = [&count]() {
      EXPECT_EQ(count, 3);
      count++;
    };
    graph.AddNode(fa, "fa");
    graph.AddNode(fb, "fb");
    graph.AddNode(fc, "fc");
    graph.AddNode(fd, "fd");
    graph.AddDependency("fb", "fa");
    graph.AddDependency("fc", "fb");
    graph.AddDependency("fc", "fa");
    graph.AddDependency("fd", "fc");
    graph.AddDependency("fd", "fb");

    engine->Process(graph);
    EXPECT_EQ(count, 4);
  }

  {
    // test Process Dependency 2
    count = 0;
    Graph graph;
    auto fa = [&count]() {
      EXPECT_EQ(count, 0);
      count++;
    };
    auto fb = [&count]() {
      EXPECT_GE(count, 1);
      EXPECT_LE(count, 2);
      count++;
    };
    auto fc = [&count]() {
      EXPECT_GE(count, 1);
      EXPECT_LE(count, 2);
      count++;
    };
    auto fd = [&count]() {
      EXPECT_EQ(count, 3);
      count++;
    };
    graph.AddNode(fa, "fa");
    graph.AddNode(fb, "fb");
    graph.AddNode(fc, "fc");
    graph.AddNode(fd, "fd");
    graph.AddDependency("fb", "fa");
    graph.AddDependency("fc", "fa");
    graph.AddDependency("fd", "fc");
    graph.AddDependency("fd", "fb");

    engine->Process(graph);
    EXPECT_EQ(count, 4);
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