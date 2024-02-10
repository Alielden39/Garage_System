/* 
 * File:   Application.c
 * Author: Alielden
 *
 * Created on September 9, 2023, 6:23 PM
 */

#include "Application.h"
Std_ReturnType ret = STD_OK ;
uint8 n_cars = 0 ;
uint8 IR1_Read = 0  , IR2_Read = 0   ;


led_t green_led =
{
  .port =  GPIO_PORTB ,
  .pin =   GPIO_PIN0 ,
  .led_status = led_OFF
};

led_t red_led =
{
  .port =  GPIO_PORTB ,
  .pin =   GPIO_PIN1 ,
  .led_status = led_OFF
};

pin_config_t IR1 =
{
  .direction = GPIO_INPUT ,
  .port = GPIO_PORTC ,
  .pin = GPIO_PIN0 ,
  .logic = GPIO_LOW  
};

pin_config_t IR2 =
{
  .direction = GPIO_INPUT ,
  .port = GPIO_PORTC ,
  .pin = GPIO_PIN1  ,
  .logic = GPIO_LOW
};


lcd_8bit_t lcd = 
{
  .lcd_rs.port      = GPIO_PORTE ,
  .lcd_rs.pin       = GPIO_PIN1 ,
  .lcd_rs.logic     = GPIO_LOW ,
  .lcd_rs.direction = GPIO_OUTPUT ,
  
  .lcd_en.port      = GPIO_PORTE ,
  .lcd_en.pin       = GPIO_PIN0 ,
  .lcd_en.logic     = GPIO_LOW ,
  .lcd_en.direction = GPIO_OUTPUT ,
  
  .lcd_data[0].port      = GPIO_PORTD ,
  .lcd_data[0].pin       = GPIO_PIN0 ,
  .lcd_data[0].logic     = GPIO_LOW ,
  .lcd_data[0].direction = GPIO_OUTPUT ,
  
  .lcd_data[1].port      = GPIO_PORTD ,
  .lcd_data[1].pin       = GPIO_PIN1 ,
  .lcd_data[1].logic     = GPIO_LOW ,
  .lcd_data[1].direction = GPIO_OUTPUT ,
  
  .lcd_data[2].port      = GPIO_PORTD ,
  .lcd_data[2].pin       = GPIO_PIN2 ,
  .lcd_data[2].logic     = GPIO_LOW ,
  .lcd_data[2].direction = GPIO_OUTPUT ,
  
  .lcd_data[3].port      = GPIO_PORTD ,
  .lcd_data[3].pin       = GPIO_PIN3 ,
  .lcd_data[3].logic     = GPIO_LOW ,
  .lcd_data[3].direction = GPIO_OUTPUT ,
  
  .lcd_data[4].port      = GPIO_PORTD ,
  .lcd_data[4].pin       = GPIO_PIN4 ,
  .lcd_data[4].logic     = GPIO_LOW ,
  .lcd_data[4].direction = GPIO_OUTPUT ,
  
  .lcd_data[5].port      = GPIO_PORTD ,
  .lcd_data[5].pin       = GPIO_PIN5 ,
  .lcd_data[5].logic     = GPIO_LOW ,
  .lcd_data[5].direction = GPIO_OUTPUT ,
  
  .lcd_data[6].port      = GPIO_PORTD ,
  .lcd_data[6].pin       = GPIO_PIN6 ,
  .lcd_data[6].logic     = GPIO_LOW ,
  .lcd_data[6].direction = GPIO_OUTPUT ,
  
  .lcd_data[7].port      = GPIO_PORTD ,
  .lcd_data[7].pin       = GPIO_PIN7 ,
  .lcd_data[7].logic     = GPIO_LOW ,
  .lcd_data[7].direction = GPIO_OUTPUT ,
};



timer2_t  tmr2 = 
{
  .timer2_call_back = NULL ,
  .priority = LOW_PRIORITY ,
  .prescaler_value = TIMER2_PRESCALER_DIV_BY_1 ,
  .postscaler_value = timer2_postscaler_div_1 ,
  .Timer2_Preload_Value = 0
};

 
ccp_t ccp_obj =
{
  .PWM_Freq = 15000 ,
  .Sub_Mode = CCP_PWM_MODE ,
  ._mode = _PWM_MODE ,
  .ccp1_call_back = NULL ,
  .ccp1_priority = LOW_PRIORITY ,
  .Timer2_postscaler_value = CCP_TIMER2_PRESCALER_DIV_BY_1 ,
  .Timer2_prescaler_value = CCP_timer2_postscaler_div_1 ,
  .pin.direction = GPIO_OUTPUT ,
  .pin.logic = GPIO_LOW ,
  .pin.port = GPIO_PORTC ,
  .pin.pin = GPIO_PIN2  
};


int main() {
    
    APPLICATION_INTIALIZE();
    ret = CCP_PWM_Set_Duty(&ccp_obj , 0 );
    ret = CCP_PWM_Start(&ccp_obj) ;
    lcd_welcome();
    lcd_main(EMPTY);
    
    while(1)
    { 
        ret = gpio_pin_read_logic(&IR1 , &IR1_Read );
        ret = gpio_pin_read_logic(&IR2 , &IR2_Read );
        
        if( (IR1_Read == CAR) && ( IR2_Read == NONE ) )
        {
            if( n_cars != FULL )
            {
                green_led_turn_on();
                ret = CCP_PWM_Set_Duty(&ccp_obj , 50 );
                while( (IR1_Read == CAR) || ( IR2_Read == NONE ) )
                {
                    ret = gpio_pin_read_logic(&IR1 , &IR1_Read );
                    ret = gpio_pin_read_logic(&IR2 , &IR2_Read );
                }
                n_cars++ ;
                lcd_main(n_cars);
                ret = CCP_PWM_Set_Duty(&ccp_obj , 0 );
                green_led_turn_off();
            }
            else if( n_cars = FULL )
            {
                red_led_turn_on();
                ret = lcd_8bit_send_command(&lcd , _LCD_CLEAR );
                ret = lcd_8bit_send_string_data(&lcd , " GARAGE IS FULL !! ");
                while( IR1_Read == CAR )
                {
                    ret = gpio_pin_read_logic(&IR1 , &IR1_Read );
                }
                lcd_main(n_cars);
                red_led_turn_off();
            }
        }
        
        else if( (IR2_Read == CAR) && ( IR1_Read == NONE ) )
        {
            ret = CCP_PWM_Set_Duty(&ccp_obj , 50 );
            while( (IR2_Read == CAR) || ( IR1_Read == NONE ) )
            {
               ret = gpio_pin_read_logic(&IR1 , &IR1_Read );
               ret = gpio_pin_read_logic(&IR2 , &IR2_Read );
            }
            n_cars-- ;
            lcd_main(n_cars);
            ret = CCP_PWM_Set_Duty(&ccp_obj , 0 );
        }
        
        while( (IR1_Read == CAR) || ( IR2_Read == CAR ) ) 
        {
            ret = gpio_pin_read_logic(&IR1 , &IR1_Read );
            ret = gpio_pin_read_logic(&IR2 , &IR2_Read );
        }
        
        

        
    }
    return (EXIT_SUCCESS);
}


void APPLICATION_INTIALIZE(void)
{
    ret = CCP_Init(&ccp_obj);
    ret = Timer2_init(&tmr2) ;
    ret = lcd_8bit_intialize(&lcd);
    ret = led_intialize(&green_led);
    ret = led_intialize(&red_led);
    ret = gpio_pin_intialize(&IR1) ;
    ret = gpio_pin_intialize(&IR2) ;
}



void lcd_welcome(void)
{
    ret = lcd_8bit_send_string_data(&lcd , "Welcome");
    ret = lcd_8bit_send_char_data(&lcd , '.' ) ;
    __delay_ms(15) ;
    ret = lcd_8bit_send_char_data(&lcd , '.' ) ;
    __delay_ms(15) ;
    ret = lcd_8bit_send_char_data(&lcd , '.' ) ;
    __delay_ms(15) ;
    ret = lcd_8bit_send_char_data_pos(&lcd , 1 , 8 , ' ' );
    ret = lcd_8bit_send_char_data_pos(&lcd , 1 , 9 , ' ' );
    ret = lcd_8bit_send_char_data_pos(&lcd , 1 , 10 , ' ' );
     __delay_ms(15) ;
     ret = lcd_8bit_send_char_data_pos(&lcd , 1 , 8 , '.' ) ;
    __delay_ms(15) ;
     ret = lcd_8bit_send_char_data_pos(&lcd , 1 ,9 , '.' ) ;
    __delay_ms(15) ;
     ret = lcd_8bit_send_char_data_pos(&lcd , 1 , 10 , '.' ) ;
    __delay_ms(15) ;
    ret = lcd_8bit_send_command(&lcd , _LCD_CLEAR );

}


void lcd_main(uint8 n_cars)
{
     ret = lcd_8bit_send_command(&lcd , _LCD_CLEAR );
     
     switch( n_cars )
     {
         case EMPTY :
             ret = lcd_8bit_send_string_data(&lcd , " THE CARS IN GARAGE ( 0 / 3 ) ") ;
             break;
         
         case ONE_CAR :
             ret = lcd_8bit_send_string_data(&lcd , " THE CARS IN GARAGE ( 1 / 3 ) ") ;
             break;
             
         case TWO_CARS :
             ret = lcd_8bit_send_string_data(&lcd , " THE CARS IN GARAGE ( 2 / 3 ) ") ;
             break;
             
         case FULL :
             ret = lcd_8bit_send_string_data(&lcd , " THE CARS IN GARAGE ( 3  / 3 ) ") ;
             break;
             
             
     }
     
}


void green_led_turn_on(void)
{
    ret = led_turn_on(&green_led);
}

void green_led_turn_off(void)
{
    ret = led_turn_off(&green_led);
}

void red_led_turn_on(void)
{
    ret = led_turn_on(&red_led);
}

void red_led_turn_off(void)
{
    ret = led_turn_off(&red_led);
}

