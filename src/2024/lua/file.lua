File = File or {}
File.__index = File

function File:new()
  self = {
    read = function(filepath)
      local file = io.open(filepath, "r")
      assert(file, "ERROR -- File: cannot open \"" .. filepath .. "\"")
      local contents = file:read("*a")
      file:close()
      return contents
    end,
    to_lines = function(filepath)
      local lines = {}
      for line in io.lines(filepath) do
        lines[#lines + 1] = line
      end
      return lines
    end
  }
  setmetatable(self, File)
  return self
end
