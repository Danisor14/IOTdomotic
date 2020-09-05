#line 1 "F:/feria/feria.c"
sbit led_sala at RB0_bit;
sbit led_cocina at RB1_bit;
sbit led_bano at RB2_bit;
sbit led_c1 at RB3_bit;
sbit led_c2 at RB4_bit;
sbit sensor_incendio at RB5_bit;
sbit led_incendio at RD6_bit;
sbit sensor_movimiento at RD5_bit;
sbit puerta1 at RB6_bit;
sbit puerta2 at RB7_bit;
sbit led_d at RD3_bit;
sbit led_movimiento at RD7_bit;
sbit motobomba at RD4_bit;
unsigned int Aseguridad;
unsigned int Aincendio;
unsigned int in;
unsigned int se;
unsigned char i ;
unsigned int codigo;
float temperatura;

char trama[150];
char trama_largo[20];
char captu[30];

int var_largo;



enum estados_ {cap_l,cap_u,cap_e,cap_co,esp_final} ;

 char estado_rec=cap_l;


float valor_s;
float valor_c;
float valor_c1;
float valor_c2;
float valor_b;
float valor_p1;
float valor_p2;
float valor_m;
float valor_i;
float valor_d;
char trama_rx[50];
char contador_trama=0;
unsigned long demora=0;

void sonido_seguridad(){
 Sound_play(440, 500);
 Sound_play(440, 500);
 Sound_play(440, 500) ;
 Sound_play(349, 350) ;
 Sound_play(523, 150);
 Sound_play(440, 500);
 Sound_play(349, 350) ;
 Sound_play(523, 150);
 Sound_play(440, 650);

 delay_ms(150) ;

 Sound_play(659, 500);
 Sound_play(659, 500);
 Sound_play(659, 500);
 Sound_play(698, 350);
 Sound_play(523, 150);
 Sound_play(415, 500);
 Sound_play(349, 350);
 Sound_play(523, 150);
 Sound_play(440, 650);

 delay_ms(150);

}

void abrir1(){
 for(i=0;i<100;i++){
 puerta1 = 1;
 delay_us(1000);
 puerta1 = 0;
 delay_us(19000);
 }
}
void cerar1(){
 for(i=0;i<100;i++){
 puerta1 = 1;
 delay_us(2000);
 puerta1 = 0;
 delay_us(19000);
 }
}
void abrir2(){
 for(i=0;i<100;i++){
 puerta2 = 1;
 delay_us(1000);
 puerta2 = 0;
 delay_us(19000);
 }
}
void cerar2(){
 for(i=0;i<100;i++){
 puerta2 = 1;
 delay_us(2000);
 puerta2 = 0;
 delay_us(19000);
 }
}



char lee_trama()
{
char dato_rx;

memset(trama_rx,50,0);
demora=0;
contador_trama=0;
estado_rec=cap_l;

RCIF_bit=0;
CREN_bit=0;
CREN_bit=1;

while (demora<=66667)
{
 demora++;
 switch (estado_rec)
 {
 case cap_l:
 if (RCIF_bit==1)
 {
 dato_rx=UART_Read();
 trama_rx[contador_trama]=dato_rx;
 if (dato_rx!='l') contador_trama=0;
 else
 {
 contador_trama++;
 estado_rec=cap_u;
 }
 RCIF_bit=0;
 }
 break;
 case cap_u:
 if (RCIF_bit==1)
 {
 dato_rx=UART_Read();
 trama_rx[contador_trama]=dato_rx;
 if (dato_rx!='u')
 {
 contador_trama=0;
 estado_rec=cap_l;
 }
 else
 {
 contador_trama++;
 estado_rec=cap_e;
 }
 RCIF_bit=0;
 }
 break;

 case cap_e:
 if (RCIF_bit==1)
 {
 dato_rx=UART_Read();
 trama_rx[contador_trama]=dato_rx;
 if (dato_rx!='e')
 {
 contador_trama=0;
 estado_rec=cap_l;
 }
 else
 {
 contador_trama++;
 estado_rec=cap_co;
 }
 RCIF_bit=0;
 }
 break;


 case cap_co:
 if (RCIF_bit==1)
 {
 dato_rx=UART_Read();
 trama_rx[contador_trama]=dato_rx;
 if (dato_rx!='\"')
 {
 contador_trama=0;
 estado_rec=cap_l;

 }
 else
 {
 contador_trama++;
 estado_rec=esp_final;
 }
 RCIF_bit=0;
 }

 break;

 case esp_final:
 if (RCIF_bit==1)
 {
 dato_rx=UART_Read();
 trama_rx[contador_trama]=dato_rx;
 contador_trama++;
 if (dato_rx=='}') return(1);
 RCIF_bit=0;
 }
 break;


 }

 }
 return(0);

}



unsigned long strlen_const(const char *const_pun)
{
 unsigned int cuen_const=0;

 while (*const_pun++!=0) cuen_const++;
 return (cuen_const);

}


void manda_serial_const(const char *info)
{
 while(*info) UART1_Write(*info++);
}



void manda_esp8266_const(const char *info)
{
unsigned int largo;
 manda_serial_const("AT+CIPSEND=");
 UART1_Write('4');
 UART1_Write(',');
 largo=strlen_const(info);
 sprinti(captu,"%1u",largo);
 UART1_Write_Text(captu);
 manda_serial_const("\r\n");
 Delay_ms(100);
 manda_serial_const(info);
}


void manda_esp8266(char *info)
{
unsigned int largo;
 manda_serial_const("AT+CIPSEND=");
 UART1_Write('4');
 UART1_Write(',');
 largo=strlen(info);
 sprinti(captu,"%1u",largo);
 UART1_Write_Text(captu);
 manda_serial_const("\r\n");
 Delay_ms(100);
 UART1_Write_Text(info);
}




void main()
{ adcon1 = 0b1101;
 trisd = 0;
 trisd5_bit = 1 ;

 trisb = 0b00100000;
 trisa0_bit = 1;
 trisa1_bit = 1;
 TRISC6_bit=0;
 TRISC7_bit=1;
 Sound_Init(&portd,0);

 UART1_Init(115200);



manda_serial_const("ATE0\r\n");
Delay_ms(200);

manda_serial_const("AT+CWMODE=1\r\n");
Delay_ms(200);


 manda_serial_const("AT+CWDHCP=1,0\r\n");
 Delay_ms(300);



 manda_serial_const("AT+CIPSTA=\"192.168.1.150\",\"192.168.1.254\",\"255.255.255.0\"\r\n");

 Delay_ms(300);




 manda_serial_const("AT+CWJAP=\"MARELVIS\",\"nacional14\"\r\n");



 Delay_ms(6000);
 manda_serial_const("AT+CIPMUX=1\r\n");
 Delay_ms(200);
 manda_serial_const("AT+CIFSR\r\n");
 Delay_ms(1000);



 while (1)
 {





 manda_serial_const("AT+CIPSTART=4,\"TCP\",\"industrial.api.ubidots.com\",80\r\n");
 Delay_ms(700);

 rd2_bit = 1;
 codigo=ADC_Read(0);
 temperatura=codigo*500.0/1023.0;
 codigo=ADC_Read(1);





 manda_esp8266_const("GET /api/v1.6/devices/micasa/luzsala/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_s=atof(captu);
 if (valor_s==3.0) led_sala=1;
 if (valor_s==2.0) led_sala=0;
 Delay_ms(120);


 manda_esp8266_const("GET /api/v1.6/devices/micasa/luzcocina/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_c=atof(captu);
 if (valor_c==3.0) led_cocina=1;
 if (valor_c==2.0) led_cocina=0;
 Delay_ms(120);


 manda_esp8266_const("GET /api/v1.6/devices/micasa/luzbano/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_b=atof(captu);
 if (valor_b==3.0) led_bano=1;
 if (valor_b==2.0) led_bano=0;
 Delay_ms(120);


 manda_esp8266_const("GET /api/v1.6/devices/micasa/luzcuarto1/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_c1=atof(captu);
 if (valor_c1==3.0) led_c1=1;
 if (valor_c1==2.0) led_c1=0;
 Delay_ms(120);


 manda_esp8266_const("GET /api/v1.6/devices/micasa/luzcuarto2/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_c2=atof(captu);
 if (valor_c2==3.0) led_c2=1;
 if (valor_c2==2.0) led_c2=0;
 Delay_ms(120);



 manda_esp8266_const("GET /api/v1.6/devices/micasa/ducha/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_d=atof(captu);
 if (valor_d==3.0) led_d=1;
 if (valor_d==2.0) led_d=0;
 if (valor_d==3.0 && codigo >= 150) motobomba = 1;
 Delay_ms(120);




 manda_esp8266_const("GET /api/v1.6/devices/micasa/puerta1/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_p1=atof(captu);
 if (valor_p1==3.0){
 abrir1();
 }
 if (valor_p1==2.0){
 cerar1();
 }


 manda_esp8266_const("GET /api/v1.6/devices/micasa/puerta2/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_p2=atof(captu);
 if (valor_p2==3.0){
 abrir2();
 }
 if (valor_p2==2.0){
 cerar2();
 }



 manda_esp8266_const("GET /api/v1.6/devices/micasa/interruptorai/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_i=atof(captu);

 delay_ms(120);

 manda_esp8266_const("GET /api/v1.6/devices/micasa/interruptoras/values?page_size=1&token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\n\n");

 lee_trama();

 strcpy(captu, strtok(trama_rx, ":"));
 strcpy(captu, strtok(0, "}"));

 valor_m=atof(captu);

 if (sensor_incendio==0) in = 3; else in = 2;
 if (sensor_movimiento==1)se = 3; else Se = 2;

 if (valor_i == 3.0 && in == 3){
 Aincendio = 3;
 led_incendio = 1;
 }
 if(valor_i == 2.0){
 Aincendio = 2;
 led_incendio = 0;
 }

 if (valor_m == 3.0 && se == 3){
 Aseguridad = 3;
 led_movimiento = 1;
 }
 if(valor_m == 2.0) {
 Aseguridad = 2;
 led_movimiento = 0;
 }


 memset(trama_largo,150,0);
 memset(trama,20,0);


 sprintf(trama,"{\"temperatura\": %5.2f,\"aseguridad\": %u,\"aincendio\": %u}\n",temperatura,Aseguridad,Aincendio);


 var_largo=strlen(trama)-1;

 sprinti(trama_largo,"%u\n\n",var_largo);


 manda_esp8266_const("POST /api/v1.6/devices/micasa/?token=BBFF-fg0pPB0VSmA8IDVRmNT2ruve9gZEVr HTTP/1.1\nHost: industrial.api.ubidots.com\nContent-Type: application/json\nContent-Length: ");

 Delay_ms(1000);

 manda_esp8266(trama_largo);
 Delay_ms(500);

 manda_esp8266(trama);
 Delay_ms(500);

 }

}
