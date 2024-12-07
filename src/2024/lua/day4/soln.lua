-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/?.lua"

require("debugging")
require("file")

--[[
    problem 1: wordsearching: finding "XMAS"

    ie: given this body

        MMMSXXMASM
        MSAMXMSMSA
        AMXSXMAAMM
        MSAMASMSMX
        XMASAMXAMM
        XXAMMXXAMA
        SMSMSASXSS
        SAXAMASAAA
        MAMMMXMMMM
        MXMXAXMASX

      this is the parsed output, assuming that any characters that were not included in a spelling/occurence of "XMAS"
      were replaced with ".":

        ....XXMAS.
        .SAMXMS...
        ...S..A...
        ..A.A.MS.X
        XMASAMX.MM
        X.....XA.A
        S.S.S.S.SS
        .A.A.A.A.A
        ..M.M.M.MM
        .X.X.XMASX


    "XMAS" can occur: horizontally, vertically, diagonally, backwards, and overlapping other words

    -> this almost sounds like a DFS/connected paths where the path we're hoping to find is "XMAS" or "XMAS"

    so what does our DFS look like?

    for every starting position, we explore:
      in exploring we:

      1. search horizontally: right and left
      2. search vertically: up and down
      3.


    problem 2: given this new text

      M.S
      .A.
      M.S

    the occurences we want to search for now actually look like an "X" so what we parse is the "MAS" but in the shape of an X

]]--

function solve(input_file)
  local debug = Debug:new()
  local input = File:new()
  local lines = input:to_lines(input_file)

  print("input lines:")
  debug:dump_table(lines)

  local grid = {}
  for _, line in ipairs(lines) do
    local v_chars = {}
    for c in line:gmatch("%w") do
      table.insert(v_chars, c)
    end
    table.insert(grid, v_chars)
  end

  local function is_valid_range(grid, x, y)
    return (x >= 1) and (y >= 1) and (x <= #grid) and (y <= #grid[x])
  end

  local xmas_occurences = 0
  local x_mas_occurences = 0
  for x = 1, #grid do
    for y = 1, #grid[x] do
      -- look right for XMAS
      if is_valid_range(grid, x, y + 3) then
        if grid[x][y] == "X" and grid[x][y + 1] == "M" and grid[x][y + 2] == "A" and grid[x][y + 3] == "S" then
          xmas_occurences = xmas_occurences + 1
        end
      end
      -- look right for SAMX
      if is_valid_range(grid, x, y + 3) then
        if grid[x][y] == "S" and grid[x][y + 1] == "A" and grid[x][y + 2] == "M" and grid[x][y + 3] == "X" then
          xmas_occurences = xmas_occurences + 1
        end
      end
      -- look down for XMAS
      if is_valid_range(grid, x + 3, y) then
        if grid[x][y] == "X" and grid[x + 1][y] == "M" and grid[x + 2][y] == "A" and grid[x + 3][y] == "S" then
          xmas_occurences = xmas_occurences + 1
        end
      end
      -- look down for SAMX
      if is_valid_range(grid, x + 3, y) then
        if grid[x][y] == "S" and grid[x + 1][y] == "A" and grid[x + 2][y] == "M" and grid[x + 3][y] == "X" then
          xmas_occurences = xmas_occurences + 1
        end
      end
      -- look down and right diag for XMAS
      if is_valid_range(grid, x + 3, y + 3) then
        if grid[x][y] == "X" and grid[x + 1][y + 1] == "M" and grid[x + 2][y + 2] == "A" and grid[x + 3][y + 3] == "S" then
          xmas_occurences = xmas_occurences + 1
        end
      end
      -- look down and right diag for SAMX
      if is_valid_range(grid, x + 3, y + 3) then
        if grid[x][y] == "S" and grid[x + 1][y + 1] == "A" and grid[x + 2][y + 2] == "M" and grid[x + 3][y + 3] == "X" then
          xmas_occurences = xmas_occurences + 1
        end
      end
      -- look up and left diag for XMAS
      if is_valid_range(grid, x - 3, y + 3) then
        if grid[x][y] == "X" and grid[x - 1][y + 1] == "M" and grid[x - 2][y + 2] == "A" and grid[x - 3][y + 3] == "S" then
          xmas_occurences = xmas_occurences + 1
        end
      end
      -- look up and left diag for SAMX
      if is_valid_range(grid, x - 3, y + 3) then
        if grid[x][y] == "S" and grid[x - 1][y + 1] == "A" and grid[x - 2][y + 2] == "M" and grid[x - 3][y + 3] == "X" then
          xmas_occurences = xmas_occurences + 1
        end
      end
      -- look down and right for MAS shaped X
      if is_valid_range(grid, x + 2, y + 2) then
        if grid[x][y] == "M" and grid[x + 2][y] == "M" and grid[x + 1][y + 1] == "A" and grid[x][y + 2] == "S" and grid[x + 2][y + 2] == "S" then
          x_mas_occurences = x_mas_occurences + 1
        end
      end
      -- look down and right for SAM shaped X
      if is_valid_range(grid, x + 2, y + 2) then
        if grid[x][y] == "S" and grid[x + 2][y] == "M" and grid[x + 1][y + 1] == "A" and grid[x][y + 2] == "S" and grid[x + 2][y + 2] == "M" then
          x_mas_occurences = x_mas_occurences + 1
        end
      end
      -- look up and left for MAS shaped X
      if is_valid_range(grid, x - 2, y - 2) then
        if grid[x][y] == "M" and grid[x - 2][y] == "M" and grid[x - 1][y - 1] == "A" and grid[x][y - 2] == "S" and grid[x - 2][y - 2] == "S" then
          x_mas_occurences = x_mas_occurences + 1
        end
      end
      -- look up and left for SAM shaped X
      if is_valid_range(grid, x - 2, y - 2) then
        if grid[x][y] == "S" and grid[x - 2][y] == "M" and grid[x - 1][y - 1] == "A" and grid[x][y - 2] == "S" and grid[x - 2][y - 2] == "M" then
          x_mas_occurences = x_mas_occurences + 1
        end
      end
    end
  end
  print("soln1::xmas_occurences::{" .. xmas_occurences .. "}")
  print("soln1::xmas_occurences::{" .. x_mas_occurences .. "}")

end

--solve("example-input.txt")
solve("input.txt")
