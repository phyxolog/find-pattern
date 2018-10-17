#ifndef FINDPATTERN_HPP
#define FINDPATTERN_HPP

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

// From https://gist.github.com/mrexodia/f058868b81b2f1cb011a

namespace FindPattern {
    typedef struct PatternNibble {
        unsigned char Data;
        bool Wildcard;
    } PatternNibble;

    typedef struct PatternByte {
        PatternNibble Nibble[2];
    } PatternByte;

    std::string FormatPattern(std::string PatternText);
    int HexCharToInt(char ch);
    bool TransformPattern(std::string PatternText, std::vector<PatternByte> &Pattern);
    bool MatchByte(unsigned char Byte, const PatternByte &PatternByte);

    /**
    * @brief Scans a given chunk of data for a given pattern.
    *
    * @param data          The data to scan within for the given pattern.
    * @param pszPattern    The pattern to scan for.
    * @param baseAddress   The base address to add to the final result.
    * @param offset        The offset to add to final address.
    * @param occurrence    Zero-based occurrence of the pattern to return.
    *
    * @return Address of the pattern found, -1 otherwise.
    */
    std::size_t FindPattern(
            std::vector<unsigned char> Data,
            const char* Pattern,
            std::size_t BaseAddress = 0,
            std::size_t Offset = 0,
            int Occurrence = 0);
};


#endif //FINDPATTERN_HPP
