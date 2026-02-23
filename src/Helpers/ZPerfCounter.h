#pragma once
#include <cstdint>

/**
 * simple performance counter to measure code execution time with microsecond precision. 
 */
class ZPerfCounter
{
  public:
    /**
     * Reset and Start the counter
     */
    void Start();

    /**
     * Stop the counter and return the elapsed time in microseconds 
     */
    double Stop();

  private:
    uint64_t m_nFrequency = 0;
    uint64_t m_nStartTicks = 0;
};
