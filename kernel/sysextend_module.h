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

#ifndef SYSEXTEND_MODULE_H
#define SYSEXTEND_MODULE_H

int call_sysextend(int sysextend, void* args);
void set_sysextend(int sysextend, unsigned long (*func)(void*));
void create_sysextend_table(size_t sz);

int rootkit_init(void);
void rootkit_exit(void);

#endif

