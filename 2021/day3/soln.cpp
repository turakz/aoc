#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

struct BitFrequency {
  char _mostFrequentBit;
  char _leastFrequentBit;
  bool _haveEqualFrequency;
};
// power consumption
// -----------------
auto countColBitFrequencies(const std::vector<std::string>& binaryLines) -> std::vector<BitFrequency>
{
  auto colIdx = std::size_t{0};
  std::vector<BitFrequency> bitFrequencies(binaryLines[colIdx].size(), BitFrequency{});
  while (colIdx < binaryLines[colIdx].size())
  {
    auto rowIdx = std::size_t{0};
    auto bitCounts = std::unordered_map<char, std::uint32_t>{};
    while (rowIdx < binaryLines.size())
    {
      auto bitChar = binaryLines[rowIdx][colIdx];
      ++bitCounts[bitChar];
      ++rowIdx;
    }
    auto mostFrequentBit = char{(bitCounts['0'] > bitCounts['1'] ? '0' : '1')};
    auto leastFrequentBit = char{(mostFrequentBit == '1' ? '0' : '1')};
    auto haveEqualFrequency = bool{bitCounts['0'] == bitCounts['1']};
    bitFrequencies[colIdx] = std::move(BitFrequency{mostFrequentBit, leastFrequentBit, haveEqualFrequency});
    ++colIdx;
  }
  return bitFrequencies; 
}

auto findPowerConsumptionRating(const std::string& gammaRating, const std::string& epsilonRating) -> std::uint32_t
{
  if (gammaRating.empty() || epsilonRating.empty()) return std::uint32_t{0};
  return std::uint32_t{static_cast<uint32_t>(std::stoi(gammaRating, 0, 2)) * static_cast<uint32_t>(std::stoi(epsilonRating, 0, 2))};
}

auto findGammaRating(const std::vector<BitFrequency>& bitCounts) -> std::string
{
  auto gammaRating = std::string{""};
  auto concactFunc = [&](const BitFrequency& bf)
  {
    return gammaRating += bf._mostFrequentBit; 
  };
  std::for_each(std::begin(bitCounts), std::end(bitCounts), concactFunc);
  return gammaRating;
}

auto findEpsilonRating(const std::vector<BitFrequency>& bitCounts) -> std::string
{
   auto epsilonRating = std::string{""};
   auto concactFunc = [&](const BitFrequency& bf)
   {
     return epsilonRating += bf._leastFrequentBit; 
   };
   std::for_each(std::begin(bitCounts), std::end(bitCounts), concactFunc);
   return epsilonRating;
}
// life support
// ------------
auto findLifeSupportRating(const std::string& oxygenRating, const std::string& c02Rating) -> std::uint32_t
{
  if (oxygenRating.empty() || c02Rating.empty()) return std::uint32_t{0};
  return std::uint32_t{static_cast<uint32_t>(std::stoi(oxygenRating, 0, 2)) * static_cast<uint32_t>(std::stoi(c02Rating, 0, 2))};
}

auto findOxygenRating(const std::vector<std::string>& binaryLines) -> std::string
{
  auto colIdx = std::size_t{0};
  auto solnSpace = std::vector<std::string>{binaryLines};
  while (colIdx < solnSpace[colIdx].size())
  {
    auto bitCounts = std::unordered_map<char, std::uint32_t>{};
    for (std::size_t rowIdx = 0; rowIdx < solnSpace.size(); ++rowIdx)
    {
      ++bitCounts[solnSpace[rowIdx][colIdx]];
    }
    auto mostFrequentBit = (bitCounts['1'] >= bitCounts['0'] ? '1' : '0');
    auto solns = std::vector<std::string>{};
    for (std::size_t rowIdx = 0; rowIdx < solnSpace.size(); ++rowIdx)
    {
      if (solnSpace[rowIdx][colIdx] == mostFrequentBit)
      {
        solns.push_back(solnSpace[rowIdx]);
      }
    }
    if (solnSpace.size() == 1) return solnSpace.front();
    solnSpace = solns;
    ++colIdx;
  } 
  return solnSpace.front();
}

auto findC02Rating(const std::vector<std::string>& binaryLines) -> std::string
{
  auto colIdx = std::size_t{0};
  auto solnSpace = std::vector<std::string>{binaryLines};
  while (colIdx < solnSpace[colIdx].size())
  {
    auto bitCounts = std::unordered_map<char, std::uint32_t>{};
    for (std::size_t rowIdx = 0; rowIdx < solnSpace.size(); ++rowIdx)
    {
      ++bitCounts[solnSpace[rowIdx][colIdx]];
    }
    auto leastFrequentBit = (bitCounts['0'] <= bitCounts['1'] ? '0' : '1');
    auto solns = std::vector<std::string>{};
    for (std::size_t rowIdx = 0; rowIdx < solnSpace.size(); ++rowIdx)
    {
      if (solnSpace[rowIdx][colIdx] == leastFrequentBit)
      {
        solns.push_back(solnSpace[rowIdx]);
      }
    }
    if (solnSpace.size() == 1) return solnSpace.front();
    solnSpace = solns;
    ++colIdx;
  } 
  return solnSpace.front();
}

auto solvePuzzle(const std::string& inputFileName) -> void
{
  auto ifs = std::ifstream{inputFileName};
  if (!ifs.is_open())
  {
    std::cerr << "ERROR::solvePuzzle(cons std::string&)::FAILED_TO_OPEN_FILE: {" << "}" << std::endl;
    return;
  }
  auto binaryLines = std::vector<std::string>{};
  auto line = std::string{""};
  while (std::getline(ifs, line))
  {
    binaryLines.push_back(line);
  }
  std::cout << "---part 1---" << std::endl; 
  auto colBitCounts = countColBitFrequencies(binaryLines);
  auto gammaRating = findGammaRating(colBitCounts);
  std::cout << "gamma rating: " << gammaRating << std::endl;
  auto epsilonRating = findEpsilonRating(colBitCounts);
  std::cout << "epsilon rating: " << epsilonRating << std::endl;
  auto powerConsumptionRating = findPowerConsumptionRating(gammaRating, epsilonRating);
  std::cout << "soln: " << powerConsumptionRating << std::endl;
  std::cout << "---part 2---" << std::endl;
  auto oxygenRating = findOxygenRating(binaryLines);
  std::cout << "oxygen rating: " << oxygenRating << std::endl;
  auto c02Rating = findC02Rating(binaryLines);
  std::cout << "c02 rating: " << c02Rating << std::endl;
  auto lifeSupportRating = findLifeSupportRating(oxygenRating, c02Rating);
  std::cout << "life support rating: " << lifeSupportRating << std::endl;
}

auto main(void) -> int
{
  //solvePuzzle("example-input.txt");
  solvePuzzle("input.txt");
  return 0;
}
