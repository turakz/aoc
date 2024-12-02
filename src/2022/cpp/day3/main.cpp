#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

auto trimItemDuplicates(const std::string& items) -> std::string
{
  auto trimmedItems = std::string {};
  if (!items.empty())
  {
    auto seen = std::unordered_set<char> {};
    for (const auto& item : items)
    {
      if (seen.insert(item).second)
      {
        trimmedItems += item;
      }
    }
  }
  else std::cerr << "ERROR::main.cpp::trimItemDuplicates(std::string&)::ITEMS_IS_EMPTY" << std::endl;
  return trimmedItems;
}

auto generateItemWeights() -> std::unordered_map<char, uint16_t>
{
  auto weights = std::unordered_map<char, uint16_t> {};
  const auto letters = std::string {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
  for (std::size_t pos = 1; pos <= letters.size(); ++pos)
  {
    weights.insert( {letters[pos - 1], pos} );
  }
  return weights;
}

auto findItemWithNOccurences(const std::string& items, const std::size_t n) -> char
{
  auto item = char {};
  if (!items.empty())
  {
    auto occurences = std::unordered_map<char, std::size_t> {};
    for (const auto& item : items)
    {
      ++occurences[item];
      if (occurences[item] == n) return item;
    }
  }
  else std::cerr << "ERROR::main.cpp::trimItemDuplicates(std::string&)::ITEMS_IS_EMPTY" << std::endl;
  return item;
}

auto parseRucksacksSharedItems(const std::vector<std::pair<std::string, std::string>>& rucksacks, const std::size_t nOccurences) -> std::vector<char>
{
  auto sharedItems = std::vector<char> {};
  if (!rucksacks.empty())
  {
    for (const auto& rucksack : rucksacks)
    {
      auto compartmentOne = rucksack.first;
      auto compartmentTwo = rucksack.second;
      auto trimmedCompartmentsUnion = std::string {trimItemDuplicates(compartmentOne) + trimItemDuplicates(compartmentTwo)};
      auto sharedItem = findItemWithNOccurences(trimmedCompartmentsUnion, nOccurences);
      sharedItems.push_back(sharedItem);
    }
  }
  else std::cerr << "ERROR::main.cpp::parseRucksacksSharedItems(const std::vector<std::pair<std::string, std::string>>&)::RUCKSACKS_IS_EMPTY" << std::endl;
  return sharedItems;
}

auto parseRucksackGroupsSharedItems(const std::vector<std::vector<std::string>>& groups, const std::size_t nOccurences) -> std::vector<char>
{
  auto sharedItems = std::vector<char> {};
  if (!groups.empty())
  {
    for (const auto& group : groups)
    {
      auto groupUnion = std::string {};
      for (const auto& items : group)
      {
        auto trimmedItems = trimItemDuplicates(items);
        groupUnion += trimmedItems;
      }
      auto sharedItem = findItemWithNOccurences(groupUnion, nOccurences);
      sharedItems.push_back(sharedItem);
    }
  }
  else std::cerr << "ERROR::main.cpp::parseRucksackGroupsSharedItems(const std::vector<std::vector<std::string>>&)::GROUPS_IS_EMPTY" << std::endl;
  return sharedItems;
}

auto parseRucksackCompartments(const std::vector<std::string>& fileData) -> std::vector<std::pair<std::string, std::string>>
{
  auto rucksacks = std::vector<std::pair<std::string, std::string>> {};
  if (!fileData.empty())
  {
    for (const auto& line : fileData)
    {
      auto start = std::size_t {0};
      auto mid = std::size_t {(start + (line.size() - start)/2)}; 
      auto rucksackOne = line.substr(start, mid);
      auto rucksackTwo = line.substr(mid, mid);
      rucksacks.push_back({rucksackOne, rucksackTwo});
    }
  }
  else std::cerr << "ERROR::main.cpp::parseRuckSacks(const std::vector<std::string>&)::FILE_DATA_IS_EMPTY" << std::endl;
  return rucksacks;
}

auto parseRucksackGroups(const std::vector<std::string>& fileData, const std::size_t totalElvesInGroup) -> std::vector<std::vector<std::string>>
{
  auto rucksackGroups = std::vector<std::vector<std::string>> {};
  if (!fileData.empty())
  {
    auto lineIdx = std::size_t {0};
    while (lineIdx < fileData.size())
    {
      auto groupIdx = std::size_t {0};
      auto group = std::vector<std::string> {};
      while (groupIdx < totalElvesInGroup && lineIdx < fileData.size() )
      {
        group.push_back(fileData[lineIdx + groupIdx]);
        ++groupIdx;
      }
      rucksackGroups.push_back(group);
      lineIdx += groupIdx;
    }
  }
  else std::cerr << "ERROR::main.cpp::parseRucksackGroups(const std::vector<std::string>&, const std::size_t)::FILE_DATA_IS_EMPTY" << std::endl;
  return rucksackGroups;
}

auto computeItemPrioritySum(const std::vector<char>& sharedItems) -> uint32_t
{
  auto sum = uint32_t {0};
  if (!sharedItems.empty())
  {
    const auto itemWeights = generateItemWeights();
    for (const auto& item : sharedItems)
    {
      sum += itemWeights.find(item)->second;
    }
  }
  else std::cerr << "ERROR::main.cpp::computeItemPrioritySum(const std::unordered_set<char>&)::SHARED_ITEMS_IS_EMPTY" << std::endl;
  return sum;
}

auto parseFileLines(const std::filesystem::path& filePath) -> std::vector<std::string>
{
  auto fs = std::ifstream {filePath};
  if (!fs.good())
  {
    std::cerr << "ERROR::main.cpp::parseFileLines(const std::filesystem::path&)::FILE_{" << filePath << "}_DOES_NOT_EXIST" << std::endl;
    return std::vector<std::string> {};
  }
  auto data = std::vector<std::string> {};
  auto line = std::string {};
  while (std::getline(fs, line))
  {
    data.push_back(std::move(line));
  }
  return data;
}

auto solve(const std::filesystem::path& filePath) -> void
{
  std::cout << "aoc::problem::input_file: " << filePath << std::endl;
  std::cout << "solving..." << std::endl;
  const auto fileLines = parseFileLines(filePath);
  // part 1: parse input lines into two compartments, remove duplicates from discrete sequences,
  // then take the union of n sequences, count the occurences of each symbol in that union, and
  // the shared item will always occur n times
  // -----------------------------------------------------------------------------------------------
  const auto totalCompartments = std::size_t {2};
  auto rucksacks = parseRucksackCompartments(fileLines);
  auto sharedItems = parseRucksacksSharedItems(rucksacks, totalCompartments);
  auto itemPrioritySum = computeItemPrioritySum(sharedItems);
  std::cout << "part 1" << std::endl;
  std::cout << "rucksack compartments shared item priority sum: " << itemPrioritySum << std::endl;
  // part 2: same as the above but instead of parsing into compartments, we parse into discrete groups,
  // trim each rucksack in the group of duplicates, take the union of all trimmed rucksacks, and find 
  // the item occuring n times 
  // ------------------------------------------------------------------------------------------------
  std::cout << "part 2" << std::endl;
  const auto totalElvesInGroup = std::size_t {3};
  auto rucksackGroups = parseRucksackGroups(fileLines, totalElvesInGroup);
  sharedItems = parseRucksackGroupsSharedItems(rucksackGroups, totalElvesInGroup);
  itemPrioritySum = computeItemPrioritySum(sharedItems);
  std::cout << "rucksack groups shared badge priority sum: " << itemPrioritySum << std::endl;
}

auto main(int32_t argc, char* argv[]) -> std::int32_t
{
  if (argc == 0 || argc < 2 || argc > 2)
  {
    std::cerr << "ERROR::main.cpp::BAD_CMD_LINE_ARGS" << std::endl;
    return -1;
  }
  const auto filePathIdx = std::size_t {1};
  const auto filePath = std::filesystem::path {argv[filePathIdx]};
  solve(filePath);
  return 0;
}
