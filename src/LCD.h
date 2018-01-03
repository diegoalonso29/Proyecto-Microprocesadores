#ifndef __LCD_H_
#define __LCD_H_
//Librerias
#include "stm32l1xx_gpio.h"
#include "strings.h"

//Constantes---------------------
#define E 		GPIO_Pin_0		//PA0
#define RS 		GPIO_Pin_1		//PA1
#define D0  	GPIO_Pin_0		//PB0
#define D1		GPIO_Pin_1		//PB1
#define D2		GPIO_Pin_2		//PB2
#define D3 		GPIO_Pin_3		//PB3
//PULSADORES PARA MOVERSE A TRAVES DEL MENU-----
#define Up		GPIO_Pin_4
#define Down	GPIO_Pin_8
#define ok		GPIO_Pin_9
//Puertos que utilizamos para la configuracion de la pantalla
#define PortA	GPIOA
#define PortB	GPIOB
//----------------------------------------------
#define MAX_PRECISION	(10)
#define time_read 37	//Tiempo que es necesario para leer la pantalla LCD los comandos
#define mask 0x0F
#define mask2 0XF0
#define clear_display 0x01
#define return_home 0x80
#define mode_set 0x06
#define display_on_off 0x0D
#define function_Set 0x28
#define line1 0x80
#define line2 0xC0
#define line3 0X94
#define line4 0XD4

//-------------------------------


//Funciones-----------------------------
void init_port(void);
void init_lcd(void);
void toggleEnable(void);
void introducir(void);
void function_set(void);
void display_on(void);
void modeset(void);
void clear(void);
void returnHome(void);
void inicializar(void);
void write_char(char palabra[20]);
void write_uint8(uint8_t cadena[20]);
void write_int(int numero);
void write_float(float numero,int decimales);
void movercursor(int fila, int columna);
//----------------------------------------
//Funciones utilizadas en nuestro proyecto
void menuPrincipal(int posicion);
void conf_inte(void);
void mensaje_inicial(void);
void menu(void);
//void EXTI4_IRQHandler(void);
//void EXTI9_5_IRQHandler(void);
//void entrar_menu_opciones(void);
//-----------------------------------------
#endif

