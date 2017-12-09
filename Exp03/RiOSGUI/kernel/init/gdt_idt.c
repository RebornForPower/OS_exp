/*
 *  RiOS/init/gdt_idt.c
 *
 *  (C) 2017  Curie （邱日）
 */

/*--Global Descriptor Table-&-Global Interrupt Table------*/

void init_gdt(void)
{
		struct Ri_GDT* RiOS_gdt=(struct Ri_GDT*)gdt_template_addr;
		int i=0;
		for(i=0;i<=8192;i++){
				setRecord_gdt(RiOS_gdt+i,0,0,0);
		}

		return;
}//~

void init_idt(void)
{
		struct Ri_IDT *RiOS_idt=(struct Ri_IDT*)idt_template_addr;
		int i=0;
		for(i=0;i<=256;i++){
				setRecord_idt(RiOS_idt+i,0,0,0);
		}
		Ri_load_idt(idt_length, idt_template_addr);
		/*for(i=0;i<256;i++){
		 *setRecord_idt(RiOS_idt+ i,(int)gas_Handle_myinterruptProcessB,2<<3,0x8e);
		 */
		return;
}//~



void set_gdt(void)
{
		struct Ri_GDT *RiOS_gdt=(struct Ri_GDT*)gdt_template_addr;
		setRecord_gdt(RiOS_gdt+0,0x00000000,0x00000000,0x0000);//gdt-0 NULL segment
		setRecord_gdt(RiOS_gdt+1,0xffffffff,0x00000000,0xc092);//gdt-1 Data Segment:all memory
		setRecord_gdt(RiOS_gdt+2,0x0000ffff,0x00000000,0xc09a);//gdt-2 RiOShead.S
//setRecord_gdt(RiOS_gdt+2,0x000007ff,0x00000000,0xc09a);//gdt-2 RiOShead.S
		setRecord_gdt(RiOS_gdt+3,0x0000ffff,0x00280000,0xc09a);//gdt-3 main.c
		Ri_load_gdt(gdt_length,gdt_template_addr);
		return;
		//access:0x92 preserve for OS,rw_
		//access:0x9a preserve for OS,r_x
		//			0xf2            user,rw_
		//      0xfa            user,r_x
		//0xffffffff=>4294967295,4G,all memory
};//~

void set_idt(void)
{
		#define InterruptEnable 0x008e
		struct Ri_IDT *RiOS_idt=(struct Ri_IDT*)idt_template_addr;
		setRecord_idt(RiOS_idt+ 0x21,(int)gas_Handle_myinterruptProcessB,3<<3,InterruptEnable);//Code Segment,segmnet 2
		setRecord_idt(RiOS_idt+0x27,(int)gas_Handle_myinterruptProcessA,3<<3,InterruptEnable);//Code Segment,segmnet 2
		/*--------set 8253 interrupt 20h-------------*/
		//8253 时钟中断中断类型20h，
		setRecord_idt(RiOS_idt+0x20,(int)gas_Handle_8253timer,3<<3,InterruptEnable);
		/*---------------------*/

		Ri_load_idt(0x7ff,0x0026f800);
		//attribu_4b_4nouse_8b
		//----------------------------------
		//xxxx		0000		xxxx		xxxx			|
		//0000    0000    1000    1110
		//《从实模式到保护模式》P201
		//P=1,DPL=00,S=0 TYPE=1110 rwx
		//P:segment present
		//high12   							---->high23 |
		//44b      							----->55b		|
		//----------------------------------
		//0x008e=10001110,interrupt is valid
		return;
};//~


void setRecord_gdt(struct Ri_GDT* mygdt,int length,int base,int prog_access)
{
		//length<0fffff ,real mode
		if(length>0xffff)prog_access|=0x8000;//G bit =1,段界限以4KB为单位，是32位模式

		//错误length/=0x1000;//被这句话坑死了，不停重启  //limit>>16;
		//似乎是段长越界这个电脑就重启，底层不太明白
		mygdt->length_low=length&0xffff;//16bits //location:0~15bits
		//0~15
		mygdt->base_low=base&0xff;//location:16~31bits
		//16~31
		mygdt->base_mid=(base>>16)&0xff; //location:32~39bits(high0~high7)
		//32~39
		mygdt->access_right=prog_access&0xff;//location:40~47(high8~high15)
		//access type itself(4bits) uses 40~43bits(high8~high11),eg0x9a,0x
		//40~47
		mygdt->length_high=((length>>16)&0x0f)|((prog_access>>8)&0xf0);//location:(occupy)48~55bits(high16~high23)
		//length high itself:(4bits 48~51bits(high16~high19))
		//48~55
		mygdt->base_high=(base>>24)&0xff;//location:56~63bits(high24~high31)
		//56~63
	//prog_acess:segment access attribute 12bit
	//xxxx 0000 xxxx xxxx
	//GD:D=1,32mode
		return;
}//~



void setRecord_idt(struct Ri_IDT* myidt,int offset,int selector,int attribu_4b_4nouse_8b)
{
	///segment attribute
	int seg_attr12b=attribu_4b_4nouse_8b;
	//----------------------------------
	//xxxx		0000		xxxx		xxxx			|
	//high12   							---->high23 |
	//44b      							----->55b		|
	//----------------------------------
	// short offset_low;//0~15;
	// short selector;//16~31
	// char occupy8bits;//32~39
	// char access_right;//40~48
	// short offset_high;//48~56
	myidt->offset_low=offset&0xffff;
	myidt->selector =selector;
	myidt->occupy8bits=(seg_attr12b>>8)&0xff;
	myidt->access_right=(char)(seg_attr12b&0xff);
	//NOTE!!这里之前把&错写成了&&，查了一天才查出来
	myidt->offset_high=(offset>>16)&0xffff;
	///!!!!!!!!!!important!!!!!!!!!
	//access_right 1 Bytes
	return;
}//~
