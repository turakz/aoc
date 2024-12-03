-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/2023/lua/?.lua"

require("debugging")
require("file")

--[[
  problem 1: given a set of alphanumeric strings, parse the first occuring digit and the last occuring digit from the string
  into a 2digit string
  -> find the integral sum of all these two digit strings

  problem 2: so you have to parse literal digits from the string, but also now have to parse their text representations and convert
  them to their numerical value (ie: "five" => "5") for inputs like "three6mafia" => "36", as these are the first and last
  occuring "digits" in the sequence
]]--

function solve(input_file)
  local debug = Debug:new()
  local input = File:new()
  local lines = input.to_lines(input_file)

  print("input lines:")
  debug.dump_table(lines)

  local s1_sum = 0
  local s2_sum = 0
  for _, line in ipairs(lines) do
    -- parse line into an array of characters
    local chars = {}
    line:gsub(".", function(c) table.insert(chars, c) end)
    local s1_digits = {}
    local s2_digits = {}
    local digit_strs = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}
    -- for each character in the line
    for c_idx, c in ipairs(chars) do
      -- soln1: time n*m/space m
      -- if the line is a digit, track it
      if c:match("%d") then
        table.insert(s1_digits, c)
        table.insert(s2_digits, c)
      end
      -- soln2: time n*m^2(?)/space m
      -- indexing digit string representations
      for v_idx, val in ipairs(digit_strs) do
        -- if the substring in the line starting at our current character index begins/starts with the string val
        if string.sub(string.sub(line, c_idx, #line), 1, #val) == val then
          -- then we know we have a digit, albeit in its string representation, so insert its enumerated value idx 4 == "three", idx 5 == "four" => idx - 1 == str_value mapping
          table.insert(s2_digits, tostring(v_idx - 1))
        end
      end
    end
    if #s1_digits > 0 then
      --debug.dump_table(s1_digits)
      s1_sum = s1_sum + (tonumber(s1_digits[1] .. "" .. s1_digits[#s1_digits]))
    end
    if #s2_digits > 0 then
      --debug.dump_table(s2_digits)
      s2_sum = s2_sum + (tonumber(s2_digits[1] .. "" .. s2_digits[#s2_digits]))
    end
  end
  print("soln1::sum::{" .. s1_sum .. "}")
  print("soln2::sum::{" .. s2_sum .. "}")

  -- soln2
end

--solve("example-input.txt")
solve("input.txt")
