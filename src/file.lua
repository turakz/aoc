File = File or {}
function File:new()
  -- give file objects their own sense of self
  local self = {}
  setmetatable(self, {
    __index = File
  })
  return self
end

function File:read(filepath)
  local file = io.open(filepath, "r")
  assert(file, "ERROR -- File: cannot open \"" .. filepath .. "\"")
  local contents = file:read("*a")
  file:close()
  return contents
end

function File:to_lines(filepath)
  local lines = {}
  for line in io.lines(filepath) do
    lines[#lines + 1] = line
  end
  return lines
end
