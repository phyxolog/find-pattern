#include "FindPattern.hpp"

namespace FindPattern {
    std::string FormatPattern(std::string PatternText) {
        std::string Result;
        unsigned long len = PatternText.length();
        for (unsigned long i = 0; i < len; i++) {
            if (PatternText[i] == '?' || isxdigit(PatternText[i])) {
                Result += std::toupper(PatternText[i]);
            }
        }

        return Result;
    }

    int HexCharToInt(char ch) {
        return std::stoi(&ch, nullptr, 16);
    }

    bool TransformPattern(std::string PatternText, std::vector<PatternByte> &Pattern) {
        PatternByte newByte;
        Pattern.clear();
        PatternText = FormatPattern(PatternText);
        unsigned long Length = PatternText.length();

        if(!Length)
            return false;

        if (Length % 2) {
            PatternText += '?';
            Length++;
        }

        for(int i = 0, j = 0; i < Length; i++) {
            if (PatternText[i] == '?') {
                newByte.Nibble[j].Wildcard = true;
            } else {
                newByte.Nibble[j].Wildcard = false;
                newByte.Nibble[j].Data = static_cast<unsigned char>(HexCharToInt(PatternText[i]) & 0xF);
            }

            j++;
            if (j == 2) {
                j = 0;
                Pattern.push_back(newByte);
            }
        }

        return true;
    }

    bool MatchByte(const unsigned char Byte, const PatternByte &PatternByte) {
        int matched = 0;

        auto n1 = (Byte >> 4) & 0xF;
        if (PatternByte.Nibble[0].Wildcard) {
            matched++;
        } else if (PatternByte.Nibble[0].Data == n1) {
            matched++;
        }

        auto n2 = Byte & 0xF;
        if (PatternByte.Nibble[1].Wildcard) {
            matched++;
        } else if (PatternByte.Nibble[1].Data == n2) {
            matched++;
        }

        return (matched == 2);
    }

    std::size_t FindPattern(
            std::vector<unsigned char> Data,
            const char* Pattern,
            std::size_t BaseAddress,
            std::size_t Offset,
            int Occurrence) {
        std::vector<PatternByte> PatternData;
        if (!TransformPattern(Pattern, PatternData)) {
            return -1;
        }

        int ResultCount = 0;
        auto ScanStart = Data.begin();

        for(;;) {
            auto ret = search(ScanStart, Data.end(), PatternData.begin(), PatternData.end(), MatchByte);

            if (ret != Data.end()) {
                if (Occurrence == 0 || ResultCount == Occurrence) {
                    return BaseAddress + distance(Data.begin(), ret) + Offset;
                }

                ResultCount++;
                ScanStart = ++ret;
            } else {
                break;
            }
        }

        return -1;
    }
};