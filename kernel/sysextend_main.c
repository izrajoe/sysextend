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
#include <asm/mwait.h>
#include <linux/sysctl.h>
#include <linux/slab.h>

#include "sysextend_module.h"
#include "sysextend.h"


unsigned long sys_dummy(void* args){
  printk("dummy syscall invoked\n");
	return 0;
}

unsigned long sys_printk(void* wrap){
	struct printk_args* args = (struct printk_args*)wrap;
	char buff[256];
	memset(buff, '\0', 256);
	strncpy(buff, args->buff, 255);
  printk(buff);
	printk("\n");
	return 0;
}


unsigned long sys_monitor(void* wrap){
	//struct mwait_args* args = (struct mwait_args*)wrap;
	printk("monitor invoked\n");
	//__monitor(args->ptr,args->ecx, args->edx);
	return 0;
}

unsigned long sys_mwait(void* wrap){
	//struct mwait_args* args = (struct mwait_args*)wrap;
	printk("mwait invoked\n");
	//__mwait(args->ecx, args->edx);
	return 0;
}

static int __init sysextend_init(void) {

	// log init and create table
	printk(KERN_ALERT "sysextend loaded\n");
	create_sysextend_table(__SYSEXTEND_SZ);

	// populate table
	set_sysextend(__NR_dummy,&sys_dummy);
	set_sysextend(__NR_printk,&sys_printk);
	set_sysextend(__NR_mwait,&sys_mwait);
	set_sysextend(__NR_monitor,&sys_monitor);

	// create rootkit and return
	return rootkit_init();
}

static void __exit sysextend_exit(void) {
	// TODO: free the sysextend table.  Right now we leak it.
	// This is tricky because we might delete it out from under
	// an accessing thread....  

	// A lock would solve this, but we'd prefer better performance
	printk(KERN_ALERT "sysextend exit\n");
	rootkit_exit();
}


module_init(sysextend_init);
module_exit(sysextend_exit);

