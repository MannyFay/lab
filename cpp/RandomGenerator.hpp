// A very old one I've found ...from 2018 in my apprenticeship
// (German "Ausbildung").
// This was my very first C++ project :)

#pragma once

#include <random>
#include <vector>

class RandomGenerator
{
  private:
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;
    int lowerBound;
    int upperBound;

  public:
    RandomGenerator(int lower, int upper)
    {
      lowerBound = lower;
      upperBound = upper;
      std::random_device rd;
      generator = std::mt19937(rd());
      distribution = std::uniform_int_distribution<int>(lowerBound, upperBound);
    }

    int generate()
    {
      return distribution(generator);
    }

    std::vector<int> generateMultiple(int count)
    {
      std::vector<int> numbers;
      for (int i = 0; i < count; i++)
      {
        numbers.push_back(generate());
      }

      return numbers;
    }
};
