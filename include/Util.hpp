#pragma once

#include <memory>

template <typename T, typename... Args>
std::unique_ptr<T> MakeUnique(Args && ...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// OMG, I must be tired.
void SleepMs(unsigned ms);