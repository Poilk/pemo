//
// Created by poilk on 2024/2/10.
//

#include <gtest/gtest.h>
#include "pemo/common/concurrent_queue.h"
#include <mutex>
#include <thread>

TEST(ConCurrentQueue, basic){
  int32_t sum = 0;
  std::mutex mtx;
  pemo::ConcurrentBlockingQueue<int32_t> cbq;

  auto readFunc = [&sum, &mtx, &cbq](){
    int32_t val;
    bool ret = cbq.Pop(&val);
    EXPECT_EQ(ret, true);
    {
      std::lock_guard<std::mutex> lockGuard(mtx);
      sum += val;
    }
  };
  auto writeFunc = [&cbq](int32_t val){
    cbq.Push(val);
  };
  std::vector<std::thread> threads;
  for(int i = 0; i < 5; i++){
    threads.push_back(std::thread(readFunc));
    threads.push_back(std::thread(writeFunc, i+1));
  }
  for(auto &thread : threads){
    thread.join();
  }
  EXPECT_EQ(sum, 15);
  EXPECT_EQ(cbq.Size(), 0);
}