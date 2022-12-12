#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>

/*

if (!.empty())
{

}
else std::cerr << "ERROR::main.cpp::<functionName>::ERROR_MSG" << std::endl;

*/

// brute force without sliding window
auto containsDuplicate(const std::string& packet) -> bool
{
  auto seen = std::unordered_set<char> {};
  for (const auto& c : packet)
  {
    auto inserted = seen.insert(c).second;
    if (!inserted) return true;
  }
  return false;
}

// sliding window
auto findPacketMarker(const std::string& dataStream, const std::size_t maxPacketLen) -> std::pair<std::size_t, std::string>
{
  auto seen = std::unordered_set<char> {};
  auto windowStartIdx = std::size_t {0};
  for (std::size_t windowEndIdx = 0; windowEndIdx < dataStream.size(); ++windowEndIdx)
  {
    while (!seen.insert(dataStream[windowEndIdx]).second)
    {
      seen.erase(dataStream[windowStartIdx++]);
    }
    seen.insert(dataStream[windowEndIdx]);
    if (windowEndIdx - windowStartIdx + 1 == maxPacketLen)
    {
      return {windowEndIdx + 1, dataStream.substr(windowStartIdx, maxPacketLen)};
    }
  }
  std::cerr << "ERROR::main.cpp::findPacketMarker(const std::string&, const std::size_t)::DATA_STREAM_IS_EMPTY" << std::endl;
  return std::pair<std::size_t, std::string> {};
}

auto processDatastream(const std::vector<std::string>& fileLines) -> std::string
{
  auto dataStream = std::string {};
  if (!fileLines.empty() || fileLines.size() < 1 || fileLines.size() > 1)
  {
    const auto dataStreamIdx = std::size_t {0};
    dataStream = fileLines[dataStreamIdx];
  }
  else std::cerr << "ERROR::main.cpp::processDatastream(const std::vector<std::string>&)::FILE_LINES_IS_EMPTY" << std::endl;
  return dataStream;
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
  auto dataStream = processDatastream(fileLines);
  std::cout << "data stream: " << dataStream << std::endl;
  std::cout << "part 1" << std::endl;
  const auto maxPacketLen = std::size_t {4};
  auto [packetMarker, packetStr] = findPacketMarker(dataStream, maxPacketLen);
  std::cout << "packet marker: " << packetMarker << ", packet str: " << packetStr << std::endl;
  std::cout << "part 2" << std::endl;
  const auto maxMessageLen = std::size_t {14};
  auto [messageMarker, messageStr] = findPacketMarker(dataStream, maxMessageLen);
  // part 2:
  // -----------------------------
  std::cout << "message marker: " << messageMarker << ", message str: " << messageStr << std::endl;
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
