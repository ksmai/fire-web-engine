#include <algorithm>
#include "App/abort.h"
#include "App/Logger.h"
#include "Process/ProcessRunner.h"

const std::size_t FW::ProcessRunner::MAX_PROCESSES = 64;

void FW::ProcessRunner::update(double dt) {
  std::size_t numProcesses{processes.size()};
  std::uint64_t removedFlags{0uLL};
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
      removedFlags |= (1uLL << i);
    }
  }

  processes.erase(
    std::remove_if(
      processes.begin(),
      processes.end(),
      [this, removedFlags](const Process::StrongPtr& ptr) {
        std::size_t i{static_cast<std::size_t>(&ptr - &*processes.begin())};
        return removedFlags & (1uLL << i);
      }
    ),
    processes.end()
  );
}

void FW::ProcessRunner::attachProcess(Process::StrongPtr process) {
  if (processes.size() == MAX_PROCESSES) {
    Logger::error("Exceeded MAX_PROCESSES");
    abort();
  }
  processes.push_back(std::move(process));
}
