#include <iostream>
#include <vector>
#include <algorithm>

#define REALLOC_BY 2048

using Buffer = std::vector<unsigned char>;

void readInput(Buffer& buffer) {
    int c;
    while ((c = std::cin.get()) != EOF) {
        buffer.push_back(c);
    }
}

void reallocateBuffer(Buffer& buffer) {
    if (buffer.size() % REALLOC_BY == 0) {
        buffer.reserve(buffer.size() + REALLOC_BY);
    }
}

void appendToBuffer(Buffer& buffer, unsigned char byte) {
    buffer.push_back(byte);
    reallocateBuffer(buffer);
}

unsigned int SH_in() {
    static Buffer buffer;
    static unsigned long virtualInputPos = 0;
    static unsigned long buffer1Size = 0;

    if (virtualInputPos >= buffer1Size) {
        return 0xffff;
    }
    return buffer[virtualInputPos++];
}

Buffer buffer2;
unsigned long buffer2Size = 0;

void SH_out(unsigned char byte) {
    appendToBuffer(buffer2, byte);
}

void freeBuffer2() {
    buffer2.clear();
    buffer2Size = 0;
}

bool compress() {
    freeBuffer2();
    appendToBuffer(buffer2, 1);

    SH_compress(); // Call to shitpress compression function

    buffer2[0] = buffer2Size < buffer1Size;
    return buffer2[0];
}

bool decompress() {
    if (buffer1[0]) {
        freeBuffer2();
        virtualInputPos = 1;

        SH_decompress(); // Call to shitpress decompression function

        buffer1 = buffer2;
        buffer1Size = buffer2Size;
        buffer2Size = 0;
    }
    return buffer1[0];
}

void outputBuffer(const Buffer& buffer) {
    std::cout.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
}

int main(int argc, char** argv) {
    char param = (argc > 1 && argv[1][0] == '-' && argv[1][2] == '\0') ? argv[1][1] : '\0';

    Buffer buffer1;
    unsigned long buffer1Size = 0;

    if (param == 'x') {
        readInput(buffer1);
        while (decompress()) {
            // Do nothing
        }
        outputBuffer(buffer1.begin() + 1, buffer1.end());
    } else if (param == 'h') {
        std::cout << "supershitpress: modified shitpress compression utility\n"
                  << "usage: stdin -> supershitpress [-x] -> out\n"
                  << "by erik\n";
    } else {
        appendToBuffer(buffer1, 0);
        readInput(buffer1);
        while (compress()) {
            buffer1 = buffer2;
            buffer1Size = buffer2Size;
            buffer2Size = 0;
        }
        outputBuffer(buffer1);
    }

    return 0;
}