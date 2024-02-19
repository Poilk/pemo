//
// Created by poilk on 2024/2/10.
//

#include "threaded_engine.h"

#include <assert.h>

#include <utility>

namespace pemo {
namespace engine {

void ThreadedEngine::WaitForAll() {
  std::unique_lock<std::mutex> lock(m_mtx);
  m_cv.wait(lock, [this]() { return m_pending == 0; });
}

void ThreadedEngine::OnComplete(void* threadNode) {
  std::lock_guard<std::mutex> lockGuard(m_mtx);
  m_pending--;
  if (threadNode) {
    // check node dependency
    ThreadNodeIns* threadNodeInsPtr = static_cast<ThreadNodeIns*>(threadNode);
    assert(threadNodeInsPtr->m_state == TNI_STATE_PENDING_PROCESSING);
    threadNodeInsPtr->m_state = TNI_STATE_DONE;
    for (auto& nextPtr : threadNodeInsPtr->m_nextThreadNodePtrList) {
      nextPtr->m_pendingNode--;
      if (!nextPtr->m_pendingNode) {
        // push
        assert(nextPtr->m_state == TNI_STATE_WAIT_DEP);
        nextPtr->m_state = TNI_STATE_PENDING_PROCESSING;
        ++m_pending;
        PushToExecute(nextPtr->m_oprBlock);
      }
    }
  }
  // check group done
  for (auto it = m_graphInsList.begin(); it != m_graphInsList.end();) {
    bool remove = true;
    for (const auto& nodePtr : it->m_threadNodePtrList) {
      if (nodePtr->m_state != TNI_STATE_DONE) {
        remove = false;
        break;
      }
    }
    if (remove) {
      // todo notify graph done???
      it = m_graphInsList.erase(it);
    } else {
      it++;
    }
  }
  // notify all node done
  if (!m_pending && m_graphInsList.empty()) {
    m_cv.notify_all();
  }
}

void ThreadedEngine::OnStart(void* threadNode) {
}

void ThreadedEngine::Process(SyncFunc func) {
  // todo use pool to process???
  func();
}

void ThreadedEngine::Push(AsyncFunc func) {
  OprBlock oprBlock;
  oprBlock.m_func = func;
  ++m_pending;
  PushToExecute(oprBlock);
}

void ThreadedEngine::Process(Graph& graph) {
  ThreadGraphIns graphIns;
  const auto nodes = graph.GetNodes();
  graphIns.m_threadNodePtrList.reserve(nodes.size());
  for (const auto& node : nodes) {
    auto ptr = std::shared_ptr<ThreadNodeIns>(new ThreadNodeIns());
    ptr->m_oprBlock.m_func = node.m_func;
    ptr->m_oprBlock.m_name = node.m_name;
    ptr->m_oprBlock.threadNodePtr = ptr.get();
    ptr->m_state = TNI_STATE_WAIT_DEP;
    graphIns.m_threadNodePtrList.push_back(ptr);
  }
  const auto nodesDep = graph.GetNodeDependency();
  for (int i = 0; i < nodesDep.size(); i++) {
    // graphIns.m_threadNodePtrList[i]->m_pendingNode++;
    for (const auto& dstIdx : nodesDep[i]) {
      graphIns.m_threadNodePtrList[dstIdx]->m_nextThreadNodePtrList.push_back(graphIns.m_threadNodePtrList[i].get());
      graphIns.m_threadNodePtrList[i]->m_pendingNode++;
    }
  }
  {
    std::lock_guard<std::mutex> lockGuard(m_mtx);
    for (const auto& threadNodePtr : graphIns.m_threadNodePtrList) {
      if (threadNodePtr->m_pendingNode == 0) {
        // push to execute
        assert(threadNodePtr->m_state == TNI_STATE_WAIT_DEP);
        threadNodePtr->m_state = TNI_STATE_PENDING_PROCESSING;
        ++m_pending;
        PushToExecute(threadNodePtr->m_oprBlock);
      }
    }
    m_graphInsList.push_back(std::move(graphIns));
  }

  WaitForAll();
}

}  // namespace engine
}  // namespace pemo