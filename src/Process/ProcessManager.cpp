#include <iostream>
#include "Process/ProcessManager.h"

void FW::ProcessManager::update(double dt) {
  auto it = processes.begin();
  while (it != processes.end()) {
    Process::StrongPtr currentProcess = *it;

    if (currentProcess->isUninitialized()) {
      currentProcess->init();
    }

    if (currentProcess->isRunning()) {
      currentProcess->update(dt);
    }

    if (currentProcess->isDead()) {
      if (currentProcess->isSucceeded()) {
        currentProcess->onSuccess();
        Process::StrongPtr child = currentProcess->removeChild();
        if (child) {
          attachProcess(child);
        }
      } else if (currentProcess->isFailed()) {
        currentProcess->onFail();
      } else if (currentProcess->isAborted()) {
        currentProcess->onAbort();
      }
      it = processes.erase(it);
    } else {
      ++it;
    }
  }
}

void FW::ProcessManager::attachProcess(Process::StrongPtr process) {
  processes.push_back(process);
}
