/*
 *  RiOS/console/console.h
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

/*-----80*25 Text MODE ------------------
 *全局变量SCREEN_X,SCREEN_Y定义在kernel/main.c里面
 * unsigned char SCREEN_X=0;
 * unsigned char SCREEN_Y=0;
 */
#define value2ascii(n) (n+48)
/*由数值到ascii 码值要加上48
 *$man ascii
 *NOTE!!之前我putchar时没有把value值转成ascii码，导致输不出来，这个把值加48编程ascii码值，这样才能打印
 */
void Ri_putchar(unsigned char ascii_char);
void putdec(int value);//此函数意义重大，这意味着我可以输出变量的值了
/*终于能打印变量的值了，之前什么都看不到，这下 NOTE!!Debug方便多了
 * putdec 把一个数值转为十进制每个字符依次输出
 *Usage:putdec(1234); 结果屏幕上依次输出1 2 3 4
 *eg putdec(20171028);!!!成功输出数值 2 0 1 7 1 0 2 8 ，离printf又近了一步
 */
void xy_putchar(unsigned char ascii_char,int row,int col);
void Ri_putchar_colorful(unsigned char ascii_char,unsigned char color);
void xy_putdec(int value,int row,int col);
void console_cls();
void console_cls_black();
void println(char *str);
void printlnColorful(char *str);
void printnoln(char *str);
void resetSCREENYX(int x,int y);
void endl();
void tab();
void Sleep(int secs);
void littleSleep(int secs);
void _Debug_showcharFullScreen();
void _Debug_showA2Z();
