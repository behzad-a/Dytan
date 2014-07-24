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

   * Neither the name of the Georgia Tech Research Coporation nor the
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

#ifndef __SYSCALL_MONITOR_H
#define __SYSCALL_MONITOR_H

#include <string.h>
#include <syscall.h>
#include "pin.H"

#include <map>
#include <vector>

typedef struct
{
  ADDRINT num;

  ADDRINT arg0;
  ADDRINT arg1;
  ADDRINT arg2;
  ADDRINT arg3;
  ADDRINT arg4;
  ADDRINT arg5;

  ADDRINT ret;
  ADDRINT err;
} syscall_arguments;

typedef void (*SyscallMonitorCallback)(INT32, syscall_arguments, void *);

class SyscallMonitor {
  
public:
  SyscallMonitor()
  {
      InitLock(&syscallLock);
  }
    
  void addObserver(INT32 syscall_number, SyscallMonitorCallback callback,
		   void * v)
  {
    
    observers[syscall_number].push_back(pair<SyscallMonitorCallback, void *>(callback, v));
    
  }
  
  void setDefaultObserver(SyscallMonitorCallback callback)
  {
    defaultObserver = callback;
  }

  void beginSyscall(UINT32 tid, INT32 num, 
		    ADDRINT arg0, ADDRINT arg1, ADDRINT arg2,
		    ADDRINT arg3, ADDRINT arg4, ADDRINT arg5)
  {
    GetLock(&syscallLock, 1);
    pendingSyscalls[tid] = (syscall_arguments) {num, arg0, arg1, arg2, 
						arg3, arg4, arg5};

    ReleaseLock(&syscallLock);

  }

  void endSyscall(UINT32 tid, ADDRINT ret, ADDRINT err)
  {
    GetLock(&syscallLock, 1);
    syscall_arguments args = pendingSyscalls[tid];
    args.ret = ret;
    args.err = err;

    if(observers.find(args.num) != observers.end()) {
      
      for(vector<pair<SyscallMonitorCallback, void *> >::iterator iter = observers[args.num].begin(); iter != observers[args.num].end(); iter++) {
          iter->first(args.num, args, iter->second);
      }
    }
    else {
      if(defaultObserver) {
	defaultObserver(args.num, args, 0);
      }
    }
    ReleaseLock(&syscallLock);
  }
  
private:
  
  map<UINT32, syscall_arguments> pendingSyscalls;
  map<INT32, vector< pair<SyscallMonitorCallback, void *> > > observers;
  
  SyscallMonitorCallback defaultObserver;
  PIN_LOCK syscallLock;

}; 

#endif
