#define USE_AND_OR /* To enable AND_OR mask setting */
#include "config.h"
#include "pic24_i2c.h"
#include <i2c.h>


//******************** Interrupt service routine for I2C1 Master ***************************
void __attribute__((interrupt,no_auto_psv)) _MI2C1Interrupt(void) {
  
  //Clear Interrupt status of I2C1
  MI2C1_Clear_Intr_Status_Bit;  
}


/******************************************************************************
****                                                                       ****
**                                                                           **
i2c1_open()

**                                                                           **
****                                                                       ****
******************************************************************************/
void i2c1_open(void) { // unsigned int config1, unsigned int config2) {
	unsigned int config1 = 0;
	unsigned int config2 = 0;

	//Disable I2C1 module if enabled previously.
	CloseI2C1();    

	// We're not using interrupts.
	ConfigIntI2C1(MI2C_INT_OFF); 

	//
	//        I2C1 enabled
	//        continue I2C module in Idle mode
	//        IPMI mode not enabled
	//        I2CADD is 7-bit address
	//        Disable Slew Rate Control for 100KHz
	//        Enable SM bus specification
	//        Disable General call address
	//        Baud @ 8MHz = 39 into I2CxBRG
	//
	config1 = (I2C_ON | I2C_7BIT_ADD);
	config2 = 4*39; // aek for 32MHz
	OpenI2C1(config1,config2); 
	  
} /* i2c1_open() */
	


/******************************************************************************
****                                                                       ****
**                                                                           **
i2c1_write()

**                                                                           **
****                                                                       ****
******************************************************************************/
int i2c1_write(unsigned char addr,
               unsigned char * data, 
               unsigned int len) {
	unsigned int i;
                 

	// Start, then idle. Assumes we were idling ...
	StartI2C1();								// Send start bit
	IdleI2C1();									// Wait to complete

	// Write Slave address
	MasterWriteI2C1((addr<<1)&0xFE);  			// Write Slave address and set master for transmission
    IdleI2C1();									// Wait to complete

    // Write data
	for (i=0; i<len; i++) {						 
	  MasterWriteI2C1(*data++);  				// Transmit next byte of data
	  IdleI2C1();								// Wait to complete
	}
	
  	// Stop, then idle
	StopI2C1();              					// Send stop bit
	IdleI2C1();              					// Wait to complete   

	return 0;
} /* i2c1_write() */



/******************************************************************************
****                                                                       ****
**                                                                           **
i2c1_read()

**                                                                           **
****                                                                       ****
******************************************************************************/
int i2c1_read(unsigned char addr,
              unsigned char * data, 
              unsigned int len) {
                 

	// Start, then idle. Assumes we were idling ...
	StartI2C1();								// Send start bit
	IdleI2C1();									// Wait to complete

	// Write Slave address
	MasterWriteI2C1((addr<<1)|0x01);  			// Write Slave address and set master for reception
    IdleI2C1();									// Wait to complete

    // Read data
    MastergetsI2C1(len,data,1000);              // Read requested bytes
	IdleI2C1();								    // Wait to complete
	
  	// Stop, then idle
	StopI2C1();              					// Send stop bit
	IdleI2C1();              					// Wait to complete   

	return 0;
} /* i2c1_read() */




#if 0 //aek saved 20131025
/******************************************************************************
****                                                                       ****
**                                                                           **
i2c1_read()

**                                                                           **
****                                                                       ****
******************************************************************************/
int i2c1_read(unsigned char addr,
                  unsigned char * data, 
                  unsigned int len) {
	unsigned int i=0;
                 
	
	// From idle condition, write Start bit.
	IdleI2C1();
	StartI2C1();
	while(I2C1CONbits.SEN );  					// Wait till Start sequence is completed
	MI2C1_Clear_Intr_Status_Bit; 				// Clear interrupt flag	
	
	// Master sends Slave Address with read signal
	// Now write Slave address, followed by string of data.
	MasterWriteI2C1((addr<<1)|0x01);  	// Write Slave address and set master for transmission
	while(I2C1STATbits.TBF);     				// Wait till address is transmitted
	while(!IFS1bits.MI2C1IF);    				// Wait for ninth clock cycle
	MI2C1_Clear_Intr_Status_Bit; 				// Clear interrupt flag
	while(I2C1STATbits.ACKSTAT);				// Wait for Slave's ACK	

	// Wait a while for Slave to get ready, post-ACK.
	for(i=0;i<1000;i++);

	
	
	// Master Recieves from slave 
	MastergetsI2C1(len,data,1000); //Master recieves from Slave

	// Stop condition
	IdleI2C1();              //wait for the I2C to be Idle
	StopI2C1();              //Terminate communication protocol with stop signal
	while(I2C1CONbits.PEN);  //Wait till stop sequence is completed

	return 0;
} /* i2c1_read() */




/******************************************************************************
****                                                                       ****
**                                                                           **
i2c1_write()

**                                                                           **
****                                                                       ****
******************************************************************************/
int i2c1_write(unsigned char addr,
                   unsigned char * data, 
                   unsigned int len) {
	unsigned int i=0, j=0;
                 

	// From idle condition, write Start bit.
	IdleI2C1();
	StartI2C1();
	while(I2C1CONbits.SEN );  					// Wait till Start sequence is completed
	MI2C1_Clear_Intr_Status_Bit; 				// Clear interrupt flag

	// Now write Slave address, followed by string of data.
	MasterWriteI2C1((addr<<1)&0xFE);  	// Write Slave address and set master for transmission
	while(I2C1STATbits.TBF);     				// Wait till address is transmitted
	while(!IFS1bits.MI2C1IF);    				// Wait for ninth clock cycle
	MI2C1_Clear_Intr_Status_Bit; 				// Clear interrupt flag
	while(I2C1STATbits.ACKSTAT);				// Wait for Slave's ACK	

	// Wait a while for Slave to get ready, post-ACK.
	for(i=0;i<1000;i++);

	// Master Data transmission after slave ACK 
	for (i=0; i<len; i++) {	
	  MasterWriteI2C1(*data++);  				//Transmit next byte of data
		while(I2C1STATbits.TBF);     
		while(!IFS1bits.MI2C1IF);    
		MI2C1_Clear_Intr_Status_Bit; 
        while(I2C1STATbits.ACKSTAT);     
		for(j=0; j<1000 ;j++);
	}
	
  // Stop condition
	IdleI2C1();              						//wait for the I2C to be Idle
	StopI2C1();              						//Terminate communication protocol with stop signal
	while(I2C1CONbits.PEN);  						//Wait till stop sequence is completed

	return 0;
} /* i2c1_write() */
#endif

