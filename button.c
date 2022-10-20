#include "button.h"

uint8_t prev_button1_state = BUTTON_RELEASE;
uint8_t prev_button2_state = BUTTON_RELEASE;
uint8_t prev_button3_state = BUTTON_RELEASE;
uint8_t prev_button4_state = BUTTON_RELEASE;

// 1. GPIO  2. GPIO PIN  3. 이전의 버튼 상태
int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t *prev_button_state)
{
   unsigned char current_state;

   current_state = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

   if(current_state == BUTTON_PRESS && *prev_button_state == BUTTON_RELEASE) //처음 누른 상태
   {
      *prev_button_state = current_state;
      HAL_Delay(30); // noise가 지나가길 기다림.
      return BUTTON_PRESS;  // 아직 버튼이 눌러지지 않는 것으로 처리. 0을 리턴.
   }
   else if (current_state == BUTTON_RELEASE && *prev_button_state == BUTTON_PRESS)
   {
      *prev_button_state = current_state;  // 릴리즈 상태
      return BUTTON_RELEASE; // 버튼이 완전히 눌렀다가 떼어진 상태로 판단하고 1을 리턴.
   }

   return BUTTON_RELEASE; // 버튼이 눌렀다가 떼어진 상태가 아니다.
}
