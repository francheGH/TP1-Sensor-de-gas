/*==================[inlcusiones]============================================*/

#include "sapi.h"       // <= Biblioteca sAPI
#include "stdint.h"
//#include "c_i18n_es.h" // <= para traducir el codigo C al espaï¿½ol (opcional)
//#include "c_i18n_es.h" // <= para traducir la sAPI al espaï¿½ol (opcional)

/*==================[definiciones y macros]==================================*/

//#define UART_SELECTED   UART_GPIO
//#define UART_SELECTED   UART_485
//#define UART_SELECTED   UART_USB
//#define UART_SELECTED   UART_ENET
//#define UART_SELECTED   UART_232

/*==================[definiciones de datos internos]=========================*/



/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

/*==================[funcion principal]======================================*/


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
void lcd_sapi_init( void ){

   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   // Inicializar UART_USB como salida de consola
   //consolePrintConfigUart( UART_USB, 115200 );


   // Inicializar LCD de 16x2 (caracteres x lineas) con cada caracter de 5x2 pixeles
   lcdInit(16 ,2 ,5 ,8 );

   lcdClear(); // Borrar la pantalla

   lcdGoToXY( 1, 1 ); // Poner cursor en 1, 1
}

void lcd_sapi_print( char* str)
{
      lcdSendStringRaw( str);
}
void lcd_sapi_XY( uint8_t x, uint8_t y)
{
lcdGoToXY(x,y);
}
/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

/*==================[fin del archivo]========================================*/
