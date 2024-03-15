/*
 * button.h
 *
 *  Created on: Nov 28, 2023
 *      Author: macie
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

//states for machine
 typedef enum
 {
	 IDLE =  0,
	 DEBOUNCE,
	 PRESSED,
	 REPEAT,
	 RELEASED

 }BUTTON_STATE;

//struct for button
 typedef struct{

	 BUTTON_STATE state;

	 GPIO_TypeDef * GpioPort;
	 uint16_t GpioPin;

	 uint32_t lastTick;  // Fixed
	 uint32_t TimerDebounce;
	 uint32_t TimerLongPress;
	 uint32_t TimerRepeat;

	 void (*ButtonPressed)(void);
	 void (*ButtonLongPress) (void);
	 void (*ButtonRepeat) (void);
	 void (*ButtonReleased) (void);

 }Tbutton;

//public function
 void ButtonInitKey(Tbutton * Key, GPIO_TypeDef * GpioPort, uint16_t GpioPin,
 		uint32_t TimerDebounce, uint32_t TimerLognPress, uint32_t TimerRepeat);

 void ButtonSetDebounceTime(Tbutton * Key, uint32_t TimerDebounce);

 void ButtonSetLongPressTime(Tbutton * Key, uint32_t TimerLongPress);

 void ButtonSetRepeatTime(Tbutton * Key, uint32_t TimerRepeat);

 void ButtonRegisterPressCallback(Tbutton *key, void * callback);

 void ButtonRegisterLongPressCallback(Tbutton *key, void * callback);

 void ButtonRegisterRepeatCallback(Tbutton *key, void * callback);

 void ButtonRegisterReleaseCallback(Tbutton *key, void * callback);

 void ButtonTask(Tbutton * Key);

#endif /* INC_BUTTON_H_ */
