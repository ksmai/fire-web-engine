#include <iostream>
#include "Process/DelayProcess.h"

FW::DelayProcess::DelayProcess(double timeToDelay):
  timeToDelay{timeToDelay},
  timeDelayedSoFar{0.0} {
}

void FW::DelayProcess::update(double dt) {
  timeDelayedSoFar += dt;
  if (timeDelayedSoFar >= timeToDelay) {
    succeed();
  }
}

void FW::DelayProcess::onSuccess() {
  std::cout << timeToDelay << " seconds have passed!\n";
}
