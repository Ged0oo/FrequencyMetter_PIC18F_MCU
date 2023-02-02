#include "application.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/ADC/hal_adc.h"

Std_ReturnType ret = E_NOT_OK;
void application_intialize(void);
void tmr0_handeler();
void tmr1_handeler();
volatile uint16 Freq = 0;

uint8 freqArray[5];



chr_lcd_4bit_t lcd_1 ={
	.lcd_rs.port=PORTC_INDEX,
	.lcd_rs.pin=GPIO_PIN2,
	.lcd_rs.direction=GPIO_DIRECTION_OUTPUT,
	.lcd_rs.logic=GPIO_LOW,
	
    .lcd_en.port=PORTC_INDEX,
	.lcd_en.pin=GPIO_PIN3,
	.lcd_en.direction=GPIO_DIRECTION_OUTPUT,
	.lcd_en.logic=GPIO_LOW,
	
    .lcd_data[0].port=PORTC_INDEX,
	.lcd_data[0].pin=GPIO_PIN4,
	.lcd_data[0].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[0].logic=GPIO_LOW,
	
    .lcd_data[1].port=PORTC_INDEX,
	.lcd_data[1].pin=GPIO_PIN5,
	.lcd_data[1].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[1].logic=GPIO_LOW,
	
    .lcd_data[2].port=PORTC_INDEX,
	.lcd_data[2].pin=GPIO_PIN6,
	.lcd_data[2].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[2].logic=GPIO_LOW,
	
    .lcd_data[3].port=PORTC_INDEX,
	.lcd_data[3].pin=GPIO_PIN7,
	.lcd_data[3].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[3].logic=GPIO_LOW,
	
};


keypad_t keypad_1 ={
	.keypad_row_pins[0].port=PORTB_INDEX,
	.keypad_row_pins[0].pin=GPIO_PIN0,
	.keypad_row_pins[0].direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pins[0].logic=GPIO_LOW,	
	
    .keypad_row_pins[1].port=PORTB_INDEX,
	.keypad_row_pins[1].pin=GPIO_PIN1,
	.keypad_row_pins[1].direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pins[1].logic=GPIO_LOW,
	
    .keypad_row_pins[2].port=PORTB_INDEX,
	.keypad_row_pins[2].pin=GPIO_PIN2,
	.keypad_row_pins[2].direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pins[2].logic=GPIO_LOW,
	
    .keypad_row_pins[3].port=PORTB_INDEX,
	.keypad_row_pins[3].pin=GPIO_PIN3,
	.keypad_row_pins[3].direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pins[3].logic=GPIO_LOW,
	
    .keypad_columns_pins[0].port=PORTB_INDEX,
	.keypad_columns_pins[0].pin=GPIO_PIN4,
	.keypad_columns_pins[0].direction=GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[0].logic=GPIO_LOW,
	
    .keypad_columns_pins[1].port=PORTB_INDEX,
	.keypad_columns_pins[1].pin=GPIO_PIN5,
	.keypad_columns_pins[1].direction=GPIO_DIRECTION_INPUT,
	.keypad_columns_pins[1].logic=GPIO_LOW,
	
    .keypad_columns_pins[2].port=PORTB_INDEX,
	.keypad_columns_pins[2].pin=GPIO_PIN6,
	.keypad_columns_pins[2].direction=GPIO_DIRECTION_INPUT,
	.keypad_columns_pins[2].logic=GPIO_LOW,
	
    .keypad_columns_pins[3].port=PORTB_INDEX,
	.keypad_columns_pins[3].pin=GPIO_PIN7,
	.keypad_columns_pins[3].direction=GPIO_DIRECTION_INPUT,
	.keypad_columns_pins[3].logic=GPIO_LOW,				
};

adc_conf_t adc_1 = 
{
    .ADC_InterruptHandler = adc_interrept_handeler,
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN0,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED
};

 


timer1_t tmr_1 =
{
    .TMR1_InterruptHandler = tmr0_handeler,
    .priority = INTERRUPT_LOW_PRIORITY,
    .timer1_mode = TIMER1_COUNTER_MODE,
    .timer1_prescaler_value = TIMER1_PRESCALER_DIV_BY_1,
    .timer1_preload_value = 0,
    .timer1_reg_wr_mode = TIMER1_RW_REG_16Bit_MODE,
    .timer1_counter_mode = TIMER1_SYNC_COUNTER_MODE
};
timer0_t tmr_0 =
{
    .TMR0_InterruptHandler = tmr0_handeler,
    .timer0_mode = TIMER0_TIMER_MODE,
    .timer0_register_size = TIMER0_16BIT_REGISTER_MODE,
    .prescaler_enable = TIMER0_PRESCALER_ENABLE_CFG,
    .prescaler_value = TIMER0_PRESCALER_DIV_BY_16,
    .timer0_preload_value = 3036
};

led_t led1 =
{
    .port_name = PORTD_INDEX,
    .pin = GPIO_PIN1,
    .led_status = LED_OFF
};



int main()
{ 
    Std_ReturnType ret = E_NOT_OK;
    application_intialize();    
    
    ret = lcd_4bit_send_string_pos(&lcd_1, 2, 3,"Frequency Metter");
    __delay_ms(2000);
    ret = lcd_4bit_send_command(&lcd_1 , _LCD_CLEAR);
    ret = lcd_4bit_send_string_pos(&lcd_1, 2, 1,"Frequency = ");
    
    while(1)
    {
        ret = convert_uint16_to_string(Freq, &freqArray);
        ret = lcd_4bit_send_string_pos(&lcd_1, 2, 13, freqArray);
        ret = lcd_4bit_send_string_pos(&lcd_1, 2, 18, "Hz");
    }
    return (EXIT_SUCCESS);
}


void application_intialize(void)
{
    Std_ReturnType ret = E_NOT_OK;
    //ret = ADC_Init(&adc_1);
    ret = lcd_4bit_intialize(&lcd_1);
    //ret = keypad_initialize(&keypad_1);
    ret = led_initialize(&led1);
    ret = Timer0_Init(&tmr_0);
    ret = Timer1_Init(&tmr_1);
}


void tmr1_handeler()
{

}


void tmr0_handeler()
{
    Std_ReturnType ret = E_NOT_OK;
    ret = Timer1_Read_Value(&tmr_1, &Freq);
    ret = Timer1_Write_Value(&tmr_1, 0x00);
}


