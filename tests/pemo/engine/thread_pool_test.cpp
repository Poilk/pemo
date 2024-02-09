//
// Created by poilk on 2024/2/9.
//

#include "thread_pool.h"

#include "gtest/gtest.h"

using namespace pemo;
using namespace pemo::engine;

TEST(ThreadPool, Base) {
  std::atomic<int32_t> count{};
  std::mutex mutex;
  {
    auto func = [&count]() { count++; };
    ThreadPool threadPool(10, func);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(count, 10);
  }
  count = 0;
  {
    auto func = [&count](const std::shared_ptr<ManualEvent>& ready) {
      ready->Signal();
      count++;
    };
    ThreadPool threadPool(10, func, true);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(count, 10);
  }
}