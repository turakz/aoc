-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/?.lua"

require("debugging")
require("file")

--[[
  problem 1: given a grid of dot delimited numbers, where an non-numeric symbol can occur, sum all numbers that occur
  who are adjacent to one of these symbols -- diagonal adjacency is still valid.

  -> the elves are doing some assessments on some engines and are looking at engine schematics to figure out missing engine parts:
  example input:

        467..114..
        ...*......
        ..35..633.
        ......#...
        617*......
        .....+.58.
        ..592.....
        ......755.
        ...$.*....
        .664.598..

    numbers 114 and 58 do not have a symbol adjacent to them (any of their character positions)

  -> find the missing part numbers, and add them all together
  => in this example that's SUM(467 + 35 + 633 + 617 + 592 + 755 + 664 + 598)

  -> there's two immediate approaches i can think of
    1. we parse the input typically, line by line, and represent it as a grid/2d array, so that each character has a position
      -> we then parse everything from the perspective of being on a line, where we try to identify a subset that's all digits,
      and then once that's identified, we explore all adjacent positions for some kind of alphanumeric symbol occurence, including diagonals
    2. we parse the input typically, except we only parse it for symbol positions, and then we spiral outwards to see if we connect with any numbers
      -> this makes checking for a character's literal-ness easier because a number is more identifiable than any possible symbol occurence (which, this
      problem doesn't mention, so i'm assuming any grammatical character, possibly even alphabetical)
      -> from here we just expand horizontally until the number sequence terminates on both ends, and then read it as a whole integer left to right,
      track that number, and sum it later

    => what if we just parse the grid for number [start, end) tuples
]]--

function solve(input_file)
  local debug = Debug:new()
  local input = File:new()
  local lines = input.to_lines(input_file)

  print("input lines:")
  debug.dump_table(lines)

end

--solve("example-input.txt")
solve("input.txt")
