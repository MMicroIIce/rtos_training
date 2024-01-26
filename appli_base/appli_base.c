#include "tp.h"
#include "tpl_os.h"

int test = 0;

FUNC(int, OS_APPL_CODE) main(void)
{
  initBoard();
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  StartOS(OSDEFAULTAPPMODE);
  return 0;
}



// --------------- Etape 1 a 2 ---------------------

/*
TASK(a_task)
{
  //ActivateTask(task_1);
  //ActivateTask(task_2);
  ActivateTask(task_blink);   // Activation 1: Active la tâche task_blink
  //delay(1000);
  //ledToggle(LED3);
  ChainTask(task_blink);      // Activation 2: Active la tâche task_blink après avoir chaîné la tâche actuelle (a_task)
  ActivateTask(task_blink);   // Activation 3: Active à nouveau la tâche task_blink
  TerminateTask();
}


TASK(task_1)
{
  delay(1000);
  ledToggle(LED4);
  TerminateTask();
}

TASK(task_2)
{
  delay(1000);
  ledToggle(LED5);
  TerminateTask();
}

TASK(task_blink) {
  ledToggle(LED3);
  delay(500);
  ledToggle(LED3);
  delay(500);
  TerminateTask();  
} */

// ----------------------- ETAPE 3 ------------------------------

//  (T_1, T_2, T_3) exécuté dans leur ordre de priorité

/*
TASK(T_master) {
  EventMaskType event_got;

  ActivateTask(T_1);
  ActivateTask(T_2);
  ActivateTask(T_3);

  while(1) {
    WaitEvent(ev_1 | ev_2 | ev_3);  // Attend l'un des événements : ev_1, ev_2 ou ev_3

    // Enregistre l'événement reçu :
    GetEvent(T_master, &event_got);

    if (event_got & ev_1) {
      ClearEvent(ev_1);
      ActivateTask(T_1);  // Active T_1 si l'événement ev_1 est reçu
    }
    if (event_got & ev_2) {
      ClearEvent(ev_2);
      ActivateTask(T_2);  // Active T_2 si l'événement ev_2 est reçu
    }
    if (event_got & ev_3) {
      ClearEvent(ev_3);
      ActivateTask(T_3);  // Active T_3 si l'événement ev_3 est reçu
    }
  }

  TerminateTask();
}

TASK(T_1) {
  ledToggle(LED3);
  delay(500);
  ledToggle(LED3);
  delay(500);
  SetEvent (T_master,ev_1);
  TerminateTask(); 
}

TASK(T_2) {
  ledToggle(LED4);
  delay(500);
  ledToggle(LED4);
  delay(500);
  SetEvent (T_master,ev_2);
  TerminateTask(); 
}

TASK(T_3) {
  ledToggle(LED5);
  delay(500);
  ledToggle(LED5);
  delay(500);
  SetEvent (T_master,ev_3);
  TerminateTask(); 
} */

// -------------------------- LES ISR -----------------------

//int led_status;

/*
ISR(BoutLed) {
  //ActivateTask(T_BLINK_BOUT); //APP1
  //TerminateTask(); //APP1
  
  if(led_status == 3) {
    led_status = 0;
    SetEvent(T_LED_GLOB,ev_bout); //VERSION 2 regarde la V2
  } else {
    led_status ++;
    SetEvent(T_LED_GLOB,ev_bout); //VERSION 2
  }
  
}*/

/* //APP 1
TASK(T_BLINK_BOUT){
  ledToggle(LED4);
  delay(500);
  ledToggle(LED4);
  delay(500);
  TerminateTask();

} */

//APP 2 VERSION 1 :
/*
TASK(T_LED_GLOB){
  while(1) {
    switch(led_status) {
      case 0 :
        ledToggle(LED3);
        delay(500);
        ledToggle(LED3);
        delay(500);
        break;
      case 1 :
        ledToggle(LED4);
        delay(500);
        ledToggle(LED4);
        delay(500);
        break;
      case 2 :
        ledToggle(LED5);
        delay(500);
        ledToggle(LED5);
        delay(500);
        break;
      case 3 :
        ledToggle(LED6);
        delay(500);
        ledToggle(LED6);
        delay(500);
        break;
      default :
        //should not be here.
        break;
  }
  }
}
*/

/*
void activateLed(void) { //APP 2 VERSION 2
  switch(led_status) {
    case 0 :
      ledToggle(LED3);
      delay(500);
      ledToggle(LED3);
      delay(500);
      break;
    case 1 :
      ledToggle(LED5);
      delay(500);
      ledToggle(LED5);
      delay(500);
      break;
    case 2 :
      ledToggle(LED6);
      delay(500);
      ledToggle(LED6);
      delay(500);
      break;
    case 3 :
      ledToggle(LED4);
      delay(500);
      ledToggle(LED4);
      delay(500);
      break;
    default :
      //should not be here.
      break;
  }
}

TASK(T_LED_GLOB) {
  while(1) {
    activateLed();
    WaitEvent(ev_bout);
    ClearEvent(ev_bout);
  }
}
*/

// -------------------------- LES ALARMES ----------------------
/*
int alarm_status;

ISR(BoutLed) {
  if(alarm_status) {
    alarm_status = 0;
  } else {
    alarm_status = 1;
  }
}

TASK(T_BLINK) {
  while(1) {
    ledToggle(LED4);
    WaitEvent(ev_alarm);
    ClearEvent(ev_alarm);
  }
  
  TerminateTask();
} 

TASK(T_EVENT) {
  if(alarm_status) {
    SetEvent(T_BLINK,ev_alarm);
  }
  //on coupe l'appel à l'evenement.
  TerminateTask();
} */


//-------------------------------- CHENILLARD ------------------
/*
int chenillard_index;
int chenillard_way;

ISR(BoutLed) {
  if(chenillard_way) {
    chenillard_way = 0;
  } else {
    chenillard_way = 1;
  }
}

TASK(T_CHASE) {
	EventMaskType event_got;
	int entrance = 0;
	while(1){
		WaitEvent(ev_chenillard_1 | ev_chenillard_2 | ev_chenillard_3 | ev_chenillard_4);

		//We record wich event we got:
		GetEvent (T_CHASE , &event_got );

		if(event_got & ev_chenillard_1) {
			if(entrance == 1 && !chenillard_way) {
				ledToggle(LED5);
			} else if(chenillard_way){
				ledToggle(LED4);
			}
			ClearEvent(ev_chenillard_1);
			ledToggle(LED3);
			entrance = 1;
		}
		if(event_got & ev_chenillard_2) {
			ClearEvent(ev_chenillard_2);
			if(!chenillard_way) {
				ledToggle(LED3);
				ledToggle(LED4);
			} else {
				ledToggle(LED3);
				ledToggle(LED5);
			}
		}
		if(event_got & ev_chenillard_3) {
			ClearEvent(ev_chenillard_3);
			if(!chenillard_way) {
				ledToggle(LED4);
	      ledToggle(LED6);
			} else {
				ledToggle(LED5);
				ledToggle(LED6);
			}

		}
		if(event_got & ev_chenillard_4) {
			ClearEvent(ev_chenillard_4);
			if(!chenillard_way) {
				ledToggle(LED6);
      	ledToggle(LED5);
			} else {
				ledToggle(LED6);
				ledToggle(LED4);
			}
		}
	}

}*/

// --------------------- TD Analyse appel systeme --------------------------

TASK(T_MASTER) {
  TIM2->CNT = 0;
  ActivateTask(T_1);
  TerminateTask();
}

TASK(T_1) {
  test = TIM2->CNT;
  ledToggle(LED6);
  TerminateTask();
}

/*
 * This is necessary for ST libraries
 */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}
