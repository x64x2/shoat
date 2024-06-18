#include <iostream>
#include <vector>
#include <algorithm>

#define REALLOC_BY 2048

using buffer_t = std::vector<unsigned char>;

int buffer;
unsigned long buffer1Size = 0, buffer2Size = 0;
unsigned long virtualInputPos = 0;

void buffer1Size(buffer, unsigned char byte) {
    buffer.push_back(byte);
    if (buffer.size() % REALLOC_BY == 0) {
        buffer.reserve(buffer.size() + REALLOC_BY);
    }
}

void freeBuffer1() {
    buffer1.clear();
}

void freeBuffer2() {
    buffer2.clear();
}

unsigned int SPR_in() {
    if (virtualInputPos >= buffer1Size) {
        return 0xffff;
    }
    return buffer1[virtualInputPos++];
}

void SPR_out(unsigned char byte) {
    bufferAppend(buffer2, byte);
}

void readInput() {
    int c;
    while ((c = std::cin.get()) != EOF) {
        bufferAppend(buffer1, c);
    }
}

void copy2To1() {
    freeBuffer1();
    std::swap(buffer1, buffer2);
    buffer1Size = buffer2Size;
    buffer2Size = 0;
}

unsigned char compress() {
    virtualInputPos = 0;
    freeBuffer2();
    bufferAppend(buffer2, 1);
    SPR_compress(); // Call to shitpress compression function
    buffer2[0] = buffer2Size < buffer1Size;
    return buffer2[0];
}

unsigned char decompress() {
    if (buffer1[0]) {
        virtualInputPos = 1;
        freeBuffer2();
        SPR_decompress(); // Call to shitpress decompression function
        copy2To1();
    }
    return buffer1[0];
}

void outputBuffer(const buffer_t& buffer) {
    std::cout.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

int main(int argc, char** argv) {
    char param = (argc > 1 && argv[1][0] == '-' && argv[1][2] == '\0') ? argv[1][1] : '\0';

    if (param == 'x') {
        readInput();
        while (decompress());
        outputBuffer(buffer1.begin() + 1, buffer1.end());
    } else if (param == 'h') {
        std::cout << "supershitpress: modified shitpress compression utility\n"
                  << "usage: stdin -> supershitpress [-x] -> out\n"
                  << "by drummyfish, released under CC0 1.0\n";
    } else {
        bufferAppend(buffer1, 0);
        readInput();
        while (compress()) {
            copy2To1();
        }
        outputBuffer(buffer1);
    }

    freeBuffer1();
    freeBuffer2();

    return 0;
}```