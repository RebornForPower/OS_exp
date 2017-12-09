本次实验继续在裸机上做，基于上一次实验的RiOS
用过UNIX或Linux的人都知道，它们可以使用图形界面也可以init 5进入字符界面
RiOS在开发之初对于图形界面和字符界面其实都尝试过
后来发现如果深入搞图形界面的话，那将变成了研究图形学
而与操作系统kernel的最基本功能离开得比较远
应此次实验的要求，我重拾图形界面
在语言上，也改为C++。
图形界面和字符界面在系统刚引导时需要经历一个选择，是进video mode还是进80*25的字符终端模式
在代码上，体现在RiOS/kernel/RiOShead.S这个用汇编写成的与显示模式选择相关的代码
在RiOShead.S有这么一段代码
```Assembly
/*------80*25  TEXT mode,default--*/
	mov $0x03,%al
	xor %ah,%ah
	int $0x10//main.c RiOSmain
```        
这里BIOS调用选用的是0x03功能，进入80×25的文本显示模式，上次实验在此模式下进行
由于本次实验要使用图形界面，故本次实验不能再用文本显示模式，而应当使用视频显示模式
（video mode）。体现在代码上,就是要调用BIOS的0x13功能，启用320*200分辨率
（当然，也可以更高）的彩色视频显示模式。
```Assembly
/*------320*200 VIDEO mode,simple xwindow--*/
	mov $0x13,%al
 	xor %ah,%ah
 	int $0x10	//main.c _xwindowRiOSmain
```
仅仅把0x03改为0x13，这个改变是深远的，整个操作系统将进入不同的显示模式，
从原来的字符文本模式变为图形界面的，我的整个代码都需要调整。
我的字符显示模式下的主函数是RiOSmain,
与本次实验图形界面相关的代码文件有RiOS/kernel/include/rios/xwindow.h
RiOS/kernel/xwindow/xwindow.c这两个相当于我RiOS的图形库。
由于我在initialize函数中封装了很多初始化的操作，所以，GUI下RiOSmain还是要先
调用initialize函数的。
我的第一次实验其实很多函数和中断函数中的do_timer有关，这次把它暂时注释掉，待日后整合，此次不使用
