/*
 *  RiOS/kernel/include/asm/x86_gas.c
 *
 *  Copyright (C) 2017  Curie （邱日）
 */

void gas_set_eflags(int eflags)
{
  __asm__ volatile("pushfl; popl %0":"=r"(eflags));
}

int gas_get_eflags(void)
{
  __asm__ volatile("popfl %0; popfl"::"r"(eflags));
  return eflags;
}
