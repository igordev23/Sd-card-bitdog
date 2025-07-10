#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ff.h"

// SPI Pins
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

FATFS fs;  // File system object
FIL fil;   // File object

// Função exigida pelo FatFs para timestamps
DWORD get_fattime(void) {
    return ((DWORD)(2025 - 1980) << 25)
         | ((DWORD)7 << 21)
         | ((DWORD)9 << 16)
         | ((DWORD)16 << 11)
         | ((DWORD)45 << 5)
         | ((DWORD)0 >> 1);
}

void init_spi_sdcard() {
    spi_init(spi0, 1000 * 1000); // 1 MHz para init
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1); // Deseleciona SDCard

    printf("SPI SDCard initialized\n");
}

void write_to_sd() {
    FRESULT fr;

    fr = f_mount(&fs, "", 1);
    if (fr != FR_OK) {
        printf("❌ Falha ao montar o SD Card (erro: %d)\n", fr);
        return;
    }
    printf("✅ SD Card montado\n");

    fr = f_open(&fil, "coords.txt", FA_WRITE | FA_OPEN_ALWAYS);
    if (fr == FR_OK) {
        f_lseek(&fil, f_size(&fil)); // append

        const char *text = "Lat: -5.12345, Lon: -42.98765\n";
        UINT bw;
        fr = f_write(&fil, text, strlen(text), &bw);
        if (fr == FR_OK && bw == strlen(text)) {
            printf("✅ Dados escritos (%u bytes)\n", bw);
        } else {
            printf("❌ Erro ao escrever (erro: %d)\n", fr);
        }
        f_close(&fil);
    } else {
        printf("❌ Erro ao abrir arquivo (erro: %d)\n", fr);
    }
}

void read_from_sd() {
    FRESULT fr;
    char buffer[128];
    UINT br;

    fr = f_open(&fil, "coords.txt", FA_READ);
    if (fr != FR_OK) {
        printf("❌ Falha ao abrir arquivo para leitura (erro: %d)\n", fr);
        return;
    }

    printf("📄 Conteúdo de coords.txt:\n");
    do {
        fr = f_read(&fil, buffer, sizeof(buffer) - 1, &br);
        if (fr != FR_OK) {
            printf("❌ Erro ao ler arquivo (erro: %d)\n", fr);
            break;
        }
        buffer[br] = '\0'; // Null-terminate para imprimir string
        printf("%s", buffer);
    } while (br == sizeof(buffer) - 1);

    f_close(&fil);
}

int main() {
    stdio_init_all();
    sleep_ms(2000);

    printf("Inicializando SD Card...\n");
    init_spi_sdcard();
    sleep_ms(500);

    while (true) {
        write_to_sd();
        read_from_sd();
        sleep_ms(5000);
    }
}
