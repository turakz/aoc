Debug = Debug or {}
Debug.__index = Debug

function Debug:new()
  self = {
    dump_table = function(table, label, indent)
      if not label then
        label = ""
      end
      if not indent then
        indent = 0
      end
      print(string.rep(" ", indent) .. "" ..label .. "{")
      for k, v in pairs(table) do
        local format = string.rep(" ", indent + 2) .. "[" .. k .. "]" .. ": "
        if type(v) == "table" then
          print(format)
          self.dump_table(v, "", indent + 2)
        elseif type(v) == "boolean" then
          print(format .. tostring(v))
        else
          print(format .. v)
        end
      end
      if indent > 0 then
        print(string.rep(" ", indent) .. "},")
      else
        print(string.rep(" ", indent) .. "}")
      end
    end,
  }
  setmetatable(self, Debug)
  return self
end
