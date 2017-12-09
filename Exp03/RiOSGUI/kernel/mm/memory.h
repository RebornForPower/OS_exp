/*
 *  RiOS/kernel/mm/memory.h
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

typedef struct Mem_record
{
    unsigned int mem_addr;
    unsigned int mem_size;
}Mem_record;

typedef struct Mem_table
{
    unsigned int mem_record_total;
    Mem_record mem_record[4090];
}Mem_table;

void init_zero_mem_table( Mem_table *Mtable);
int mem_record_free( Mem_table *Mtable,unsigned int upointer,unsigned psize);
void init_mem_table(Mem_table *Mtable);
void* Ri_malloc(unsigned int size);
/*int Ri_free(unsigned int upointer,int size);*/
int Ri_free(void* upointer,int size);
/*int mem_record_alloc( Mem_table *Mtable,unsigned int psize);*/
void* mem_record_alloc( Mem_table *Mtable,unsigned int psize);
int getmemorysize_bit();
int getmemorysize_MB();
/*  void* 这个类型可以转化为任意类型的指针，其实就是unsigned int
 *之前所有void*的地方我都用的是unsigned int 类型
 *但是这样直接把一个无符号整型变为指针，不安全，
 *编译器老报WARNING:'makes integer from pointer without a cast'
 *我就把所有unsigned int 指针的地方改成了 void *
 */
