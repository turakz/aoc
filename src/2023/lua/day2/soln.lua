-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/?.lua"

require("debugging")
require("file")

--[[
  problem 1: the elves want to play a game with you where you have a bag, red, green, and blue cubes, and a pre-deteremined
  amount of cubes that can be used to play a game

  your input is a series of games played, where each line is a game with an ID

  each game will have one or more sets of semi-colon delimited games, each set denoting a game outcome where cubes were used

  determine if a game can be played given a certain configuration of cubes (that is, a certain amount of cubes alloted for each color)

  ie for input "Game 42: 1 blue, 2 green; 3 blue, 2 red; 11 green", and with a configuration of only having at most 12 red cubes, 13 green cubes,
  and 14 blue cubes, is this a possible game that can be played?

  -> yes, each game set uses an amount of cubes for the color, that are allowed by the configuration

  find the sum of all possible game ids

  problem 2: what is the fewest number of cubes of each color that could have been in the bag to make the game possible?
  and then once you've identified the fewest number of cubes for each color needed to play the game, multiplty all the counts
  together for that color, to get the cube's power.

  for each game, find the minimum set of cubes that must have been present: what is the sum of the power of these sets?

  -> parse each set in a game and keep track of a max count for each cube color
  -> calculate the powers of each cube
  -> sum the powers
]]--

function solve(input_file)
  local debug = Debug:new()
  local input = File:new()
  local lines = input.to_lines(input_file)

  print("input lines:")
  debug.dump_table(lines)

  local function split(s, sep)
    local splits = {}
    local chars = {}
    s:gsub(".", function(c)
      table.insert(chars, c)
    end)
    local c_idx = 1
    while c_idx < #s do
      local next_idx = c_idx
      while next_idx < #s and chars[next_idx] ~= sep do
        next_idx = next_idx + 1
      end
      if next_idx == #s then
        local substr = string.sub(s, c_idx, next_idx)
        table.insert(splits, substr)
      else
        local substr = string.sub(s, c_idx, next_idx - 1)
        table.insert(splits, substr)
      end
      c_idx = next_idx + 1
    end
    return splits
  end

  local possible_games = {}
  local game_powers = {}
  for _, line in ipairs(lines) do
    -- parse game id from line
    local game_id = split(line, ":")[1]
    -- parse game sets from line
    local game_sets = split(split(line, ":")[2], ";")

    debug.dump_table(game_sets, "game_sets = ")

    -- iterate over each set and count the total number of cubes
    local is_ok = true

    local max_cube_counts = {}
    for _, set in ipairs(game_sets) do
      print("set: " .. set)
      -- parse numbers from set
      local cube_counts = {}
      for count in string.gmatch(set, "%d+") do
        table.insert(cube_counts, tonumber(count))
      end

      debug.dump_table(cube_counts, "cube_counts = ")

      -- parse colors from set
      local cube_colors = {}
      for color in string.gmatch(set, "[^%s%d,]%w+") do
        table.insert(cube_colors, color)
      end

      debug.dump_table(cube_colors, "cube_colors = ")

      assert(#cube_counts == #cube_colors)

      -- apply cube total thresholds to the tracked counts for a set
      local ok_thresholds = {
        ["red"] = 12,
        ["green"] = 13,
        ["blue"] = 14,
      }

      --soln1: time n*m^2 (?)/space n-ish
      -- analyze games for okay-ness by comparing the max threshold against a tracked cube's weight
      for color_idx, _ in ipairs(cube_counts) do
        local color = cube_colors[color_idx]
        if ok_thresholds[color] then
          local max_count = ok_thresholds[color]
          local current_count = cube_counts[color_idx]
          if current_count > max_count then
            is_ok = false
          end
        end
      end

      if is_ok then
        table.insert(possible_games, tonumber(game_id:match("%d+")))
      end

      --soln 2: same as above, the addition is linear, but it is still within the inner loop
      -- track max cube counts for a game, compute a cube set's power, and sum up all the cube set powers
      for color_idx, _ in ipairs(cube_counts) do
        local color = cube_colors[color_idx]
        local count = cube_counts[color_idx]
        if max_cube_counts[color] then
          local current_count = max_cube_counts[color]
          if count >= current_count then
            max_cube_counts[color] = count
          end
        else
          max_cube_counts[color] = count
        end
        debug.dump_table(max_cube_counts, "max_cube_counts = ")
      end
    end

    local power = 1
    for color, max_count in pairs(max_cube_counts) do
      power = power * max_count
    end

    table.insert(game_powers, power)

  end

  -- once we have game_ids/powers, add them all up for possible_games/powers sums
  local game_id_sum = 0
  for _, id in ipairs(possible_games) do
    game_id_sum = game_id_sum + id
  end
  print("soln1::game_ids_sum::{" .. game_id_sum .. "}")

  local game_powers_sum = 0
  for _, power in ipairs(game_powers) do
    game_powers_sum = game_powers_sum + power
  end
  print("soln2::game_powers_sum::{" .. game_powers_sum .. "}")

end

--solve("example-input.txt")
solve("input.txt")
