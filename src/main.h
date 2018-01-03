#ifndef MAIN_H_
#define MAIN_H_

#include "MPU6050.h"
#include "USART_Lib.h"
#include "Timer.h"
#include <math.h>
#include <stdio.h>
#include <Timer.h>
#include "stm32l1xx_nucleo.h"
#include "LCD.h"
#include "strings.h"

typedef enum {
	Initialization=0,
	Wait=1,
	Measurement=2,
	Calibration=3,
	ExportData=4,
	Error=5,
	ShutDown=6,
} StateMachine;

void StateMachineSystem(void);

#endif /* MAIN_H_ */
