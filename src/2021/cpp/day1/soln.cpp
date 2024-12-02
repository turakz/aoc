#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

auto findWindowDepthIncreaseFrequency(const std::vector<std::uint32_t>& depths, const std::size_t windowSize) -> std::uint32_t
{
  auto idx = std::size_t{0};
  auto f = std::uint32_t{0};
  while (idx < depths.size() && idx + windowSize < depths.size())
  {
    f += (depths[idx] < depths[idx + windowSize]);
    ++idx;
  }
  return f;
}

auto solvePuzzle(const std::string& inputFileName) -> void
{
  auto ifs = std::ifstream{inputFileName};
  if (!ifs.is_open())
  {
    std::cerr << "ERROR::solvePuzzle(const std::string&)::FAILED_TO_OPEN_FILE: {" << inputFileName << "}" << std::endl;
    return;
  }
  auto depths = std::vector<std::uint32_t>{};
  auto line = std::string{""};
  while (std::getline(ifs, line))
  {
    depths.push_back(std::stoi(line));
  }
  std::cout << "---part 1---" << std::endl;
  auto depthIncreaseFrequency = findWindowDepthIncreaseFrequency(depths, 1);
  std::cout << "soln: " << depthIncreaseFrequency << std::endl;
  std::cout << "---part 2---" << std::endl;
  depthIncreaseFrequency = findWindowDepthIncreaseFrequency(depths, 3);
  std::cout << "soln: " << depthIncreaseFrequency << std::endl;
}

auto main(void) -> int
{
  //solvePuzzle("example-input.txt");
  solvePuzzle("input.txt");
  return 0;
}
