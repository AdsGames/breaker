#include "Timer.h"

// Available modes, fit to needs
template double Timer::GetElapsedTime<milliseconds>();
template double Timer::GetElapsedTime<seconds>();

// Init
Timer::Timer()
    : t1(high_resolution_clock::now()),
      t2(high_resolution_clock::now()),
      running(false) {}

// Start Timer
void Timer::Start() {
  t1 = high_resolution_clock::now();
  t2 = high_resolution_clock::now();
  running = true;
}

// Stop
void Timer::Stop() {
  running = false;
  t2 = high_resolution_clock::now();
}

// Is running
bool Timer::IsRunning() const {
  return running;
}

// Get ms since started
template <typename Precision>
double Timer::GetElapsedTime() {
  // Get time now
  if (running) {
    t2 = high_resolution_clock::now();
  }

  // Choose precision
  auto time_diff = duration_cast<Precision>(t2 - t1);

  // Return time as double
  return time_diff.count();
}
