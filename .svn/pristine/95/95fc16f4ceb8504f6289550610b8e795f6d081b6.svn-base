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

#include "include/taint_source_func.h"

/**
 * This class taints the return value of a function. This application
 * is designed for GCC compiler for x86 architecture and hence we are
 * assuming that the code has been compiled using CDECL calling convention.
 * It is not possible to provide a generic function that taints the 
 * return value of a function as the way values are returned from a 
 * function varies a lot. The following assumptions are made regarding
 * the function return value 
 * (http://www.angelcode.com/dev/callconv/callconv.html)
 *
 * 1. Primitive data types, except floating point values, are returned in 
 * EAX or EAX:EDX depending on the size.
 * 2. float and double are returned in fp0, i.e. the first floating point
 * register.
 * 3. All structures and classes are returned in memory regardless of 
 * complexity or size.
 * 4. When a return is made in memory the caller passes a pointer to 
 * the memory location as the first parameter (hidden). The callee
 * populates the memory, and returns the pointer. The callee pops 
 * the hidden pointer from the stack when returning.
 * 5. Classes that have a destructor are always passed by reference, 
 * even if the parameter is defined to be by value. 
 */

VOID ImageLoad(IMG img, VOID *v);

FunctionTaintSource::FunctionTaintSource()
{
    IMG_AddInstrumentFunction(ImageLoad, (VOID *)this);
}

FunctionTaintSource::~FunctionTaintSource()
{
}

void FunctionTaintSource::addFunctionSource(string funcName, TaintGenerator *gen)
{
    //add function name to vector 
    functions.push_back(funcName);
}	

/*
// Called every time a new image is loaded
// Look for routines that we want to probe
VOID FunctionTaintSource::ImageLoad(IMG img, VOID *v)
{
    vector<string>::iterator iter = functions.begin();
    while ( iter != functions.end()) {
        RTN rout = RTN_FindByName(img, *iter);
        if (RTN_Valid(rout)) {
            // do something
        }
        iter++;
    }
}*/

void taint_return_val() {
}

// Called every time a new image is loaded
// Look for routines that we want to probe
VOID ImageLoad(IMG img, VOID *v)
{
    FunctionTaintSource *myself = static_cast<FunctionTaintSource *>(v);
    vector<string>::iterator iter = myself->functions.begin();
    while ( iter != myself->functions.end()) {
        string s = *iter;
        RTN rout = RTN_FindByName(img, (CHAR *)s.c_str());
        if (RTN_Valid(rout)) {
            RTN_InsertCall(rout, IPOINT_AFTER, AFUNPTR(taint_return_val));
        }
        iter++;
    }
}


