-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/?.lua"

require("debugging")
require("file")

--[[
  problem 1: given a string of digits, find the sum of all digits who have an adjacent match. the string is circular,
  meaning that the last element's adjacent element is the first element
    ie: 1122 sums to 3 because 1 matches its adjacent element, so we track 1 as our first sum value, and then 2 has an adjacent match,
    so then we track 2 to our sum giving us 1 + 2 = 3

    linear scan, nothing crazy

  problem 2: instead of looking at the adjacent element for a match, the adjacent element is going to be the index that is half of the
  total digits in the string
    ie: 1212 produces a sum of 6 because we have 4 total digits, therefore if we look for 1's adjacent match, we must consider
    the index 4/2 => 2, which in this case (since it's lua and 1-based), is 1, and matches, so we track 1
    -> in this case, all 4 digits match their adjacent element, producing 6

    linear scan with a circular index using modolo arithmetic: digit[idx] == digit[(idx + (#digits / 2)) % (#digits)], will always give us
    a valid index
]]--

function solve(input_file)
  local debug = Debug:new()
  local input = File:new()
  local lines = input:to_lines(input_file)

  print("input lines:")
  debug:dump_table(lines)

  for _, line in ipairs(lines) do
    local digits = {}
    for c in line:gmatch("%d") do
      table.insert(digits, c)
    end

    -- soln1: time n/space 1
    local sum = 0
    for idx = 2, #digits do
      if digits[idx - 1] == digits[idx] then
        sum = sum + tonumber(digits[idx])
      end
    end
    -- we gotta wrap around and handled the last element's adjacent
    if digits[#digits] == digits[1] then
      sum = sum + tonumber(digits[1])
    end
    -- soln2: time n + k/space 1
    local mod_sum = 0
    for idx = 1, #digits do
      if digits[idx] == digits[(idx + (#digits / 2)) % (#digits)] then
        mod_sum = mod_sum + tonumber(digits[(idx + (#digits / 2)) % (#digits)])
      end
    end
    if digits[#digits] == digits[(#digits + (#digits / 2)) % (#digits)] then
      mod_sum = mod_sum + tonumber(digits[(#digits + (#digits / 2)) % (#digits)])
    end
    print("digits", line)
    print("soln1::sum::{" .. sum .. "}")
    print("soln2::mod_sum::{" .. mod_sum .. "}")
  end

end

--solve("example-input.txt")
solve("input.txt")
