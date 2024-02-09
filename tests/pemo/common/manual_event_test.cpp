//
// Created by poilk on 2024/2/9.
//

#include "pemo/common/manual_event.h"

#include <chrono>
#include <thread>

#include "gtest/gtest.h"

TEST(ManualEvent, Base) {
  pemo::ManualEvent event;
  // test 01
  {
    EXPECT_EQ(event.IsSignaled(), false);
    event.Signal();
    const auto start = std::chrono::high_resolution_clock::now();
    event.Wait();
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsed = end - start;
    EXPECT_GE(elapsed.count(), 0);
    EXPECT_LE(elapsed.count(), 100);
    EXPECT_EQ(event.IsSignaled(), true);
  }

  // test 02
  event.Reset();
  EXPECT_EQ(event.IsSignaled(), false);
  {
    constexpr int sleepMilliseconds = 2000;
    auto func = [&event, &sleepMilliseconds]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(sleepMilliseconds));
      event.Signal();
    };
    std::thread t(func);
    const auto start = std::chrono::high_resolution_clock::now();
    event.Wait();
    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> elapsed = end - start;
    EXPECT_GE(elapsed.count(), sleepMilliseconds - 100);
    EXPECT_LE(elapsed.count(), sleepMilliseconds + 100);
    EXPECT_EQ(event.IsSignaled(), true);
    t.join();
  }
}
