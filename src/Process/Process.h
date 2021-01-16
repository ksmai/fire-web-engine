#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <memory>

namespace FW {
  class Process {
  public:
    enum class State {
      Uninitialized,
      Running,
      Succeeded,
      Aborted
    };

    using StrongPtr = std::unique_ptr<Process>;

    Process() =default;

    Process(const Process&) =delete;

    Process& operator=(const Process&) =delete;

    virtual ~Process() {
    }

    virtual void onInit() {
    }

    virtual void update(double dt) =0;

    virtual void onSuccess() {
    }

    virtual void onAbort() {
    }

    void init() {
      state = State::Running;
    }

    void succeed() {
      state = State::Succeeded;
    }

    void abort() {
      state = State::Aborted;
    }

    bool isUninitialized() const {
      return state == State::Uninitialized;
    }

    bool isRunning() const {
      return state == State::Running;
    }

    bool isDone() const {
      return isSucceeded() || isAborted();
    }

    bool isSucceeded() const {
      return state == State::Succeeded;
    }

    bool isAborted() const {
      return state == State::Aborted;
    }

    void attachChild(StrongPtr newChild) {
      child = std::move(newChild);
    }

    StrongPtr removeChild() {
      StrongPtr oldChild = std::move(child);
      child.reset();
      return oldChild;
    }

  private:
    State state = State::Uninitialized;
    StrongPtr child;
  };
}

#endif
