#include<reg52.h>
#include<intrins.h>
#include<string.h>
#define LCD P0
sbit rs=P1^5;
sbit rw=P1^6;
sbit en=P1^7;
sbit lcd_bsy = P0^7;
sbit MTR0= P2^7;
void LCD_Busy_Chk(void);
void init_lcd(void);
void cmd_lcd(unsigned char);
void data_lcd(unsigned char);
void msgdisplay(unsigned char b[]);
void delay(unsigned int value);
void gsm_init(void);
void modem_send(unsigned char *);
void enter(void);
void Readmessage(void);
void sendbyte(unsigned char *);
void sendslot(unsigned char);
35
void send_sms1(unsigned char *cmd1,unsigned char *cmd2);
unsigned char dat[10];
unsigned char number[15];
unsigned char interruptno[13];
void integer_lcd(int);
void float_lcd(float f);
float adc_convert (void);
unsigned char byte_write_read(unsigned char);
void select_channel(void);
unsigned char channel,A,C;
int f,f1,x,y;
unsigned char SlotNo;
bit MsgRead;
void comp(void);
bit flag;
//////////////////////////////////////////////////////////////////
void serialintr(void) interrupt 4
{
unsigned char i;
if(SBUF == '+')
{
EA=0;
ES=0;
RI=0;
for(i=0;i<11;i++)
{
while(!RI);
interruptno[i]=SBUF;
RI=0;
}
while(!RI);
SlotNo=SBUF;
RI=0;
MsgRead=1;
}
RI=0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void main()
{
SCON = 0x50;
TMOD = 0x20;
TH1 = -3;///////////////GSM 9600
TR1 = 1;
MTR0=0;
init_lcd(); init_lcd(); init_lcd();
cmd_lcd(0x01);
cmd_lcd(0x80);
msgdisplay("WIRELESS MOTOR");
cmd_lcd(0xC0);
msgdisplay("STAT USING GSM ");
delay(1000);
delay(200);delay(200);delay(200);delay(200);
gsm_init();
EA=1;
ES=1;
MsgRead=0;
delay(200);delay(200);delay(200);delay(200);
delay(200);delay(200);delay(200);delay(200);
while(1)
{
if(MsgRead)
{
EA=0;
Readmessage();
MsgRead=0;
EA=1;
ES=1;
}
}
}
////////////////////////////////////////////////////////////////
void gsm_init(void)
{
modem_send("AT");
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("AT");
modem_send("ATE0");
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("ATE0");
modem_send("AT+CSMS=0");
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("AT+CSMS=0");
modem_send("AT+IPR=9600");
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("AT+IPR=9600");
modem_send("AT+CMGF=1");
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("AT+CMGF=1");
modem_send("AT&W");
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("AT&W");
modem_send("AT+CNMI=2,1,0,0,0");
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("AT+CNMI=2,1,0,0,0");
cmd_lcd(0x01);cmd_lcd(0x80);
modem_send("AT+CMGD=1,4");
delay(200);delay(200);
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("GSM Initialized");
delay(200);delay(200);
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("WAITING FOR SMS");
delay(200);delay(200);
}
//////////////////////////////////////////////////////////////////////////
void modem_send(unsigned char *ch)
{
unsigned char cha;
while(*ch)
{
SBUF = *ch;
while(TI == 0);
TI = 0;
ch++;
}
enter();
TI = 0;
RI = 0;
while(1)
{
while(RI == 0);
RI = 0;
cha = SBUF;
if(cha == 'O');
while(RI == 0);
RI = 0;
cha = SBUF;
if(cha == 'K')
break;
38
SBUF = 'A';
while(TI == 0);
TI = 0;
SBUF = '/';
while(TI == 0);
TI = 0;
}
TI = 0;
RI = 0;
}
//////////////////////////////////////////////////////////////////////////
void enter(void)
{
SBUF = 0x0a;
while(TI == 0);
TI = 0;
SBUF = 0x0d;
while(TI == 0);
TI = 0;
}
////////////////////////////////////////////////////////////////////////
void sendbyte(unsigned char *str)
{
EA=0;
while(*str!='\0')
{
TI=0;
SBUF=(*str++);
while(!TI);
TI=0;
}
}
//////////////////////////////////////////////////////////////////
void sendslot(unsigned char slot)
{
EA=0;
TI=0;
SBUF=slot;
while(!TI);
TI=0;
}
//////////////////////////////////////////////////////////////
void Readmessage(void)
{
    unsigned char *no;
unsigned char i,cha='X';
*no=SlotNo;
cmd_lcd(0x01);cmd_lcd(0x80);
delay(200);delay(200);
msgdisplay("Message Received");
delay(200);delay(200);delay(200);delay(200);
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("Slot No:");
msgdisplay(no);
delay(200);delay(200);delay(200);delay(200);delay(200);delay(200);
cmd_lcd(0x01);
cmd_lcd(0x80);
msgdisplay("Message Reading");
delay(200);delay(200);delay(200);delay(200);delay(200);delay(200);
sendbyte("AT+CMGR=");
sendslot(SlotNo);
enter();
while(SBUF!=':')
{
while(!RI);
RI=0;
}
while(SBUF!=',')
{
while(!RI);
RI=0;
}
while(SBUF!=',')
{
while(!RI);
RI=0;
}
for(i=0;i<4;i++)
{
while(!RI);
RI=0;
}
for(i=0;i<10;i++)
{
while(!RI);
number[i] = SBUF;
RI=0;
}
number[i]='\0';
while(SBUF!='+')
{
while(!RI);
RI=0;
}
for(i=0;i<5;i++)
{
while(!RI);
RI=0;
}
for(i=0;i<10;i++)
{
while(!RI);
dat[i]=SBUF;
RI=0;
}
dat[i] = '\0';
while(1)
{
while(RI == 0);
cha = SBUF;
RI = 0;
if(cha == 'O');
while(RI == 0);
cha = SBUF;
RI = 0;
if(cha=='K')
break;
}
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("Message Showing");
delay(200);delay(200);delay(200);delay(200);delay(200);delay(200);
cmd_lcd(0x01);
cmd_lcd(0x80);
msgdisplay(number);
delay(200);delay(200);delay(200);delay(200);
cmd_lcd(0x01);
cmd_lcd(0x80);
msgdisplay(dat);
delay(200);delay(200);delay(200);delay(200);delay(200);delay(200);
comp();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void send_sms1(unsigned char *cmd1,unsigned char *cmd2)
{
unsigned char cha='X',P;
while(*cmd1)
{
RI=0;
TI=0;
SBUF=*cmd1;
while(TI==0);
TI=0;
RI=0;
cmd1++;
}
for(P=0;P<10;P++)
{
RI=0;
TI=0;
SBUF=number[P];
while(TI==0);
TI=0;
RI=0;
}
sendbyte(cmd2);
enter();
while(cha!='>')
{
while(RI==0);
cha=SBUF;
RI=0;
}
RI=0;
TI=0;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void comp(void)
{
if(!strcmp(dat,"ACMOTOR ON"))
{
cmd_lcd(0x01);cmd_lcd(0xC0);
msgdisplay("ACMOTOR ON");
MTR0=1; //MTR1=0; //WATER=P2^0;BULB=P2^1;FAN=P2^2;LHT=P2^3;
delay(2000);delay(1000);
modem_send("AT+CMGD=1");
TI=0;RI=0;
delay(200);delay(200);delay(200);
cmd_lcd(0x01);cmd_lcd(0xC0);
msgdisplay("SENDIND SMS......");
send_sms1("AT+CMGS=\"","\"");
delay(200);delay(200);delay(200);
////////////////////////////////////////
SBUF='A';while(TI==0);TI=0;
SBUF='C';while(TI==0);TI=0;
SBUF='M';while(TI==0);TI=0;
SBUF='O';while(TI==0);TI=0;
SBUF='T';while(TI==0);TI=0;
SBUF='O';while(TI==0);TI=0;
SBUF='R';while(TI==0);TI=0;
SBUF='O';while(TI==0);TI=0;
SBUF='N';while(TI==0);TI=0;
enter();
//////////////////////////
SBUF=0x1a;
while(TI==0);
TI=0;
RI=0;
delay(200);delay(200);delay(200);
delay(200);delay(200);delay(200);
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("SMS SENT");
delay(200);delay(200);delay(200);
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("WAITING FOR SMS");
delay(200);delay(200);delay(200);
}
//////////////////////////////////////////////////////////////////////////////
else if(!strcmp(dat,"ACMOTOROFF"))
{
cmd_lcd(0x01);cmd_lcd(0xC0);
msgdisplay("ACMOTOROFF");
MTR0=0;
delay(2000);delay(1000);
modem_send("AT+CMGD=1");
TI=0;RI=0;
delay(200);delay(200);delay(200);
cmd_lcd(0x01);cmd_lcd(0xC0);
msgdisplay("SENDIND SMS......");
send_sms1("AT+CMGS=\"","\"");
delay(200);delay(200);delay(200);
////////////////////////////////////////
SBUF='A';while(TI==0);TI=0;
SBUF='C';while(TI==0);TI=0;
SBUF='M';while(TI==0);TI=0;
SBUF='O';while(TI==0);TI=0;
SBUF='T';while(TI==0);TI=0;
SBUF='O';while(TI==0);TI=0;
SBUF='R';while(TI==0);TI=0;
SBUF='O';while(TI==0);TI=0;
SBUF='F';while(TI==0);TI=0;
SBUF='F';while(TI==0);TI=0;
enter();
///////////////////////////
SBUF=0x1a;
while(TI==0);
TI=0;
RI=0;
delay(200);delay(200);delay(200);
delay(200);delay(200);delay(200);
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("SMS SENT");
delay(200);delay(200);delay(200);
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("WAITING FOR SMS");
delay(200);delay(200);delay(200);
}
//////////////////////////////////////////////
else
{
cmd_lcd(0x01);cmd_lcd(0x80);
msgdisplay("WAITING FOR SMS");
delay(200);delay(200);delay(200);
}
}
/***************************************/
void init_lcd(void)
{
cmd_lcd(0x30);
cmd_lcd(0x30);
cmd_lcd(0x38);
cmd_lcd(0x0c);
cmd_lcd(0x06);
cmd_lcd(0x01);
delay(10);
}
void cmd_lcd(unsigned char c)
{
LCD_Busy_Chk();
LCD=c;
rs=0;
rw=0;
en=1;
_nop_();
en=0;
}
void data_lcd(unsigned char c)
{
LCD_Busy_Chk();
LCD=c;
rs=1;
rw=0;
en=1;
_nop_();
en=0;
}
void LCD_Busy_Chk(void)
{
lcd_bsy=1;
rs=0;
rw=1;
while(lcd_bsy==1)
{
en=0;
_nop_();
_nop_();
_nop_();
_nop_();
en=1;
}
}
/***************************************/ 
void msgdisplay(unsigned char b[])
{
unsigned char s,count=0;
for(s=0;b[s]!='\0';s++)
{
if(s==16)
cmd_lcd(0xc0);
if(s==32)
cmd_lcd(0x01);
data_lcd(b[s]);
}
}
///////////////////////////////////////////////////////////////
void delay(unsigned int value)
{
unsigned int x,y;
for(x=0;x<100;x++)
for(y=0;y<value;y++);
} //END OF THE PROGRAM
