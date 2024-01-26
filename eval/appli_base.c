#include "tp.h"
#include "tpl_os.h"

int fuelInTank = 100; //taux de remplissage du réservoir
int green_status = 0;
int red_status = 0;
int blue_status = 0;
int alarmMonitoring_status = 1;
int alarmDistance_status = 1;
int distance = 0;

FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}

TASK(taskControl)
{
  EventMaskType event_got;
  while(1)
  {
    WaitEvent(evFuel | evFillTank | evDistance);
    GetEvent(taskControl, &event_got);

    if (event_got & evFuel)
    {
      ClearEvent(evFuel);
      if(fuelInTank > 20)
      {
        if(!green_status){
          ledToggle(GREEN);
          green_status = 1;
        }
        if(red_status){
          ledToggle(RED);
          red_status = 0;
        }
        if(!alarmDistance_status)
        {
          SetRelAlarm(alarmDistance,100,100);
          alarmDistance_status = 1;
        }
      }
      else if(fuelInTank <= 0)
      {
        if (alarmMonitoring_status)
        {
          CancelAlarm(alarmMonitoring);
          alarmMonitoring_status = 0;
        }
        if(alarmDistance_status)
        {
          CancelAlarm(alarmDistance_status);
          alarmDistance_status = 0;
        }
        if(green_status)
        {
          ledToggle(GREEN);
          green_status = 0;
        }
        if(!red_status)
        {
          ledToggle(RED);
          red_status = 1;
        }
      }
      else
      {
        if(green_status)
        {
          ledToggle(GREEN);
          green_status = 0;
        }
        if(!red_status)
        {
          ledToggle(RED);
          red_status = 1;
        }
      }
    }
    else if (event_got & evFillTank)
    {
      ClearEvent(evFillTank);
      fuelInTank = 100;
      if(!alarmMonitoring_status)
      {
        SetRelAlarm(alarmMonitoring,500,500);
      }
    }
    else if(event_got & evDistance)
    {
      if (fuelInTank<=0 && alarmDistance_status)
      {
        CancelAlarm(alarmDistance);
        alarmDistance_status = 0;
      }
      if (distance%2 == 0)
      {
        if(blue_status)
        {
          ledToggle(BLUE);
        }
      }
      else
      {
        if(!blue_status)
        {
          ledToggle(BLUE);
        }
      }
    }
  }
}

TASK(taskMonitoring)
{
  if (fuelInTank >= 5)
  {
    fuelInTank -= 5;
    SetEvent(taskControl, evFuel);
  }
}

ISR(ISRFillTank)
{
  SetEvent(taskControl, evFillTank);
}

TASK(taskDistance)
{
  distance++;
  SetEvent(taskControl, evDistance);
}

/*
 * This is necessary for ST libraries
 */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

 

