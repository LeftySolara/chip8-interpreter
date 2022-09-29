#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 4096
#define FONT_START_LOCATION 0x050
#define FONT_END_LOCATION 0x09F
#define PROGRAM_START_LOCATION 0x200
#define PROGRAM_END_LOCATION 0xE8F

uint8_t gRAM[MEMORY_SIZE];

/**
 * @brief Check if the system is big or little endian.
 *
 * @return int 1 if the system is little endian, or 0 if it is bit endian.
 */
int is_little_endian()
{
    int n = 1;
    return (*(char *)&n == 1);
}

void print_memory()
{
    for (int i = 0; i < MEMORY_SIZE; ++i) {
        printf("%02x ", gRAM[i]);
    }
    printf("\n");
}

void load_font()
{
    uint8_t sprites[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
        0x20, 0x60, 0x20, 0x20, 0x70,  // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80   // F
    };

    for (int i = 0; i < FONT_END_LOCATION - FONT_START_LOCATION; ++i) {
        gRAM[FONT_START_LOCATION + i] = sprites[i];
    }
}

/**
 * @brief Load the program at the specified path.
 *
 * @param path The path to the program to load.
 */
void load_program(const char *path)
{
    FILE *fp = fopen(path, "rb");
    uint8_t *loaded_program = gRAM + PROGRAM_START_LOCATION;

    fread(loaded_program, sizeof(gRAM), 2, fp);

    if (is_little_endian()) {
        /* Reverse each pair of bytes. */
        uint8_t temp;
        for (int i = PROGRAM_START_LOCATION; i <= PROGRAM_END_LOCATION; i = i + 2) {
            temp = gRAM[i];
            gRAM[i] = gRAM[i + 1];
            gRAM[i + 1] = temp;
        }
    }

    fclose(fp);
}

int main(int argc, char **argv)
{
    memset(gRAM, 0, MEMORY_SIZE);

    load_font();
    load_program(argv[1]);
    print_memory();

    return 0;
}
