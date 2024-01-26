#include "tp.h"
#include "tpl_os.h"
#include "lcd/lcd.h"
#include "sr04/sr04.h"

#define APP_Task_displayTask_START_SEC_CODE
#include "tpl_memmap.h"

unsigned long range;

static int secondes = 50;
static int minutes = 59;


FUNC(int, OS_APPL_CODE) main(void)
{
	initBoard();
	lcdInit();
	USInit();
	StartOS(OSDEFAULTAPPMODE);
	return 0;
}

TASK(displayTask)
{
	ActivateTask(timer);
    	delay(500);
	TerminateTask();
}

TASK(LCD)
{
	EventMaskType event_got ;
	lcdClear();
	    
	    
	while (1) {
	    WaitEvent ( ev_1 | ev_2 | ev_3);
	    
	    GetEvent ( LCD , &event_got );
	    
	    if ( event_got & ev_1 ) {
	      	ClearEvent ( ev_1 );
	      
	      	lcdSetPos(1, 9);
	
		lcdPrintString("sonar ");
		lcdPrintInt(range, 1);
	    }
	    
	    if ( event_got & ev_2 ) {
	        ClearEvent ( ev_2 );
	    	lcdSetPos(1, 9);
		lcdPrintString("fail ");
	    }
	    
	    if ( event_got & ev_3 ) {
	    	ClearEvent ( ev_3 );
	    	
	    	lcdSetPos(0, 9);
		lcdPrintString("timer ");

		if (minutes < 10){
			lcdPrintInt(0, 1);
			lcdPrintInt(minutes, 1);
		}else if (minutes >= 10){
			lcdPrintInt(minutes, 1);
		}

		lcdPrintString(":");

		if (secondes < 10){
			lcdPrintInt(0, 1);
			lcdPrintInt(secondes, 1);		
		}else if (secondes >= 10){
			lcdPrintInt(secondes, 1);
		}
	    }
	}
	
	TerminateTask();
}

TASK (timer) 
{
	secondes += 1;
		
	if (minutes >= 59 && secondes >= 59){
		secondes = 0;
		minutes = 0;
	}

	if (secondes == 59){
		secondes = 0;
		minutes += 1;
	}
	
	SetEvent ( LCD , ev_3 );
	TerminateTask();
}

TASK (measureTask) 
{
	USStartTimer();
	USTrigger();
	CancelAlarm(activateTimeout);
	SetRelAlarm(activateTimeout,35,110);
	SetRelAlarm(activateDisplay,1000,1000);
	
	TerminateTask();
}

TASK (timeoutTask)
{
	CancelAlarm(activateDisplay);
	//lcdClear();
	
	SetEvent ( LCD , ev_2 );

	TerminateTask();
}

ISR(echoISR) 
{
	CancelAlarm(activateTimeout);
	range = 0.034 * USReadTimer();	
	
	SetEvent ( LCD , ev_1 );
	TerminateTask();		
}



#define APP_Task_displayTask_STOP_SEC_CODE
#include "tpl_memmap.h"


#define OS_START_SEC_CODE
#include "tpl_memmap.h"
/*
 *  * This is necessary for ST libraries
 *   */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{
}

FUNC(void, OS_CODE) PreTaskHook()
{
    TaskType task_id = 0;
    GetTaskID(&task_id);
    if (task_id == displayTask) {
        ledOn(ORANGE);
    }
}

FUNC(void, OS_CODE) PostTaskHook()
{
    TaskType task_id = 0;
    GetTaskID(&task_id);
    if (task_id == displayTask) {
        ledOff(ORANGE);
    }
}
#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

