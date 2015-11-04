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
#include <linux/slab.h>

#include "sysextend_module.h"

static unsigned long (**sysextend_table)(void*) = NULL;

int call_sysextend(int sysextend, void* args){
	if(sysextend_table[sysextend] != NULL){
		return (sysextend_table[sysextend])(args);
	}
	return -1;
}

void set_sysextend(int sysextend, unsigned long (*func)(void*)){
	sysextend_table[sysextend] = func;
}

void create_sysextend_table(size_t sz) {
	sysextend_table = kmalloc(sizeof(unsigned long *)*sz,GFP_KERNEL);
	memset(sysextend_table,0, sizeof(unsigned long *)*sz);
}

