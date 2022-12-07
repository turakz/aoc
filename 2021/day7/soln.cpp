#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

// brute force
auto solvePuzzle(const std::string& inputFileName, const char delim) -> void
{
  auto ifs = std::fstream{inputFileName};
  auto minPos = std::int64_t{0};
  auto maxPos = std::int64_t{0};
  auto positions = std::vector<int64_t>{};
  if (ifs.is_open())
  {
    auto horizontalPosition = std::string{""};
    while (std::getline(ifs, horizontalPosition, delim))
    {
      auto position = int64_t{std::stoi(horizontalPosition)};
      minPos = std::min(minPos, position);
      maxPos = std::max(maxPos, position);
      positions.push_back(std::move(position));
    }
  }
  else
  {
    std::cerr << "ERROR::solvePuzzle(const std::string&)::FILE_FAILED_TO_OPEN: {" << inputFileName << "}" << std::endl;
    return;
  }
  // part 1: for every possible position in the range of numbers we've seen in our input
  auto minCost = std::numeric_limits<int64_t>::max();
  for (int64_t start = minPos; start <= maxPos; ++start)
  {
    auto cost = int64_t{0};
    // calculate each crab's cost to move from start to end (all possible moves)
    for (const auto& end : positions)
    {
      cost += std::abs(start - end);
    }
    minCost = std::min(cost, minCost);
  }
  std::cout << "---part 1---" << std::endl;
  std::cout << "soln: " << minCost << std::endl;
  // part 2: for every possible position
  auto sumToDistance = [](int64_t n)
  {
    auto sum = int64_t{0};
    for (int64_t i = 0; i < n; ++i)
    {
      sum += (i + 1);
    }
     return sum;
  };
  auto minCostSum = std::numeric_limits<int64_t>::max();
  // calculate a sum of the distance between start to end (all possible distances)
  for (int64_t start = minPos; start <= maxPos; ++start)
  {
    auto cost = int64_t{0};
    for (const auto& end : positions)
    {
      cost += sumToDistance(std::abs(start - end));
    }
    minCostSum = std::min(cost, minCostSum);
  }
  std::cout << "---part 2--" << std::endl;
  std::cout << "soln: " << minCostSum << std::endl;
}


auto main(void) -> int
{
  const auto delim = char{','};
  //solvePuzzle("example-input.txt", delim);
  solvePuzzle("input.txt", delim);
  return 0;
}
