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
		if(flags==salir_programa) return 0;
	}

	return 0;
}


void StateMachineSystem(void)
{
	int hello;//nombre de identificacion del fichero
	uint8_t error=0;

	switch (STATE) {

	case Initialization:

		SystemInit();
		Systick_Configuration();
		USART2_Init(57600);

		/****************************INICIALIZACIONES DE DANI **********************/
		USART_Send(USART2, "\n\nInicializando LCD... \n\n");
		//Inicializamos la pantalla LCD
		inicilizar_variables();
		init_lcd();
		mensaje_inicial();
		flags=menu_inicial;
		/***************************************************************************/
		rpy.pitch = 0;
		rpy.roll = 0;


		USART_Send(USART2, "Inicializando MPU6050... \n\n");
		status = MPU6050_InitConfig(ACCEL_FS, GYRO_FS, SAMPLE_FREQ);
		if(status)
		{
			STATE = Error;
			break;
		}

		USART_Send(USART2, "Inicializando la SD...\n\n");
		error=FAT_Init(SD_Init, SD_ReadSectors, SD_WriteSectors);//inicializacion SD
		if (error==1){
			STATE=Error;
			break;
		}
		//ponemos por defecto al principio del fichero, aunque tenga datos dentro los sobreescribirá
		FAT_MoveRdPtr(hello,0);
		FAT_MoveWrPtr(hello,0);
		lenRead=0;
		USART_Send(USART2, "Inicializaciones acabadas\n\n");


		STATE = Wait;

		break;

	case Wait:

		hello=FAT_CloseFile("HELLO   TXT");//se cierra el fichero por defecto
		MPU6050_Config_ContinuousMeasurement(0);

		if(flags == Paso_a_menu_principal){
			menuPrincipal(posicion_cursor);
			Delay_lcd(10000);
		}

		movimientoCursor();
		entrar_user_menu();

		switch (enviar_a_opcion)
		{
		case 2:

			STATE =ExportData;
			transferir_fin=0;
			hello = FAT_OpenFile("HELLO   TXT");
			break;

		case 3:

			STATE =	Calibration;
			hello=FAT_CloseFile("HELLO   TXT");
			break;

		case 4:

			STATE =	Measurement;
			hello = FAT_OpenFile("HELLO   TXT");
			MPU6050_Config_ContinuousMeasurement(1);
			break;

		case 5:

			STATE =	ShutDown;
			break;

		default:
			break;

		}
		break;

		case Measurement:

			entrar_user_menu();

			if(flags==parar_de_tomar_datos)
			{
				flags=Paso_a_menu_principal;
				cursor=reposo;
				entrar_opcion=stop;
				STATE=Wait;
				break;
			}
			if(flags==tomando_datos)
			{
				menu_opciones();
				flags=menu_secundario;
			}

			if(read_flag == 0 && data_available_0 > 0)
			{
				read_flag = 1;
				uint8_t m = data_available_0;
				int i;
				for(i=0;i<m; i++)
				{

					MPU6050_Get_RPY_Data(&rpy, &Buffer_Data_0[i]);
					data = MPU6050_GetData(Buffer_Data_0[i]);

					FAT_SDWriteFloatFile(hello,rpy.roll);
					FAT_WriteFile(hello,(uint8_t*)" ",1);
					FAT_SDWriteFloatFile(hello,rpy.pitch);
					FAT_WriteFile(hello,(uint8_t*)";",1);
					USART_Send(USART2, "Roll: ");
					USART_SendFloat(USART2, rpy.roll,2);
					USART_Send(USART2, "\tPitch: ");
					USART_SendFloat(USART2, rpy.pitch,2);
					USART_Send(USART2, "\n");
				}
				pos_buffer_0 = 0;
				data_available_0 = 0;
			}

			if(read_flag == 1 && data_available_1 > 0)
			{
				read_flag = 0;
				int i;
				uint8_t m = data_available_1;

				for(i=0;i<m; i++)
				{
					MPU6050_Get_RPY_Data(&rpy, &Buffer_Data_1[i]);
					data = MPU6050_GetData(Buffer_Data_1[i]);

					FAT_SDWriteFloatFile(hello,rpy.roll);
					FAT_WriteFile(hello,(uint8_t*)" ",1);
					FAT_SDWriteFloatFile(hello,rpy.pitch);
					FAT_WriteFile(hello,(uint8_t*)";",1);
					USART_Send(USART2, "Roll: ");
					USART_SendFloat(USART2, rpy.roll,2);
					USART_Send(USART2, "\tPitch: ");
					USART_SendFloat(USART2, rpy.pitch,2);
					USART_Send(USART2, "\n");
				}
				pos_buffer_1 = 0;
				data_available_1 = 0;
			}

			break;

		case Calibration:


			menu_opciones();
			movercursor(2,1);
			write_char("Calibrando...");
			USART_Send(USART2, "Realizando Calibración... \n\n");

			status = MPU6050_Calibration();
			if(status)
			{
				STATE = Error;
				break;
			}
			movercursor(3,1);
			write_char("Proceso acabado");


			STATE = Wait;
			break;

		case ExportData:

			menu_opciones();
			int RdPtr=0,tipo=0;
			uint8_t recibido[20];
			int i;
			for(i=0;i<20;i++)recibido[i]=0;

			for(tipo=0;tipo<2;tipo++)
			{

				switch (tipo)
				{
				case 0:
					USART_Send(USART2,"\n");
					USART_Send(USART2, "roll:\t");
					break;
				case 1:
					USART_Send(USART2,"\n");
					USART_Send(USART2, "pitch:\t");
					break;
				default:
					break;
				}

				lenRead=FAT_ReadFile(hello,recibido,6);
				RdPtr=FAT_RdPtr(hello);
				FAT_MoveRdPtr(hello, RdPtr+1);//ya que son 4 espacios(tab)+1 | + 4 espacios(tab)
				USART_Send(USART2, recibido);
				USART_Send(USART2,"\n");
				for(i=0;i<20;i++)recibido[i]=0;//limpiamos el buff de envio de datos
			}

			if (transferir_fin==1)STATE=Wait;
			break;

		case Error:

			DisplayErrorCode(status);
			if (error==1){
				clear();
				movercursor(1,1);
				write_char("*La SD no funciona*");
			}
			STATE=ShutDown;
			break;

		case ShutDown:

			hello=FAT_CloseFile("HELLO   TXT");
			Delay(2000);
			menu_opciones();
			Delay(2000);

			return;
			break;

		default:

			break;

	}
}



