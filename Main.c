/*
 * File:   main_I2C.c
 * Author: orell
 *
 * Created on 6 de marzo de 2021, 10:22 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#define _XTAL_FREQ   4000000


// Incluimos la libreria a utilizar 
#include <xc.h>
#include "I2CLIB.h"



char REF = 0;
char led = 0;       //Variables para almacenar datos de UART y del I2C


void setup(void); 



void __interrupt() ISR(void) {
    if (PIR1bits.RCIF == 1) {
        PIR1bits.RCIF = 0; // RCIF en reinicio 
        led = RCREG; // Lectura de datos para almacenar
    }
    if (led == 0X0A) {      //RX tiene un dato y lo indica en las leds
            PORTE = 0;   // BBOTONES DEL IOT CLOUD
    } else if (led == 0X0B) {
        PORTE = 1;
    } else if (led == 0X0C) {
        PORTE = 0;
    } else if (led == 0X0D) {
        PORTE = 2;
    }
    return;
}// Interrupcion que se utiliza (DATASHEET))




void setup(void) {
    ANSEL = 0;  //PUERTOS EN MODO DIGITAL
    ANSELH = 0;     

    TRISA = 00000000;
    TRISB = 00000000;
    TRISC = 00000000;       //INPUTS
    TRISCbits.TRISC3 = 0; //RC3 OUTPUT SCL
    TRISCbits.TRISC7 = 1; //RC7 INPUT RX
    TRISD = 00000000;
    TRISE = 00000000;
    PORTE = 00000000;       //PUERTOS DE LEDS 

    I2C_Master_Init(9); //ADRESS Y EL  I2C

    TXSTAbits.BRGH = 1; 
    BAUDCTLbits.BRG16 = 0;
    SPBRG = 25;

    //TRANSFERECNIA 
    TXSTAbits.TXEN = 1; // TX ON
    TXSTAbits.SYNC = 0; //ASINCRONOMODE
    RCSTAbits.SPEN = 1; // RX ON
    TXSTAbits.TX9 = 0; //TRASFERENCIA 8 BITS

    //READ
    RCSTAbits.CREN = 1; //RECIBIR ON
    RCSTAbits.RX9 = 0; //RECIBIR 8 BITS
    //INTERRUPCIONES ON
    PIE1bits.RCIE = 1;
    PIE1bits.TXIE = 0; 
    PIR1bits.RCIF = 0; 

    INTCONbits.GIE = 1; //TIMER
    

    
         
            
    
    return;

}//CONFIG PUERTOS PIC16F887



void main(void) {
    setup(); 
    
    while (1) {
        TXREG = REF;   //TX ENVIA DATOS A COM I2C
        I2C_Master_Start(); //I2C STARTUP
        I2C_Master_Write(0b11101100); //Escribimos el adress de 7 bits y el bit de escritura
        I2C_Master_Write(0xD0); 
        I2C_Master_Stop(); 
        __delay_ms(200);
        I2C_Master_Start(); // I2C STRARUP
        I2C_Master_Write(0b11101101); 
        REF = I2C_Master_Read(0); //LECTURA Y ALMACENAMIENTO DEL DATO
        I2C_Master_Stop(); 
        __delay_ms(200);
        
        
    }
} // cODIGO MAIN PARA EJECUTARB ACCIONES

