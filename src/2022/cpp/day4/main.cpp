#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct section {
  std::uint32_t low;
  std::uint32_t high;
};

auto splitStr(const std::string& s, const char delim) -> std::pair<std::string, std::string>
{
  if (!s.empty())
  {
    auto splitPos = s.find(delim);
    const auto start = std::size_t {0};
    auto lhs = std::string {s.substr(start, splitPos)};
    auto rhs = std::string {s.substr(splitPos + 1, s.size() - lhs.size())};
    return std::pair<std::string, std::string> {lhs, rhs};
  }
  else std::cerr << "ERROR::main.cpp::splitStr(const std::string&, const char delim)::S_IS_EMPTY" << std::endl;
  return std::pair<std::string, std::string> {};
}

auto toNumber(const std::string& digit) -> std::uint32_t
{
  auto number = std::uint32_t {0}; 
  if (!digit.empty())
  {
    auto ss = std::stringstream {digit};
    ss >> number;
  }
  else std::cerr << "ERROR::main.cpp::toDigit(const std::string&)::DIGIT_IS_EMPTY" << std::endl;
  return number;
}

auto analyzeSectionPairRanges(const std::vector<std::pair<section, section>>& sectionPairRanges) -> std::pair<uint32_t, uint32_t>
{
  auto numberOfFullyContainedSubranges = std::uint32_t {0};
  auto numberOfOverlappingRanges = std::uint32_t {0};
  if (!sectionPairRanges.empty())
  {
    for (const auto& sectionPair : sectionPairRanges)
    {
      auto leftSection = sectionPair.first;
      auto rightSection = sectionPair.second;
      if ((leftSection.low <= rightSection.low && leftSection.high >= rightSection.high)
        || (leftSection.low >= rightSection.low && leftSection.high <= rightSection.high))
      {
        ++numberOfFullyContainedSubranges;
      }
      if (leftSection.low <= rightSection.high && leftSection.high >= rightSection.low)
      {
        ++numberOfOverlappingRanges;
      }
    }
  }
  else std::cerr << "ERROR::main.cpp::analyzeSectionPairRanges(const std::vector<std::pair<uint32_t, uint32_t>>&)::SECTION_PAIR_RANGES_IS_EMPTY" << std::endl;
  return std::pair<std::uint32_t, std::uint32_t> {numberOfFullyContainedSubranges, numberOfOverlappingRanges};
}

auto parseSectionPairRanges(const std::vector<std::pair<std::string, std::string>>& sectionPairs, const char sectionPairRangeDelim) -> std::vector<std::pair<section, section>>
{
  auto sectionPairRanges = std::vector<std::pair<section, section>> {};
  if (!sectionPairs.empty())
  {
    for (const auto& sectionPair : sectionPairs)
    {
      auto lhStr = splitStr(sectionPair.first, sectionPairRangeDelim);
      auto rhStr = splitStr(sectionPair.second, sectionPairRangeDelim);
      auto leftSectionRange = section {toNumber(lhStr.first), toNumber(lhStr.second)};
      auto rightSectionRange = section {toNumber(rhStr.first), toNumber(rhStr.second)};
      sectionPairRanges.push_back({leftSectionRange, rightSectionRange});
    }
  }
  else std::cerr << "ERROR::main.cpp::parseSectionPairRanges(const std::vector<std::pair<std::string, std::string>>&, const char)::SECTION_PAIRS_IS_EMPTY" << std::endl;
  return sectionPairRanges;
}

auto parseSectionPairs(const std::vector<std::string>& sectionPairLines, const char sectionPairDelim) -> std::vector<std::pair<std::string, std::string>>
{
  auto sectionPairs = std::vector<std::pair<std::string, std::string>> {};
  if (!sectionPairLines.empty())
  {
    for (const auto& sectionPairLine : sectionPairLines)
    {
      auto sectionPair = splitStr(sectionPairLine, sectionPairDelim);
      sectionPairs.push_back({sectionPair.first, sectionPair.second});
    }
  }
  else std::cerr << "ERROR::main.cpp::parseSectionPairs(const std::vector<std::string>&)::SECTION_PAIR_LINES_IS_EMPTY" << std::endl;
  return sectionPairs;
}

auto parseFileLines(const std::filesystem::path& filePath) -> std::vector<std::string>
{
  auto fs = std::ifstream {filePath};
  if (!fs.good())
  {
    std::cerr << "ERROR::main.cpp::parseFileLines(const std::filesystem::path&)::FILE_{" << filePath << "}_DOES_NOT_EXIST" << std::endl;
    return std::vector<std::string> {};
  }
  auto data = std::vector<std::string> {};
  auto line = std::string {};
  while (std::getline(fs, line))
  {
    data.push_back(line);
  }
  return data;
}

auto solve(const std::filesystem::path& filePath) -> void
{
  std::cout << "aoc::problem::input_file: " << filePath << std::endl;
  std::cout << "solving..." << std::endl;
  const auto fileLines = parseFileLines(filePath);
  // part 1: 
  // -----------------------------
  const auto sectionDelim = char {','};
  auto sectionPairs = parseSectionPairs(fileLines, sectionDelim);
  const auto sectionPairRangeDelim = char {'-'};
  auto sectionPairRanges = parseSectionPairRanges(sectionPairs, sectionPairRangeDelim);
  auto analyzedSections = analyzeSectionPairRanges(sectionPairRanges);
  std::cout << "part 1" << std::endl;
  std::cout << "number of sections fully contained by the other: " << analyzedSections.first << std::endl;
  // part 2:
  // -----------------------------
  std::cout << "part 2" << std::endl;
  std::cout << "number of overlapping sections: " << analyzedSections.second << std::endl;
}

auto main(int32_t argc, char* argv[]) -> std::int32_t
{
  if (argc == 0 || argc < 2 || argc > 2)
  {
    std::cerr << "ERROR::main.cpp::BAD_CMD_LINE_ARGS" << std::endl;
    return -1;
  }
  const auto filePathIdx = std::size_t {1};
  const auto filePath = std::filesystem::path {argv[filePathIdx]};
  solve(filePath);
  return 0;
}
