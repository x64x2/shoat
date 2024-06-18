#include <iostream>
#include "sh.h"

unsigned int SPR_in() {
    int v = std::cin.get();
    return v != EOF ? v : 0xffff;
}

void SPR_out(unsigned char byte) {
    std::cout.put(byte);
}

int main(int argc, char **argv) {
    char param = argc > 1 && argv[1][0] == '-' && argv[1][2] == 0 ? argv[1][1] : 0;

    if (param == 'x')
        SPR_decompress();
    else if (param == 'h') {
        std::cout << "sh: simple compression util\n"
                  "usage: stdin -> sh [-x] -> out\n";
    }
    else
        SPR_compress();

    return 0;
}