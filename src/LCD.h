#ifndef __LCD_H_
#define __LCD_H_
//Librerias
#include "stm32l1xx_gpio.h"
#include "strings.h"
#include "USART_Lib.h"
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
#define time_read 37			//Tiempo que es necesario para leer la pantalla LCD los comandos
#define mask 0x0F				//Mascara para evitar interferir a mis compañeros
#define mask2 0XF0				//Mascara para evitar interferir a mis compañeros
#define clear_display 0x01		//Valor para limpiar la pantalla
#define return_home 0x80
#define mode_set 0x06			//
#define display_on_off 0x0C		//
#define function_Set 0x28		//
#define line1 0x80				//Valor para acceder a la primera fila de la pantalla
#define line2 0xC0				//Valor para acceder a la segunda fila de la pantalla
#define line3 0X94				//Valor para acceder a la tercera fila de la pantalla
#define line4 0XD4				//Valor para acceder a la cuarta fila de la pantalla

//-------------------------------


//Funciones-----------------------------
void init_port(void);							//Inicializamos los pines que vamos a utilizar
void init_lcd(void);							//Inicializamos la LCD
void toggleEnable(void);						//Alternamos el valor de ENABLE para poder introducir los comandos en la pantalla
void introducir(void);							//Cambiamos el valor de ENABLE a 0 para introducir el comando a la LCD y despues lo volvemos a poner a 1
void function_set(void);						//
void display_on(void);							//
void modeset(void);								//
void clear(void);								//Funcion para limpiar la pantalla
void returnHome(void);
void inicializar(void);							//Funcion para inicializar losp puertos a un valor especifico
void write_char(char palabra[20]);				//Funcion para imprimir una palabra, frase...
void write_uint8(uint8_t cadena[20]);			//Funcion para imprimir un dato de tipo uint8
void write_int(int numero);						//Funcion para imprimir un dato de tipo int
void write_float(float numero,int decimales);	//Funcion para imprimir un dato de tipo float con una cantidad de decimales variable
void movercursor(int fila, int columna);		//Funcion para mover el cursor a una zona de la pantalla especifica
//----------------------------------------
//Variables que usamos en nuestro proyecto
uint8_t posicion_cursor;		//Variable que usamos para saber en que opcion del menu estamos
uint8_t enviar_a_opcion;		//Variable que se envia al programa principal para saber en que opcion estamos
uint8_t entrar_menu;			//Con esta variable sabemos cuando hemos pulsado el boton ok
typedef enum {
	menu_inicial=0,
	Paso_a_menu_principal=1,
	mantenerse_menu_principal=2,
	menu_secundario=3,
	menu_mediciones=4,
	tomando_datos=5,
	parar_de_tomar_datos=6,
	salir_programa=7

} LCDmachine;
LCDmachine flags;

typedef enum {
	reposo=0,
	up=1,
	down=2,
} movimiento_cursor;
movimiento_cursor cursor;

typedef enum {
	salir=0,
	entrar=1,
	stop=2,
	entre_entrar_salir=3

} entrar_opcion_user;
entrar_opcion_user entrar_opcion;


//----------------------------------------
//Funciones utilizadas en nuestro proyecto
void menuPrincipal(int posicion);		//Imprimimos el menu principal en el cual estan las ocpiones del usuario
void menu_2(void);						//Menu de la opcion de salir (pagina dos de opciones)
void menu_opciones(void);				//Contenido de los diferentes menus que tiene a disposicion el usuario
void conf_inte(void);					//Configuracion de las interrupciones
void mensaje_inicial(void);				//Mensaje que se muestra al inicio del programa
void movimientoCursor(void);			//Funcion que hace moverse el cursor en el menu principal
void entrar_user_menu(void);			//Configuracion del boton ok para que podamos entrar y salir de las diferenctes opciones
void menu_medida(void);					//Contenido del menu de tomar medidas
void inicilizar_variables(void);		//Funcion que utilizamos para inicializar las variables a un valor especifico

void EXTI4_IRQHandler(void);			//Contenido de la interrupcion del boton down
void EXTI9_5_IRQHandler(void);			//Contenido de la interrupcion del boton up y del boton ok

//-----------------------------------------
#endif

