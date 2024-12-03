-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/?.lua"

require("debugging")
require("file")

--[[
  problem 1: given a file of line delimited number sequences, parse the input with the following rules
    1. each line is a report
    2. each report contains one or more levels

    search for safe reports in the input and count the total amount

    - a safe report is:
      * either has all increasing levels or all decreasing levels
      * adjacent levels must differ by at least one and at most three

    soln: in-place comparisons/arithmetic, with helper functions for identifying strict orderedness and calculating adjacent differences

    problem 2: introduce a safety tolerence requirement

    - if a report can remove 1 bad level and achieve a safe report, we can include that in our solution set
      -> edit distance length problem (?)
      -> look at a report, remove 1 level, and seeing if the result is unsafe?
      -> and just doing this for each level until we've found a solution?
      => for each level in a set of levels, consider the other n - 1 levels
        => the first subset which meets our requirements means we have a tolerent safe report
]]--

function solve(input_file)
  local debug = Debug:new()
  local input = File:new()
  local lines = input.to_lines(input_file)

  print("input lines:")
  debug.dump_table(lines)

  -- soln1: time n*m/space n*(m - 1) (?)
  -- helper function for checking strict orderedness
  local function is_valid_report(levels)
    local strictly_ordered = true
    for idx = 2, #levels do
      if not strictly_ordered then break end
      strictly_ordered = (strictly_ordered and (levels[idx - 1] < levels[idx]))
    end
    if not strictly_ordered then
      strictly_ordered = true
      for idx = 2, #levels do
        if not strictly_ordered then break end
        strictly_ordered = (strictly_ordered and (levels[idx - 1] > levels[idx]))
      end
    end
    return strictly_ordered
  end

  -- helper function for calculating adjacent differences
  local function is_safe_report(levels)
    local differences = {}
    for idx = 2, #levels do
      local lhs = levels[idx]
      local rhs = levels[idx - 1]
      differences[#differences + 1] = math.abs(lhs - rhs)
    end

    local valid_diffs = 0
    local min_v = 1; local max_v = 3
    for _, diff in ipairs(differences) do
      if diff >= min_v and diff <= max_v then
        valid_diffs = valid_diffs + 1
      end
    end
    return valid_diffs == #differences
  end

  -- parse input lines and analyze reports
  local safe_reports = {}
  local unsafe_reports = {}
  for _, report in ipairs(lines) do
    local levels = {}
    for level in string.gmatch(report, "%d+") do
      levels[#levels + 1] = tonumber(level)
    end
    if is_valid_report(levels) and is_safe_report(levels) then
      --debug.dump_table(levels)
      table.insert(safe_reports, levels)
    else
      table.insert(unsafe_reports, levels)
    end
  end
  print("soln1::safe_reports::{" .. #safe_reports .. "}")

  -- soln2: approx. n*m^2
  local tolerence_safe_reports = {}
  for _, report in ipairs(unsafe_reports) do
    for idx = 1, #report do
      -- copy over report, excluding report[idx]
      local buffer = {}
      for buf_idx = 1, #report do
        if buf_idx ~= idx then
          buffer[#buffer + 1] = report[buf_idx]
        end
      end
      if is_valid_report(buffer) and is_safe_report(buffer) then
        --debug.dump_table(buffer)
        table.insert(tolerence_safe_reports, report)
        break
      end
    end
  end
  print("soln2::safe_reports_with_tolerence::{" .. (#safe_reports + #tolerence_safe_reports) .. "}")
end

--solve("example-input.txt")
solve("input.txt")
