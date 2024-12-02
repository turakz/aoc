#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <numeric>

auto solvePuzzle(const std::string& inputFileName, uint64_t maxDays, const char delim) -> void 
{
  auto timerToPopulation = std::unordered_map<uint64_t, uint64_t>{{0, 0},{1, 0},{2, 0},{3, 0},{4, 0},{5, 0},{6, 0},{7, 0},{8, 0}};
  auto ifs = std::ifstream{inputFileName};
  if (ifs.is_open())
  {
    auto fish = std::string{""};
    while (std::getline(ifs, fish, delim))
    {
      auto fishTimer = std::stoi(fish);
      ++timerToPopulation[fishTimer];
    }
  }
  else
  {
    std::cerr << "ERROR::solvePuzzle(const std::string&, uint64_t, const char)::FAILED_TO_OPEN_FIL: {" << inputFileName << "}" << std::endl;
  }
  auto day = uint64_t{0};
  while (day < maxDays)
  {
    auto totalFishResetting = timerToPopulation[0];
    timerToPopulation[0] = timerToPopulation[1];
    timerToPopulation[1] = timerToPopulation[2];
    timerToPopulation[2] = timerToPopulation[3];
    timerToPopulation[3] = timerToPopulation[4];
    timerToPopulation[4] = timerToPopulation[5];
    timerToPopulation[5] = timerToPopulation[6];
    timerToPopulation[6] = timerToPopulation[7];
    timerToPopulation[7] = timerToPopulation[8];
    // update
    timerToPopulation[6] += totalFishResetting;
    timerToPopulation[8] = totalFishResetting;
    ++day;
  }
  auto totalFish = uint64_t{0};
  for (const auto& fish : timerToPopulation)
  {
    auto timerPopulation = fish.second;
    totalFish += timerPopulation; 
  }
  std::cout << "soln: " << totalFish << std::endl;
}

auto main(void) -> int
{
  auto delim = char{','};
  auto testDays = uint64_t{18};
  solvePuzzle("example-input.txt", testDays, delim);
  auto days = uint64_t{256};
  solvePuzzle("input.txt", days, delim);
  return 0;
}
