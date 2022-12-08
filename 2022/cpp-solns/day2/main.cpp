#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

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

auto parseStrategyCardRounds(const std::vector<std::string>& fileLines) -> std::vector<std::pair<char, char>>
{
  auto rounds = std::vector<std::pair<char, char>> {};
  if (!fileLines.empty())
  {
    for (const auto& line : fileLines)
    {
      auto ss = std::stringstream {line};
      auto lhs = char {};
      auto rhs = char {};
      ss >> lhs >> rhs;
      rounds.push_back(std::pair<char, char> {lhs, rhs});
    }
  }
  else std::cerr << "ERROR::parseStrategyCardRounds(const std::vector<std::string>&)::FILE_LINES_IS_EMPTY" << std::endl;
  return rounds;
}

auto computeStrategyCardScore(const std::vector<std::pair<char, char>>& rounds) -> uint32_t
{
  auto strategyCardScore = uint32_t {0};
  if (!rounds.empty())
  {
    auto choiceWeights = std::unordered_map<char, char>
    {
      {'X', 1},
      {'Y', 2},
      {'Z', 3}
    };
    auto win = std::unordered_map<char, char>
    {
      {'X', 'C'},
      {'Y', 'A'},
      {'Z', 'B'},
    };
    auto loss = std::unordered_map<char, char>
    {
      {'X', 'B'},
      {'Y', 'C'},
      {'Z', 'A'},
    };
    auto draw = std::unordered_map<char, char>
    {
      {'X', 'A'},
      {'Y', 'B'},
      {'Z', 'C'},
    };
    enum OUTCOMES {WIN, LOSS, DRAW};
    auto outcomeWeights = std::vector<uint32_t> {6, 0, 3};

    for (const auto& round : rounds)
    {
      const auto opponent = char {round.first};
      const auto suggestedStrat = char {round.second};
      strategyCardScore += choiceWeights[suggestedStrat];
      if (win[suggestedStrat] == opponent)
      {
        strategyCardScore += outcomeWeights[OUTCOMES::WIN];
      }
      else if (loss[suggestedStrat] == opponent)
      {
        strategyCardScore += outcomeWeights[OUTCOMES::LOSS];
      }
      else if (draw[suggestedStrat] == opponent)
      {
        strategyCardScore += outcomeWeights[OUTCOMES::DRAW];
      }
    }
  }
  else std::cerr << "ERROR::computeStrategyCardScore(const std::vector<std::pair<char, char>>&)::ROUNDS_IS_EMPTY" << std::endl;
  return strategyCardScore;
}

auto transformCardFromDesiredOutcomesToSuggestedStrategies(std::vector<std::pair<char, char>>& rounds) -> std::vector<std::pair<char, char>>&
{
  auto choiceWeights = std::unordered_map<char, char>
  {
    {'X', 1},
    {'Y', 2},
    {'Z', 3}
  };
  auto win = std::unordered_map<char, char>
  {
    {'C', 'X'},
    {'A', 'Y'},
    {'B', 'Z'},
  };
  auto loss = std::unordered_map<char, char>
  {
    {'B', 'X'},
    {'C', 'Y'},
    {'A', 'Z'},
  };
  auto draw = std::unordered_map<char, char>
  {
    {'A', 'X'},
    {'B', 'Y'},
    {'C', 'Z'},
  };
  enum OUTCOMES {WIN, LOSS, DRAW};
  auto outcomeWeights = std::vector<uint32_t> {6, 0, 3};
  auto desiredOutcomes = std::unordered_map<char, OUTCOMES>
  {
    {'X', OUTCOMES::LOSS},
    {'Y', OUTCOMES::DRAW},
    {'Z', OUTCOMES::WIN}
  };
  for (auto& round : rounds)
  {
    const auto opponent = char {round.first};
    const auto desiredOutcome = char {round.second};
    switch (desiredOutcomes[desiredOutcome])
    {
      case OUTCOMES::WIN:
        round.second = win[opponent];
        break;
      case OUTCOMES::LOSS:
        round.second = loss[opponent];
        break;
      case OUTCOMES::DRAW:
        round.second = draw[opponent];
        break;
    }
  }
  return rounds;
}

auto computeSuggestedStrategyCardScore(const std::vector<std::string>& fileLines) -> uint32_t
{
  auto score = std::uint32_t {0};
  if (!fileLines.empty())
  {
    auto rounds = parseStrategyCardRounds(fileLines); 
    score = computeStrategyCardScore(rounds);
  }
  else std::cerr << "ERROR::computeSuggestedStrategyCardScore(const std::vector<std::string>&)::FILE_LINES_IS_EMPTY" << std::endl;
  return score;
}

auto computeDesiredOutcomeStrategyCardScore(const std::vector<std::string>& fileLines) -> uint32_t
{
  auto score = uint32_t {0};
  if (!fileLines.empty())
  {
    auto rounds = parseStrategyCardRounds(fileLines);
    // note: part 2 can re-use the same card computation so long as the card is converted from desired outcome to suggested strat
    //  -> in other words, the suggested strat is whatever achieves the desired outcome
    //  -> therefore, we can overwrite the desired outcome in our input lines with suggested strats (transform), and then compute 
    // the card's score just like we computed it in part 1
    rounds = transformCardFromDesiredOutcomesToSuggestedStrategies(rounds);
    score = computeStrategyCardScore(rounds);
  }
  else std::cerr << "ERROR::computeDesiredOutcomeStrategyCardScore(const std::vector<std::pair<char, char>>&)::FILE_LINES_IS_EMPTY" << std::endl;
  return score;
}


auto solve(const std::filesystem::path& filePath) -> void
{
  std::cout << "aoc::problem::input_file: " << filePath << std::endl;
  std::cout << "solving..." << std::endl;
  const auto fileLines = parseFileLines(filePath);
  // part 1: compute suggested strategy card 
  // ---------------------------------------
  auto strategyCardScore = computeSuggestedStrategyCardScore(fileLines);
  std::cout << "part 1" << std::endl;
  std::cout << "suggested strategy card score: " << strategyCardScore << std::endl;
  // part 2: compute strategy card by desired outcome 
  // ------------------------------------------------
  strategyCardScore = computeDesiredOutcomeStrategyCardScore(fileLines);
  std::cout << "part 2" << std::endl;
  std::cout << "desired outcome strategy card score: " << strategyCardScore << std::endl;
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
