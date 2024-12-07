-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/?.lua"

require("file")

--[[
  problem 1: given two lists side by side, take the smallest element in one list
  and find the difference between that and the smallest element in the other list,
  and then do this for the second smallest in the first list, and also in the other list,
  and so on and so forth, summing the total differences

  aka: given two lists, sort them and sum their adjacent differences

  problem 2: given those same lists, find out how many times each element in the left list
  occurs in the right list, and then multiply the element's scalar value by the scalar value
  of the occurences to compute a similarity score, and then find the sum of similarity scores
  for each pair-wise element for both lists
]]--

function solve(input_file)
  local file = File:new()
  local lines = file:to_lines(input_file)

  local left_list = {}
  local right_list = {}
  for idx, line in ipairs(lines) do
    local lhs, rhs = string.match(line, "(%d+)%s+(%d+)")
    left_list[#left_list + 1] = tonumber(lhs)
    right_list[#right_list + 1] = tonumber(rhs)
  end
  assert(#left_list == #right_list)

  -- soln 1, time nlogn/space n
  table.sort(left_list)
  table.sort(right_list)
  local sum = 0
  for idx = 1, #left_list do
    sum = (sum + (math.abs(left_list[idx] - right_list[idx])))
  end
  print("soln1::sum_of_adjacent_differences::{" .. sum .. "}")

  -- soln 2, time n/space n
  local hmp = {}
  for idx = 1, #right_list do
    if hmp[right_list[idx]] then
      hmp[right_list[idx]] = hmp[right_list[idx]] + 1
    else
      hmp[right_list[idx]] = 1
    end
  end

  sum = 0
  for idx = 1, #left_list do
    local score = 0
    if hmp[left_list[idx]] then
      score = left_list[idx] * hmp[left_list[idx]]
    else
      score = (left_list[idx] * score)
    end
    sum = sum + score
  end
  print("soln2::sum_of_similiarity_scores::{" .. sum .. "}")
end

--solve(AocUtility.EXAMPLE_INPUT_FILE)
solve("input.txt")
