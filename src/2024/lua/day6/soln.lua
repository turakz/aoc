-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/?.lua"

require("debugging")
require("file")
require("set")

--[[
      problem1: given a map of a room where "." denotes empty room, "#" denotes an obstacle, and a carrot (directionally: < ^ > v)
      denotes a "guard"
      -> the guard patrols the room from a starting position, adhering to a few rules:
        1. if there is something directly in front of you, turn right 90 degrees
        2. otherwise take a step forward

      do this repeatedly

      => so given the map and a starting position, "map" out the patrol guards path until all rooms have been mapped/visited,
      or the guard arrives at a point where they cannot turn right because of an obstacle, and are therefore stuck
      => mark rooms that were legally visited with "X": the final result is the total number of "X" markers
]]--

function solve(input_file)
  local debug = Debug:new()
  local input = File:new()
  local lines = input:to_lines(input_file)

  print("input lines:")
  debug:dump_table(lines)

  local puzzle_map = {}
  for _, line in ipairs(lines) do
    local chars = {}
    for c in line:gmatch(".") do
      table.insert(chars, c)
    end
    table.insert(puzzle_map, chars)
  end

  print("puzzle_map:")
  debug:dump_table(puzzle_map)

  local function is_valid_move(puzzle_map, i, j)
    return (i > 0 and j > 0) and (i <= #puzzle_map and j <= #puzzle_map[i])
  end

  local guard_idx = {0, 0}
  for i, _ in ipairs(puzzle_map) do
    for j, _ in ipairs(puzzle_map[i]) do
      if puzzle_map[i][j] == "<" or puzzle_map[i][j] == "^" or puzzle_map[i][j] == ">" or puzzle_map[i][j] == "v" then
        guard_idx[1] = i
        guard_idx[2] = j
        print("found guard at", i, j)
      end
    end
  end
  assert(#guard_idx == 2)

  local function roate_directional(directional)
    if directional == "<" then return "^" end
    if directional == "^" then return ">" end
    if directional == ">" then return "v" end
    if directional == "v" then return "<" end
    return directional
  end

  local function render_map(puzzle_map, label)
    if not label then label = "" end
    print(string.rep("-", #puzzle_map))
    print(label)
    for _, tc in ipairs(puzzle_map) do
      print(table.concat(tc, ""))
    end
    print(string.rep("-", #puzzle_map))
  end

  local gi = guard_idx[1]
  local gj = guard_idx[2]

  local directionals = {
    ["<"] = {0, -1},
    ["^"] = {-1, 0},
    [">"] = {0, 1},
    ["v"] = {1, 0},
  }
  assert(directionals[puzzle_map[gi][gj]])
  local directional = puzzle_map[gi][gj]

  render_map(puzzle_map, "starting state")

  for r, _ in ipairs(puzzle_map) do
    for c, _ in ipairs(puzzle_map[r]) do
      -- simulate


    end
  end
end


solve("example-input.txt")
--solve("input.txt")
