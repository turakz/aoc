/*
  problem: hypohermal vents

    -> given a of newline-delimited-text in the following form:

      (number1, number2) -> (number3, number4)

    -> where each text-line can be thought of as a line segment on the integer number line:

      LineSegment = (number1, number2) -> (number3, number4)

    -> and where each pair of numbers can be thought of as 2d coordinate points:

      Point = (number1, number2)

    -> we can then interpret each input line as:

      LineSegment = Point1 -> Point2
                  = (number1, number2) -> (number3, number4)
                  = (x1, y1) -> (x2, y2)

    -> and from there, answer the following questions:
      1. does a given coordinate in our space exist on any given line segment?

      2. how many points are contained by 2 or more lines?


  idea_land:
    -> our solution space is an NxM adjacency matrix, all cells initialized to 0 
    -> for each kind of line we enumerate all of the coordinates that make up the line
        => every enumerated point is an edge in our adjacency matrix, so we can simply add 1 to the
value at that coordinate (the coordinate position itself stores the amount of occurrences) using the adjacency matrix
    -> after we've processed every line, we can simply iterate over the coordinate space and count how many
coordinates, or edges have 2 or more occurrences

*/
#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>

struct Point {
  int x;
  int y;
};

struct LineSegment {
  Point start;
  Point end;
};

auto solvePuzzle(const std::string& inputFileName) -> void
{
  auto ifs = std::ifstream{inputFileName};
  auto max_x = std::numeric_limits<int>::min();
  auto max_y = std::numeric_limits<int>::min();
  auto lineSegments = std::vector<LineSegment>{};
  if (ifs.is_open())
  {
    auto line = std::string{""};
    while (std::getline(ifs, line))
    {
      // fuck it, parse it literally
      auto delim = char{'\0'};

      auto x1 = int{0};
      auto y1 = int{0};

      auto x2 = int{0};
      auto y2 = int{0};

      auto ss = std::stringstream{line};
      ss >> x1 >> delim >> y1 >> delim >> delim >> x2 >> delim >> y2;

      Point p1;
      p1.x = x1;
      p1.y = y1;

      Point p2;
      p2.x = x2;
      p2.y = y2;

      LineSegment lineSegment;
      lineSegment.start = p1;
      lineSegment.end = p2;
      lineSegments.push_back(std::move(lineSegment));

      max_x = std::max(max_x, std::max(p1.x, p2.x));
      max_y = std::max(max_y, std::max(p1.y, p2.y));
    }
  }
  else
  {
    std::cerr << "ERROR::solvePuzzle(const std::string&)::FAILED_TO_OPEN_FILE: {" << inputFileName << '}' << std::endl;
  }
  std::cout << "generating adjacency matrix and enumerating lines..." << std::endl; 
  std::cout << "max_x: " << max_x << std::endl;
  std::cout << "max_y: " << max_y << std::endl;
  if (max_x > 0 && max_y > 0)
  {
    std::vector<std::vector<int>> grid(max_y + 1, std::vector<int>(max_x + 1, 0));
    std::vector<std::vector<int>> diag(max_y + 1, std::vector<int>(max_x + 1, 0)); 
    for (const auto& lineSegment : lineSegments)
    {
      // oriented row-wise
      std::cout << "(" << lineSegment.start.x << ", " << lineSegment.start.y << ") -> (" << lineSegment.end.x << ", " << lineSegment.end.y << ")" << std::endl;
      // vertical
      if (lineSegment.start.x == lineSegment.end.x)
      {
        auto start_y = std::min(lineSegment.start.y, lineSegment.end.y);
        auto end_y = std::max(lineSegment.start.y, lineSegment.end.y);
        while (start_y <= end_y)
        {
          ++grid[start_y][lineSegment.start.x];
          ++start_y;
        }
      }
      // horizontal
      else if (lineSegment.start.y == lineSegment.end.y)
      {
        auto start_x = std::min(lineSegment.start.x, lineSegment.end.x);
        auto end_x = std::max(lineSegment.start.x, lineSegment.end.x);
        while (start_x <= end_x)
        {
          ++grid[lineSegment.start.y][start_x];
          ++start_x;
        }
      }
      // left to right diag
      else if (lineSegment.start.x - lineSegment.start.y == lineSegment.end.x - lineSegment.end.y)
      {
        auto start_x = std::min(lineSegment.start.x, lineSegment.end.x);
        auto end_x = std::max(lineSegment.start.x, lineSegment.end.x); 
        auto start_y = std::min(lineSegment.start.y, lineSegment.end.y);
        while (start_x <= end_x)
        {
          ++diag[start_y][start_x];
          ++start_x;
          ++start_y;
        }
      }
      // right to left diag 
      else if (lineSegment.start.x + lineSegment.start.y == lineSegment.end.x + lineSegment.end.y)
      {
        auto start_x = std::min(lineSegment.start.x, lineSegment.end.x);
        auto end_x = std::max(lineSegment.start.x, lineSegment.end.x); 
        auto end_y = std::max(lineSegment.start.y, lineSegment.end.y);
        while (start_x <= end_x)
        {
          ++diag[end_y][start_x];
          ++start_x;
          --end_y;
        }
      }
    }
    std::cout << "grid {" << std::endl;
    auto soln1 = int{0};
    auto soln2 = int{0};
    for (std::size_t i = 0; i < grid.size(); ++i)
    {
      std::cout << '\t' << '{' << ' ';
      for (std::size_t j = 0; j < grid[i].size(); ++j)
      {
        std::cout << grid[i][j] + diag[i][j] << ' ';
        if (grid[i][j] > 1)
        {
          ++soln1;
        }
        if (grid[i][j] + diag[i][j] > 1)
        {
          ++soln2;
        }
      }
      std::cout << '}' << std::endl;
    }
    std::cout << '}' << std::endl;
    std::cout << "---part 1---" << std::endl;
    std::cout << "soln: " << soln1 << std::endl;
    std::cout << "---part 2---" << std::endl;
    std::cout << "soln: " << soln2 << std::endl;
  }
  else
  {
    std::cerr << "ERROR::solvePuzzle(const std::string&)::FAILED_TO_GENERATE_SOLN_SPACE" << std::endl;
  }
}

auto main(void) -> int
{
  //solvePuzzle("example-input.txt");
  solvePuzzle("input.txt");
  return 0;
}
