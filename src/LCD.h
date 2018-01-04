#ifndef __LCD_H_
#define __LCD_H_
//Librerias
#include "stm32l1xx_gpio.h"
#include "strings.h"

#include "Timer.h"

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
//Variables que usamos en nuestro proyecto
uint8_t comprobar_menu_inicial;
uint8_t posicion_cursor;
uint8_t movimiento_cursor;
uint8_t pulse_ok;
uint8_t entrar_menu_user;
uint8_t mantenerse_opcion;
uint8_t volver;
uint8_t enviar_a_opcion;
uint8_t parar_medidas;
uint8_t salir_programa;


uint8_t menu_medidas;
//----------------------------------------
//Funciones utilizadas en nuestro proyecto
void menuPrincipal(int posicion);
void menu_2(void);
void menu_opciones(void);
void conf_inte(void);
void mensaje_inicial(void);
void movimientoCursor(void);
void entrar_user_menu(void);
void menu_medida(void);
void inicilizar_variables(void);

void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

//-----------------------------------------
#endif

