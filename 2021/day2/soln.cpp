#include <functional>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>

struct Position {
  std::uint32_t _aim;
  std::uint32_t _y;
  std::uint32_t _depth;
};

auto moveAimSubmarine(const Position& subPosition, const std::vector<std::pair<std::string, std::uint32_t>>& directionValuePairs) -> Position
{
  auto moveSubmarineForward = [](Position& subPos, std::uint32_t units)
  {
    subPos._y += units;
    subPos._depth += (subPos._aim * units);
  };
  auto moveSubmarineUp = [](Position& subPos, std::uint32_t units)
  {
    subPos._aim -= units;
  };
  auto moveSubmarineDown = [](Position& subPos, std::uint32_t units)
  {
    subPos._aim += units;
  };
  std::unordered_map<std::string, std::function<void(Position&, std::uint32_t)>> operateSubmarine
  {
    {"forward", moveSubmarineForward},
    {"up", moveSubmarineUp},
    {"down", moveSubmarineDown}
  };
  auto newPos = Position{subPosition._aim, subPosition._y, subPosition._depth};
  for (const auto& reading : directionValuePairs)
  {
    auto direction = reading.first;
    auto subOp = operateSubmarine.find(direction);
    if (subOp != std::end(operateSubmarine))
    {
      auto units = reading.second;
      subOp->second(newPos, units);
    }
  }
  return newPos;
}
auto moveSubmarine(const std::pair<std::uint32_t, std::uint32_t>& subPosition, const std::vector<std::pair<std::string, std::uint32_t>>& directionValuePairs) -> std::pair<std::uint32_t, std::uint32_t>
{
  auto moveSubmarineForward = [](std::pair<std::uint32_t, std::uint32_t>& subPos, std::uint32_t units)
  {
    subPos.first += units;
  };
  auto moveSubmarineUp = [](std::pair<std::uint32_t, std::uint32_t>& subPos, std::uint32_t units)
  {
    subPos.second -= units;
  };
  auto moveSubmarineDown = [](std::pair<std::uint32_t, std::uint32_t>& subPos, std::uint32_t units)
  {
    subPos.second += units;
  };
  std::unordered_map<std::string, std::function<void(std::pair<std::uint32_t, std::uint32_t>&, std::uint32_t)>> operateSubmarine
  {
    {"forward", moveSubmarineForward},
    {"up", moveSubmarineUp},
    {"down", moveSubmarineDown}
  };
  auto newPos = std::pair<std::uint32_t, std::uint32_t>{subPosition};
  for (const auto& reading : directionValuePairs)
  {
    auto direction = reading.first;
    auto subOp = operateSubmarine.find(direction);
    if (subOp != std::end(operateSubmarine))
    {
      auto units = reading.second;
      subOp->second(newPos, units);
    }
  }
  return newPos;
}

auto solvePuzzle(const std::string& inputFileName) -> void
{
  auto ifs = std::ifstream{inputFileName};
  if (!ifs.is_open())
  {
    std::cerr << "ERROR::solvePuzzle(const std::string&)::FAILED_TO_OPEN_FILE: {" << inputFileName << "}" << std::endl;
    return;
  }
  auto directionValuePairs = std::vector<std::pair<std::string, std::uint32_t>>{};
  auto line = std::string{""};
  while (std::getline(ifs, line))
  {
    auto direction = std::string{""};
    auto value = std::uint32_t{0};
    auto ss = std::stringstream{line};
    ss >> direction >> value;
    directionValuePairs.push_back({direction, value});
    std::cout << "dir: " << direction << ", value: " << value << std::endl;
  }
  std::cout << "---part 1---" << std::endl;
  std::cout << "start position: (horizontal: " << 0 << ", depth: " << 0 << ")" << std::endl;
  auto newPos = moveSubmarine({0, 0}, directionValuePairs);
  std::cout << "new position: (horizontal: " << newPos.first << ", depth: " << newPos.second << "), soln: " << (newPos.first * newPos.second) << std::endl;
  std::cout << "---part 2---" << std::endl;
  std::cout << "start position: (aim: " << 0 << ", horizontal: " << 0 << ", depth: " << 0 << ")" << std::endl;
  auto finalPos = moveAimSubmarine(Position{0, 0, 0}, directionValuePairs);
  std::cout << "final position: (aim: " << finalPos._aim << ", horizontal: " << finalPos._y << ", depth: " << finalPos._depth << "), soln: " << (finalPos._y * finalPos._depth) << std::endl;
}

auto main(void) -> int
{
  //solvePuzzle("example-input.txt");
  solvePuzzle("input.txt");
  return 0;
}
