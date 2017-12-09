/*
 *  RiOS/init/gdt_idt.h
 *
 *  Copyright (C) 2017  Curie （邱日）
 */
 extern int gas_Handle_myinterruptProcessA();
 extern int gas_Handle_myinterruptProcessB();
 extern int gas_Handle_8253timer();
 /**gas_handlers END----*/
 extern void Ri_load_gdt(int length,int address);//set Global Descriptor Table's limit and start address
 extern void Ri_load_idt(int length,int address);//set Interrupt Descriptor Table's limit and start address
 extern void Ri_load_ldt(int length,int address);//set Local Descriptor Table's limit and start address
 //~ in main_gas.S

 /*--Global Descriptor Table-&-Global Interrupt Table------*/

 //sizeof(short):2 sizeof(char):1
 struct Ri_GDT
 {//short 2Bytes,16bits;char 1Bytes,8bits
 		short length_low;	//0~15
 		short base_low;//16~31

 		char  base_mid;//32~39
 		char  access_right;	//40~47
 		char  length_high;//48~55
 		char  base_high;	//56~63
 };

 struct Ri_IDT
 {//short 2Bytes,16bits;char 1Bytes,8bits
 		short offset_low;//0~15;
 		short selector;//16~31

 		char occupy8bits;//32~39
 		char access_right;//40~47
 		short offset_high;//48~55
 };

 #define idt_template_addr 0x0026f800 //idt start address
 #define idt_length 0x000007ff //limit 2048=8*255
 //0x270000-0x26f800=2048=256*8 =>256 interrupt
 #define gdt_template_addr 0x00270000 //gdt start addrss
 #define gdt_length 0x0000ffff // limit 65536=8*8192

 void init_gdt(void);
 void init_idt(void);
 void set_gdt(void);
 void set_idt(void);
 void setRecord_gdt(struct Ri_GDT* mygdt,int length,int base,int prog_access);
 void setRecord_idt(struct Ri_IDT* myidt,int offset,int selector,int attribu_4b_4nouse_8b);

 /*--Global Descriptor Table-&-Global Interrupt Table END------*/
 // #define FIRST_TSS_ENTRY 4
 // #define FIRST_LDT_ENTRY (FIRST_TSS_ENTRY+1)
 // #define _TSS(n) ((((unsigned long) n)<<4)+(FIRST_TSS_ENTRY<<3))
 // #define _LDT(n) ((((unsigned long) n)<<4)+(FIRST_LDT_ENTRY<<3))
 // #define ltr(n) __asm__("ltr %%ax"::"a" (_TSS(n)))
 // #define lldt(n) __asm__("lldt %%ax"::"a" (_LDT(n)))
