#include "main.h"   //for HAL GPIO
#include "button.h"

extern int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state);
extern volatile int TIM11_150ms_counter;
extern volatile int t2ms_counter;

extern uint8_t prev_button1_state;
extern uint8_t prev_button2_state;
extern uint8_t prev_button3_state;
extern uint8_t prev_button4_state;
/*
   1분 : 60sec : 1,000,000us x 60 = 60,000,000us
   1초 : 1000ms ==> 1,000,000us
   rpm : revolutions per minutes
   60,000,000/ 4096 / speed(1~13)
 */
#define stop 0
#define one_floor 1
#define two_floor 3
#define third_floor 5
#define fourth_floor 7
#define FORWARD 5
#define BACKWARD 6

int motor_state = 0;
int flow = 0;
int flow_state = 0;
int ext_state = 0;
int current ;
int previous = 1;
int btn = 0;
int step = 0; //전역변수 처럼 동작
int cnt=0;
int i;

#define STEPS_PER_REV 4096
//#define IDLE     0
//#define FORWARD  1
//#define BACKWARD 2
//#define stop	0
//#define	one_floor	1
//#define	two_floor	2
//#define	third_floor	3
//#define fourth_floor	4
//
//int motor_state = 0;


void set_rpm(int rpm)
{
   delay_us(60000000 /STEPS_PER_REV /rpm);
   //최대스피드 기준 (13) : delay_us(1126)
}

#if 1
void stepmotor_drive(int direction)  // drirection: forward or backward
{
	static int step = 0;  // 전역변수처럼 동작

   switch(step)
   {
      case 0:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 1:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 2:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 3:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 4:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 5:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
         break;
      case 6:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
         break;
      case 7:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
         break;
   }
   if(direction == FORWARD)  // for(int i = 0; i < 8; i++)문 분리
   {
	   step++;
	   // if(step >= 8)   cnt++;

	   step %= 8;


               // 다음 실행 할 step 번호 지정

               // step = 0;
   }
   else if(direction == BACKWARD)  // for(int i = 7; i >= 0; i--)
   {
	   step--;
	   if (step < 0)
	   step = 7;


   }
}
#else  // original
void stepmotor_drive(int step)
{
   switch(step)
   {
      case 0:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 1:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 2:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 3:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 4:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
         break;
      case 5:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
         break;
      case 6:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
         break;
      case 7:
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
         break;
   }
}
#endif
// btn1: forward / backward
// btn2 or btn3: stop <-> forward
//             stop <-> backward

//void stepmotor_forward_backward_stop()
//{
//   switch(motor_state)
//   {
//      case IDLE:
//        if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
//         {
//            motor_state = FORWARD;
//         }
//        if(get_button(BUTTON_2_GPIO_Port, BUTTON_2_Pin, &prev_button2_state) == BUTTON_PRESS)
//       {
//            motor_state = FORWARD;
//         }
//         if(get_button(BUTTON_3_GPIO_Port, BUTTON_3_Pin, &prev_button3_state) == BUTTON_PRESS)
//         {
//            motor_state = BACKWARD;
//         }
//         break;
//      case FORWARD:
//         stepmotor_drive(FORWARD);
//         set_rpm(13); //rpm : 13 (최대 speed)
//         if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
//         {
//            motor_state = BACKWARD;
//         }
//         if(get_button(BUTTON_2_GPIO_Port, BUTTON_2_Pin, &prev_button2_state) == BUTTON_PRESS)
//         {
//            motor_state = IDLE;
//         }
//         break;
//      case BACKWARD:
//         stepmotor_drive(BACKWARD);
//         set_rpm(13); //rpm : 13 (최대 speed)
//         if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
//         {
//            motor_state = FORWARD;
//         }
//         if(get_button(BUTTON_3_GPIO_Port, BUTTON_3_Pin, &prev_button3_state) == BUTTON_PRESS)
//         {
//            motor_state = IDLE;
//         }
//         break;
//
//   }
//}

/*
   시계방향  <---> 반시계 방향
   1바퀴 회전하는데 필요한 총 step : 4096
   4096 / 8(0.7도) == 512 sequence : 360도
   모터가 360도 회전하는데 512개의 sequence 를 진행해야한다.
   1 sequence 당 : 0.70312도
   0.70312 x 512 = 360도
   이걸 이용해서 동작
*/

//void stepmotor_forward_backward_stop()
//{
//   switch(motor_state)
//   {
//      case IDLE:
//    	 if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
//         {
//            motor_state = FORWARD;
//         }
//    	 if(get_button(BUTTON_2_GPIO_Port, BUTTON_2_Pin, &prev_button2_state) == BUTTON_PRESS)
//		 {
//            motor_state = FORWARD;
//         }
//         if(get_button(BUTTON_3_GPIO_Port, BUTTON_3_Pin, &prev_button3_state) == BUTTON_PRESS)
//         {
//            motor_state = BACKWARD;
//         }
//         break;
//      case FORWARD:
//         stepmotor_drive(FORWARD);
//         set_rpm(13); //rpm : 13 (최대 speed)
//         if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
//         {
//            motor_state = BACKWARD;
//         }
//         if(get_button(BUTTON_2_GPIO_Port, BUTTON_2_Pin, &prev_button2_state) == BUTTON_PRESS)
//         {
//            motor_state = IDLE;
//         }
//         break;
//      case BACKWARD:
//         stepmotor_drive(BACKWARD);
//         set_rpm(13); //rpm : 13 (최대 speed)
//         if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
//         {
//            motor_state = FORWARD;
//         }
//         if(get_button(BUTTON_3_GPIO_Port, BUTTON_3_Pin, &prev_button3_state) == BUTTON_PRESS)
//         {
//            motor_state = IDLE;
//         }
//         break;
//
//   }
//}

//void stepmotor_forward_backward()
//{
//   switch(motor_state)
//   {
//      case IDLE:
//         if()
//         {
//            motor_state = FORWARD;
//         }
//         break;
//      case FORWARD:
//         stepmotor_drive(FORWARD);
//         set_rpm(13);
//         if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
//         {
//            motor_state = BACKWARD;
//         }get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS
//
//         break;
//      case BACKWARD:
//         stepmotor_drive(BACKWARD);
//         set_rpm(13);
//         if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
//         {
//            motor_state = FORWARD;
//         }
//         break;
//   }
//}


void stepmotor_main_test()
{
   for(int i= 0; i < 512; i++)   //시계 방향 회전
   {
      for(int j = 0; j < 8; j++)   // 1 sequence : 0.7도
      {
         stepmotor_drive(j);
         set_rpm(13);  // rpm: 13 (최대 speed)
      }
   }

   for(int i= 0; i < 512; i++)   //반시계 방향 회전
   {
      for(int j = 7; j >= 0; j--)
      {
         stepmotor_drive(j);
         // rpm 만큼 wait
         set_rpm(13);
      }
   }
}

int nowfloor=0;
int hopefloor=0;
char buff[40];
void button_move()
{
	if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
   {
       hopefloor = 1;
       flow_state = 1;

//       HAL_GPIO_WritePin(GPIOB, 0xff, GPIO_PIN_RESET);
//       HAL_GPIO_WritePin(GPIOB, 0x01, GPIO_PIN_SET);
   }
   else if(get_button(BUTTON_2_GPIO_Port, BUTTON_2_Pin, &prev_button2_state) == BUTTON_PRESS)
   {
	   hopefloor = 2;
	  flow_state = 1;

//      HAL_GPIO_WritePin(GPIOB, 0xff, GPIO_PIN_RESET);
//      HAL_GPIO_WritePin(GPIOB, 0x03, GPIO_PIN_SET);
   }
   else if(get_button(BUTTON_3_GPIO_Port, BUTTON_3_Pin, &prev_button3_state) == BUTTON_PRESS)
   {
	   hopefloor = 3;
	  flow_state = 1;

//      HAL_GPIO_WritePin(GPIOB, 0xff, GPIO_PIN_RESET);
//      HAL_GPIO_WritePin(GPIOB, 0x07, GPIO_PIN_SET);
   }
   else if(get_button(BUTTON_4_GPIO_Port, BUTTON_4_Pin, &prev_button4_state) == BUTTON_PRESS)
   {

	   hopefloor = 4;
	  flow_state = 1;

//      HAL_GPIO_WritePin(GPIOB, 0xff, GPIO_PIN_RESET);
//      HAL_GPIO_WritePin(GPIOB, 0x0f, GPIO_PIN_SET);
   }


   switch(flow_state)
   {
      case stop:
    	  move_cursor(0,0);
		  lcd_string("=== Elevator ===");
		  move_cursor(1,0);
		  sprintf(buff, "==   %d Floor    ==", nowfloor);

		  HAL_GPIO_WritePin(GPIOB, 1 & 1 << nowfloor, GPIO_PIN_SET);

       	break;
      case 1:
    	if(nowfloor == hopefloor)
		{
			flow_state = 0;
		}
    	if(hopefloor > nowfloor)
    	{

    		stepmotor_drive(FORWARD);
    	  	  set_rpm(13);
    	  	HAL_GPIO_WritePin(GPIOB, 0xff, GPIO_PIN_RESET);
    	  	HAL_GPIO_WritePin(GPIOB, 0x01 <<TIM11_150ms_counter , GPIO_PIN_SET);

    	}
    	if(hopefloor < nowfloor)
    	{

    		stepmotor_drive(BACKWARD);
    		set_rpm(13);

    		HAL_GPIO_WritePin(GPIOB, 0xff, GPIO_PIN_RESET);
		    HAL_GPIO_WritePin(GPIOB, 0x80 >> TIM11_150ms_counter , GPIO_PIN_SET);
    	}
           break;
   }
}








