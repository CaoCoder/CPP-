#pragma once

#include <iostream>
#include <ctime>

std::ostream& Log()
{
    std::cout << "FOt Debug |" << "timestamp: " << (uint64_t)time(nullptr) << " | ";
    return std::cout;
}