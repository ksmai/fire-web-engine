#include <algorithm>
#include <functional>
#include "Process/ProcessRunner.h"

void FW::ProcessRunner::update(double dt) {
  std::size_t numProcesses = processes.size();
  for (std::size_t i = 0; i < numProcesses; ++i) {
    auto currentProcess = processes[i].get();

    if (currentProcess->isUninitialized()) {
      currentProcess->init();
      currentProcess->onInit();
    }

    if (currentProcess->isRunning()) {
      currentProcess->update(dt);
    }

    if (currentProcess->isDone()) {
      if (currentProcess->isSucceeded()) {
        currentProcess->onSuccess();
        Process::StrongPtr child = currentProcess->removeChild();
        if (child) {
          attachProcess(std::move(child));
        }
      } else if (currentProcess->isAborted()) {
        currentProcess->onAbort();
      }
      currentProcess->remove();
    }
  }

  processes.erase(
    std::remove_if(
      processes.begin(),
      processes.end(),
      std::mem_fn(&FW::Process::isRemoved)
    ),
    processes.end()
  );
}

void FW::ProcessRunner::attachProcess(Process::StrongPtr process) {
  processes.push_back(std::move(process));
}
