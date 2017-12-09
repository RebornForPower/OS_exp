/*
 *  RiOS/xwindow/xwindow.c
 *
 *  (C) 2017  Curie （邱日）
 */
/*----------xwindow function-----------------*/

int _xwindowRiOSmain()
{//xwindow main,no use under console
		Draw();
		Sleep(3);
		while(1);
		return 0;
}//~

void point(int x,int y,int color)
{
		unsigned char *pVRAM=(char*)0xa0000;//VRAM address=>0xa0000
		// pVRAM[y*320+x]=Ubuntu_RED;
		 pVRAM[y*320+x]=color;
		return;
}//~
// void pointcolor(int x,int y,int color)
// {
// 		unsigned char *pcolorVRAM=(char*)0xa0000;//VRAM address=>0xa0000
// 		pcolorVRAM[y*320+x]=color;
// 		return;
// }//~


void rectangle(unsigned char color,int left,int top,int right,int bottom)
{
//left x//top y//right x//bottom y
//const int SCREEN_XSIZE=320;
		unsigned char *pVRAM=(char*)0xa0000;
		int x, y;

		for(x=left;x<right;x++){
				for(y=top;y<bottom;y++){
						// point(y,x,color);
						pVRAM[y*320+x]=color;
				}
		}
		return;
}//~
// void Pagerectangle(unsigned char color,int left,int top,int right,int bottom)
void Pagerectangle(unsigned char color,int left,int top,int right,int bottom)
{
//left x//top y//right x//bottom y
//const int SCREEN_XSIZE=320;
		unsigned char *pVRAM=(char*)0xa0000;
		int x, y;

		for(x=left;x<right;x++){
				for(y=top;y<bottom;y++){
						// pVRAM[y*320+x]|=LIGHT_GREEN;
						if(color!=DARK_BLUE)	pVRAM[y*320+x]|=color;
						else{
							pVRAM[y*320+x]=DARK_BLUE;
						}
/*非常重要，我由pVRAM[y*320+x]=color;变为pVRAM[y*320+x]|=color;这样可以不覆盖线框*/
				//320: 0x13 interrupt:VGA graphic mode=>(320*20)*8 color
				}
		}
		/*画线框*/
		y=top;{for(x=left;x<right;x++)pVRAM[y*320+x]=WHITE;}
		y=bottom;{for(x=left;x<right;x++)pVRAM[y*320+x]=WHITE;}
		x=left;{for(y=top;y<bottom;y++)pVRAM[y*320+x]=WHITE;}
		x=right;{for(y=top;y<bottom;y++)pVRAM[y*320+x]=WHITE;}
		return;
}//~
void cls()
{
		rectangle(BLACK,0,0,320,200);
};//~

void Draw(void)
{//draw ascii chars
		rectangle(LIGHT_GREEN ,0,0,320,200);
		// #include "RiASCII.txt"
		int i=0;
		int x=0,y=0;
		int counter=0;
		const int LINEMAX=38;
		for(i=32;i<126;i++){
				Riputch_int(i,x,y);
				x+=8;
				counter++;
				if(counter%LINEMAX==0){y+=16;x=0;counter%=LINEMAX;}
		}
						// #include "RiAlphaNum.txt"
						//int i=0;
						// for(i=0;i<8;i++)Draw_en(Rialphabet+16*i,200+8*i,100);
		return;
}//~

void Draw_en(char *en32,int pos_x,int pos_y)
{
	unsigned char *pVRAM=(char*)0xa0000;//VRAM address=>0xa0000
	// pVRAM[y*320+x]=Ubuntu_RED;
	//  pVRAM[y*320+x]=color;
		int x,y;
		int i,j;
		unsigned char hexi=0x80;
		for(i=0;i<16;i++){
				hexi=0x80;
					// int Black=0;
				for(j=0;j<8;j++){
						if ((en32[i] & hexi) != 0)
						point(pos_x+j, pos_y+i,WHITE);
/*字默认用白色*/
						hexi=hexi>>1;
				}
		}
}//~

void Draw_zh(char *zh32,int pos_x,int pos_y)
{
		int x,y;
		char MyWord1,MyWord2;
		int i,j;
		unsigned char hexi=0x80;
		for(i=0;i<16;i++){
				MyWord1=zh32[2*i];
				MyWord2=zh32[2*i+1];
				hexi=0x80;
				int Black=0;
				for(j=0;j<8;j++){
						if ((MyWord1 & hexi) != 0)point(pos_x+j,i+pos_y,Black);//j is pos_x
						if ((MyWord2 & hexi) != 0)point(pos_x+j+8,i+pos_y,Black);
/*字默认用黑色*/
						hexi=hexi>>1;
				}
		}
}//~

void Riputch_int(int ch,int x,int y)
{
		// #include "RiASCII.txt"
		int i=0,j=0;//if(*ch==0)Draw_en(RiASCII+(i-32)*16,200,100);
		Draw_en(RiASCII+(ch-32)*16,x,y);
};//~
