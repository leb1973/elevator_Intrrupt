#include "main.h"

extern TIM_HandleTypeDef htim2;
extern volatile int TIM11_10ms_servomotor_counter;

// 84000000Hz / 1680 == 50kHz
// T=1/f 1/ 50000 --> 0.00002sec
// 2ms ==> 0.00002sec *100 --> 0.002sec(2ms) : arm : 180도 회전
// 1ms ==> 0.00002sec * 50 : arm 0도 회전
// 1.5ms ==> 0.00002sec * 75 : arm 90도 회전


int arm_roates_indicator =0;
extern int distance_a;
int open =0; // 0일때 open 1일때 close
void servo_motor_control_main()
{
#if 1

	if(TIM11_10ms_servomotor_counter >= 500)
	{
		TIM11_10ms_servomotor_counter =0;
		if(!arm_roates_indicator)
		{
			// 180도 회전
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 110);	// PORTB. 10
		}
		else
		{
			// 0도
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 10);
		}
		arm_roates_indicator = !arm_roates_indicator;
	}

#else
	while(1)
	{
		// 180도 회전
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 100);	// PORTB. 10
		HAL_Delay(1000);	// 1초 유지
		// 0도

		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 50);
		HAL_Delay(1000);
	}

#endif
}

void open_door_close_door()
{
	if(distance_a <=5)
	{
		TIM11_10ms_servomotor_counter =0;
		open =1;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 110);	// PORTB. 10
	}
	else if(TIM11_10ms_servomotor_counter >=500 && open == 1)
	{
		 open =0;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 10);
	}
}


