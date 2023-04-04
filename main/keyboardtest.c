#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// -- Nomeando as saídas e entradas do ESP32 --;
#define SH GPIO_NUM_2       // -- Pino do registrador de deslocamento (74HC595) - SERIAL IN --;
#define CLK GPIO_NUM_4      // -- Pino do registrador de deslocamento (74HC595) - CLOCK --;
#define WR GPIO_NUM_16      // -- Pino do registrador de deslocamento (74HC595) - LATCH --;
#define RD GPIO_NUM_15      // -- Pino do registrador de deslocamento (74HC165) - SH/LD --;
#define column 4            // -- Definindo 4 colunas --;
#define rows 4              // -- Definindo 4 linhas --;

// -- Defininições de variaveis --;
static const char *TAG = "@vyniexec";

// -- Definindo os valores das teclas --;
const uint8_t teclas[rows][column] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// -- Inicializando e limpando as portas do teclado --;
void keyboardInit(void)
{
    gpio_reset_pin(SH);                 // -- Resetando todos os pinos a serem usados --;
    gpio_reset_pin(CLK);                // -- Resetando todos os pinos a serem usados --;
    gpio_reset_pin(WR);                 // -- Resetando todos os pinos a serem usados --;
    gpio_reset_pin(RD);                 // -- Resetando todos os pinos a serem usados --;

    // -- Configurandos as portas --;
    esp_rom_gpio_pad_select_gpio(SH);
    gpio_set_direction(SH, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(CLK);
    gpio_set_direction(CLK, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(WR);
    gpio_set_direction(WR, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(RD);
    gpio_set_direction(RD, GPIO_MODE_INPUT);

    gpio_set_level(SH, 0);              // -- Zerando as portas --;
    gpio_set_level(CLK, 0);             // -- Zerando as portas --;
    gpio_set_level(WR, 0);              // -- Zerando as portas --;
    gpio_set_level(RD, 0);              // -- Zerando as portas --;
}

// -- Criando a função de escrita e leitura do teclado --;
void varredura(void)
{
    int linha = 0, coluna = 0;
    for (linha = 0; linha < rows; linha++)
    {   
        for (coluna = 0; coluna < column; coluna++)
        {
            gpio_set_level(WR, 1);
            gpio_set_level(CLK, 1);
            gpio_set_level(CLK, 0);
            gpio_set_level(SH, 1);
            if(gpio_get_level(RD) == 1)
                ESP_LOGI(TAG, "Tecla pressionada: %c", teclas[linha][coluna]);
            
        }
        gpio_set_level(WR, 0);
        gpio_set_level(SH, 0);
    }
}

void app_main(void)
{
    while (1)
    {
        keyboardInit();
        varredura();
        vTaskDelay(10 / portTICK_PERIOD_MS); // -- Espera 10 milisegundos para proxima varredura --;
    }
}