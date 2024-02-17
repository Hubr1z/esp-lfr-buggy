#include "mbed.h"
#include "bluetooth.h"
#include "constants.h"
#include <stdlib.h>


/* Uses default BT baud rate if not specified and initialise BT objects using init() */
Bluetooth::Bluetooth(PinName TX_pin, PinName RX_pin, int baud_rate): bt_serial(TX_pin, RX_pin, baud_rate) {init();};
Bluetooth::Bluetooth(PinName TX_pin, PinName RX_pin): bt_serial(TX_pin, RX_pin, BT_BAUD_RATE) {init();};


void Bluetooth::init(void)
{
    /* This function used to initialise the Bluetooth object */
    bt_serial.attach(callback(this, &Bluetooth::data_recieved_ISR), Serial::RxIrq);
    rx_index = 0;
    data_complete = false;
}


void Bluetooth::data_recieved_ISR(void)
{
    /*  This ISR will run for every character recieved by the bluetooth module.
        Each time this ISR is ran one character is stored into rx_buffer. 
        rx_index stores the location of next memory location to store the next character */
    char c = bt_serial.getc();
    if (!data_complete)
    {
        rx_buffer[rx_index++] = c;
        if (c == '-' || rx_index == BT_BUFFER_SIZE)
        {
            data_complete = true;
        }
    }
}


bool Bluetooth::data_recieved_complete(void)
{   
    /* Returns true if incoming data is fully recieved*/
    return data_complete;
}


void Bluetooth::reset_rx_buffer(void)
{   
    /* Resets the rx_buffer. Ideally used after processing recieved data*/ 
    memset(rx_buffer, '\0', BT_BUFFER_SIZE);
    data_complete = false;
    rx_index = 0;
}


char* Bluetooth::get_data()
{   
    /* returns data recieved as a character array*/
    return rx_buffer;
}


Bluetooth::BluetoothCommand Bluetooth::parse_data()
{   
    /*  This function reads the incoming data and checks for 
        specific command format and returns the command type */

    // command parsing -> to-do 
    if (rx_buffer[0] == 'V')
    {
        sscanf(rx_buffer, "%*s %f", &float_data1);
        return set_value;
    }
    return get_run_time;
}


void Bluetooth::send_buffer(char* char_arr)
{   
    /*  sends the char array one by one */
    for(int i = 0; i < BT_BUFFER_SIZE; i++)
    {
        bt_serial.putc(char_arr[i]); 
    }
}


void Bluetooth::send_fstring(const char* format, ...) 
{   
    /*  This one is a bit advanced but the macros used are common to be 
        used to pass printf arguments into a function */
    va_list args;               // standard macro
    va_start(args, format);     // standard macro
    vsnprintf(tx_buffer, BT_BUFFER_SIZE, format, args); // converts the format string input to a character array and stores into tx_buffer
    va_end(args);               // standard macro
    send_buffer(tx_buffer);     // sends the character array to be transmitted using bluetooth
    memset(tx_buffer, '\0', BT_BUFFER_SIZE);            // reset the tx_buffer
}


bool Bluetooth::writeable(void)
{   
    /* returns true if bluetooth module is ready */
    return bt_serial.writeable();
}