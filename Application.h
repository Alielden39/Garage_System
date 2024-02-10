/* 
 * File:   Application.h
 * Author: Alielden
 *
 * Created on September 9, 2023, 7:04 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/* Section :  Includes */
#include "Mca_Layer/GPIO/hal_gpio.h"
#include "Ecua_Layer/LED/ecu_led.h"
#include "Ecua_Layer/BUTTON/ecu_button.h"
#include "Ecua_Layer/RELAY/ecu_relay.h"
#include "Ecua_Layer/DC_MOTOR/ecu_dc_motor.h"
#include "Ecua_Layer/7_SEGMENT/ecu_seven_seg.h"
#include "Ecua_Layer/KEYPAD/ecu_keypad.h"
#include "Ecua_Layer/LCD/ecu_lcd.h"
#include "Mca_Layer/INTERRUPT/hal_external_interrupt.h"
#include "Mca_Layer/INTERRUPT/hal_internal_interrupt.h"
#include "Mca_Layer/INTERRUPT/hal_interrupt_manager.h"
#include "Mca_Layer/INTERRUPT/hal_interrupt_config.h"
#include "Mca_Layer/EEPROM/hal_eeprom.h"
#include "Mca_Layer/ADC/hal_adc.h"
#include "Mca_Layer/TIMER0/hal_timer0.h"
#include "Mca_Layer/TIMER1/hal_timer_1.h"
#include "Mca_Layer/TIMER2/hal_timer2.h"
#include "Mca_Layer/TIMER3/hal_timer3.h"
#include "Mca_Layer/CCP1/hal_ccp.h"
#include "Mca_Layer/usart/hal_usart.h"
#include <xc.h>


 /* Section : Macros */

#define EMPTY    0
#define ONE_CAR  1
#define TWO_CARS 2
#define FULL     3

#define CAR  1
#define NONE 0

#define IR1_IN_IR2_OUT 0
#define IR1_OUT_IR2_IN 1

/* Section : Macros Functions */


/* Section : Data type Declaration */


/* Section : Functions Declaration */

void APPLICATION_INTIALIZE(void);
void lcd_welcome(void);
void green_led_turn_on(void);
void green_led_turn_off(void);
void red_led_turn_on(void);
void red_led_turn_off(void);
void lcd_main(uint8 n_cars);




#endif	/* APPLICATION_H */

