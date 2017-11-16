/*
 *  RiOS/console/console.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

 void Ri_putchar(unsigned char ascii_char)
 {//注意，要传ascii码值，传value过来没有用，要加上48
 	unsigned char *pVRAM=(char*)0xb8000;
 	if(ascii_char=='~'){//\n
 		SCREEN_X=0;
 		SCREEN_Y+=1;
 		if(SCREEN_Y%25==0)SCREEN_Y=0;
 	}
        else if(ascii_char=='\n'){//\n
                SCREEN_X=0;
                SCREEN_Y+=1;
                if(SCREEN_Y%25==0)SCREEN_Y=0;
        }
 	else{
 		pVRAM[(80*SCREEN_Y+SCREEN_X)*2]=ascii_char;
                SCREEN_X++; //SCREEN_X init value=0;
 		if(SCREEN_X%80==0)SCREEN_X=0,SCREEN_Y++;
 		if(SCREEN_Y%25==0)SCREEN_Y=0;
 	}
 	return;
 }//~

 /*Ri_putchar(value2ascii(i));
  *本来应当实现printf的，但实现printf需要先实现sprintf
  *而且还要学习可变参数列表的使用。现阶段先实现一个阉割版的
  *虽然很简单，但也够用了，用putdec打印%d变量的值,Debug比较方便
  */
  void putdec(int value)
  {//此函数意义重大，这意味着我可以输出变量的值了
  /*终于能打印变量的值了，之前什么都看不到，这下debug方便多了*/
  /* putdec 把一个数值转为十进制每个字符依次输出
   *Usage:putdec(1234); 结果屏幕上依次输出1 2 3 4
   *#define value2ascii(n) (n+48)
   */
                if(value==0){Ri_putchar('0');return;}
                int rawvalue=value;
  	        int a[32];
  	        int l=0;
 /*
  *循环获取每一位的值，并存到数组a中。
  *其中l表示已经获取到的位数。
  *由于每次获取的是个位n%10，所以存在数组a中的是各个位数的逆序。
  */
  	        while(value){
  	                      a[l++] = value%10;
  	                      value/=10;
  	        }
  	    //反序输出每一位值。
                 if(rawvalue!=0){
  	                      while(--l>=0)
  	                      Ri_putchar(value2ascii(a[l]));// printf("%d ", a[l]);
                 }
  		return;
  }//~

 void xy_putchar(unsigned char ascii_char,int row,int col)
 {
 		unsigned char *pVRAM=(char*)0xb8000;
 		pVRAM[(80*row+col)*2]=ascii_char;
 		return;
 }//~

 void Ri_putchar_colorful(unsigned char ascii_char,unsigned char color)
 {
 	unsigned char *pVRAM=(char*)0xb8000;
 	if(ascii_char=='~'){//\n
                SCREEN_X=0;
		SCREEN_Y+=1;
 		if(SCREEN_Y%25==0)SCREEN_Y=0;
 	}
        else if(ascii_char=='\n'){//\n
                SCREEN_X=0;
                SCREEN_Y+=1;
                if(SCREEN_Y%25==0)SCREEN_Y=0;
        }
 	else{
		pVRAM[(80*SCREEN_Y+SCREEN_X)*2]=ascii_char;
 		pVRAM[(80*SCREEN_Y+SCREEN_X)*2+1]=color;
 		SCREEN_X++; //SCREEN_X init value=0;
 		if(SCREEN_X%80==0)SCREEN_X=0,SCREEN_Y++;
 		if(SCREEN_Y%25==0)SCREEN_Y=0;
 	}
 	return;
 }//~


 void xy_putdec(int value,int row,int col)
 {//此函数意义重大，这意味着我可以输出变量的值了
  /*终于能打印变量的值了，之前什么都看不到，这下debug方便多了*/
  /* putdec 把一个数值转为十进制每个字符依次输出
   *Usage:putdec(1234); 结果屏幕上依次输出1 2 3 4
   *#define value2ascii(n) (n+48)
   */
                if(value==0){xy_putchar('0',row,col);return;}
  	        int a[32];
  	        int l=0;
 /*循环获取每一位的值，并存到数组a中。
  *其中l表示已经获取到的位数。
  *由于每次获取的是个位n%10，所以存在数组a中的是各个位数的逆序。
  */
  	        while(value){
  	                      a[l++] = value%10;
  	                       value/=10;
  	        }
  	        //反序输出每一位值。
  	        while(--l>=0)
  	              xy_putchar(value2ascii(a[l]),row,col++);// printf("%d ", a[l]);
  	        return;
 }//~

 void console_cls()
 {
  	       //pVRAM[(80*SCREEN_Y+SCREEN_X)*2]
  	       unsigned char *pVRAM=(char*)0xb8000;
  	       int sc_x,sc_y;
  	       for(sc_x=0;sc_x<80;sc_x++){
  		        for(sc_y=0;sc_y<25;sc_y++){
  			            pVRAM[(80*sc_y+sc_x)*2]='#';
  	              }
  	        }
  	        return;
 }//~

void console_cls_black()
{
                 //pVRAM[(80*SCREEN_Y+SCREEN_X)*2]
                unsigned char *pVRAM=(char*)0xb8000;
                int sc_x,sc_y;
                for(sc_x=0;sc_x<80;sc_x++){
                         for(sc_y=0;sc_y<25;sc_y++){
                                 pVRAM[(80*sc_y+sc_x)*2]=' ';
                         }
                 }
                 return;
}//~

 void println(char *str)
 {
                int ch,i;
                for(i=0;str[i]!='\n';i++){
                        ch=str[i];//if(ch>=127|ch<32)ch=32;
   	                Ri_putchar(ch);
                }
                Ri_putchar('\n');
                return;
 }//～

 void printlnColorful(char *str)
 {
                unsigned char *pVRAM=(char*)0xb8000;
                #define Black_greenT 0b00000010
                // #ifndef _setTcolor
                // #define _setTcolor pVRAM[(80*SCREEN_Y+SCREEN_X)*2+1]=Black_greenT
                // #endif
                int ch,i;
                for(i=0;str[i]!='\n';i++){
                        ch=str[i];//if(ch>=127|ch<32)ch=32;
                        pVRAM[(80*SCREEN_Y+SCREEN_X)*2+1]=Black_greenT;
                        Ri_putchar(ch);
                //        _setTcolor,Ri_putchar(ch);
                }
                Ri_putchar('\n');
                return;
 }//～

 void printnoln(char *str)
 {
                int ch,i;
                for(i=0;str[i]!='\n';i++){
                        ch=str[i];//if(ch>=127|ch<32)ch=32;
   	                Ri_putchar(ch);
                }//  Ri_putchar('\n');
                return;
 }

void endl()
{
                Ri_putchar('\n');
}//~


void tab()
{
                int i;
                for(i=0;i<4;i++)Ri_putchar(' ');
}//~

void Sleep(int secs)
{
              int i=0,j=0;
              for(;i<secs;i++)for(j=0;j<5000000;j++);
}//~

void littleSleep(int secs)
{
              int i=0,j=0;
              for(;i<secs;i++)for(j=0;j<100000;j++);
}//~

void _Debug_showcharFullScreen()
{
 		unsigned char ascii_char='A';
 		int i;
 		for(i=0;i<300;i++){
 				Ri_putchar(ascii_char);
 				ascii_char++;
 				if(ascii_char>126)ascii_char=65;
 		}
 		return;
 }//~

void resetSCREENYX(int y,int x)
{
        SCREEN_Y=y;
        SCREEN_X=x;
}//~

void _Debug_showA2Z()
{//screen filled with A ~Z
 		int row=0,col=0;
 		int i=0,j=0;
 		unsigned char ascii_char='A';
 		//Video mode :80*25
 		for(j=0;j<25;j++){
 			for(i=0;i<79;i++)col++,xy_putchar(ascii_char,row,col);
 			ascii_char++;row++;col=0;
 		}
 }//~
