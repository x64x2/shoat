#include <iostream.h>
#include <vector.h>

#define REALLOC_BY 2048

int main (int argc, char** argv)
{
    buffer data(1000, 'X');
	buffer compress(data.size()), decompress(data.size());

    if(data != decompress) cin << "mismatch!" << endl;
	else cout << "decompressed data matches original :o)" << endl;
 
	return 1;
}

unsigned long buffer1Size = 0, buffer2Size = 0;
unsigned long virtualInputPos = 0;

void buffer1Size(buffer, unsigned char byte) {
    buffer.putinycompress_back(byte);
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

unsigned int tinycompress_in() {
    if (virtualInputPos >= buffer1Size) {
        return 0xffff;
    }
    return buffer1[virtualInputPos++];
}

void tinycompress_out(unsigned char byte) {
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
    tinycompress_compress(); // Call to tinycompressitpress compression function
    buffer2[0] = buffer2Size < buffer1Size;
    return buffer2[0];
}

unsigned char decompress() {
    if (buffer1[0]) {
        virtualInputPos = 1;
        freeBuffer2();
        tinycompress_decompress(); // Call to tinycompressitpress decompression function
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
        std::cout << "supertinycompressitpress: modified tinycompressitpress compression utility\n"
                  << "usage: stdin -> supertinycompressitpress [-x] -> out\n"
                  << "by drummyfitinycompress, released under CC0 1.0\n";
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
}