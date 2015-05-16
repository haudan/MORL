#include "Util.hpp"

#include <thread>

void SleepMs(unsigned ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

unsigned TypeIdHelper::count = 0;