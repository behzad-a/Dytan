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

#include <stdio.h>
#include <stdlib.h>

#ifndef TARGET_WINDOWS
#include <syscall.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <linux/net.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

#include <map>

#include "pin.H"
#include "include/bitset.h"
#include "include/syscall_monitor.H"
#include "include/syscall_functions.h"
#include "include/replace_functions.h"
#include "include/RoutineGraph.H"
#include "include/BasicBlock.H"

using namespace XED;

FILE *log;
FILE *taintAssignmentLog;

/* the maximum allowable number of taint mark */
/* currently the assignment of taint marks will wrap around */
#define NUMBER_OF_TAINT_MARKS 4096
int currentTaintMark = 0;

/* controls if detailed logging is used */
//#define TRACE 1

/*
  controls if registers are considered to propagate taint if they're
   used to access memory.  For example: load [%eax], %eax will propagate
   taint if IMPLICIT is defined and it won't if IMPLICIT is not defined
*/
#define IMPLICIT 1

/*
  map that stores taint marks for registers
*/
map<REG, bitset *> regTaintMap;
/*
  map that stores taint marks for memory address, currently this is
  per byte
*/
map<ADDRINT, bitset *> memTaintMap;

/*
  map that stores taint marks active due to control flow
 */
map<ADDRINT, bitset *> controlTaintMap;


typedef void (*InstrumentFunction)(INS ins, void *v);
InstrumentFunction instrument_functions[XEDICLASS_LAST];

/* global storage to hold taint marks */
bitset *dest;
bitset *src;
bitset *eax;
bitset *edx;
bitset *base;
bitset *idx;
bitset *eflags;
bitset *cnt;

/* finer grained control for when logging should happen */
bool tracing;

/*
  Dumps the instruction to the log file
*/
void Print(ADDRINT address, string *disas)
{
  if(tracing) {
    PIN_LockClient();
    RTN rtn = RTN_FindByAddress(address);
    PIN_UnlockClient();
    
    fprintf(log, "%#x: %s [%s]\n", address, disas->c_str(),
	    RTN_Valid(rtn) ? RTN_Name(rtn).c_str(): "");
    fflush(log);
  }
}

void ClearTaintSet(bitset *set)
{
  bitset_reset(set);
}

/* copies the taint marks for the register into the out bitset parameter */
void TaintForRegister(REG reg, bitset *set)
{
  map<REG, bitset *>::iterator iter = regTaintMap.find(reg);
  if(regTaintMap.end() != iter) {
    bitset_set_bits(set, iter->second);
  }
  else {
    // this is important becuase we use global storage it's possible that
    // set will already have values
    bitset_reset(set);
  }

#ifdef TRACE
  if(tracing) {
    char *sep = "";
    if(REG_valid(reg)) {
      fprintf(log, "\t-%s[", REG_StringShort(reg).c_str());
      for(int i = 0; i < set->nbits; i++) {
	if(bitset_test_bit(set, i)) {
	  fprintf(log, "%s%d", sep, i);
	  sep = ", ";
	}
      }
      fprintf(log, "]\n"); 
    }
  }
#endif

}

/* Return in the out parameter, set, the union of the taint marks
   from memory address start to start + size - 1, and if IMPLICIT is
   defined the taint marks for the base and index registers used to
   access memory
*/
void TaintForMemory(ADDRINT start, ADDRINT size,
		    REG baseReg, REG indexReg,
		    bitset *set)
{

  // need to clear out set incase there are preexisting values
  bitset_reset(set);

  for(ADDRINT addr = start; addr < start + size; addr++) {
    map<ADDRINT, bitset *>::iterator iter = memTaintMap.find(addr);
    if(memTaintMap.end() != iter) {
      bitset_union(set, iter->second);  
    }
  }

#ifdef TRACE
  char *sep = "";
  if(tracing) {
    fprintf(log, "\t-%#x-%#x[", start, start + size - 1);
    for(int i = 0; i < set->nbits; i++) {
      if(bitset_test_bit(set, i)) {
	
	fprintf(log, "%s%d", sep, i);
	sep = ", ";
      }
    }
    fprintf(log, "]"); 
  }
#endif

#ifdef IMPLICIT
  if(REG_valid(baseReg)) {
    map<REG, bitset *>::iterator iter = regTaintMap.find(baseReg);
    if(regTaintMap.end() != iter) {
      bitset_union(set, iter->second);
      
#ifdef TRACE
      if(tracing) {
	sep = "";
	fprintf(log, ", %s[", REG_StringShort(baseReg).c_str());
	for(int i = 0; i < iter->second->nbits; i++) {
	  if(bitset_test_bit(iter->second,  i)) {
	    fprintf(log, "%s%d", sep, i);
	    sep =", ";
	  }
	}
	fprintf(log, "]"); 
      }
#endif     
      
    }
  }
  
  if(REG_valid(indexReg)) {
    map<REG, bitset *>::iterator iter = regTaintMap.find(indexReg);
    if(regTaintMap.end() != iter) {
      bitset_union(set, iter->second);

#ifdef TRACE
      if(tracing) {
	sep = "";
	fprintf(log, ", %s[", REG_StringShort(indexReg).c_str());
	for(int i = 0; i < iter->second->nbits; i++) {
	  if(bitset_test_bit(iter->second, i)) {
	    fprintf(log, "%s%d", sep, i);
	    sep = ", ";
	  }
	}
	fprintf(log, "]"); 
      }
#endif

    }
  }
#endif

#ifdef TRACE
  if(tracing) {
    fprintf(log, "\n");
  }
#endif
  
}

/* 
   sets the taint marks associated with the dest register to the union
   of the bitsets passed in the varargs parameter
 */
void SetTaintForRegister(REG dest, ...)
{
  va_list ap;
  bitset *src;

  if(LEVEL_BASE::REG_ESP == dest ||
     LEVEL_BASE::REG_EBP == dest) return;

  bitset *tmp = bitset_init(NUMBER_OF_TAINT_MARKS);

  va_start(ap, dest);
  
  while((src = va_arg(ap, bitset *)) != NULL) {
    bitset_union(tmp, src);
  }
  
  va_end(ap);

  // control flow
  bitset *controlTaint = bitset_init(NUMBER_OF_TAINT_MARKS);
  for(map<ADDRINT, bitset *>::iterator iter = controlTaintMap.begin();
      iter != controlTaintMap.end(); iter++) {
    bitset_union(controlTaint, iter->second);
  }
  bitset_union(tmp, controlTaint);

  /* This is where we account for subregisters */
  /*
    This isn't totally complete yet.  For example edi and esi are not
     included and setting [A|B|C|D]X won't set the super or subregisters
  */

  //eax
  if(LEVEL_BASE::REG_EAX == dest) {
    //ax
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_AX)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_AX], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_AX] = bitset_copy(tmp);
    }
    
    //al
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_AH)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_AH], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_AH] = bitset_copy(tmp);
    }
    
    //ah
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_AL)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_AL], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_AL] = bitset_copy(tmp);
    }
  }
  
  //ebx
  else if(LEVEL_BASE::REG_EBX == dest) {
    //bx
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_BX)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_BX], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_BX] = bitset_copy(tmp);
    }
    
    //bl
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_BH)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_BH], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_BH] = bitset_copy(tmp);
    }
    
    //bh
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_BL)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_BL], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_BL] = bitset_copy(tmp);
    }
  }

  //ecx
  else if(LEVEL_BASE::REG_ECX == dest) {
    //cx
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_CX)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_CX], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_CX] = bitset_copy(tmp);
    }
    
    //cl
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_CH)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_CH], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_CH] = bitset_copy(tmp);
    }
    
    //ch
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_CL)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_CL], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_CL] = bitset_copy(tmp);
    }
  }

  //edx
  else if(LEVEL_BASE::REG_EDX == dest) {
    //dx
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_DX)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_DX], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_DX] = bitset_copy(tmp);
    }
    
    //dl
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_DH)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_DH], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_DH] = bitset_copy(tmp);
    }
    
    //dh
    if(regTaintMap.end() != regTaintMap.find(LEVEL_BASE::REG_DL)) {
      bitset_set_bits(regTaintMap[LEVEL_BASE::REG_DL], tmp);
    }
    else {
      regTaintMap[LEVEL_BASE::REG_DL] = bitset_copy(tmp);
    }
  }


  if(regTaintMap.end() != regTaintMap.find(dest)) {
    bitset_set_bits(regTaintMap[dest], tmp);
  }
  else {
    regTaintMap[dest] = bitset_copy(tmp);
  }

#ifdef TRACE
  if(tracing) {
    char  *sep = "";
    fprintf(log, "\t%s[", REG_StringShort(dest).c_str());
    bitset *set = regTaintMap[dest];
    for(int i = 0; i < set->nbits; i++) {
      if(bitset_test_bit(set, i)) {
	fprintf(log, "%s%d", sep, i);
	sep =", ";
      }
    }
    fprintf(log, "] <- cf["); 

    sep = "";
    for(int i = 0; i < controlTaint->nbits; i++) {
      if(bitset_test_bit(controlTaint, i)) {
	fprintf(log, "%s%d", sep, i);
	sep = ", ";
      }
    }
    fprintf(log, "]\n");
  }
  
#endif

  bitset_free(tmp);
  bitset_free(controlTaint);
}

/* Clears the taint marks associated with a register */
void ClearTaintForRegister(REG reg)
{
  // control flow
  bitset *controlTaint = bitset_init(NUMBER_OF_TAINT_MARKS);
  for(map<ADDRINT, bitset *>::iterator iter = controlTaintMap.begin();
      iter != controlTaintMap.end(); iter++) {
    bitset_union(controlTaint, iter->second);
  }

  map<REG, bitset *>::iterator iter = regTaintMap.find(reg);
  if(regTaintMap.end() != iter) {
    bitset_set_bits(iter->second, controlTaint);
  }


#ifdef TRACE
  if(tracing) {
    char *sep = "";
    fprintf(log, "\t%s <- cf[", REG_StringShort(reg).c_str());

    sep = "";
    for(int i = 0; i < controlTaint->nbits; i++) {
      if(bitset_test_bit(controlTaint, i)) {
	fprintf(log, "%s%d", sep, i);
	sep = ", ";
      }
    }
    fprintf(log, "]\n");
  }
#endif

  bitset_free(controlTaint);
}

/* Clears taint marks associted with the range of memory */
void ClearTaintForMemory(ADDRINT start, ADDRINT size)
{
  // control flow
  bitset *controlTaint = bitset_init(NUMBER_OF_TAINT_MARKS);
  for(map<ADDRINT, bitset *>::iterator iter = controlTaintMap.begin();
      iter != controlTaintMap.end(); iter++) {
    bitset_union(controlTaint, iter->second);
  }

  for(ADDRINT addr = start; addr < start + size; addr++) {
    map<ADDRINT, bitset *>::iterator iter = memTaintMap.find(addr);
    if(memTaintMap.end() != iter) {
      bitset_set_bits(iter->second, controlTaint);
      
    }
  }

#ifdef TRACE
  if(tracing) {
    char *sep = "";
    fprintf(log, "\t%#x-%#x <- cf[", start, start + size - 1);

    sep = "";
    for(int i = 0; i < controlTaint->nbits; i++) {
      if(bitset_test_bit(controlTaint, i)) {
	fprintf(log, "%s%d", sep, i);
	sep = ", ";
      }
    }
    fprintf(log, "]\n");
  }
#endif
  
  bitset_free(controlTaint);
}

/* Set the taint marks associated with the memory range to the union
   of the bitsets passed in the varargs parameter
*/
void SetTaintForMemory(ADDRINT start, ADDRINT size, ...)
{
  va_list ap;
  bitset *src;

  bitset *tmp = bitset_init(NUMBER_OF_TAINT_MARKS);
  
  va_start(ap, size);
  
  while((src = va_arg(ap, bitset *)) != NULL) {
    bitset_union(tmp, src);
  }
  
  va_end(ap);

  // control flow
  bitset *controlTaint = bitset_init(NUMBER_OF_TAINT_MARKS);

  for(map<ADDRINT, bitset *>::iterator iter = controlTaintMap.begin();
      iter != controlTaintMap.end(); iter++) {
    bitset_union(controlTaint, iter->second);
  }
  bitset_union(tmp, controlTaint);


  for(ADDRINT addr = start; addr < start + size; addr++) {
    if(memTaintMap.end() != memTaintMap.find(addr)) {
      bitset_set_bits(memTaintMap[addr], tmp);
    }
    else {
      memTaintMap[addr] = bitset_copy(tmp);
    }
  }
  
#ifdef TRACE
  if(tracing) {
    char *sep = "";
    fprintf(log, "\t%#x-%#x[", start, start + size - 1);
    for(int i = 0; i < tmp->nbits; i++) {
      if(bitset_test_bit(tmp, i)) {
	fprintf(log, "%s%d", sep, i);
      sep = ", ";
      }
    }
    fprintf(log, "] <- cf[");

    sep = "";
    for(int i = 0; i < controlTaint->nbits; i++) {
      if(bitset_test_bit(controlTaint, i)) {
	fprintf(log, "%s%d", sep, i);
	sep = ", ";
      }
    }
    fprintf(log, "]\n");
  }
#endif
  
  bitset_free(tmp);
  bitset_free(controlTaint);
}

#include "instrument_functions.c"

void PushControl(ADDRINT addr)
{
#ifdef TRACE
  if(tracing) {
    fprintf(log, "\tpush control: %#x\n", addr);
    fflush(log);
  }
#endif

  if(regTaintMap.end() == regTaintMap.find(LEVEL_BASE::REG_EFLAGS) ||
     bitset_is_empty(regTaintMap[LEVEL_BASE::REG_EFLAGS])) return;

  if(controlTaintMap.end() == controlTaintMap.find(addr)) {
    controlTaintMap[addr] = bitset_init(NUMBER_OF_TAINT_MARKS);
  }

  bitset_union(controlTaintMap[addr], regTaintMap[LEVEL_BASE::REG_EFLAGS]);

  //dump control taint map
#ifdef TRACE
  if(tracing) {
    for(map<ADDRINT, bitset *>::iterator iter = controlTaintMap.begin();
	iter != controlTaintMap.end(); iter++) {
      
      char *sep = "";
      fprintf(log, "\t\t-%#x - [", iter->first);
      for(int i = 0; i < iter->second->nbits; i++) {
	if(bitset_test_bit(iter->second, i)) {
	  fprintf(log, "%s%d", sep, i);
	  sep = ", ";
	}
      }
      fprintf(log, "]\n");
    }
  }
#endif
}

void PopControl(int n, ...)
{
#ifdef TRACE
  if(tracing) {
    fprintf(log, "\tpop control: ");
  }
#endif

  va_list ap;
  ADDRINT addr;
  char *sep = "";

  va_start(ap, n);
  
  for (; n; n--) {
    
    addr = va_arg(ap, ADDRINT);

#ifdef TRACE
    if(tracing) {
      fprintf(log, "%s%#x\n", sep, addr);
      fflush(log);
      sep = ", ";
    }
#endif

    if(controlTaintMap.end() == controlTaintMap.find(addr)) return;
    
    bitset *s = controlTaintMap[addr];
    bitset_free(s);
    
    controlTaintMap.erase(addr);
  }
  va_end(ap);

#ifdef TRACE
  if(tracing) {
    fprintf(log, "\n");
    fflush(log);
  }
#endif

  // dump control taint map
#ifdef TRACE
  if(tracing) {
    for(map<ADDRINT, bitset *>::iterator iter = controlTaintMap.begin();
	iter != controlTaintMap.end(); iter++) {
      
      sep = "";
      fprintf(log, "\t\t%#x - [", iter->first);
      for(int i = 0; i < iter->second->nbits; i++) {
      if(bitset_test_bit(iter->second, i)) {
	fprintf(log, "%s%d", sep, i);
	sep = ", ";
      }
      }
      fprintf(log, "]\n");
    }
  }
#endif
}

static void Controlflow(RTN rtn, void *v)
{
  string rtn_name = RTN_Name(rtn);

  IMG img = SEC_Img(RTN_Sec(rtn));

  if(LEVEL_CORE::IMG_TYPE_SHAREDLIB == IMG_Type(img)) return;

  RTN_Open(rtn);

  RoutineGraph *rtnGraph = new RoutineGraph(rtn);

  map<ADDRINT, set<ADDRINT> > controls;

  for(INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {

    if(XED_CATEGORY_COND_BR == INS_Category(ins)) {

      ADDRINT addr = INS_Address(ins);
      BasicBlock *block = rtnGraph->addressMap[addr];
      if(NULL == block) {
	printf("block is null\n");
	fflush(stdout);
      }

      BasicBlock *ipdomBlock = block->getPostDominator();
      if(NULL == ipdomBlock) {
	printf("ipdomBlock is null in %s\n", rtn_name.c_str());
	fflush(stdout);
      }
      ADDRINT ipdomAddr = ipdomBlock->startingAddress;
   
      if(controls.find(ipdomAddr) == controls.end()) {
	controls[ipdomAddr] = set<ADDRINT>();
      }

      controls[ipdomAddr].insert(addr);

      //      printf("placing push call: %#x - %s\n", addr,
      //     INS_Disassemble(ins).c_str());
      
      INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(PushControl),
		     IARG_PTR, addr,
		     IARG_END);
    }
  }
  
  for(INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins)) {

    ADDRINT addr = INS_Address(ins);
   
    if(controls.end() == controls.find(addr)) continue;

    IARGLIST args = IARGLIST_Alloc();

    for(set<ADDRINT>::iterator iter = controls[addr].begin();
	iter != controls[addr].end(); iter++) {
      IARGLIST_AddArguments(args, IARG_ADDRINT, *iter, IARG_END);
      //      printf("\t%#x\n", *iter);
    }
      

    INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(PopControl),
		   IARG_UINT32, controls[addr].size(),
		   IARG_IARGLIST, args,
		   IARG_END);
    IARGLIST_Free(args);
  }
  
  delete rtnGraph;

  RTN_Close(rtn);
}

static void Dataflow(INS ins, void *v)
{
  xed_iclass_t opcode = (xed_iclass_t) INS_Opcode(ins);

  (*instrument_functions[opcode])(ins, v);
}

static void Trace(INS ins, void *v)
{
  INS_InsertCall(ins, IPOINT_BEFORE, AFUNPTR(Print),
		 IARG_ADDRINT, INS_Address(ins),
		 IARG_PTR, new string(INS_Disassemble(ins)),
		 IARG_END);
}

void TaintMainArgs(ADDRINT *argcAddr, ADDRINT *argvAddr)
{
  int argc = (int) *argcAddr;

  bitset *tmp = bitset_init(NUMBER_OF_TAINT_MARKS);
  bitset_set_bit(tmp, currentTaintMark);
 
  for(ADDRINT addr = (ADDRINT) argcAddr;
      addr < (ADDRINT) argcAddr + sizeof(int); addr++) {
    memTaintMap[addr] = bitset_copy(tmp);
  }
  
#ifdef TRACE
  char *sep = "";
  fprintf(log, "@%#x-%#x[", argcAddr, (ADDRINT) argcAddr + sizeof(int) - 1);
  for(int i = 0; i < tmp->nbits; i++) {
    if(bitset_test_bit(tmp, i)) {
      fprintf(log, "%s%d", sep, i);
      sep = ", ";
    }
  }
  fprintf(log, "]\n");
#endif
  
  fprintf(taintAssignmentLog, "%d - argc ->%#x-%#x\n",
	  currentTaintMark, 
	  (ADDRINT) argcAddr,
	  (ADDRINT) argcAddr + sizeof(int) - 1);
  fflush(taintAssignmentLog);
  
  currentTaintMark++;
  
  bitset_free(tmp);
  
  
  char **argv = (char **) *argvAddr;
  
  for(int i = 0; i < argc; i++) {
    bitset *s = bitset_init(NUMBER_OF_TAINT_MARKS);
    bitset_set_bit(s, currentTaintMark);
    
    for(ADDRINT addr = (ADDRINT) &argv[i];
	addr < (ADDRINT) argv[i] + strlen(argv[i]) + 1; addr++) {
      memTaintMap[addr] = bitset_copy(s);
    }
    
#ifdef TRACE
    sep = "";
    fprintf(log, "@%#x-%#x[", argv[i], (ADDRINT) argv[i] + strlen(argv[i]));
    for(int j = 0; j < s->nbits; j++) {
      if(bitset_test_bit(s, j)) {
	fprintf(log, "%s%d", sep, j);
	sep = ", ";
      }
    }
    fprintf(log, "]\n");
#endif
    
    fprintf(taintAssignmentLog, "%d - argv[%d] ->%#x-%#x\n",
	    currentTaintMark, 
	    i,
	    (ADDRINT) argcAddr,
	    (ADDRINT) argv[i] + strlen(argv[i]));
    fflush(taintAssignmentLog);
    
    currentTaintMark++;
    
    bitset_free(s);
  }
}

static void WrapMain(RTN rtn, void *v)
{
  string rtn_name = RTN_Name(rtn);

  if("main" != rtn_name) return;

  RTN_Open(rtn);
  RTN_InsertCall(rtn, IPOINT_BEFORE, AFUNPTR(TaintMainArgs),
		 IARG_FUNCARG_ENTRYPOINT_REFERENCE, 0,
		 IARG_FUNCARG_ENTRYPOINT_REFERENCE, 1,
		 IARG_END);
  RTN_Close(rtn);
}

int main(int argc, char* argv[])
{
  PIN_InitSymbols();
  PIN_Init(argc, argv);

  log = fopen("out.log", "w");
  taintAssignmentLog = fopen("taint-log.log", "w");

  dest = bitset_init(NUMBER_OF_TAINT_MARKS);
  src = bitset_init(NUMBER_OF_TAINT_MARKS);
  eax = bitset_init(NUMBER_OF_TAINT_MARKS);
  edx = bitset_init(NUMBER_OF_TAINT_MARKS);
  base = bitset_init(NUMBER_OF_TAINT_MARKS);
  idx = bitset_init(NUMBER_OF_TAINT_MARKS);
  eflags = bitset_init(NUMBER_OF_TAINT_MARKS);
  cnt = bitset_init(NUMBER_OF_TAINT_MARKS);

  tracing = false;

  IMG_AddInstrumentFunction(ReplaceUserFunctions, 0);

  RTN_AddInstrumentFunction(WrapMain, 0);

#ifdef TRACE
  INS_AddInstrumentFunction(Trace, 0);
#endif

  INS_AddInstrumentFunction(Dataflow, 0);
  RTN_AddInstrumentFunction(Controlflow, 0);


  
  /*
    This the large dispatch table that associated a dataflow instrumentation
    function with an instruction opcode.  See instrument_functions.c for
    the actualy instrumentation functions.  
   */

  // set a default handling function that aborts.  This makes sure I don't
  // miss instructions in new applications
  for(int i = 0; i < XEDICLASS_LAST; i++) {
    instrument_functions[i] = &UnimplementedInstruction;
  }

  instrument_functions[XEDICLASS_ADD] = &Instrument_ADD; // 1
  instrument_functions[XEDICLASS_PUSH] = &Instrument_PUSH; // 2
  instrument_functions[XEDICLASS_POP] = &Instrument_POP; // 3
  instrument_functions[XEDICLASS_OR] = &Instrument_OR; // 4

  instrument_functions[XEDICLASS_ADC] = &Instrument_ADC; // 6
  instrument_functions[XEDICLASS_SBB] = &Instrument_SBB; // 7
  instrument_functions[XEDICLASS_AND] = &Instrument_AND; // 8

  //  instrument_functions[XEDICLASS_DAA] = &Instrument_DAA; // 11
  instrument_functions[XEDICLASS_SUB] = &Instrument_SUB; // 12

  //  instrument_functions[XEDICLASS_DAS] = &Instrument_DAS; // 14
  instrument_functions[XEDICLASS_XOR] = &Instrument_XOR; // 15

  //  instrument_functions[XEDICLASS_AAA] = &Instrument_AAA; // 17
  instrument_functions[XEDICLASS_CMP] = &Instrument_CMP; // 18

  //  instrument_functions[XEDICLASS_AAS] = &Instrument_AAS; // 20
  instrument_functions[XEDICLASS_INC] = &Instrument_INC; // 21
  instrument_functions[XEDICLASS_DEC] = &Instrument_DEC; // 22

  //  instrument_functions[XEDICLASS_PUSHAD] = &Instrument_PUSHAD; // 25
  //  instrument_functions[XEDICLASS_POPAD] = &Instrument_POPAD; // 27
  //  instrument_functions[XEDICLASS_BOUND] = &Instrument_BOUND; // 28
  //  instrument_functions[XEDICLASS_ARPL] = &Instrument_ARPL; // 29

  instrument_functions[XEDICLASS_IMUL] = &Instrument_IMUL; // 35
  //  instrument_functions[XEDICLASS_INSB] = &Instrument_INSB; // 36

  //  instrument_functions[XEDICLASS_INSD] = &Instrument_INSD; // 38
  //  instrument_functions[XEDICLASS_OUTSB] = &Instrument_OUTSB; // 39

  //  instrument_functions[XEDICLASS_OUTSD] = &Instrument_OUTSD; // 41
  instrument_functions[XEDICLASS_JO] = &Instrument_Jcc; //42
  instrument_functions[XEDICLASS_JNO] = &Instrument_Jcc; //43
  instrument_functions[XEDICLASS_JB] = &Instrument_Jcc; //43
  instrument_functions[XEDICLASS_JNB] = &Instrument_Jcc; //45
  instrument_functions[XEDICLASS_JZ] = &Instrument_Jcc; //46
  instrument_functions[XEDICLASS_JNZ] = &Instrument_Jcc; //47
  instrument_functions[XEDICLASS_JBE] = &Instrument_Jcc; //48
  instrument_functions[XEDICLASS_JNBE] = &Instrument_Jcc; //49
  instrument_functions[XEDICLASS_JS] = &Instrument_Jcc; //50
  instrument_functions[XEDICLASS_JNS] = &Instrument_Jcc; //51
  instrument_functions[XEDICLASS_JP] = &Instrument_Jcc; //52
  instrument_functions[XEDICLASS_JNP] = &Instrument_Jcc; //53
  instrument_functions[XEDICLASS_JL] = &Instrument_Jcc; //54
  instrument_functions[XEDICLASS_JNL] = &Instrument_Jcc; //55
  instrument_functions[XEDICLASS_JLE] = &Instrument_Jcc; //56
  instrument_functions[XEDICLASS_JNLE] = &Instrument_Jcc; //57

  instrument_functions[XEDICLASS_TEST] = &Instrument_TEST; //59
  instrument_functions[XEDICLASS_XCHG] = &Instrument_XCHG; //60
  instrument_functions[XEDICLASS_MOV] = &Instrument_MOV; //61
  instrument_functions[XEDICLASS_LEA] = &Instrument_LEA; //62

  instrument_functions[XEDICLASS_PAUSE] = &Instrument_PAUSE; //64

  instrument_functions[XEDICLASS_CWDE] = &Instrument_CWDE; //67

  instrument_functions[XEDICLASS_CDQ] = &Instrument_CDQ; //70
  //  instrument_functions[XEDICLASS_CALL_FAR] = &Instrument_CALL_FAR; //71
  //  instrument_functions[XEDICLASS_WAIT] = &Instrument_WAIT; //72

  instrument_functions[XEDICLASS_PUSHFD] = &Instrument_PUSHFD; //74

  instrument_functions[XEDICLASS_POPFD] = &Instrument_POPFD; //77

  //  instrument_functions[XEDICLASS_POPFD] = &Instrument_SAHF; //79
  //  instrument_functions[XEDICLASS_POPFD] = &Instrument_LAHF; //80
  instrument_functions[XEDICLASS_MOVSB] = &Instrument_MOVSB; //81
  instrument_functions[XEDICLASS_MOVSW] = &Instrument_MOVSW; //82
  instrument_functions[XEDICLASS_MOVSD] = &Instrument_MOVSD; //83

  instrument_functions[XEDICLASS_CMPSB] = &Instrument_CMPSB; //85

  //  instrument_functions[XEDICLASS_CMPSD] = &Instrument_CMPSD; //87

  instrument_functions[XEDICLASS_STOSB] = &Instrument_STOSB; //89
  //  instrument_functions[XEDICLASS_STOSW] = &Instrument_STOSW; //90
  instrument_functions[XEDICLASS_STOSD] = &Instrument_STOSD; //91
  
  //  instrument_functions[XEDICLASS_LODSB] = &Instrument_LODSB; //93

  //  instrument_functions[XEDICLASS_LODSD] = &Instrument_LODSD; //95

  instrument_functions[XEDICLASS_SCASB] = &Instrument_SCASB; //97

  //  instrument_functions[XEDICLASS_SCASD] = &Instrument_SCASD; //99

  instrument_functions[XEDICLASS_RET_NEAR] = &Instrument_RET_NEAR; //102
  //  instrument_functions[XEDICLASS_LES] = &Instrument_LES; //103
  //  instrument_functions[XEDICLASS_LDS] = &Instrument_LDS; //104

  //  instrument_functions[XEDICLASS_ENTER] = &Instrument_ENTER; //106
  instrument_functions[XEDICLASS_LEAVE] = &Instrument_LEAVE; //107
  //  instrument_functions[XEDICLASS_RET_FAR] = &Instrument_RET_FAR; //108
  //  instrument_functions[XEDICLASS_INT3] = &Instrument_INT3; //109
  instrument_functions[XEDICLASS_INT] = &Instrument_INT; //110
  //  instrument_functions[XEDICLASS_INT0] = &Instrument_INT0; //111

  //  instrument_functions[XEDICLASS_IRETD] = &Instrument_IRETD; //113

  //  instrument_functions[XEDICLASS_AAM] = &Instrument_AAM; //115
  //  instrument_functions[XEDICLASS_AAD] = &Instrument_AAD; //116
  //  instrument_functions[XEDICLASS_SALC] = &Instrument_SALC; //117
  //  instrument_functions[XEDICLASS_XLAT] = &Instrument_XLAT; //118

  //  instrument_functions[XEDICLASS_LOOPNE] = &Instrument_LOOPNE; //120
  //  instrument_functions[XEDICLASS_LOOPE] = &Instrument_LOOPE; //121
  //  instrument_functions[XEDICLASS_LOOP] = &Instrument_LOOP; //122
  instrument_functions[XEDICLASS_JrCXZ] = &Instrument_Jcc; //123
  //  instrument_functions[XEDICLASS_IN] = &Instrument_IN; //124
  //  instrument_functions[XEDICLASS_OUT] = &Instrument_OUT; //125
  instrument_functions[XEDICLASS_CALL_NEAR] = &Instrument_CALL_NEAR; //126
  instrument_functions[XEDICLASS_JMP] = &Instrument_JMP; //127
  //  instrument_functions[XEDICLASS_JMP_FAR] = &Instrument_JMP_FAR; //128

  //  instrument_functions[XEDICLASS_INT_l] = &Instrument_INT_l; //130

  instrument_functions[XEDICLASS_HLT] = &Instrument_HLT; //133
  //  instrument_functions[XEDICLASS_CMC] = &Instrument_CMC; //134

  //  instrument_functions[XEDICLASS_CLC] = &Instrument_CLC; //136
  //  instrument_functions[XEDICLASS_STC] = &Instrument_STC; //137
  //  instrument_functions[XEDICLASS_CLI] = &Instrument_CLI; //138
  //  instrument_functions[XEDICLASS_STI] = &Instrument_STI; //139
  instrument_functions[XEDICLASS_CLD] = &Instrument_CLD; //140
  instrument_functions[XEDICLASS_STD] = &Instrument_STD; //141

  instrument_functions[XEDICLASS_RDTSC] = &Instrument_RDTSC; //169
  
  instrument_functions[XEDICLASS_CMOVB] = &Instrument_CMOVcc; //177
  instrument_functions[XEDICLASS_CMOVNB] = &Instrument_CMOVcc; //178
  instrument_functions[XEDICLASS_CMOVZ] = &Instrument_CMOVcc; //179
  instrument_functions[XEDICLASS_CMOVNZ] = &Instrument_CMOVcc; //180
  instrument_functions[XEDICLASS_CMOVBE] = &Instrument_CMOVcc; //181
  instrument_functions[XEDICLASS_CMOVNBE] = &Instrument_CMOVcc; //182

  //  instrument_functions[XEDICLASS_EMMS] = &Instrument_EMMS; //216
  
  instrument_functions[XEDICLASS_SETB] = &Instrument_SETcc; //222
  instrument_functions[XEDICLASS_SETNB] = &Instrument_SETcc; //223
  instrument_functions[XEDICLASS_SETZ] = &Instrument_SETcc; //224
  instrument_functions[XEDICLASS_SETNZ] = &Instrument_SETcc; //225
  instrument_functions[XEDICLASS_SETBE] = &Instrument_SETcc; //226
  instrument_functions[XEDICLASS_SETNBE] = &Instrument_SETcc; //227
  //  instrument_functions[XEDICLASS_CPUID] = &Instrument_CPUID; //228
  //  instrument_functions[XEDICLASS_BT] = &Instrument_BT; //229
  instrument_functions[XEDICLASS_SHLD] = &Instrument_SHLD; //230
  instrument_functions[XEDICLASS_CMPXCHG] = &Instrument_CMPXCHG; //231

  //  instrument_functions[XEDICLASS_BTR] = &Instrument_BTR; //233
  
  instrument_functions[XEDICLASS_MOVZX] = &Instrument_MOVZX; //236
  instrument_functions[XEDICLASS_XADD] = &Instrument_XADD; //237

  //  instrument_functions[XEDICLASS_PSRLQ] = &Instrument_PSRLQ; //250  
  //  instrument_functions[XEDICLASS_PADDQ] = &Instrument_PADDQ; //251  

  //  instrument_functions[XEDICLASS_MOVQ] = &Instrument_MOVQ; //255  

  //  instrument_functions[XEDICLASS_MOVQ2Q] = &Instrument_MOVDQ2Q; //258

  //  instrument_functions[XEDICLASS_PSLLQ] = &Instrument_PSLLQ; //272
  //  instrument_functions[XEDICLASS_PMULUDQ] = &Instrument_PMULUDQ; //273

  //  instrument_functions[XEDICLASS_UD2] = &Instrument_UD2; //281

  instrument_functions[XEDICLASS_CMOVS] = &Instrument_CMOVcc; //307
  instrument_functions[XEDICLASS_CMOVNS] = &Instrument_CMOVcc; //308

  instrument_functions[XEDICLASS_CMOVL] = &Instrument_CMOVcc; //311
  instrument_functions[XEDICLASS_CMOVNL] = &Instrument_CMOVcc; //312
  instrument_functions[XEDICLASS_CMOVLE] = &Instrument_CMOVcc; //313
  instrument_functions[XEDICLASS_CMOVNLE] = &Instrument_CMOVcc; //314

  //  instrument_functions[XEDICLASS_MOVD] = &Instrument_MOVD; //350
  //  instrument_functions[XEDICLASS_MOVDQU] = &Instrument_MOVDQU; //351

  //  instrument_functions[XEDICLASS_MOVDQA] = &Instrument_MOVDQA; //354

  instrument_functions[XEDICLASS_SETS] = &Instrument_SETcc; //361

  instrument_functions[XEDICLASS_SETL] = &Instrument_SETcc; //365
  instrument_functions[XEDICLASS_SETNL] = &Instrument_SETcc; //366
  instrument_functions[XEDICLASS_SETLE] = &Instrument_SETcc; //367
  instrument_functions[XEDICLASS_SETNLE] = &Instrument_SETcc; //368

  //  instrument_functions[XEDICLASS_BTS] = &Instrument_BTS; //370
  instrument_functions[XEDICLASS_SHRD] = &Instrument_SHRD; //371

  //  instrument_functions[XEDICLASS_BSF] = &Instrument_BSF; //376
  //  instrument_functions[XEDICLASS_BSR] = &Instrument_BSR; //377
  instrument_functions[XEDICLASS_MOVSX] = &Instrument_MOVSX; //378
  instrument_functions[XEDICLASS_BSWAP] = &Instrument_BSWAP; //379

  //  instrument_functions[XEDICLASS_PAND] = &Instrument_PAND; //383

  //  instrument_functions[XEDICLASS_PSUBSW] = &Instrument_PSUBSW; //389

  //  instrument_functions[XEDICLASS_POR] = &Instrument_POR; //391

  //  instrument_functions[XEDICLASS_PXOR] = &Instrument_PXOR; //395

  //  instrument_functions[XEDICLASS_ROL] = &Instrument_ROL; //472
  //  instrument_functions[XEDICLASS_ROR] = &Instrument_ROR; //473
  //  instrument_functions[XEDICLASS_RCL] = &Instrument_RCL; //474
  //  instrument_functions[XEDICLASS_RCR] = &Instrument_RCR; //475
  instrument_functions[XEDICLASS_SHL] = &Instrument_SHL; //476
  instrument_functions[XEDICLASS_SHR] = &Instrument_SHR; //477
  instrument_functions[XEDICLASS_SAR] = &Instrument_SAR; //478
  instrument_functions[XEDICLASS_NOT] = &Instrument_NOT; //479
  instrument_functions[XEDICLASS_NEG] = &Instrument_NEG; //480
  instrument_functions[XEDICLASS_MUL] = &Instrument_MUL; //481
  instrument_functions[XEDICLASS_DIV] = &Instrument_DIV; //482
  instrument_functions[XEDICLASS_IDIV] = &Instrument_IDIV; //483

  instrument_functions[XEDICLASS_LDMXCSR] = &Instrument_LDMXCSR; //507
  instrument_functions[XEDICLASS_STMXCSR] = &Instrument_STMXCSR; //508

  instrument_functions[XEDICLASS_FLDCW] = &Instrument_FLDCW; //527

  instrument_functions[XEDICLASS_FNSTCW] = &Instrument_FNSTCW; //529

  /*
    SyscallMonitor takes care of the dirty work of handling system calls
    all that you need to do it give it the system call number of monitor
    and a callback that will be called after the system call and give the
    arguments and the return value.  See syscall_monitor.H for the system 
    call monitor and also syscall_functions.c for the call back functions.
  */

  SyscallMonitor monitor;
  monitor.activate();
  // set a default observer that aborts when a program uses a system
  // call that we don't provide a handling function for.
  monitor.setDefaultObserver(UnimplementedSystemCall);

  monitor.addObserver(SYS_access, Handle_ACCESS, 0);
  monitor.addObserver(SYS_alarm, Handle_ALARM, 0);
  monitor.addObserver(SYS_brk, Handle_BRK, 0);
  monitor.addObserver(SYS_chmod, Handle_CHMOD, 0);
  monitor.addObserver(SYS_close, Handle_CLOSE, 0);
  monitor.addObserver(SYS_dup, Handle_DUP, 0);
  monitor.addObserver(SYS_fcntl64, Handle_FSTAT64, 0);
  monitor.addObserver(SYS_flock, Handle_FLOCK, 0);
  monitor.addObserver(SYS_fstat64, Handle_FSTAT64, 0);
  monitor.addObserver(SYS_fsync, Handle_FSYNC, 0);
  monitor.addObserver(SYS_ftruncate, Handle_FTRUNCATE, 0);
  monitor.addObserver(SYS_getdents64, Handle_GETDENTS64, 0);
  monitor.addObserver(SYS_getpid, Handle_GETPID, 0);
  monitor.addObserver(SYS_gettimeofday, Handle_GETTIMEOFDAY, 0);
  monitor.addObserver(SYS_getuid32, Handle_GETUID32, 0);
  monitor.addObserver(SYS_ioctl, Handle_IOCTL, 0);
  monitor.addObserver(SYS_link, Handle_LINK, 0);
  monitor.addObserver(SYS__llseek, Handle_LLSEEK, 0);
  monitor.addObserver(SYS_lseek, Handle_LSTAT64, 0);
  monitor.addObserver(SYS_lstat64, Handle_LSTAT64, 0);
  monitor.addObserver(SYS_mmap, Handle_MMAP, 0);
  monitor.addObserver(SYS_mmap2, Handle_MMAP2, 0);
  monitor.addObserver(SYS_munmap, Handle_MUNMAP, 0);
 
  /* Pin has problems instrumenting the nanosleep system call so we skip it */
  // monitor.addObserver(SYS_nanosleep, Handle_NANOSLEEP, 0);
  monitor.addObserver(SYS_open, Handle_OPEN, 0);
  monitor.addObserver(SYS_read, Handle_READ, 0);
  monitor.addObserver(SYS_readlink, Handle_READLINK, 0);
  monitor.addObserver(SYS_rename, Handle_RENAME, 0);
  monitor.addObserver(SYS_rt_sigaction, Handle_RT_SIGACTION, 0);
  monitor.addObserver(SYS_rt_sigprocmask, Handle_RT_SIGPROCMASK, 0);
  monitor.addObserver(SYS_set_thread_area, Handle_SET_THREAD_AREA, 0);
  monitor.addObserver(SYS_stat64, Handle_STAT64, 0);
  monitor.addObserver(SYS_socketcall, Handle_SOCKETCALL, 0);
  monitor.addObserver(SYS_time, Handle_TIME, 0);
  monitor.addObserver(SYS_uname, Handle_UNAME, 0);
  monitor.addObserver(SYS_unlink, Handle_UNLINK, 0);
  monitor.addObserver(SYS_utime, Handle_UTIME, 0);
  monitor.addObserver(SYS_write, Handle_WRITE, 0);
  monitor.addObserver(SYS_writev, Handle_WRITEV, 0);


  PIN_StartProgram();
}
