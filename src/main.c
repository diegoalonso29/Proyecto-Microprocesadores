#include "main.h"

I2C_Error_Code status;

MPU6050_Data_RPY rpy;
MPU6050_Data_Float data;

StateMachine STATE;


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
			if(status)
			{
				STATE = Error;
				break;
			}

			STATE = Wait;

			break;

		case Wait:

			MPU6050_Config_ContinuousMeasurement(0);

			if(comprobar_menu_inicial==1) menuPrincipal(posicion_cursor);

			movimientoCursor();
			entrar_user_menu();

			switch (enviar_a_opcion)
			{
				case 2:
					STATE =ExportData;
					break;
				case 3:
					STATE =	Calibration;
					break;
				case 4:
					STATE =	Measurement;
					menu_medidas=0;
					MPU6050_Config_ContinuousMeasurement(1);
					break;
				case 5:
					STATE =	ShutDown;
					break;
				default:
					break;
			}
			enviar_a_opcion=0;
			break;

		case Measurement:

			if(menu_medidas==0) menu_opciones();
			parar_medidas=0;
			entrar_user_menu();

			if(parar_medidas==1)
			{
				STATE = Wait;
				break;
			}


			if(data_available)
			{
				uint8_t m = data_available;
				int i;

				for(i=0;i<m; i++)
				{

					MPU6050_Get_RPY_Data(&rpy, &Buffer_Data[i]);
					data = MPU6050_GetData(Buffer_Data[i]);

					USART_Send(USART2, "Roll: ");
					USART_SendFloat(USART2, rpy.roll,2);
					USART_Send(USART2, "\tPitch: ");
					USART_SendFloat(USART2, rpy.pitch,2);
					USART_Send(USART2, "\n");

					/*** LIBERTO TIENES QUE GUARDAR LO SIGUIENTE :
					 *
					 * 	 rpy.roll / rpy.pitch / data.accel_x / data.accel_y / data.accel_z
					 *
					 * 	 tambien molaria poner un cabezera del archivo txt en la que apareciese entre otras cosas
					 * 	 frecuencia de muestreo de la imu ( 1/SAMPLE_FREQ(variable global mia) o simplemente la frec SAMPLE_FREQ)
					 *
					 */

					data_available--;
				}
				pos_buffer = 0;
			}


			/* AQU� DANI TIENES QUE, EN CASO DE QUE SE HAYA PULSADO LA OPCION DE PARAR O STOP, REDIRIGIR AL ESTADO WAIT(ESPERA)
			 * ES DECIR IF(LO QUE SEA) -> STATE = Wait; SI NO SE PULSA NADA NO PONGAS NADA YA QUE VOLVER� A ENTRAR EN EL MISMO ESTADO
			 * EN LA SIGUIENTE ITERACI�N
			 */

			break;

		case Calibration:
			menu_opciones();

			USART_Send(USART2, "Realizando Calibraci�n... \n\n");
			status = MPU6050_Calibration();
			if(status)
			{
				STATE = Error;
				break;
			}

			STATE = Wait;
			break;

		case ExportData:
			menu_opciones();

			/*
			 *  AQUI LIBERTO TIENES QUE PONER TODO EL PROCESO PARA QUE MANDE POR USART TODO LO QUE HAYA EN LA SD
			 */
			STATE = Wait;
			break;

		case Error:

			DisplayErrorCode(status);
			STATE=ShutDown;
			break;

		case ShutDown:
			menu_opciones();
			/*
			 *  AQUI DANI TIENES QUE PONER UN MENSAJE QUE PONGA APAGADO EL SISTEMA Y METER EN EL ENUM QUE TE HE DICHO UN
			 *  UN NUMERO MAS QUE SEA SI HEMOS LLEGAD AL ESTADO ShutDown PARA QUE DESDE EL MAIN LO PREGUNTEMOS Y SALGAMOS DEL
			 *  PRGRAMA
			 */
			return;
			break;

		default:

			break;

	}
}



