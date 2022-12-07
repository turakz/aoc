    /*

      ...thar be dragons below...

      ---

      -> not gonna lie this one was a little difficult for me to wrap my head around
      -> brute force using a contains(s, t) function to test for the existence of s in t, sorting to reason top-down by process
      of elimination, and sorting to normalize any signal occurrences hashed as keys
    
      -> this took me way to fucking long to figure out, christ

    */
    #include <iostream>
    #include <fstream>
    #include <unordered_map>
    #include <unordered_set>
    #include <vector>
    #include <string>
    #include <sstream>
    #include <algorithm>
    
    auto contains(const std::string& s, const std::string& t) -> bool
    {
      auto lookingFor = std::unordered_map<char, uint32_t>{};
      for (const auto& c : t)
      {
        ++lookingFor[c];
      }
      for (const auto& c : s)
      {
        if (lookingFor[c] < 1) return false;
      }
      return true;
    }
    
    auto analyzeSignal(std::vector<std::string>& knownSignals, const std::string& unknownSignal) -> std::vector<std::string>
    {
      if (knownSignals.empty())
      {
        std::cerr << "ERROR::analyzeSignal(...)::KNOWN_SIGNALS_NOT_PARSED" << std::endl;
        return knownSignals;
      }
      auto nine = std::string{knownSignals[9]};
      auto eight = std::string{knownSignals[8]};
      auto seven = std::string{knownSignals[7]};
      auto six = std::string{knownSignals[6]};
      auto five = std::string{knownSignals[5]};
      auto four = std::string{knownSignals[4]};
      auto three = std::string{knownSignals[3]};
      auto two = std::string{knownSignals[2]};
      auto one = std::string{knownSignals[1]};
      auto zero = std::string{knownSignals[0]};
      if (contains(unknownSignal, eight))
      {
        // parse 0, 6, and 9 first
        if (unknownSignal.size() == 6)
        {
          if (contains(seven, unknownSignal) && contains(one, unknownSignal) && contains(four, unknownSignal))
          {
            std::cout << "parsed " << unknownSignal << " -> 9" << std::endl;
            knownSignals[9] = unknownSignal;
          }
          else if (contains(seven, unknownSignal) && contains(one, unknownSignal) && !contains(four, unknownSignal))
          {
            std::cout << "parsed " << unknownSignal << " -> 0" << std::endl;
            knownSignals[0] = unknownSignal;
          }
          else if (!contains(seven, unknownSignal) && !contains(one, unknownSignal) && !contains(four, unknownSignal))
          {
            std::cout << "parsed " << unknownSignal << " -> 6" << std::endl;
            knownSignals[6] = unknownSignal;
          }
        }
        // get the remaining 2, 3, 5
        else if (unknownSignal.size() == 5)
        {
          if (contains(unknownSignal, nine) && !contains(unknownSignal, six) && contains(seven, unknownSignal) && contains(one, unknownSignal) && !contains(four, unknownSignal))
          {
            std::cout << "parsed " << unknownSignal << " -> 3" << std::endl;
            knownSignals[3] = unknownSignal;
          }
          else if (contains(unknownSignal, six) && !contains(unknownSignal, zero) && !contains(seven, unknownSignal) && !contains(one, unknownSignal) && !contains(four, unknownSignal))
          {
            std::cout << "parsed " << unknownSignal << " -> 5" << std::endl;
            knownSignals[5] = unknownSignal;
          }
          else 
          {
            std::cout << "parsed " << unknownSignal << " -> 2" << std::endl;
            knownSignals[2] = unknownSignal;
          }
        }
        // now we have all knowns in the current configuration
      }
      else
      {
        std::cerr << "ERROR::analyzeSignal(...):::FAILED_TO_PARSE_SIGNALS_CORRECTLY" << std::endl;
      }
      return knownSignals;
    }
    
    auto generateSignalMappings(const std::vector<std::string>& knownPatterns) -> std::unordered_map<std::string, std::size_t>
    {
      auto toDigit = std::unordered_map<std::string, std::size_t>{};
      for (std::size_t valueIdx = 0; valueIdx < knownPatterns.size(); ++valueIdx)
      {
        std::cout << "knownPattern: " << knownPatterns[valueIdx] << " -> " << valueIdx << std::endl;
        auto knownPattern = knownPatterns[valueIdx];
        std::sort(std::begin(knownPattern), std::end(knownPattern));
        toDigit.insert({knownPattern, valueIdx});
      }
      return toDigit;
    }
    
    auto splitPatternIntoKnownsAndUnknowns(const std::string& pattern) -> std::pair<std::vector<std::string>, std::vector<std::string>>
    {
      auto uniqueSizeToDigit = std::unordered_map<std::size_t, std::uint32_t>{{2, 1}, {3, 7}, {4, 4}, {7, 8}};
      auto signalPatternStream = std::stringstream{pattern};
      auto signalPatternToken = std::string{""};
      auto unknownPatterns = std::vector<std::string>{};
      std::vector<std::string> knownPatterns(10, std::string{""});
      while (std::getline(signalPatternStream, signalPatternToken, ' '))
      {
        auto sizeKey = std::size_t{signalPatternToken.size()};
        if (uniqueSizeToDigit.find(sizeKey) != std::end(uniqueSizeToDigit))
        {
          auto valueIdx = uniqueSizeToDigit[sizeKey];
          knownPatterns[valueIdx] = signalPatternToken;
        }
        else
        {
          unknownPatterns.push_back(signalPatternToken);
        }
      }
      return {knownPatterns, unknownPatterns};
    }
    
    auto processSignalPatterns(const std::vector<std::string>& signalPatterns, const std::vector<std::string>& outputPatterns) -> std::uint32_t 
    {
      auto solnSum = std::uint32_t{0};
      auto outputValueIdx = std::size_t{0};
      for (const auto& signalPattern : signalPatterns)
      {
        std::cout << "signalPattern: " << signalPattern << std::endl;
        auto parsedPatterns = splitPatternIntoKnownsAndUnknowns(signalPattern);
        auto knownPatterns = parsedPatterns.first;
        auto unknownPatterns = parsedPatterns.second;
        // process and analyze
        auto sortPatternsBySizeAsc = [](const std::string& s, const std::string& t)
        {
          return s.size() >= t.size();
        };
        std::sort(std::begin(unknownPatterns), std::end(unknownPatterns), sortPatternsBySizeAsc);
        for (const auto& unknownSignal : unknownPatterns)
        {
          knownPatterns = analyzeSignal(knownPatterns, unknownSignal);
        }
        // establish mappings
        auto toDigit = generateSignalMappings(knownPatterns);
        // read output pattern
        auto decodedOutput = std::string{""};
        if (outputValueIdx < outputPatterns.size())
        {
          // parse and decode
          auto outputPattern = outputPatterns[outputValueIdx];
          auto outputPatternStream = std::stringstream{outputPattern};
          auto outputSignal = std::string{""};
          while (std::getline(outputPatternStream, outputSignal, ' '))
          {
            std::sort(std::begin(outputSignal), std::end(outputSignal));
            decodedOutput += std::to_string(toDigit[outputSignal]);
          }
          // track solution
          std::cout << "outputPattern: " << outputPattern << std::endl;
          std::cout << "decoded: " << decodedOutput << std::endl;
          solnSum += std::stoi(decodedOutput);
        }
        ++outputValueIdx;
      }
      // thank fuck, we're done
      return solnSum;
    }
    
    auto processOutputValues(const std::vector<std::string>& outputValues) -> std::uint32_t
    {
      if (outputValues.empty()) return std::uint32_t{0};
      auto uniqueDigits = std::unordered_map<std::size_t, uint32_t>{{2, 1}, {3, 1}, {4,1}, {7, 1}};
      auto uniqueOccurences = std::uint32_t{0};
      for (const auto& outputValuePattern : outputValues)
      {
        auto ss = std::stringstream{outputValuePattern};
        auto digitToken = std::string{""};
        while (std::getline(ss, digitToken, ' '))
        {
          if (uniqueDigits.find(digitToken.size()) != std::end(uniqueDigits)) ++uniqueOccurences;
        }
      }
      return uniqueOccurences;
    }
    
    auto solvePuzzle(const std::string& inputFileName, const char delim) -> void
    {
      auto ifs = std::ifstream{inputFileName};
      auto line = std::string{""};
      auto signalPatterns = std::vector<std::string>{};
      auto outputPatterns = std::vector<std::string>{};
      while (std::getline(ifs, line))
      {
        auto delimPos = line.find_first_of(delim);
        auto leftSide = std::string{std::begin(line), std::begin(line) + delimPos - 1};
        auto rightSide = std::string{line.substr(delimPos + 2, line.size() - delimPos - 2)};
        signalPatterns.push_back(std::move(leftSide));
        outputPatterns.push_back(std::move(rightSide));
      }
      std::cout << "---part 1---" << std::endl;
      auto uniqueOutputValueCounts = processOutputValues(outputPatterns);
      std::cout << "soln: " << uniqueOutputValueCounts << std::endl;
      std::cout << "---part 2---" << std::endl;
      auto totalOutputValuesSum = processSignalPatterns(signalPatterns, outputPatterns);
      std::cout << "soln: " << totalOutputValuesSum << std::endl;
    }
    
    auto main(void) -> int
    {
      //solvePuzzle("example-input.txt", '|');
      solvePuzzle("input.txt", '|');
      return 0;
    }
