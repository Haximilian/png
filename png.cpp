#include <stdint.h>

#include <iostream>
#include <fstream>

#include <arpa/inet.h>
#include <string.h>

#include "crc.h"

#define BLOCK_SIZE 512
#define MAGIC_SIZE 8
#define TYPE_SIZE 4

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
    struct ChunkHeader headerChunck;
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

    struct ChunkHeader* curr = &stream->headerChunck;

    struct ImageHeader* imageHeader = (struct ImageHeader*) curr->chunkData;
    printf("imageWidth: %u\n", htonl(imageHeader->width));
    printf("imageHeight: %u\n", htonl(imageHeader->height));

    for (;;) {
        printf("chunkType: %.4s\n", curr->chunkType);
        printf("chunkLength: %u\n", htonl(curr->length));
        if (strncmp(curr->chunkType, "IEND", TYPE_SIZE) == 0) {
            break;
        }
        curr = (struct ChunkHeader*) ((char*) curr + htonl(curr->length) + 12);
    }

    return(0);
}