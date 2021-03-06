#include "FindPattern.hpp"

namespace FindPattern {
    std::string FormatPattern(std::string PatternText) {
        std::string Result;
        unsigned long len = PatternText.length();
        for (unsigned long i = 0; i < len; i++) {
            if (PatternText[i] == '?' || std::isxdigit(PatternText[i])) {
                Result += std::toupper(PatternText[i]);
            }
        }

        return Result;
    }

    int HexCharToInt(char ch) {
        return std::stoi(&ch, nullptr, 16);
    }

    bool TransformPattern(std::string PatternText, std::vector<PatternByte> &Pattern) {
        PatternByte NewByte;
        Pattern.clear();
        PatternText = FormatPattern(PatternText);
        unsigned long Length = PatternText.length();

        if (!Length)
            return false;

        if (Length % 2) {
            PatternText += '?';
            Length++;
        }

        for (int i = 0, j = 0; i < Length; i++) {
            if (PatternText[i] == '?') {
                NewByte.Nibble[j].Wildcard = true;
            } else {
                NewByte.Nibble[j].Wildcard = false;
                NewByte.Nibble[j].Data = static_cast<unsigned char>(HexCharToInt(PatternText[i]) & 0xF);
            }

            j++;
            if (j == 2) {
                j = 0;
                Pattern.push_back(NewByte);
            }
        }

        return true;
    }

    bool MatchByte(const unsigned char Byte, const PatternByte &PatternByte) {
        int Matched = 0;
        auto n1 = (Byte >> 4) & 0xF;
        auto n2 = Byte & 0xF;

        if (PatternByte.Nibble[0].Wildcard) {
            Matched++;
        } else if (PatternByte.Nibble[0].Data == n1) {
            Matched++;
        }

        if (PatternByte.Nibble[1].Wildcard) {
            Matched++;
        } else if (PatternByte.Nibble[1].Data == n2) {
            Matched++;
        }

        return Matched == 2;
    }

    int FindPattern(
            std::vector<unsigned char> Data,
            const char* Pattern,
            int BaseAddress,
            int Offset,
            int Occurrence) {
        std::vector<PatternByte> PatternData;
        if (!TransformPattern(Pattern, PatternData)) {
            return -1;
        }

        int ResultCount = 0;
        auto ScanStart = Data.begin();
        std::vector<unsigned char>::iterator Ret;

        for(;;) {
            Ret = std::search(ScanStart, Data.end(), PatternData.begin(), PatternData.end(), MatchByte);

            if (Ret != Data.end()) {
                if (Occurrence == 0 || ResultCount == Occurrence) {
                    return BaseAddress + static_cast<int>(std::distance(Data.begin(), Ret)) + Offset;
                }

                ResultCount++;
                ScanStart = ++Ret;
            } else {
                break;
            }
        }

        return -1;
    }
};