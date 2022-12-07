#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>

auto parseFileLines(const std::filesystem::path& filePath) -> std::vector<std::string>
{
  auto fs = std::ifstream {filePath};
  if (!fs.good())
  {
    std::cerr << "ERROR::main.cpp::parseCalories(const std::filesystem::path&)::FILE_{" << filePath << "}_DOES_NOT_EXIST" << std::endl;
    return std::vector<std::string> {};
  }
  auto data = std::vector<std::string> {};
  auto line = std::string {};
  while (std::getline(fs, line))
  {
    data.push_back(std::move(line));
  }
  return data;
}

auto findMaxSumCalories(const std::vector<std::string>& fileData) -> std::uint32_t
{
  auto maxCalorySum = std::uint32_t {0};
  if (!fileData.empty())
  {
    auto localCalorySum = std::uint32_t {0};
    for (const auto& line : fileData)
    {
      if (line.empty())
      {
        maxCalorySum = std::max(maxCalorySum, localCalorySum);
        localCalorySum = 0;
      }
      else
      {
        auto ss = std::stringstream {line};
        auto calory = std::uint32_t {0};
        ss >> calory;
        localCalorySum += calory;
      }
    }
  }
  else std::cerr << "ERROR::main.cpp::findMaxSumCalories(const std::vector<std::string>&)::FILE_DATA_EMPTY" << std::endl;
  return maxCalorySum;
}

auto findTopNMaxSumCaloriesSum(const std::vector<std::string>& fileData, const std::size_t n) -> std::uint32_t
{
  auto topNCalorySum = std::uint32_t {0};
  if (!fileData.empty())
  {
    auto calorySums = std::vector<std::uint32_t> {};
    auto localCalorySum = std::uint32_t {0};
    for (const auto& line : fileData)
    {
      if (line.empty())
      {
        calorySums.push_back(localCalorySum);
        localCalorySum = 0;
      }
      else
      {
        auto ss = std::stringstream {line};
        auto calory = std::uint32_t {0};
        ss >> calory;
        localCalorySum += calory;
      }
    }
    if (localCalorySum != 0) calorySums.push_back(localCalorySum);
    std::sort(std::begin(calorySums), std::end(calorySums));
    topNCalorySum = std::accumulate(std::begin(calorySums) + calorySums.size() - n, std::end(calorySums), std::uint32_t {0});
  }
  else std::cerr << "ERROR::main.cpp::findMaxSumCalories(const std::vector<std::string>&)::FILE_DATA_EMPTY" << std::endl;
  return topNCalorySum;
}

auto solve(const std::filesystem::path& filePath) -> void
{
  std::cout << "aoc::problem::input_file: " << filePath << std::endl;
  std::cout << "solving..." << std::endl;
  const auto fileLines = parseFileLines(filePath);
  // part 1: find max sum calories
  // -----------------------------
  std::cout << "part 1: find max sum calories" << std::endl;
  std::cout << "soln: " << findMaxSumCalories(fileLines) << std::endl;
  const auto n = std::size_t {3};
  std::cout << "part2: find top n {" << n << "} sum calories" << std::endl;
  std::cout << "soln: " << findTopNMaxSumCaloriesSum(fileLines, n) << std::endl;
}

auto main(int32_t argc, char* argv[]) -> std::int32_t
{
  if (argc == 0 || argc < 2 || argc > 2)
  {
    std::cerr << "ERROR::main.cpp: bad cmd line args" << std::endl;
    return -1;
  }
  const auto filePathIdx = std::size_t {1};
  const auto filePath = std::filesystem::path {argv[filePathIdx]};
  solve(filePath);
  return 0;
}
