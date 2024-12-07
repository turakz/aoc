Set = Set or {}

function Set:new(list)
  --- let each instance have its own notion of self, not a shared self through __index
  local self = {}
  self._data = {}
  self._size = 0
  for _, ele in ipairs(list) do
    self._data[ele] = true
    self._size = self._size + 1
  end
  setmetatable(self, {
    __index = Set,
    __add = Set.union,
    __sub = Set.difference,
    __mul = Set.intersection,
    __tostring = Set.tostring,
  })
  return self
end

function Set:add(ele)
  if not self._data[ele] then
    self._data[ele] = true
    self._size = self._size + 1
  end
end

function Set:remove(ele)
  if self._data[ele] then
    self._data[ele] = nil
    self._size = self._size - 1
  end
end

function Set:contains(ele)
  if self._data[ele] then return true end
  return false
end

function Set:union(t)
  local s = Set:new{}
  for ele, _ in pairs(t._data) do s:add(ele) end
  for ele, _ in pairs(self._data) do s:add(ele) end
  return s
end

function Set:difference(t)
  local s = Set:new{}
  for ele, _ in pairs(self._data) do
    if not t._data[ele] then s:add(ele) end
  end
  return s
end

function Set:intersection(t)
  local s = Set:new{}
  for ele, _ in pairs(self._data) do
    if t._data[ele] then s:add(ele) end
  end
end

function Set:size()
  if self._data then return self._size end
  return 0
end

function Set:tostring()
  if self:size() == 0 then return "{}" end
  local data = {}
  for ele, _ in pairs(self._data) do
    table.insert(data, ele)
  end
  return "{" .. table.concat(data, ", ") .. "}"
end
