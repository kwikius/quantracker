`Quan Index`_

---
I2C
---

Interrupt

I2C_port::handle_irq
   Called from the stm32 interrupt vector when there is an i2c interrupt
   The i2c subsystem is a state machine implemented using function pointers to
   the function for each state
   The handle_irq function just calls the current function in the state machine
   each function returns true unless it hit a problem
   In that case it sets the i2c errno flag

bool I2C_port::transfer_request(uint8_t slave_address,uint8_t * data_address,int32_t numbytes)
   The slave address describes if this is a transmit or a receive. 
   The data address for transmit/receieve must be valid throughout the transmission
   
   * Transmit
       The address is the address of the data to transmit. numbytes is the number of bytes to transmit

   * Receive
       The address is the address to put the received data. numbytes is the number of bytes to receieve
   
`Quan Index`_

.. _`Quan Index` : index.html
   



