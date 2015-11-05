/*
Copyright 2015 University of Rochester

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/unistd.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/stat.h>
#include <linux/preempt.h>
#include <asm/cacheflush.h>
#include <linux/sysctl.h>

#include "sysextend_module.h"

// undeprecate sysctl
#define __NR_sysctl __NR__sysctl

static unsigned long ** sys_call_table = NULL;

asmlinkage long (*original_sys_sysctl)(struct __sysctl_args *args);
asmlinkage long fake_sys_sysctl(struct __sysctl_args *args){
	if(args->name==(int*)1){
		return call_sysextend(args->nlen, args->oldval);
	}
	else{
	  return original_sys_sysctl(args);
	}
}

// find the syscall table
// Based on:
// http://stackoverflow.com/questions/13876369/system-call-interception-in-linux-kernel-module-kernel-3-5
// https://gadgetweb.de/linux/40-how-to-hijacking-the-syscall-table-on
static unsigned long **find_sys_call_table(void){
	unsigned long int ptr;
	unsigned long **table;

	ptr = PAGE_OFFSET;
	while(ptr<ULLONG_MAX){
		table = (unsigned long **)ptr;

		// check it is the syscall table
		if((table[__NR_close] == (unsigned long*)sys_close)){
			return table;
		}
		ptr+=sizeof(void*);
	}
	printk("Finding syscall table failed.");
	return NULL;
}

// Disable page protections on this core
// Based on:
// http://vulnfactory.org/blog/2011/08/12/wp-safe-or-not/
static void disable_page_protection(void) {
    unsigned long cr0;
    preempt_disable();
    barrier();
    cr0 = read_cr0() ^ X86_CR0_WP;
    BUG_ON(unlikely(cr0 & X86_CR0_WP));
    write_cr0(cr0);
}
static void enable_page_protection(void) {
    unsigned long cr0;
    cr0 = read_cr0() ^ X86_CR0_WP;
    BUG_ON(unlikely(!(cr0 & X86_CR0_WP)));
    write_cr0(cr0);
    barrier();
    preempt_enable();
}


int rootkit_init(void){

  if(!(sys_call_table = find_sys_call_table())){
    return -1;
	}

	disable_page_protection();
	original_sys_sysctl = (void *)sys_call_table[__NR_sysctl];
	sys_call_table[__NR_sysctl] = (unsigned long *)fake_sys_sysctl;
	enable_page_protection();
	return 0;
}

void rootkit_exit(void){

  if(sys_call_table==NULL) {
    return;
  }

	disable_page_protection();
	sys_call_table[__NR_sysctl] = (unsigned long *)original_sys_sysctl;
	enable_page_protection();
}

