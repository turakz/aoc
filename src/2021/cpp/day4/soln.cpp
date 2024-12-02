/*
  problem: 
    -> you're still aboard the submarine deployed by the elves and you're well below
    the surface
    -> for some reason, a giant squid has attached itself to the submarine
    -> thankfully it wants to play a game a bingo
    -> in fact, it wants to play many games of bingo
    -> given a stream of numbers and a set of initial board states, determine the first board
    to have a winner, the last board to have a winner, and use each to compute a sum
      >> sum of all unmarked squares multiplied by the number that was just called
      before the (ith+1) board won
*/
#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>

class BingoNumberStream {
public:
  auto streamify(const std::string& inputLine) -> void
  {
    if (inputLine.empty()) _buffer = std::vector<int>{};
    auto readCount = std::size_t{0};
    auto ss = std::stringstream{inputLine};
    auto token = std::string{""};
    while (std::getline(ss, token, ','))
    {
      auto n = std::stoi(token);
      _buffer.push_back(std::move(n));
      ++readCount;
    }
    if (_buffer.size() != readCount)
    {
      std::cerr << "ERROR::BingoNumberStream::streamify(const std::string&)::FAILED_READ_BEFORE_EOF" << std::endl;
    }
  } 
  auto read() -> int
  {
    if (_readIdx < _buffer.size())
    {
      _current = _buffer[_readIdx];
      ++_readIdx;
      return _current;
    }
    return -1;
  }
  auto putback() -> void
  {
    if (_readIdx > 0)
    {
      --_readIdx;
      _current = _buffer[_readIdx];
    }
  }
  auto get() -> int
  {
    return _current;
  }
  auto good() -> bool
  {
    return (!_buffer.empty());
  }
  auto size() -> std::size_t
  {
    return _buffer.size();
  }
private:
  int _current;
  std::size_t _readIdx;
  std::vector<int> _buffer{};
};

class BingoBoard {
public:
  BingoBoard(const std::vector<std::string>& inputBoard)
  {
    if (inputBoard.empty()) _boardState = std::vector<std::vector<std::pair<int, int>>>{};
    for (auto inputRow : inputBoard)
    {
      auto boardRow = std::vector<std::pair<int, int>>{};
      auto ss = std::stringstream{inputRow};
      auto num = int{0};
      while (ss >> num)
      {
        boardRow.push_back({0, num});
      }
      _boardState.push_back(std::move(boardRow));
    }
  }
  auto updateBoardState(int n) -> void
  {
    for (auto& row : _boardState)
    {
      for (auto& square : row)
      {
        if (square.first == 0 && square.second == n)
        {
          square.first = 1;
        }
      }
    }
  }
  auto checkForWin() -> bool
  {
    auto gameState = bool{false};
    // check rows
    for (std::size_t rowIdx = 0; rowIdx < _boardState.size(); ++rowIdx)
    {
      auto markedSquaresCount = std::size_t{0};
      auto maxSquaresCount = std::size_t{5};
      for (std::size_t colIdx = 0; colIdx < _boardState[rowIdx].size(); ++colIdx)
      {
        auto square = _boardState[rowIdx][colIdx];
        markedSquaresCount += square.first;
      }
      gameState = (markedSquaresCount == maxSquaresCount);
      if (gameState) return gameState;
    }
    // check columns 
    for (std::size_t colIdx = 0; colIdx < _boardState[colIdx].size(); ++colIdx)
    {
      auto markedSquaresCount = std::size_t{0};
      auto maxSquaresCount = std::size_t{5};
      for (std::size_t rowIdx = 0; rowIdx < _boardState.size(); ++rowIdx)
      {
        auto square = _boardState[rowIdx][colIdx];
        markedSquaresCount += square.first;
      }
      gameState = (markedSquaresCount == maxSquaresCount);
      if (gameState) return gameState;
    }
    return gameState;
  }
  auto findUnmarkedSum() -> int
  {
    auto sum = int{0};
    for (const auto& row : _boardState)
    {
      for (const auto& square : row)
      {
        if (square.first == 0)
        {
          sum += square.second;
        }
      }
    }
    return sum;
  }
private:
  std::vector<std::vector<std::pair<int, int>>> _boardState;
};
auto solvePuzzle(const std::string& inputFileName) -> void
{
  auto boardStates = std::vector<BingoBoard>{};
  auto numberStream = BingoNumberStream{};
  auto ifs = std::ifstream{inputFileName};
  if (ifs.is_open())
  {
    std::cout << "---reading input file---" << std::endl;
    auto line = std::string{""};
    const auto maxBoardLineCount = std::size_t{5};
    while (ifs && std::getline(ifs, line))
    {
      if (!line.empty())
      {
        numberStream.streamify(line);
      }
      else if (line.empty())
      {
        auto inputBoard = std::vector<std::string>{};
        auto boardLineCount = std::size_t{0};
        while (boardLineCount < maxBoardLineCount && std::getline(ifs, line))
        {
          inputBoard.push_back(line);
          ++boardLineCount;
        }
        auto bingoBoard = BingoBoard(inputBoard);
        boardStates.push_back(std::move(bingoBoard));
      }
    }
    std::cout << "total boards: " << boardStates.size() << std::endl;
    std::cout << "total numbers in stream: " << numberStream.size() << std::endl;
    std::cout << "---finished reading input file---" << std::endl;
  }
  else
  {
    std::cerr << "ERROR::solvePuzzle(const std::string&)::FAILED_TO_OPEN_FILE: {" << inputFileName << '}' << std::endl;
    return;
  }
  std::cout << "processing games..." << std::endl;
  auto isWinner = bool{false};
  auto lastWinner = std::size_t{0};
  auto firstSoln = int{0};
  auto lastSoln = int{0};
  auto hasWon = std::unordered_map<std::size_t, bool>{};
  while (numberStream.good() && numberStream.read() >= 0)
  {
    auto n = numberStream.get();
    std::cout << "number called: " << n << std::endl;
    for (std::size_t i = 0; i < boardStates.size(); ++i)
    {
      boardStates[i].updateBoardState(n);
      isWinner = boardStates[i].checkForWin();
      if (isWinner && hasWon.find(i) == std::end(hasWon))
      {
        std::cout << "-------------------------" << std::endl;
        std::cout << "-------------------------" << std::endl;
        std::cout << "----------WINNER---------" << std::endl;
        std::cout << "-------------------------" << std::endl;
        std::cout << "-------------------------" << std::endl;
        std::cout << "last number read: " << n << std::endl;
        auto unmarkedSum = boardStates[i].findUnmarkedSum();
        lastSoln = (unmarkedSum * n);
        if (firstSoln == 0) firstSoln = lastSoln;
        std::cout << "soln: " << lastSoln << std::endl;
        lastWinner = i;
        hasWon[i] = true;
      }
    }
  }
  std::cout << "finished processing games..." << std::endl;
  std::cout << "--part 1---" << std::endl;
  std::cout << "soln: " << firstSoln << std::endl;
  std::cout << "---part 2---" << std::endl;
  std::cout << "board " << lastWinner+1 << " is the last board to win, soln: " << lastSoln << std::endl;
}

auto main(void) -> int
{
  //solvePuzzle("example-input.txt");
  solvePuzzle("input.txt");
  return 0;
}
