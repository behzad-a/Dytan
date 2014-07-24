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
#include "context_utils.h"
#include "regvalues.h"


/////////////////////
// API FUNCTIONS IMPLEMENTATION
/////////////////////

void StoreContext(const CONTEXT * ctxt)
{
    PIN_GetContextRegval(ctxt, GPRREG, &gprregval);
#if defined(TARGET_IA32E) || defined(TARGET_MIC)
    PIN_GetContextRegval(ctxt, GPR32REG, &gpr32regval);
#endif
    PIN_GetContextRegval(ctxt, GPR16REG, &gpr16regval);
    PIN_GetContextRegval(ctxt, GPRLREG, &gprlregval);
    PIN_GetContextRegval(ctxt, GPRHREG, &gprhregval);
    PIN_GetContextRegval(ctxt, STREG, &stregval);
#ifdef TARGET_MIC
    PIN_GetContextRegval(ctxt, ZMMREG, &zmmregval);
    PIN_GetContextRegval(ctxt, KREG, &kregval);
#else
    PIN_GetContextRegval(ctxt, XMMREG, &xmmregval);
    if (hasAvxSupport)
    {
        PIN_GetContextRegval(ctxt, YMMREG, &ymmregval);
    }
#endif // not TARGET_MIC
}

void ModifyContext(CONTEXT * ctxt)
{
    PIN_AssignRegval(&gprregval, reinterpret_cast<const UINT64*>(tgprval), GPRBITSIZE);
    PIN_SetContextRegval(ctxt, GPRREG, &gprregval);
    PIN_AssignRegval(&stregval, reinterpret_cast<const UINT64*>(tstval), STBITSIZE);
    PIN_SetContextRegval(ctxt, STREG, &stregval);
#ifdef TARGET_MIC
    PIN_AssignRegval(&zmmregval, reinterpret_cast<const UINT64*>(tzmmval), ZMMBITSIZE);
    PIN_SetContextRegval(ctxt, ZMMREG, &zmmregval);
    PIN_AssignRegval(&kregval, reinterpret_cast<const UINT64*>(tkval), KBITSIZE);
    PIN_SetContextRegval(ctxt, KREG, &kregval);
#else
    PIN_AssignRegval(&xmmregval, reinterpret_cast<const UINT64*>(txmmval), XMMBITSIZE);
    PIN_SetContextRegval(ctxt, XMMREG, &xmmregval);
    if (hasAvxSupport)
    {
        PIN_AssignRegval(&ymmregval, reinterpret_cast<const UINT64*>(tymmval), YMMBITSIZE);
        PIN_SetContextRegval(ctxt, YMMREG, &ymmregval);
    }
#endif // not TARGET_MIC
}
