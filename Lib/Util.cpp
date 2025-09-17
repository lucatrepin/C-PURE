#ifndef UTIL_CPP
#define UTIL_CPP

#include <iostream>
#include <SFML/System/Sleep.hpp>

namespace ut
{
void ms(std::string msg) {
    std::cout << msg << std::endl;
}
void mc(char caracter) {
    std::cout << caracter << std::endl;
}

void s(float seconds) {
    sf::Time time = sf::seconds(seconds);
    sf::sleep(time);
}
void clearCMD() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear")
    #endif
}
}

#endif