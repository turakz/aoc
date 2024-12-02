#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

/*

if (!.empty())
{

}
else std::cerr << "ERROR::main.cpp::<functionName>::ERROR_MSG" << std::endl;

*/

enum CRANE_MODEL {CrateMover9000, CrateMover9001};

struct Move {
  std::size_t nCrates;
  std::size_t startIdx;
  std::size_t destIdx;
};

auto convertAlphaToWhitespace(std::string& str) -> std::string&
{
  for (auto& c : str)
  {
    if (std::isdigit(c)) continue;
    c = ' ';
  }
  return str;
}

auto splitLinesIntoCrateStacksAndMoves(const std::vector<std::string>& fileLines) -> std::pair<std::vector<std::string>, std::vector<std::string>>
{
  auto crateLines = std::vector<std::string> {};
  auto moveLines = std::vector<std::string> {};
  if (!fileLines.empty())
  {
    auto isMoveLine = bool {false};
    for (const auto& line : fileLines)
    {
      if (line.empty())
      {
        isMoveLine = true;
        continue;
      }
      if (!isMoveLine)
      {
        crateLines.push_back(line);
      }
      else 
      {
        moveLines.push_back(line);
      }
    }
  }
  else std::cerr << "ERROR::main.cpp::splitLinesIntoCrateStacksAndMoves(const std::vector<std::string>&)::FILE_LINES_IS_EMPTY" << std::endl;
  return std::pair<std::vector<std::string>, std::vector<std::string>>{crateLines, moveLines};
}

auto readLinesIntoCrateMoves(const std::vector<std::string>& moveLines) -> std::vector<Move>
{
  auto crateMoves = std::vector<Move> {};
  if (!moveLines.empty())
  {
    for (const auto& moveLine : moveLines) 
    {
      auto tmpMoveLine = std::string {moveLine};
      auto ss = std::stringstream {convertAlphaToWhitespace(tmpMoveLine)};
      auto nCrates = std::size_t {0};
      auto startIdx = std::size_t {0};
      auto destIdx = std::size_t {0};
      ss >> nCrates >> startIdx >> destIdx;
      crateMoves.push_back(Move {nCrates, startIdx, destIdx});
    }
  }
  else std::cerr << "ERROR::main.cpp::readLinesIntoCrateMoves(const std::vector<std::string>&)::MOVE_LINES_IS_EMPTY" << std::endl;
  return crateMoves;
}

auto readLinesIntoCrateStacks(const std::vector<std::string>& crateLines) -> std::vector<std::vector<char>>
{
  auto crateStacks = std::vector<std::vector<char>> {};
  if (!crateLines.empty())
  {
    for (std::size_t rowIdx = 0; rowIdx < crateLines.size(); ++rowIdx)
    {
      for (std::size_t colIdx = 0; colIdx < crateLines[rowIdx].size(); ++colIdx)
      {
        if (std::isdigit(crateLines[rowIdx][colIdx]))
        {
          auto crateStack = std::vector<char> {};
          for (std::size_t vIdx = rowIdx; vIdx > 0; --vIdx)
          {
            if (std::isspace(crateLines[vIdx - 1][colIdx])) continue;
            crateStack.push_back(crateLines[vIdx - 1][colIdx]);
          }
          crateStacks.push_back(crateStack);
        }
      }
    }
  }
  else std::cerr << "ERROR::main.cpp::readLinesIntoCrateStacks(const std::vector<std::string>&)::CRATE_LINES_IS_EMPTY" << std::endl;
  return crateStacks;
}

auto processCrateStacksMove(std::vector<std::vector<char>>& crateStacks, const Move& crateMove, const CRANE_MODEL craneModel) -> std::vector<std::vector<char>>&
{
  if (!crateStacks.empty())
  {
    auto nCrates = crateMove.nCrates;
    auto startStk = std::move(crateStacks[crateMove.startIdx - 1]);
    auto destStk = std::move(crateStacks[crateMove.destIdx - 1]);
    auto cleanup = bool {false};
    for (std::size_t n = nCrates; n > 0; --n)
    {
      if (nCrates == 1 || craneModel == CRANE_MODEL::CrateMover9000)
      {
        destStk.push_back(startStk.back());
        startStk.pop_back();
      }
      else if (craneModel == CRANE_MODEL::CrateMover9001)
      {
        destStk.push_back(startStk[startStk.size() - n]);
        cleanup = true;
      }
    }
    if (cleanup)
    {
      startStk.erase(
          std::begin(startStk) + startStk.size() - nCrates,
          std::end(startStk)
        );
    }
    crateStacks[crateMove.startIdx - 1] = std::move(startStk);
    crateStacks[crateMove.destIdx - 1] = std::move(destStk);
  }
  else std::cerr << "ERROR::main.cpp::processCrateStacksMove(const std::vector<std::vector<char>>&, const Move& crateMove, const CRANE_MODEL)::CRATE_STACKS_IS_EMPTY" << std::endl;
  return crateStacks;
}

auto processCrateStacks(const std::vector<std::vector<char>>& crateStacks, const std::vector<Move>& crateMoves, const CRANE_MODEL craneModel) -> std::string
{
  auto crateStackLabelsSequence = std::string {};
  if (!crateStacks.empty())
  {
    // preserve original input state
    auto localCrateStacks = std::vector<std::vector<char>> {crateStacks};
    for (const auto& crateMove : crateMoves)
    {
        localCrateStacks = processCrateStacksMove(localCrateStacks, crateMove, craneModel);
    }
    for (const auto& crateStack : localCrateStacks)
    {
      crateStackLabelsSequence += crateStack.back();
    }
  }
  else std::cerr << "ERROR::main.cpp::processCrateStacks(std::vector<std::vector<char>>&, const std::vector<Move>&, const CRANE_MODEL)::CRATE_STACKS_IS_EMPTY" << std::endl;
  return crateStackLabelsSequence;
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
  auto [crateStacksLines, crateMovesLines] = splitLinesIntoCrateStacksAndMoves(fileLines);
  auto crateStacks = readLinesIntoCrateStacks(crateStacksLines);
  auto crateMoves = readLinesIntoCrateMoves(crateMovesLines);
  // part 1: 
  // -----------------------------
  auto topCratesLabelSequence = processCrateStacks(crateStacks, crateMoves, CRANE_MODEL::CrateMover9000);
  std::cout << "part 1" << std::endl;
  std::cout << "top crates label sequence: " << topCratesLabelSequence << std::endl;
  // part 2:
  // -----------------------------
  topCratesLabelSequence = processCrateStacks(crateStacks, crateMoves, CRANE_MODEL::CrateMover9001);
  std::cout << "part 2" << std::endl;
  std::cout << "top crates label sequence: " << topCratesLabelSequence << std::endl;
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
