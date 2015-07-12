`Quan OSD Technical Index`_

---
I2C
---

The OSD  works as an I2C master
Data is transferred using the transfer_request function.
The slave_address deteremines whether the transfer is a transmit or a receive
The data_address should be persistent throughout the transfer
as data is not copied, but transferred direct to/from the buffer

bool I2C_port::transfer_request(uint8_t slave_address,uint8_t * data_address,int32_t numbytes)
   The slave address describes if this is a transmit or a receive. 
   The data address for transmit/receieve must be valid throughout the transmission
   
   * Transmit
       The address is the address of the data to transmit. numbytes is the number of bytes to transmit

   * Receive
       The address is the address to put the received data. numbytes is the number of bytes to receieve
   
`Quan OSD Technical Index`_

.. _`Quan OSD Technical Index` : index.html
   



