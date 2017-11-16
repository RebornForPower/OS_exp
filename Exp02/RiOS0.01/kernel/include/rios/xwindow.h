/*
 *  RiOS/kernel/include/rios/xwindow.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

/*------我目前暂停对xwindow的开发，专注于字符终端，毕竟我搞的是kernel-----------------------------------
 *如果要xwindow 模式，要把RiOS/kernel/RiOShead.S中	mov $0x03,%al;xor %ah,%ah;int $0x10改为mov $0x13,%al;
 * -----------------xwindow-function declaration-----------------
 */
int _xwindowRiOSmain();//xwindow main,no use under 80*25 TEXT MODE
void point(int x,int y);
void rectangle(unsigned char color,int left,int top,int right,int bottom);
void cls();
void Draw(void);
void Draw_en(char *en32,int pos_x,int pos_y);//8*16
void Draw_zh(char *zh32,int pos_x,int pos_y);//16*16
void Riputch_int(int ch,int x,int y);
/*color constant*/
const unsigned char BLACK = 0;
const unsigned char LIGHT_GREEN =10;
const unsigned char DARK_GRAY =20;
const unsigned char WHITE =30;
const unsigned char RED = 40;
const unsigned char MIDLIGHT_GREEN = 50;
const unsigned char PINK_RED =60;
const unsigned char YELLOW_GREEN = 70;
const unsigned char PURPLE = 80;
const unsigned char ORANGE = 90;
const unsigned char LIGHT_BLUE  = 100;
const unsigned char DARK_RED = 110;
const unsigned char DARK_GREEN = 120;
const unsigned char DARK_PURPLE = 130;
const unsigned char DARK_ORANGE = 140;
const unsigned char DARK_BLUE = 150;
const unsigned char DARK_RED_GRAY = 160;
const unsigned char DARK_GREEN_GRAY = 170;
const unsigned char Ubuntu_RED =181;
