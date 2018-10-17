#include <iostream>
#include <string>
#include <vector>

#include "FindPattern.hpp"

int main() {
    unsigned char TestData[16] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
    std::vector<unsigned char> Data(TestData, TestData + sizeof(TestData));
    int Result = FindPattern::FindPattern(Data, "?7 8? ?? CD");
    std::cout << "Result: 0x" << std::hex << Result << std::endl;
    return 0;
}