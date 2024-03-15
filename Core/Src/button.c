/*
 * button.c
 *
 *  Created on: Nov 28, 2023
 *      Author: macie
 */

#include "main.h"

#include "button.h"

//button init

void ButtonInitKey(Tbutton * Key, GPIO_TypeDef * GpioPort, uint16_t GpioPin,
		uint32_t TimerDebounce, uint32_t TimerLognPress, uint32_t TimerRepeat)
{
	Key->state = IDLE;

	Key-> GpioPort = GpioPort;
	Key->GpioPin = GpioPin;

	Key->TimerDebounce = TimerDebounce;
	Key->TimerLongPress = TimerLognPress;
	Key->TimerRepeat = TimerRepeat;
}


//time setting function
void ButtonSetDebounceTime(Tbutton * Key, uint32_t TimerDebounce)
{
	Key->TimerDebounce = TimerDebounce;
}

void ButtonSetLongPressTime(Tbutton * Key, uint32_t TimerLongPress)
{
	Key->TimerLongPress = TimerLongPress;
}

void ButtonSetRepeatTime(Tbutton * Key, uint32_t TimerRepeat)
{
	Key->TimerRepeat = TimerRepeat;
}

//register callback
void ButtonRegisterPressCallback(Tbutton *key, void * callback)
{
	key->ButtonPressed = callback;
}

void ButtonRegisterLongPressCallback(Tbutton *key, void * callback)
{
	key->ButtonLongPress = callback;
}

void ButtonRegisterRepeatCallback(Tbutton *key, void * callback)
{
	key->ButtonRepeat = callback;
}

void ButtonRegisterReleaseCallback(Tbutton *key, void * callback)
{
	key->ButtonReleased = callback;
}

//state routine
void ButtonIdleRoutine(Tbutton * Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
	{
		Key->state = DEBOUNCE;
		Key->lastTick = HAL_GetTick();
	}
}

void ButtonDebounceRoutine(Tbutton * Key)
{
	if((HAL_GetTick()- Key->lastTick )> Key->TimerDebounce)
	{
		if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_SET)
		{
			Key->state = PRESSED;
			Key->lastTick = HAL_GetTick();

			if(!(Key->ButtonPressed == NULL))
			{
				Key->ButtonPressed();
			}
		}
		else
		{
			Key->state = IDLE;
		}
	}
}

void ButtonPressedRoutine(Tbutton * Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_RESET)
	{
		Key->state = RELEASED;
	}
	else
	{
		if((HAL_GetTick()- Key->lastTick )> Key->TimerLongPress)
		{
			Key->state = REPEAT;
			Key->lastTick = HAL_GetTick();

			if(!(Key->ButtonLongPress == NULL))
			{
				Key->ButtonLongPress();
			}
		}
	}
}

void ButtonRepeatRoutine(Tbutton * Key)
{
	if(HAL_GPIO_ReadPin(Key->GpioPort, Key->GpioPin) == GPIO_PIN_RESET)
		{
			Key->state = RELEASED;
		}
		else
		{
			if((HAL_GetTick()- Key->lastTick )> Key->TimerRepeat)
			{
				Key->lastTick = HAL_GetTick();
				if(!(Key->ButtonRepeat == NULL))
				{
					Key->ButtonRepeat();
				}
			}
		}
}

void ButtonReleasedRoutine(Tbutton * Key)
{
	Key->state = IDLE;
	if(!(Key->ButtonReleased == NULL))
	{
		Key->ButtonReleased();
	}
}

// state machine
void ButtonTask(Tbutton * Key)
{
	switch(Key->state)
	{
	case IDLE:
		ButtonIdleRoutine(Key);
		break;

	case DEBOUNCE:
		ButtonDebounceRoutine(Key);
		break;

	case PRESSED:
		ButtonPressedRoutine(Key);
		break;

	case REPEAT:
		ButtonRepeatRoutine(Key);
		break;

	case RELEASED:
		ButtonReleasedRoutine(Key);
		break;

	}
}

