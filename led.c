#include "main.h"
#include "button.h"

extern uint8_t prev_button1_state;


void button1_ledall_on_off()
{
   static int button1_count = 0; // 로컬 변수이나, static을 붙이면 전역 변수처럼 동작

   if(get_button(BUTTON_1_GPIO_Port, BUTTON_1_Pin, &prev_button1_state) == BUTTON_PRESS)
   {
      button1_count++;
      button1_count %= 2;
      if(button1_count)
      {
         led_all_on();
      }
      else
      {
         led_all_off();
      }
   }
}
void led2_toggle()
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	HAL_Delay(500);
}

void flower_on()
{
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i | 0x80 >> i, GPIO_PIN_SET);
		HAL_Delay(200);
	}
}

void flower_off()
{
	for (int i = 0; i < 4; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x08 << i | 0x10 >> i, GPIO_PIN_SET);
		HAL_Delay(200);
	}
}
void on_1_up()  // 해당되는 led만 on 0 1 2 3 4
{
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOB, 0xff << i, GPIO_PIN_RESET);
	}
}

void on_1_down()  // 해당되는 led만 on 7 6 5 4
{
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x80 >> i, GPIO_PIN_SET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOB, 0xff >> i, GPIO_PIN_RESET);
	}
}

void led_on_up()   // 0 -> 01 -> 012 -> 0123 -> 01234567
{
	for (int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, 0x01 << i, GPIO_PIN_SET);
		HAL_Delay(200);
	}
}


void led_on_down()   // 7 -> 76  -> 765 -> 76543210
{
	for(int i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(GPIOB,0b10000000 >> i, GPIO_PIN_SET);
		HAL_Delay(200);
	}
}

void led_all_on()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);
}

void led_all_off()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);
}

void led0on()
{
	HAL_GPIO_WritePin(GPIOB, 0x01 , GPIO_PIN_SET);
}

void led1on()
{
	HAL_GPIO_WritePin(GPIOB, 0b00000010 , GPIO_PIN_SET);
}
