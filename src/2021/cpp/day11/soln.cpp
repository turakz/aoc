#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <algorithm>

auto is_valid_matrix_pos(const std::vector<std::vector<std::uint32_t>>& matrix, std::size_t i, std::size_t j) -> bool
{
  return (i >= 0 && i < matrix.size() && j >= 0 && j < matrix[i].size());
}

auto to_integer_matrix(const std::vector<std::string>& input_lines) -> std::vector<std::vector<std::uint32_t>>
{
  auto integer_matrix = std::vector<std::vector<std::uint32_t>>{};
  for (const auto& line : input_lines)
  {
    auto ints = std::vector<std::uint32_t>{};
    for (const auto& n : line)
    {
      ints.push_back(n - '0');
    }
    integer_matrix.push_back(std::move(ints));
  }
  return integer_matrix;
}
auto increment_matrix(std::vector<std::vector<std::uint32_t>>& matrix, const std::uint32_t energy_delta) -> std::vector<std::vector<std::uint32_t>>
{
  // increase
  for (std::size_t i = 0; i < matrix.size(); ++i)
  {
    for (std::size_t j = 0; j < matrix[i].size(); ++j)
    {
      matrix[i][j] += energy_delta;
    }
  }
  return std::move(matrix);
}
auto init_matrix_with_existing(std::vector<std::vector<std::uint32_t>>& matrix) -> std::vector<std::vector<std::uint32_t>>
{
  for (std::size_t i = 0; i < matrix.size(); ++i)
  {
    for (std::size_t j = 0; j < matrix[i].size(); ++j)
    {
      matrix[i][j] = 0;
    }
  }
  return std::move(matrix);
}

// BFS on neighbors
auto bfs_flashpoints(
    std::vector<std::vector<std::uint32_t>>& energy_matrix, 
    std::vector<std::vector<std::uint32_t>>& visited, 
    std::size_t curr_i, std::size_t curr_j) 
  -> std::uint32_t 
{
  auto adj_directions = std::vector<std::pair<std::size_t, std::size_t>>
  {
    // down
    {1, 0},
    // right
    {0, 1}, 
    // up
    {-1, 0}, 
    // left
    {0, -1}, 
    // lower right diag 
    {1, 1}, 
    // upper right diag
    {-1, 1},
    // lower left diag
    {1, -1},
    // upper left diag, 
    {-1, -1}
  };
  auto frontier = std::queue<std::pair<std::size_t, std::size_t>>{};
  frontier.push({curr_i, curr_j});
  visited[curr_i][curr_j] = 1;
  auto n_flashes = std::uint32_t{0};
  while (!frontier.empty())
  {
    auto node = frontier.front();
    frontier.pop();
    auto i = node.first;
    auto j = node.second;
    for (const auto& adj : adj_directions)
    {
      if (is_valid_matrix_pos(visited, i + adj.first, j + adj.second) && is_valid_matrix_pos(energy_matrix, i + adj.first, j + adj.second))
      {
        if(visited[i + adj.first][j + adj.second] == 0) 
        {
          ++energy_matrix[i + adj.first][j + adj.second];
          if (energy_matrix[i + adj.first][j + adj.second] > 9) 
          {
            ++n_flashes;
            energy_matrix[i + adj.first][j + adj.second] = 0;
            visited[i + adj.first][j + adj.second] = 1;
            frontier.push({i + adj.first, j + adj.second});
          }
        }
      }
    }
  }
  return n_flashes;
}

auto step_through_flashpoints(std::vector<std::vector<std::uint32_t>>& energy_matrix, const std::uint32_t max_steps, const std::uint32_t energy_delta) -> std::uint32_t
{
  if (energy_matrix.empty())
  {
    std::cerr << "ERROR::step_through_flashpoints(...)::matrix_empty" << std::endl;
    return std::uint32_t{std::numeric_limits<std::uint32_t>::max()};
  }
  std::vector<std::vector<std::uint32_t>> visited{};
  for (std::size_t i = 0; i < energy_matrix.size(); ++i)
  {
    auto v = std::vector<std::uint32_t>{};
    for (std::size_t j = 0; j < energy_matrix[i].size(); ++j)
    {
      v.push_back(std::uint32_t{0});
    }
    visited.push_back(v);
  }
  auto n_flashpoints = std::uint32_t{0};
  auto step = std::size_t{0};
  while (step < max_steps)
  {
    
    ++step;
    energy_matrix = increment_matrix(energy_matrix, energy_delta);
    visited = init_matrix_with_existing(visited);
    // bfs
    for (std::size_t i = 0; i < energy_matrix.size(); ++i)
    {
      for (std::size_t j = 0; j < energy_matrix[i].size(); ++j)
      {
        if (energy_matrix[i][j] > 9)
        {
          energy_matrix[i][j] = 0;
          n_flashpoints += (1 + bfs_flashpoints(energy_matrix, visited, i, j));
        }
      }
    }
  }
  return n_flashpoints;
}

auto step_through_flashpoints_until_sync(std::vector<std::vector<std::uint32_t>>& energy_matrix, const std::uint32_t energy_delta) -> std::uint32_t
{
  if (energy_matrix.empty())
  {
    std::cerr << "ERROR::step_through_flashpoints(...)::matrix_empty" << std::endl;
    return std::uint32_t{std::numeric_limits<std::uint32_t>::max()};
  }
  std::vector<std::vector<std::uint32_t>> visited{};
  for (std::size_t i = 0; i < energy_matrix.size(); ++i)
  {
    auto v = std::vector<std::uint32_t>{};
    for (std::size_t j = 0; j < energy_matrix[i].size(); ++j)
    {
      v.push_back(std::uint32_t{0});
    }
    visited.push_back(v);
  }
  auto n_flashpoints = std::uint32_t{0};
  auto step = std::size_t{0};
  while (true)
  {
    
    ++step;
    std::cout << "step: " << step << std::endl;
    energy_matrix = increment_matrix(energy_matrix, energy_delta);
    visited = init_matrix_with_existing(visited);
    auto isSoln = std::uint32_t{0};
    // bfs
    for (std::size_t i = 0; i < energy_matrix.size(); ++i)
    {
      for (std::size_t j = 0; j < energy_matrix[i].size(); ++j)
      {
        if (energy_matrix[i][j] > 9)
        {
          energy_matrix[i][j] = 0;
          n_flashpoints += (1 + bfs_flashpoints(energy_matrix, visited, i, j));
        }
      }
    }
    auto n = std::size_t{energy_matrix.size()};
    auto m = std::size_t{0};
    for (const auto& row : energy_matrix)
    {
      m = row.size();
      for (const auto& energy_value : row)
      {
        isSoln += (energy_value == 0); 
        std::cout << energy_value << ' ';
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
    if (isSoln == n*m)
    {
      std::cout << "soln!" << std::endl;
      return step;
    }
  }
  return n_flashpoints;
}

auto find_n_flashpoints(const std::vector<std::string>& input_lines) -> uint32_t
{
  std::cout << "generating energy matrix..." << std::endl;
  auto energy_matrix = to_integer_matrix(input_lines);
  const auto energy_delta = std::uint32_t{1};
  const auto max_steps = std::uint32_t{100};
  return step_through_flashpoints(energy_matrix, max_steps, energy_delta);
}

auto find_first_step_when_flashpoints_sync(const std::vector<std::string>& input_lines) -> uint32_t
{
  std::cout << "generating energy matrix..." << std::endl;
  auto energy_matrix = to_integer_matrix(input_lines);
  const auto energy_delta = std::uint32_t{1};
  return step_through_flashpoints_until_sync(energy_matrix, energy_delta);
}
auto solve_puzzle(const std::string& input_file_name) -> void
{
  auto ifs = std::ifstream{input_file_name};
  if (!ifs.is_open())
  {
    std::cerr << "ERROR::solve_puzzle(const std::string&)::failed_to_open_file: {" << input_file_name << "}" << std::endl;
    return;
  }
  auto line = std::string{""};
  auto input_lines = std::vector<std::string>{};
  while (std::getline(ifs, line)) input_lines.push_back(line);
  std::cout << "---part 1---" << std::endl;
  auto n_flashpoints = find_n_flashpoints(input_lines);
  std::cout << "soln: " << n_flashpoints << std::endl;
  std::cout << "---part 2---" << std::endl;
  auto step = find_first_step_when_flashpoints_sync(input_lines);
  std::cout << "soln: " << step << std::endl;
}

auto main(void) -> int
{
  //solve_puzzle("example-input.txt");
  solve_puzzle("input.txt");
  return 0;
}
