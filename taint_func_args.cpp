/**

Copyright 2007
Georgia Tech Research Corporation
Atlanta, GA  30332-0415
All Rights Reserved

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

   * Redistributions of source code must retain the above copyright
   * notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above
   * copyright notice, this list of conditions and the following
   * disclaimer in the documentation and/or other materials provided
   * with the distribution.

   * Neither the name of the Goergia Tech Research Coporation nor the
   * names of its contributors may be used to endorse or promote
   * products derived from this software without specific prior
   * written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/

/* This is where the user specifies how to taint function arguments. */
#include "include/dytan.h"
#include <stdio.h>
#include <string.h>

/* forward declarations for wrapper functions */
void main_wrapper_func(ADDRINT *, ADDRINT *);
void main_wrapper_func_end();

/* This array holds the function names whose arguments are to be tainted */
string taint_function[] =  { "main", "RegQueryValueExW", "bar" };

/* This function is invoked from Dytan. Modify this function to
 * specify custom function wrappers
 */
void taint_routines(IMG img, void *v)
{
	RTN rtn = RTN_FindByName(img, "main");
    if (RTN_Valid(rtn)) {

		printf("instrument main function\n");
        RTN_Open(rtn);
        /* This function call inserts main_wrapper_func before the
         * function main is executed. Since main takes two parameters,
         * we have supplied two pairs of
         * IARG_FUNCARG_ENTRYPOINT_REFERENCE, <number>
         */
        RTN_InsertCall(rtn, IPOINT_BEFORE, AFUNPTR(main_wrapper_func),
                IARG_FUNCARG_ENTRYPOINT_REFERENCE, 0,
                IARG_FUNCARG_ENTRYPOINT_REFERENCE, 1,
                IARG_END);
        RTN_InsertCall(rtn, IPOINT_AFTER, AFUNPTR(main_wrapper_func_end),
                IARG_END);
        RTN_Close(rtn);
    }
/*
	else if (rtn_name == taint_function[1]) {
		RTN_Open(rtn);
        RTN_InsertCall(rtn, IPOINT_AFTER, AFUNPTR(main_wrapper_func2),
				IARG_FUNCARG_CALLSITE_VALUE, 4,
                IARG_END);
        RTN_Close(rtn);
    }
*/
}

/* This is the actual custom function wrapper. It takes in arguments of type
 * ADDRINT, which should be typecasted to the original type.
 * The arguments should then be tainted using Dytan's library routines.
 */
void main_wrapper_func(ADDRINT *argcAddr, ADDRINT *argvAddr)
{
    /* typecast the arguments to their original type */
    int argc = (int )*argcAddr;
    char **argv = (char **)*argvAddr;

	printf("main_wrapper_func\n");
    /* taint the variables using Dytan library routines */
    /* Arguments: starting address of memory, number of bytes,  */
    /* Third argument is optional; it specifies the taint mark */
    /* If not specified, the taint mark is incremented at every call */
    taintAssignmentLog << "Tainted from argc\n";
    SetNewTaintForMemory((ADDRINT)argcAddr, (ADDRINT)sizeof(int));

    for (int i = 0; i < argc; i++) {
        ADDRINT addr = (ADDRINT) &argv[i];
        taintAssignmentLog << "Tainted from argv" << i << "\n";
        SetNewTaintForMemory( addr, (ADDRINT)strlen(argv[i]));
    }
}

