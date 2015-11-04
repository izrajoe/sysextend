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

#ifndef SYSEXTEND_DEFS_H
#define SYSEXTEND_DEFS_H



// These definitions are for reserving
// slots in the extended syscall table
// and for defining associated argument types


// SYSEXTEND NUMBERS ----------------------------------
enum{
__NR_dummy, 
__NR_printk,
__NR_mwait,
__NR_monitor,
// add your syscall here!!!!
__SYSEXTEND_SZ};

// SUPPLEMENTAL SYSEXTEND TYPES -----------------------
struct printk_args {
	char buff[255];
};

struct mwait_args{
	void* ptr;
	int ecx;
	int edx;
};

// EXPORTED USER FUNCTIONS ----------------------------

#ifndef __KERNEL__
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/sysctl.h>

// This function is used to call
// into the extended syscall table

int sysextend(long func, void* args){
	struct __sysctl_args wrap;
	memset(&wrap, 0, sizeof(struct __sysctl_args));
	// flag for sysextend
	wrap.name = (int*)1;
	// what to call
	wrap.nlen = func;
	// arguments
	wrap.oldval = args;
	return syscall(SYS__sysctl, &wrap);
}
#endif


#endif

