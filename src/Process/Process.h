#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <memory>

namespace FW {
  class Process {
  public:
    enum class State {
      Uninitialized,
      Removed,
      Running,
      Paused,
      Succeeded,
      Failed,
      Aborted
    };

    using StrongPtr = std::shared_ptr<Process>;
    using WeakPtr = std::weak_ptr<Process>;

    Process() =default;

    Process(const Process&) =delete;

    Process& operator=(const Process&) =delete;

    virtual ~Process() {
    }

    virtual void init() {
      state = State::Running;
    }

    virtual void update(double dt) =0;

    virtual void onSuccess() {
    }

    virtual void onFail() {
    }

    virtual void onAbort() {
    }

    void succeed() {
      state = State::Succeeded;
    }

    void fail() {
      state = State::Failed;
    }

    void pause() {
      state = State::Paused;
    }

    void unpause() {
      state = State::Running;
    }

    bool isUninitialized() const {
        return state == State::Uninitialized;
    }

    bool isRemoved() const {
      return state == State::Removed;
    }

    bool isAlive() const {
      return state == State::Running || state == State::Paused;
    }

    bool isRunning() const {
      return state == State::Running;
    }

    bool isPaused() const {
      return state == State::Paused;
    }

    bool isDead() const {
      return state == State::Succeeded || state == State::Failed || state == State::Aborted;
    }

    bool isSucceeded() const {
      return state == State::Succeeded;
    }

    bool isFailed() const {
      return state == State::Failed;
    }

    bool isAborted() const {
      return state == State::Aborted;
    }

    void attachChild(StrongPtr newChild) {
      child = newChild;
    }

    StrongPtr removeChild() {
      StrongPtr oldChild = child;
      child.reset();
      return oldChild;
    }

    StrongPtr peekChild() const {
      return child;
    }

  private:
    void setState(State newState) {
      state = newState;
    }

    State state = State::Uninitialized;
    StrongPtr child;
  };
}

#endif
