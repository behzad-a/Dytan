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
#include "regvalue_utils.h"
#include "regvalues.h"


/////////////////////
// EXTERNAL FUNCTIONS
/////////////////////

extern "C" bool ProcessorSupportsAvx();


/////////////////////
// GLOBAL VARIABLES
/////////////////////

// Registers to Check:
//
// These are the Pin-enumerated registers to be checked.
const REG GPRREG = REG_GBX;
#if defined(TARGET_IA32E) || defined(TARGET_MIC)
const REG GPR32REG = REG_EBX;
#endif
const REG GPR16REG = REG_BX;
const REG GPRLREG = REG_BL;
const REG GPRHREG = REG_BH;
const REG STREG = REG_ST2;
#ifdef TARGET_MIC
const REG ZMMREG = REG_ZMM5;
const REG KREG = REG_K3;
#else
const REG XMMREG = REG_XMM0;
const REG YMMREG = REG_YMM1;
#endif // not TARGET_MIC

// Application Registers:
//
// The application register values will be stored in these variables by the tools.
// They will be stored and printed before and after the application changes them.
// In addition, after the application changes the registers, the values will be
// compared to the expected values.
REGVAL gprregval;
#if defined(TARGET_IA32E) || defined(TARGET_MIC)
REGVAL gpr32regval;
#endif
REGVAL gpr16regval;
REGVAL gprlregval;
REGVAL gprhregval;
REGVAL stregval;
#ifdef TARGET_MIC
REGVAL zmmregval;
REGVAL kregval;
#else
REGVAL xmmregval;
REGVAL ymmregval;
#endif // not TARGET_MIC

// Boolean indicating whether the system supports avx instructions and registers.
const bool hasAvxSupport = ProcessorSupportsAvx();


/////////////////////
// INTERNAL FUNCTIONS IMPLEMENTATION
/////////////////////

static inline UINT Bits2Uint64(UINT bits)
{
    assert(bits%8 == 0);
    UINT res = bits >> 6;
    if (bits%64) ++res;
    return res;
}

static inline void PrintRegval(REG reg, const REGVAL * regval, ostream& ost)
{
    ost << REG_StringShort(reg) << " = ";
    PrintRawValue(regval, ost);
}

static string Val2Str(const unsigned char * data, UINT bytes)
{
    string str(bytes<<1,0);
    UINT s = 0;
    // each byte in data translates to two chars (s) in the string
    while (bytes)
    {
        --bytes;
        char c = (data[bytes] & 0xf0) >> 4;
        str[s++] = (c>9) ? c-10+'a' : c+'0';
        c = data[bytes] & 0x0f;
        str[s++] = (c>9) ? c-10+'a' : c+'0';
    }
    return str;
}

template<typename SIZETYPE>
static bool CompareSizedWord(const unsigned char * value, const unsigned char * expected, UINT element,
                             UINT totalSize, ostream& ost)
{
    if (*((SIZETYPE*)(&value[element << 3])) != *((SIZETYPE*)(&expected[element << 3])))
    {
        ost << "WARNING: Expected value: 0x" << Val2Str(expected, totalSize) << endl << flush;
        ost << "WARNING: Received value: 0x" << Val2Str(value, totalSize) << endl << flush;
        return false;
    }
    return true;
}

static bool CompareValues(const unsigned char * value, const unsigned char * expected, UINT origBytes, ostream& ost)
{
    for (UINT bytes = origBytes, i = 0; bytes > 0; bytes -= 8, ++i)
    {
        if (bytes == 1) return CompareSizedWord<UINT8>(value, expected, i, origBytes, ost);
        if (bytes == 2) return CompareSizedWord<UINT16>(value, expected, i, origBytes, ost);
        if (bytes == 4) return CompareSizedWord<UINT32>(value, expected, i, origBytes, ost);
        if (bytes >= 8)
        {
            if (!CompareSizedWord<UINT64>(value, expected, i, origBytes, ost))
            {
                return false;
            }
        }
        else
        {
            ost << "ERROR: Unexpected number of bytes left: " << bytes << endl << flush;
            assert(0);
        }
    }
    return true;
}

static void FillBufferFromRegval(unsigned char * value, const REGVAL * regval, UINT bytes)
{
    UINT size = bytes >> 3; // number of UINT64 elements
    if (bytes % 8) ++ size;
    for (UINT i = 0; i < size; ++i)
    {
        // The last 64-bit chunk may be larger than the actual register size, for example with an 80-bit register.
        // This is fine since PIN_ReadRegValueQWord zero-extends to a 64-bit chunk and the 'value' buffer is a
        // multiple of UINT64.
        PIN_ReadRegvalQWord(regval, &(((UINT64*)value)[i]), i);
    }
}

static bool CheckRegval(const REGVAL * regval, const unsigned char * expected, UINT size, ostream& ost)
{
    // MAXSIZE is a multiple of UINT64. This is important because we use PIN_ReadRegValueQWord to fill the buffer.
    unsigned char value[MAXSIZE];
    UINT regvalSize = PIN_GetRegvalSize(regval);
    assert((regvalSize % 8) == 0);
    assert((regvalSize >> 3) == size);
    FillBufferFromRegval(value, regval, size);
    return CompareValues(value, expected, size, ost);
}


/////////////////////
// API FUNCTIONS IMPLEMENTATION
/////////////////////

void PrintRawValue(const REGVAL * regval, ostream& ost)
{
    ost << "0x";
    UINT qw = Bits2Uint64(PIN_GetRegvalSize(regval)); // number of UINT64 elements
    while (qw)
    {
        --qw;
        UINT64 val;
        PIN_ReadRegvalQWord(regval, &val, qw);
        ost << hex << val;
    }
    ost << endl << flush;
}

void PrintStoredRegisters(ostream& ost)
{
    PrintRegval(GPRREG, &gprregval, ost);
#if defined(TARGET_IA32E) || defined(TARGET_MIC)
    PrintRegval(GPR32REG, &gpr32regval, ost);
#endif
    PrintRegval(GPR16REG, &gpr16regval, ost);
    PrintRegval(GPRLREG, &gprlregval, ost);
    PrintRegval(GPRHREG, &gprhregval, ost);
    PrintRegval(STREG, &stregval, ost);
#ifdef TARGET_MIC
    PrintRegval(ZMMREG, &zmmregval, ost);
    PrintRegval(KREG, &kregval, ost);
#else
    PrintRegval(XMMREG, &xmmregval, ost);
    if (hasAvxSupport)
    {
        PrintRegval(YMMREG, &ymmregval, ost);
    }
#endif // not TARGET_MIC
}

REGSET GetTestRegset()
{
    static REGSET regset;
    if (REGSET_PopCountIsZero(regset))
    {
        REGSET_Clear(regset);
        REGSET_Insert(regset, GPRREG);
#if defined(TARGET_IA32E) || defined(TARGET_MIC)
        REGSET_Insert(regset, GPR32REG);
#endif
        REGSET_Insert(regset, GPR16REG);
        REGSET_Insert(regset, GPRLREG);
        REGSET_Insert(regset, GPRHREG);
        REGSET_Insert(regset, STREG);
#ifdef TARGET_MIC
        REGSET_Insert(regset, ZMMREG);
        REGSET_Insert(regset, KREG);
#else
        REGSET_Insert(regset, XMMREG);
        if (hasAvxSupport)
            REGSET_Insert(regset, YMMREG);
#endif
    }
    return regset;
}

bool CheckAllExpectedValues(ostream& ost)
{
    bool success = true;
    success &= CheckRegval(&gprregval, gprval, GPRSIZE, ost);
#if defined(TARGET_IA32E) || defined(TARGET_MIC)
    success &= CheckRegval(&gpr32regval, gpr32val, GPR32SIZE, ost);
#endif
    success &= CheckRegval(&gpr16regval, gpr16val, 2, ost);
    success &= CheckRegval(&gprlregval, gprlval, 1, ost);
    success &= CheckRegval(&gprhregval, gprhval, 1, ost);
    success &= CheckRegval(&stregval, stval, STSIZE, ost);
#ifdef TARGET_MIC
    success &= CheckRegval(&zmmregval, zmmval, ZMMSIZE, ost);
    success &= CheckRegval(&kregval, kval, KSIZE, ost);
#else
    success &= CheckRegval(&xmmregval, xmmval, XMMSIZE, ost);
    if (hasAvxSupport)
    {
        success &= CheckRegval(&ymmregval, ymmval, YMMSIZE, ost);
    }
#endif // not TARGET_MIC
    return success;
}

#ifdef TARGET_MIC
bool CheckExpectedValues(const unsigned char * gprptr, const unsigned char * stptr,
                         const unsigned char * zmmptr, const unsigned char * kptr,
                         ostream& ost)
{
    bool success = true;
    success &= CompareValues(gprptr, tgprval, GPRSIZE, ost);
    success &= CompareValues(stptr, tstval, STSIZE, ost);
    success &= CompareValues(zmmptr, tzmmval, ZMMSIZE, ost);
    success &= CompareValues(kptr, tkval, KSIZE, ost);
    return success;
}

#else // not TARGET_MIC

bool CheckExpectedValues(const unsigned char * gprptr, const unsigned char * stptr,
                         const unsigned char * xmmptr, const unsigned char * ymmptr,
                         ostream& ost)
{
    bool success = true;
    success &= CompareValues(gprptr, tgprval, GPRSIZE, ost);
    success &= CompareValues(stptr, tstval, STSIZE, ost);
    success &= CompareValues(xmmptr, txmmval, XMMSIZE, ost);
    if (hasAvxSupport)
    {
        success &= CompareValues(ymmptr, tymmval, YMMSIZE, ost);
    }
    return success;
}

#endif // not TARGET_MIC
