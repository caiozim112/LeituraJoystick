#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
// Definições dos pinos do joystick
#define JOYSTICK_X_PIN 26  // Pino ADC para eixo X (GPIO26 - ADC0)
#define JOYSTICK_Y_PIN 27  // Pino ADC para eixo Y (GPIO27 - ADC1)
#define JOYSTICK_BUTTON_PIN 22  // Pino digital para o botão do joystick

void joystick_init() {
    // Inicializa o ADC
    adc_init();
    
    // Configura os pinos do joystick
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
    
    // Configura o pino do botão como entrada com pull-up
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);
}

void read_joystick(int16_t *x, int16_t *y, bool *button) {
    // Lê o eixo X
    adc_select_input(0);  // ADC0 (GPIO26)
    *x = adc_read();
    
    // Lê o eixo Y
    adc_select_input(1);  // ADC1 (GPIO27)
    *y = adc_read();
    
    // Lê o botão (0 quando pressionado, 1 quando solto)
    *button = gpio_get(JOYSTICK_BUTTON_PIN);
}

int main() {
    stdio_init_all();
    
    joystick_init();
    
    // Aguarda a inicialização do terminal serial
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    printf("Joystick da BitDogLab - Leitura dos valores\n");
    
    int16_t x, y;
    bool button;
    
    while (true) {
        // Lê os valores do joystick
        read_joystick(&x, &y, &button);
        
        // Exibe os valores no terminal
        printf("X: %4d, Y: %4d, Botão: %s\n", 
               x, y, button ? "LIVRE" : "PRESSIONADO");
        
        // Pequena pausa para não sobrecarregar o terminal
        sleep_ms(200);
    }
    
    return 0;
}