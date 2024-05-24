#include "main.h"

object_distonary_t od;

void common_state(void);
void initial_state(void);
void case1 (void);
void case2 (void);
void case3 (void);

void device_logic(void) {
  while(1) {
    common_state();
    uart_thread();
    if(!simple_queue_is_empty(&od.usart_queue)){
    	simple_queue_data_t msg;
    	simple_queue_read(&od.usart_queue, &msg);
    	uint8_t uart_symbol = msg.data[0];
        switch(uart_symbol) {
          case 1: {
        	  case1();
          }
            break;
          case 2: {
        	  case2();
          }
            break;
          case 3: {
        	  case3();
          }
          default:
            break;
        }
    }



  }
}

void common_state(void) {
  static uint32_t time_stamp_1ms;
//  static uint8_t tx_data[128];
//  static uint8_t rx_data[128];
//  memset(tx_data, 0, 128);
//  memset(rx_data, 0, 128);
//  tx_data[0] = 1;
//  tx_data[1] = 2;
  static uint8_t tx_data[128] = {1, 2, 3, 4};
  static uint8_t rx_data[128] = {0, 0, 0, 0};
  if(get_ms_time_from(time_stamp_1ms) >= 1000) {
    static uint8_t mood = 0;
	time_stamp_1ms = get_ms_time();
	if(!mood){
		mood = 1;
		//i2c_1_for_debug_address_with_read(0xc0);
		i2c_1_read_mem(0xa0, 0x80, 2, rx_data, 1);//i2c_1_read_mem(0xc0, 0xa0, 2, rx_data, 1);
	}
	else {
		mood = 0;
		//i2c_1_for_debug_address_with_write(0xc0);
		i2c_1_write_mem(0xa0, 0x80, 2, tx_data, 1);
	}

  }
}

void initial_state(void) {
  asm("nop");
}

void case1 (void) {
	static uint8_t tx_data[1] = {0};
    uint8_t rx_data[1] = {0};
    hwSetDigitalOut(pin_out_cs_0, GPIO_ST_OFF);
    spi_write_receive_8_bit(tx_data, rx_data, 1);
    hwSetDigitalOut(pin_out_cs_0, GPIO_ST_ON);
    tx_data[0]++;
}
void case2 (void) {
    uint8_t tx_data[8] = {7, 6, 5, 4, 3, 2, 1, 0};
    uint8_t rx_data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    hwSetDigitalOut(pin_out_cs_0, GPIO_ST_OFF);
    spi_write_receive_8_bit(tx_data, rx_data, 8);
    hwSetDigitalOut(pin_out_cs_0, GPIO_ST_ON);
}
void case3 (void) {
    hwSetDigitalOut(pin_out_cs_0, GPIO_ST_OFF);
    for(int i = 0; i < 1000; i++)
       asm("nop");
    hwSetDigitalOut(pin_out_cs_0, GPIO_ST_ON);
}
