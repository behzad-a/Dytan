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
#ifndef REGVALUE_UTILS_H
#define REGVALUE_UTILS_H


/////////////////////
// INCLUDES
/////////////////////

#include <ostream>
#include <cassert>
#include <string>
#include "pin.H"

using std::ostream;
using std::endl;
using std::flush;
using std::hex;
using std::string;


/////////////////////
// GLOBAL VARIABLES
/////////////////////

// Registers to Check:
//
// These are the Pin-enumerated registers to be checked.
extern const REG GPRREG;
#if defined(TARGET_IA32E) || defined(TARGET_MIC)
extern const REG GPR32REG;
#endif
extern const REG GPR16REG;
extern const REG GPRLREG;
extern const REG GPRHREG;
extern const REG STREG;
#ifdef TARGET_MIC
extern const REG ZMMREG;
extern const REG KREG;
#else
extern const REG XMMREG;
extern const REG YMMREG;
#endif // not TARGET_MIC

// Application Registers:
//
// The application register values will be stored in these variables by the tools.
// They will be stored and printed before and after the application changes them.
// In addition, after the application changes the registers, the values will be
// compared to the expected values.
extern REGVAL gprregval;
#if defined(TARGET_IA32E) || defined(TARGET_MIC)
extern REGVAL gpr32regval;
#endif
extern REGVAL gpr16regval;
extern REGVAL gprlregval;
extern REGVAL gprhregval;
extern REGVAL stregval;
#ifdef TARGET_MIC
extern REGVAL zmmregval;
extern REGVAL kregval;
#else
extern REGVAL xmmregval;
extern REGVAL ymmregval;
#endif // not TARGET_MIC

// Boolean indicating whether the system supports avx instructions and registers.
extern const bool hasAvxSupport;


/////////////////////
// FUNCTION DECLARATIONS
/////////////////////

// Prints a hex representation of the value held in regval.
void PrintRawValue(const REGVAL * regval, ostream& ost);

// Print all the application registers (see definition above).
void PrintStoredRegisters(ostream& ost);

// Get a REGSET with all the tested registers.
REGSET GetTestRegset();

// Compare the application registers with the expected values.
bool CheckAllExpectedValues(ostream& ost);

#ifdef TARGET_MIC
bool CheckExpectedValues(const unsigned char * gprptr, const unsigned char * stptr,
                         const unsigned char * zmmptr, const unsigned char * kptr, ostream& ost);
#else // not TARGET_MIC
bool CheckExpectedValues(const unsigned char * gprptr, const unsigned char * stptr,
                         const unsigned char * xmmptr, const unsigned char * ymmptr, ostream& ost);
#endif // not TARGET_MIC

#endif // REGVALUE_UTILS_H
