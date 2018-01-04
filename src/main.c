#include "main.h"

I2C_Error_Code status;
MPU6050_Data_RPY rpy;
StateMachine STATE;

int i;

int main(void)
{


	STATE=Initialization;
	while (1)
	{
		StateMachineSystem();
	}

	return 0;
}


void StateMachineSystem(void)
{
	switch (STATE) {

		case Initialization:

			/****************************INICIALIZACIONES DE DANI **********************/
			init_lcd();			//Inicializamos la pantalla LCD
			comprobar_menu_inicial=0;
			posicion_cursor=2;
			movimiento_cursor=0;
			entrar_menu_user=0;
			pulse_ok=0;
			volver=0;
			mantenerse_opcion=0;
			enviar_a_opcion=0;
			parar_medidas=0;
			mensaje_inicial();

			/***************************************************************************/
			rpy.pitch = 0;
			rpy.roll = 0;

			SystemInit();
			Systick_Configuration();
			USART2_Init(57600);

			USART_Send(USART2, "Inicializando MPU6050... \n\n");
			status = MPU6050_InitConfig(ACCEL_FS, GYRO_FS, SAMPLE_FREQ);
	//		if(status)
	//		{
	//			STATE = Error;
	//			break;
	//		}

			STATE = Wait;

			break;

		case Wait:

			MPU6050_Config_ContinuousMeasurement(0);

			/* EN ESTE CASO DANI TIENE QUE CONDUCIR EL PROGRAMA EN FUNCION DE LOS BOTONES, O NO HACER NADA
			 * SI NO SE TOCA NINGUN BOTON
			 */
			if(comprobar_menu_inicial==1) menuPrincipal(posicion_cursor);

			movimientoCursor();
			entrar_user_menu();

			switch (enviar_a_opcion) {
				case 2:
					STATE =ExportData;
					break;
				case 3:
					STATE =	Calibration;
					break;
				case 4:
					STATE =	Measurement;
					menu_medidas=0;
					break;
				case 5:
					STATE =	ShutDown;
					break;
			}
			enviar_a_opcion=0;
			break;

		case Measurement:

			if(menu_medidas==0) menu_opciones();
			parar_medidas=0;
			entrar_user_menu();
			if(parar_medidas==1) STATE =	Wait;

			MPU6050_Config_ContinuousMeasurement(1);
			if(data_available)
			{
			uint8_t cipote_actual = data_available;

			for(i=0;i<cipote_actual; i++)
			{

			MPU6050_Get_RPY_Data(&rpy, &Buffer_Data[i]);
			USART_Send(USART2, "Roll: ");
			USART_SendFloat(USART2, rpy.roll,2);
			USART_Send(USART2, "\tPitch: ");
			USART_SendFloat(USART2, rpy.pitch,2);
			USART_Send(USART2, "\n");
			data_available--;
			}
			pos_buffer = 0;
			}


			/* AQUÍ DANI TIENES QUE, EN CASO DE QUE SE HAYA PULSADO LA OPCION DE PARAR O STOP, REDIRIGIR AL ESTADO WAIT(ESPERA)
			 * ES DECIR IF(LO QUE SEA) -> STATE = Wait; SI NO SE PULSA NADA NO PONGAS NADA YA QUE VOLVERÁ A ENTRAR EN EL MISMO ESTADO
			 * EN LA SIGUIENTE ITERACIÓN
			 */

			break;

		case Calibration:
			menu_opciones();

			USART_Send(USART2, "Realizando Calibración... \n\n");
			status = MPU6050_Calibration();
		//	if(status)
		//	{
		//		STATE = Error;
		//		break;
		//	}

			STATE = Wait;
			break;

		case ExportData:
			menu_opciones();

			STATE = Wait;
			break;

		case Error:

			DisplayErrorCode(status);
			STATE=ShutDown;
			break;

		case ShutDown:
			menu_opciones();

			return 0;
			break;

		default:

			break;

	}
}



