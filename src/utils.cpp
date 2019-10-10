#include "h/utils.h"

#include <cstdlib>

int SA::randInt(int min, int max){
    int range = max - min;
    return std::rand() % range + min;
}