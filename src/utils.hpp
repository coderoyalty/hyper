#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <random>
#include <iomanip>
#include <sstream>

std::string generateUUID()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 15);

  std::stringstream ss;
  for (int i = 0; i < 36; ++i)
  {
    if (i == 8 || i == 13 || i == 18 || i == 23)
    {
      ss << '-';
    }
    else if (i == 14)
    {
      ss << '4';
    }
    else if (i == 19)
    {
      ss << (dis(gen) & 0x3 | 0x8); // Generates a random hexadecimal in the form 8, 9, A, or B
    }
    else
    {
      ss << std::hex << dis(gen);
    }
  }
  return ss.str();
}

#endif