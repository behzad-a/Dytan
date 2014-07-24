/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2013 Intel Corporation. All rights reserved.
 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
#include <stdio.h>
#ifdef _WIN32
#include <intrin.h>
#endif
#include <string.h>

void myStart()
{
    printf("<START>\n");
    // nop
}

void myEnd()
{
    printf("<END>\n");
    // nop
}

void foo()
{
    // nop

}

void executeInt3(int shouldExecute)
{
    //std::cerr << "<executeInt3>" << std::endl;
    if (shouldExecute) 
    {
        printf("\tdebug break!");
#ifdef _WIN32
        __debugbreak();
#else
        asm volatile("int3");
#endif
    }
}

int main(int argc, char *argv[])
{
    int shouldExecuteInt3 = (argc > 1 && strcmp(argv[1],  "int3") == 0);
    int arg, i = 0, s = 0;
    for(arg=1; arg<argc; arg++)
        printf("\t> argv[%d] = %s\n", arg, argv[arg]);

    myStart();

    for(i=0; i<1000000; i++)
    {
        s += (s*i + 1000)&0xFF - (s*2*i + 7)&0xFF;
        foo();
    }

    executeInt3(shouldExecuteInt3);

    myEnd();

    printf("result = 0x%x\n", s);

    return 0;
}
