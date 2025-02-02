#include "Timer.h"
#include <time.h>

namespace brisbane {
namespace rt {

Timer::Timer() {
  for (int i = 0; i < BRISBANE_TIMER_MAX; i++) {
    total_[i] = 0.0;
    total_ull_[i] = 0ULL;
  }
  boot_ = 0.0;
  boot_ = Now();
}

Timer::~Timer() {

}

double Timer::Now() {
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t.tv_sec + 1.e-9 * t.tv_nsec - boot_;
}

size_t Timer::NowNS() {
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t.tv_sec * 1.e+9 + t.tv_nsec - 1601956000000000000;
}

double Timer::Start(int i) {
  double t = Now();
  start_[i] = t;
  return t;
}

double Timer::Stop(int i) {
  double t = Now() - start_[i];
  total_[i] += t;
  return t;
}

double Timer::Total(int i) {
  return total_[i];
}

size_t Timer::Inc(int i) {
  return Inc(i, 1UL);
}

size_t Timer::Inc(int i, size_t s) {
  total_ull_[i] += s;
  return total_ull_[i];
}

} /* namespace rt */
} /* namespace brisbane */
