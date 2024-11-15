#include <iostream.h>
#include "tinycompress.h"

unsigned int tinycompress_in() {
    int v = std::cin.get();
    return v != EOF ? v : 0xffff;
}

void tinycompress_out(unsigned char byte) {
    std::cout.put(byte);
}

int main(int argc, char **argv) {
    char param = argc > 1 && argv[1][0] == '-' && argv[1][2] == 0 ? argv[1][1] : 0;

    if (param == 'x')
        tinycompress_decompress();
    else if (param == 'h') {
        std::cout << "tinycompress: simple compression util\n"
                  "usage: stdin -> tinycompress [-x] -> out\n";
    }
    else
        _compress();

    return 0;
}