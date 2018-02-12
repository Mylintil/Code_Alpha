void UART_start(void);
void UART_emitt(char word);

#define TX BIT1
#define RX BIT2
#define UART_PORT_DIR P1DIR
#define UART_PORT_OUT P1OUT

unsigned short receive_cpt=0;
char parity=0;
char word_receive=0;
unsigned short word_emitt=0;
char receive_start = 0;
