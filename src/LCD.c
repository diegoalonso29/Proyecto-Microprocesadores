#include "LCD.h"


void init_port(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB| RCC_AHBPeriph_GPIOH, ENABLE);
	//PIN RW SE CONECTA A TIERRA PARA QUE NOS DEJE UNICAMENTE ESCRIBIR (LEER PIN=1)
		GPIO_InitTypeDef GPIO_CONFI;
		//PIN PA0
		GPIO_CONFI.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_CONFI.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_CONFI.GPIO_OType = GPIO_OType_PP;
		GPIO_CONFI.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_CONFI.GPIO_Pin = E;
		GPIO_Init(PortA, &GPIO_CONFI);
		GPIO_SetBits(PortA,E);
	//PIN PA1
		GPIO_CONFI.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_CONFI.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_CONFI.GPIO_OType = GPIO_OType_PP;
		GPIO_CONFI.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_CONFI.GPIO_Pin = RS;
		GPIO_Init(PortA, &GPIO_CONFI);
	//PIN PB0
		GPIO_CONFI.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_CONFI.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_CONFI.GPIO_OType = GPIO_OType_PP;
		GPIO_CONFI.GPIO_Pin = D0;
		GPIO_CONFI.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(PortB, &GPIO_CONFI);
	//PIN PB1
		GPIO_CONFI.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_CONFI.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_CONFI.GPIO_OType = GPIO_OType_PP;
		GPIO_CONFI.GPIO_Pin = D1;
		GPIO_CONFI.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_Init(PortB, &GPIO_CONFI);
	//PIN PA2
		GPIO_CONFI.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_CONFI.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_CONFI.GPIO_OType = GPIO_OType_PP;
		GPIO_CONFI.GPIO_Pin = D2;
		GPIO_CONFI.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_Init(PortB, &GPIO_CONFI);
	//PIN PB3
		GPIO_CONFI.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_CONFI.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_CONFI.GPIO_OType = GPIO_OType_PP;
		GPIO_CONFI.GPIO_Pin = D3;
		GPIO_CONFI.GPIO_Speed = GPIO_Speed_40MHz;
		GPIO_Init(PortB, &GPIO_CONFI);
	//PINES PARA LOS BOTONES
		GPIO_CONFI.GPIO_Pin= Up;
		GPIO_CONFI.GPIO_Mode= GPIO_Mode_IN;		//MODO ENTRADA
		GPIO_CONFI.GPIO_PuPd= GPIO_PuPd_NOPULL;	//NO HACE FALTA RESISTENCIA(INTEGRADA)
		GPIO_CONFI.GPIO_Speed=GPIO_Speed_2MHz;	//VELOCIDAD
		GPIO_Init(PortA, &GPIO_CONFI);

		GPIO_CONFI.GPIO_Pin= Down;
		GPIO_CONFI.GPIO_Mode= GPIO_Mode_IN;		//MODO ENTRADA
		GPIO_CONFI.GPIO_PuPd= GPIO_PuPd_NOPULL;	//NO HACE FALTA RESISTENCIA(INTEGRADA)
		GPIO_CONFI.GPIO_Speed=GPIO_Speed_2MHz;	//VELOCIDAD
		GPIO_Init(PortA, &GPIO_CONFI);

		GPIO_CONFI.GPIO_Pin= ok;
		GPIO_CONFI.GPIO_Mode= GPIO_Mode_IN;		//MODO ENTRADA
		GPIO_CONFI.GPIO_PuPd= GPIO_PuPd_NOPULL;	//NO HACE FALTA RESISTENCIA(INTEGRADA)
		GPIO_CONFI.GPIO_Speed=GPIO_Speed_2MHz;	//VELOCIDAD
		GPIO_Init(PortA, &GPIO_CONFI);

}
//Con esta funcion inicializamos la pantalla lcd
void init_lcd(void){
	  init_port();
	  conf_inte();
	  inicializar();
	  function_set();
	  display_on();
	  modeset();
}
void toggleEnable(void){
	GPIO_ToggleBits(PortA,E);	//ENABLE
}
void introducir(void){
	Delay_lcd(time_read);
    toggleEnable();	//Ponemos en enable a 0
    Delay_lcd(10);
    toggleEnable();	//Ponemos el enable a 1
}
void function_set(void){
	//INTRODUCIMOS LOS BITS MAS SIGNIFICATIVOS

	GPIO_ResetBits(PortA,RS);
	PortB->ODR= PortB->ODR | function_Set>>4;	//DESPLAZAMOS 4 BITS A LA DERECHA
	introducir();
	//INTRODUCIMOS LOS BIT MENOS SIGNIFICATIVOS
	GPIO_ResetBits(PortA,RS);
	PortB->ODR = (PortB->ODR & mask2) | (function_Set & mask);
    introducir();
}
void display_on(void){
	//INTRODUCIMOS LOS BITS MAS SIGNIFICATIVOS
		//GPIO_ResetBits(PortA,RS);
		//PortB->ODR =display_on_off>>4;	//DESPLAZAMOS 4 BITS A LA DERECHA
		//introducir();
		//INTRODUCIMOS LOS BIT MENOS SIGNIFICATIVOS
		GPIO_ResetBits(PortA,RS);
		PortB->ODR = (PortB->ODR & mask2) | (display_on_off & mask);
	    introducir();
}
void modeset(void){
	//INTRODUCIMOS LOS BITS MAS SIGNIFICATIVOS
		GPIO_ResetBits(PortA,RS);
		PortB->ODR =0x06>>4;	//DESPLAZAMOS 4 BITS A LA DERECHA
		introducir();
		//INTRODUCIMOS LOS BIT MENOS SIGNIFICATIVOS
		GPIO_ResetBits(PortA,RS);
		PortB->ODR = (PortB->ODR & mask2) | (mode_set & mask);
		introducir();
}
void clear(void){
	//INTRODUCIMOS LOS BITS MAS SIGNIFICATIVOS
		GPIO_ResetBits(PortA,RS);
		PortB->ODR =clear_display>>4;	//DESPLAZAMOS 4 BITS A LA DERECHA
		introducir();
		//INTRODUCIMOS LOS BIT MENOS SIGNIFICATIVOS
		GPIO_ResetBits(PortA,RS);
		PortB->ODR = (PortB->ODR & mask2) | (clear_display & mask);
		introducir();
}
void returnHome(void){
	GPIO_ResetBits(PortA,RS);
	PortB->ODR =return_home>>4;	//DESPLAZAMOS 4 BITS A LA DERECHA
	introducir();
	//INTRODUCIMOS LOS BIT MENOS SIGNIFICATIVOS
	GPIO_ResetBits(PortA,RS);
	PortB->ODR = (PortB->ODR & mask2) | (return_home & mask);
	introducir();
}
void inicializar(void){
	PortB->ODR =0x00;
	PortA->ODR =0x00;
	GPIO_SetBits(PortA,E);
}

void movercursor(int fila, int columna){
	GPIO_ResetBits(PortA,RS);
	columna--;
	//SITUAMOS EL CURSOR AL PINCIPIO DE LA FILA QUE QUERAMOS (valor hexadecimal)
	//SITUAMOS EL CURSOR EN LA COLUMNA QUE QUERAMOS +columna
	switch (fila) {
		case 1:
			PortB->ODR =(0x80+columna)>>4;
			introducir();
			PortB->ODR =(PortB->ODR & mask2) | ((0x80+columna)& mask);
			introducir();
			break;
		case 2:
			PortB->ODR =(0xC0+columna)>>4;
			introducir();
			PortB->ODR =(PortB->ODR & mask2) | ((0xC0+columna) & mask);
			introducir();
			break;
		case 3:
			PortB->ODR =(0X94+columna)>>4;
			introducir();
			PortB->ODR =(PortB->ODR & mask2) | ((0X94+columna) & mask);
			introducir();
			break;
		case 4:
			PortB->ODR =(0XD4+columna)>>4;
			introducir();
			PortB->ODR =(PortB->ODR & mask2) | ((0XD4+columna) & mask);
			introducir();
			break;
	}
}
void write_char(char palabra[20]){
	int x=0;
	while(palabra[x]!=0){
		GPIO_SetBits(PortA,RS);
		PortB->ODR =palabra[x]>>4;
		introducir();

		GPIO_SetBits(PortA,RS);
		PortB->ODR =(PortB->ODR & mask2) | (palabra[x] & mask);
		introducir();
		x++;
	}
}
void write_uint8(uint8_t cadena[20]){
	int x=0;
		while(cadena[x]!='\0'){
			GPIO_SetBits(PortA,RS);
			PortB->ODR =cadena[x]>>4;
			introducir();

			GPIO_SetBits(PortA,RS);
			PortB->ODR =(PortB->ODR & mask2) | (cadena[x] & mask);
			introducir();
			x++;
		}
}
void write_int(int numero){
	char cadena[20];
	sprintf(cadena,"%d",numero);
	write_char(cadena);
}
void write_float(float numero, int decimales){
	char cadena[20];
	ftoa(numero, cadena, decimales);
	write_char(cadena);
}
//Funciones personalizadas para nuestro proyecto
void conf_inte(void){
	//PARAMETROS DE CONFIGURACION
	EXTI_InitTypeDef Configuracion_EXTI;
	EXTI_StructInit(&Configuracion_EXTI);
//CONFIGURAMOS LA INTERRUPCION DEL BOTON USER---------------------

	 EXTI_InitTypeDef struct_EXTI;
	 	 struct_EXTI.EXTI_Line = EXTI_Line8;
	 	 struct_EXTI.EXTI_Mode = EXTI_Mode_Interrupt ;
	 	 struct_EXTI.EXTI_Trigger = EXTI_Trigger_Falling;
	 	 struct_EXTI.EXTI_LineCmd = ENABLE;
	 	 EXTI_Init(&struct_EXTI);

	 	 struct_EXTI.EXTI_Line = EXTI_Line9;
		 struct_EXTI.EXTI_Mode = EXTI_Mode_Interrupt ;
		 struct_EXTI.EXTI_Trigger = EXTI_Trigger_Falling;
		 struct_EXTI.EXTI_LineCmd = ENABLE;
		 EXTI_Init(&struct_EXTI);

		 struct_EXTI.EXTI_Line = EXTI_Line4;
		 struct_EXTI.EXTI_Mode = EXTI_Mode_Interrupt ;
		 struct_EXTI.EXTI_Trigger = EXTI_Trigger_Falling;
		 struct_EXTI.EXTI_LineCmd = ENABLE;
		 EXTI_Init(&struct_EXTI);

//------------------------------------------------------------------
//CONFIGURAMOS LA PRIORIDAD DEL NVIC SOBRE EL PULSADOR USER----------
	 NVIC_InitTypeDef NVIC_InitStructure;
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	 NVIC_Init(&NVIC_InitStructure);

	 NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	 NVIC_Init(&NVIC_InitStructure);
//----------------------------------------------------------------
}
void mensaje_inicial(void){
  clear();
  write_char("   ***Bienvenido***");
  movercursor(2,4);
  write_char("Para continuar");
  movercursor(3,5);
  write_char("Pulse START");
  movercursor(4,7);
  write_char(">START");
}
void menuPrincipal(int posicion){
	 clear();
	 movercursor(1,1);
	 write_char("**Menu de opciones**");
	 movercursor(2,2);
	 write_char("1.Transferir datos");
	 movercursor(posicion,1);
	 write_char(">");
     movercursor(3,2);
     write_char("2.Calibracion");
	 movercursor(4,2);
	 write_char("3.medida");
	 flags=mantenerse_menu_principal;
}
void menu_medida(void){
	clear();
	write_char("**Medida**");
	movercursor(2,1);
	write_char(" Tomando medidas... ");
	movercursor(4,1);
	write_char(">");
	movercursor(4,2);
	write_char("Parar proceso");
	flags=tomando_datos;
}

void menu_2(void){
	clear();
	movercursor(1,1);
	write_char("**Menu de opciones**");
	movercursor(2,1);
	write_char(">");
	movercursor(2,2);
	write_char("4.Salir");
}
void movimientoCursor(void){
	if(flags==mantenerse_menu_principal){
		if(cursor==up){		// Go up
			if(posicion_cursor<=4){
			  Delay_lcd(15000);
			  posicion_cursor++;
			  if(posicion_cursor==5) menu_2();
			  else menuPrincipal(posicion_cursor);
			  cursor=reposo;
		  }
		}
		else if(cursor==down){	//Go down
			if(posicion_cursor>2){
				  Delay_lcd(15000);
				  posicion_cursor--;
				  menuPrincipal(posicion_cursor);
				  cursor=reposo;
			 }
		}
	}
}

void entrar_user_menu(void){
	if(flags==menu_secundario  && entrar_menu==1){
		Delay_lcd(13000);
		if(entrar_opcion==stop) entrar_opcion=entrar;
		else if(entrar_opcion==entre_entrar_salir) entrar_opcion=salir;
	}
	if(entrar_opcion==entrar){
		Delay_lcd(13000);
		enviar_a_opcion=posicion_cursor;
		flags=tomando_datos;
		if(posicion_cursor==2 || posicion_cursor==3) flags=menu_secundario;
		entrar_opcion=entre_entrar_salir;
		entrar_menu=0;
	}
	else if(entrar_opcion==salir){
		Delay_lcd(13000);
		if(posicion_cursor==4){
			//menuPrincipal(posicion_cursor);
			flags=parar_de_tomar_datos;
		}
		else if(posicion_cursor==5){
			menu_2();
		}
		else{
			menuPrincipal(posicion_cursor);

		}
		entrar_menu=0;
		entrar_opcion=stop;
	}

}
void inicilizar_variables(void){
	entrar_menu=0;
	posicion_cursor=2;
	entrar_opcion=stop;
}
void menu_opciones(void){
	clear();
	movercursor(1,1);
	switch (enviar_a_opcion) {
		case 2:
			write_char("**Transferir datos**");
			movercursor(4,1);
			write_char(">");
			movercursor(4,2);
			write_char("Volver");
			break;
		case 3:
			clear();
			write_char("**Calibracion**");
			movercursor(4,1);
			write_char(">");
			movercursor(4,2);
			write_char("Volver");
			break;
		case 4:
			menu_medida();
			break;
		case 5:
			write_char("**Fin del programa**");
			movercursor(2,1);
			write_char("Retire la tarjeta SD");
			movercursor(4,1);
			write_char("Apagando sistema...");
			flags=salir_programa;
			break;
	}
	enviar_a_opcion=0;
}

void EXTI4_IRQHandler(void){
	if(EXTI_GetFlagStatus(EXTI_Line4)!=0){
		if(flags==mantenerse_menu_principal){
			cursor=down;	//Mueve el cursor hacia abajo
		}

		EXTI_ClearITPendingBit(EXTI_Line4);	// LIMPIAMOS EL FLAG
		}
}
void EXTI9_5_IRQHandler(void){
	if(EXTI_GetFlagStatus(EXTI_Line8)!=0){
		if(flags==mantenerse_menu_principal){
			cursor=up;	//Mueve el cursor hacia arriba
		}
		EXTI_ClearITPendingBit(EXTI_Line8);	// LIMPIAMOS EL FLAG
		}

	if(EXTI_GetFlagStatus(EXTI_Line9)!=0){
		if(flags==menu_inicial){
			flags=Paso_a_menu_principal;
		}
		else if(flags==mantenerse_menu_principal || flags==menu_secundario || flags==menu_mediciones){
			flags=menu_secundario;
			entrar_menu=1;
		}
		EXTI_ClearITPendingBit(EXTI_Line9);	// LIMPIAMOS EL FLAG
	}
}


//----------------------------------------------





