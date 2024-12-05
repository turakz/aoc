-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/?.lua"

require("debugging")
require("file")

--[[
  problem 1: naive template/regex parse: mul(x,y) => "%mul%(%d+%,%s*%d+%)" => treat operation signature and grammars literally, and include one or more positional
  digit sequences

  problem 2: the text sequence contains "do()" and "don't()" literals => they toggle whether or not future operations are enabled/disabled
  -> we either enable or disable multiplcation parses
  => only the most recent instructions apply, default state is enabled
  -> so we examine a substring for whether or not we toggle enable/disable
  -> then we examine a mul operation substring according to a "syntax" template

  * this took me a solid while to get right, with a little help from reference/research
  -> general concept and about 80% of the algo was there initially, but my lua pattern matching/string manipulation outright was shallow
  -> tried to do it strictly in regex first to avoid a manual parse of a multiplcation operation, and then said fuck it bc realized that sliding window over a max template
  length substring parse was the way to go
    => originally i was just parsing full sequence chunks based on enable/disabled markers, but that meant i wasn't always grabbing the most recent marker because the regex
    + string.find meant some markers get skipped over (at least that's what i presumed, i didn't verify fully because it solved example inputs but broke on actual input,
    usually being a number smaller than expected, and i didn't bother stepping through the whole parse character by character)
  -> anyway, lesson is: introduce minimum amnt of regex necessary and then refactor once you got a hold on the actual parsing
]]--

function solve(input_file)
  local debug = Debug:new()
  local input = File:new()
  local lines = input.to_lines(input_file)

  print("input lines:")
  --debug.dump_table(lines)

  local function mul(lhs, rhs)
    return lhs * rhs
  end
  local function mul_parse(line, enabled)
    if not enabled then
      enabled = true
    end
    local result = 0
    for op_str in line:gmatch("%mul%(%d+%,%s*%d+%)") do
      local args = {}
      for arg in op_str:gmatch("%d+") do
        table.insert(args, tonumber(arg))
      end

      --debug.dump_table(args)
      assert(#args == 2)
      if enabled then
        result = result + mul(args[1], args[2])
      end
    end
    return result
  end

  -- soln1: time n*m/space linear-ish
  local result = 0
  for _, line in ipairs(lines) do
    result = result + mul_parse(line)
  end
  print("soln1::result::{" .. result .. "}")

  -- soln2: sliding window, linear time/space with some factors due to the substring searches (?)
  result = 0
  local enabled = true
  local chars = {}
  for c in table.concat(lines):gmatch(".") do
    table.insert(chars, c)
  end
  local enable_marker = "do()"
  local disable_marker = "don't()"
  local op_prefix = "mul("
  local syntax_template = "mul(xxx, yyy)"
  for parse_idx = 1, #chars do
    -- examine substring for recent enable marker
    if parse_idx + (#enable_marker - 1) <= #chars then
      if table.concat(chars, "", parse_idx, parse_idx + (#enable_marker - 1)) == enable_marker then
        enabled = true
      end
    end
    -- examine substring for recent disable marker
    if parse_idx + (#disable_marker - 1) <= #chars then
      if table.concat(chars, "", parse_idx, parse_idx + (#disable_marker - 1)) == disable_marker then
        enabled = false
      end
    end
    -- parse out mul operation from substring: we can do this by examining substrings equal to the length of our syntax template
    if parse_idx + (#op_prefix - 1) <= #chars then
      -- identify an operation marker: "mul("
      local prefix = table.concat(chars, "", parse_idx, parse_idx + (#op_prefix - 1))
      if prefix == op_prefix then
        -- suffix_idx is a max substring upper bound based on syntax template
        local suffix_idx = parse_idx + (#syntax_template - 1)
        -- our template length is greater than the remaining part of the string, so we'll just treat the rest as a suffix (it might have a smaller mul suffix)
        if suffix_idx > #chars then
          suffix_idx = #chars
        end
        -- now that we have a valid suffix id, we know our parse_idx is good, examine the substring for a match on our template
        if suffix_idx <= #chars then
          local op_str = table.concat(chars, "", parse_idx, suffix_idx)
          local match = op_str:match("mul%(%d+%,%s*%d+%)")
          if match then
            if enabled then
              result = result + mul_parse(match)
              parse_idx = #match -- might as well skip what we already parsed
            end
          end
        end
      end
    end
  end
  print("soln2::result::{" .. result .. "}")

end

--solve("example-input.txt")
solve("input.txt")
