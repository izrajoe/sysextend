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

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "sysextend.h"

using namespace std;

/*
This example calls printk within the kernel.
The message appears in kernel logs.
To verify it works, use the command:

dmesg | tail 

*/

int main(int argc, char *argv[]){

	printk_args args;
	strncpy(args.buff, "sysextend: Hello World!", 255);
	sysextend(__NR_printk, &args);

}













































