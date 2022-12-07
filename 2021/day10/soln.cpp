#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>

auto findCorruptionScore(const std::vector<std::string>& sequences) -> std::uint64_t
{
  const auto symbolPairs = std::unordered_map<char, char>
  {
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'},
  };
  const auto symbolScore = std::unordered_map<char, std::uint64_t>
  {
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}
  };
  auto corruptionScore = std::uint64_t{0};
  for (const auto& sequence : sequences)
  {
    auto symbolStack = std::stack<char>{};
    for (const auto& symbol : sequence)
    {
      if (symbolStack.empty() || symbolPairs.find(symbol) != std::end(symbolPairs))
      {
        symbolStack.push(symbol);
      }
      else if (!symbolStack.empty() && (symbolPairs.find(symbol) == std::end(symbolPairs)))
      {
        auto expectedSymbol = symbolPairs.find(symbolStack.top())->second;
        if (symbol != expectedSymbol)
        {
          auto score = symbolScore.find(symbol)->second;
          corruptionScore += score;
        }
        symbolStack.pop();
      }
    }
  }
  return corruptionScore;
}

auto parseIncompleteSequences(const std::vector<std::string>& sequences) -> std::vector<std::string>
{
  std::cout << "parsing..." << std::endl;
  const auto symbolPairs = std::unordered_map<char, char>
  {
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'},
  };
  auto isCorruptedSequence = std::unordered_map<std::size_t, bool>{};
  auto sequenceIdx = std::size_t{0};
  for (const auto& sequence : sequences)
  {
    auto symbolStack = std::stack<char>{};
    for (const auto& symbol : sequence)
    {
      if (symbolStack.empty() || symbolPairs.find(symbol) != std::end(symbolPairs))
      {
        symbolStack.push(symbol);
      }
      else if (!symbolStack.empty() && (symbolPairs.find(symbol) == std::end(symbolPairs)))
      {
        auto expectedSymbol = symbolPairs.find(symbolStack.top())->second;
        if (symbol != expectedSymbol)
        {
          isCorruptedSequence.insert({sequenceIdx, true});
        }
        symbolStack.pop();
      }
    }
    ++sequenceIdx;
  }
  auto incompleteSequences = std::vector<std::string>{};
  for (std::size_t idx = 0; idx < sequences.size(); ++idx)
  {
    auto sequence = sequences[idx];
    if (isCorruptedSequence.find(idx) == std::end(isCorruptedSequence))
    {
      incompleteSequences.push_back(sequence);
    }
  }
  return incompleteSequences;
}

auto findMissingSymbols(const std::vector<std::string>& sequences) -> std::vector<std::string> 
{
  auto incompleteSequences = parseIncompleteSequences(sequences);
  const auto symbolPairs = std::unordered_map<char, char>
  {
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'},
  };
  auto missingSymbolSequences = std::vector<std::string>{};
  for (const auto& sequence : incompleteSequences)
  {
    auto symbolStack = std::stack<char>{};
    std::cout << "analyzing sequence: " << sequence;
    for (const auto& symbol : sequence)
    {
      if (symbolStack.empty() || symbolPairs.find(symbol) != std::end(symbolPairs))
      {
        symbolStack.push(symbol);
      }
      else if (!symbolStack.empty() && (symbolPairs.find(symbol) == std::end(symbolPairs)))
      {
        symbolStack.pop();
      }
    }
    auto missingSymbols = std::string{""};
    std::cout << " -> malformed close symbols: ";
    while (!symbolStack.empty())
    {
      std::cout << symbolPairs.find(symbolStack.top())->second;
      missingSymbols += symbolPairs.find(symbolStack.top())->second;
      symbolStack.pop();
    }
    std::cout << std::endl;
    missingSymbolSequences.push_back(std::move(missingSymbols));
  }
  return missingSymbolSequences;
}

auto findMissingSymbolsCompletionScore(const std::vector<std::string>& sequences) -> std::uint64_t
{
  const auto symbolScore = std::unordered_map<char, std::uint64_t>
  {
    {')', 1},
    {']', 2},
    {'}', 3},
    {'>', 4}
  };
  const auto scoreWeight = std::uint64_t{5};
  auto missingSymbols = std::vector<std::string>{findMissingSymbols(sequences)};
  auto scores = std::vector<std::uint64_t>{};
  for (const auto& sequence : missingSymbols)
  {
    auto score = std::uint64_t{0};
    for (const auto& symbol : sequence)
    {
      score *= scoreWeight;
      score += symbolScore.find(symbol)->second;
    }
    std::cout << "sequence: " << sequence << ", score: " << score << std::endl;
    scores.push_back(score);
  }
  std::sort(std::begin(scores), std::end(scores));
  auto mid = std::size_t{scores.size()/2};
  return scores[mid];
}


auto solvePuzzle(const std::string& inputFileName) -> void
{
  auto ifs = std::ifstream{inputFileName};
  if (!ifs.is_open())
  {
    std::cerr << "ERROR::solvePuzzle(const std::string&)::FAILED_TO_OPEN_FILE: {" << inputFileName << "}" << std::endl;
    return;
  }
  auto line = std::string{""};
  auto lines = std::vector<std::string>{};
  while (std::getline(ifs, line))
  {
    std::cout << line << std::endl;
    lines.push_back(line);
  }
  auto soln = std::uint64_t{0};
  std::cout << "---part 1---" << std::endl;
  soln = findCorruptionScore(lines);
  std::cout << "soln: " << soln << std::endl;
  std::cout << "---part 2---" << std::endl;
  soln = findMissingSymbolsCompletionScore(lines);
  std::cout << "soln: " << soln << std::endl;
}

auto main(void) -> int
{
  //solvePuzzle("example-input.txt");
  solvePuzzle("input.txt");
  return 0;
}
