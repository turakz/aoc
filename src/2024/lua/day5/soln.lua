-- to require local modules we have to append our "include" or "source" directories to the interpreter's global `package.path` variable
package.path = package.path .. ";/home/fractals/dev/aoc/src/?.lua"

require("debugging")
require("file")

--[[

    problem1: given a set of page order rules in the form of pg1|pg2 where the notation means that pg2 must come after pg1, and given a set of page orderings,
    identify orderings that are correct, and compute the sum of their middle pages

    problem2: given the same input, identify incorrectly ordered page sequences, and sort them according to the page rules, then compute the sum of the sorted
    pages middle pages in the sequence

    ok, i got to admit, this problem absolutely bodied me, and i got pretty close in my initial approach and solved part 1 pretty quickly. but the full soln took
    a bit of research debugging, trial and error, to finally see what it was i was missing in my original approach, and also in trying to mimic the "typical" approach
    -> my graph algos/understandings blow

    -> initial approach, parse the inputs as forward edges using a hashmap, and then implement something akin to bubble sort on adjacent pages that are seemingly
    out of order
    => whenever you find an adjacent pair i and i + 1, that are not connected, then we pull out the vertice that is not connected, and bubble it up until
    we find a vertice that it can follow/has an edge to it, and swap the positions
    => we do this until we've placed all vertices in a position, or at the end of the sequence, and then we go back to sorting, until nothing can be positioned, at which point
    our sorting invariant is no longer true, and we can stop

    -> typical approach: graph problem + topological sorting, with some extra steps bc something in lua can very easily be nil in places where you may take it for granted
    => the page orderings are our established edges/known edges. the sequences being analyzed are just sets of vertices that may or may not already be in order (DAG)
    => we can create two sets of adjacency lists in the form of a hashmap: {vertex: {set of vertices it's connected to}}
    => we can represent forward edges and backward edges. that is forward_edge[page_num] is the set of all page numbers which must come after page_num,
    and backward_edge[page_num] is the set of all page_nums that must come before page_num

]]--

function solve(input_file)
  local debug = Debug:new()
  local input = File:new()
  local lines = input:to_lines(input_file)

  print("input lines:")
  debug:dump_table(lines)

  -- our input is delimited by a double newline to separate edges from vertex sets as inputs
  -- this is just a naive toggle to distinguish between the two input handlings
  local parsing_order_rules = true

  -- backward_edges[u] is the set of all vertices that must come before vertex u
  local backward_edges = {}
  -- forward_edges[u] is the set of all vertices that must come after vertex u
  local forward_edges = {}

  -- summations variables for the problems
  local middle_page_sum = 0
  local sorted_middle_page_sum = 0

  -- parsing and solutions
  for _, line in ipairs(lines) do

    if #line == 0 then
      parsing_order_rules = false
    end

    if parsing_order_rules then

      local edge = {}
      for num in line:gmatch("%d+") do
        table.insert(edge, tonumber(num))
      end

      assert(#edge == 2)

      local u = edge[1]
      local v = edge[2]
      -- so for 47|53 this means that 47 must come before 53, or 53 must come after 47
      -- for backward_edges[53] = {47}: 47 must come before 53
      if backward_edges[v] then
        local set = backward_edges[v]
        table.insert(set, u)
        backward_edges[v] = set
      else
        local set = {}
        table.insert(set, u)
        backward_edges[v] = set
      end

      -- and for forward_edges[47] = {53}: 53 must come after 47
      if forward_edges[u] then
        local set = forward_edges[u]
        table.insert(set, v)
        forward_edges[u] = set
      else
        local set = {}
        table.insert(set, v)
        forward_edges[u] = set
      end

    else

      if #line > 0 then

        -- helper functions for edge processing
        local function ele_exists_in(ele, set)
          for _, e in ipairs(set) do
            if e == ele then
              return true
            end
          end
          return false
        end

        local function intersection(s, t)
          local shared = {}
          for _, e in ipairs(s) do
            if ele_exists_in(e, t) then
              table.insert(shared, e)
            end
          end
          for _, e in ipairs(t) do
            if ele_exists_in(e, s) and not ele_exists_in(e, shared) then
              table.insert(shared, e)
            end
          end
          return shared
        end

        -- solutions: process vertices and edges, and apply page rules
        local sorted = true
        local vertices = {}
        for num in line:gmatch("%d+") do
          table.insert(vertices, tonumber(num))
        end

        for i, u in ipairs(vertices) do
          for j, v in ipairs(vertices) do
            -- if v points toward u, or rather, if v exists in the set of edges that point towards it, and it appears after u, it's out of order
            -- in other words: if i < j and not forward_edges[u] or not exists(v, forward_edges[u]) then ... end => u does not point to v, and is out of order
            if i < j and ele_exists_in(v, backward_edges[u] or {}) then
              sorted = false
            end
          end
        end

        -- soln1: identified a sorted sequence, compute the sum by tracking the middle page
        if sorted then
          middle_page_sum = middle_page_sum + vertices[math.ceil(#vertices / 2)]
        else

          -- soln2: identified a non-sorted sequence, sort it, then compute the sum by tracking the middle page
          local sorted_pages = {}
          local queue = {}

          -- compute the in degrees of a forward edge, which is just the length of the intersction between
          -- the set of all vertices that must come after it in a graph, and the vertices being sorted
          local in_degree = {}
          for _, u in ipairs(vertices) do
            if backward_edges[u] then
              -- we find the intersection because we only care about edges that point to u, that also exist in the subgraph (vertices)
              in_degree[u] = #intersection(backward_edges[u], vertices)
            else
              in_degree[u] = 0
            end
          end

          -- BFS toplogical sort: seed our search with a sink node. that is, a vertex with no incoming edges
          for _, u in ipairs(vertices) do
            if in_degree[u] == 0 then
              table.insert(queue, u)
            end
          end

          while #queue > 0 do
            -- track the vertex which has no incoming edges
            local u = queue[1]
            table.remove(queue, 1)
            table.insert(sorted_pages, u)
            -- modify degrees of nodes u pointed to, because we've removed u from the subgraph
            for _, v in ipairs(forward_edges[u] or {}) do
              if in_degree[v] and ele_exists_in(v, forward_edges[u] or {}) then
                in_degree[v] = in_degree[v] - 1
                if in_degree[v] == 0 then
                  table.insert(queue, v)
                end
              end
            end
          end
          sorted_middle_page_sum = sorted_middle_page_sum + sorted_pages[math.ceil(#sorted_pages / 2)]

        end
      end
    end
  end
  print("soln1::middle_page_sum::{" .. middle_page_sum .. "}")
  print("soln2::sorted_middle_page_sum::{" .. sorted_middle_page_sum .. "}")
end

--solve("example-input.txt")
solve("input.txt")
