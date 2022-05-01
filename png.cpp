#include <stdint.h>

#include <iostream>
#include <fstream>

#include <arpa/inet.h>

#define BLOCK_SIZE 512
#define MAGIC_SIZE 8

struct ImageHeader {
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colourType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
} __attribute__((__packed__));

struct ChunkHeader {
    uint32_t length;
    char chunkType[4];
    char chunkData[0];
} __attribute__((__packed__));

struct DataStream {
    char magic[MAGIC_SIZE];
    struct ChunkHeader headers[0];
} __attribute__((__packed__));


char block[BLOCK_SIZE];

int main(int argc, char** argv) {
    std::fstream image;
    image.open(argv[1], std::ios::in);
    image.read((char*) &block, BLOCK_SIZE);

    struct DataStream* stream = (struct DataStream*) block;

    printf("signature: ");
    for (int i = 0; i < MAGIC_SIZE; i++) {
        printf("%hhu ", stream->magic[i]);
    }
    printf("\n");

    struct ImageHeader* imageHeader = (struct ImageHeader*) stream->headers[0].chunkData;
    printf("chunkType: %.4s\n", stream->headers[0].chunkType);
    printf("width: %u\n", htonl(imageHeader->width));
    printf("height: %u\n", htonl(imageHeader->height));

    return(0);
}