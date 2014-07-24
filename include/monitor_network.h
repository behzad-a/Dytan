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

#ifndef _NETWORK_MONITOR_H_
#define _NETWORK_MONITOR_H_

#include "syscall_monitor.H"
#include <string>

typedef int int32_t;
typedef  unsigned int uint32_t;

struct NetworkAddress {
    uint32_t ip;
    short port;
    string strAddress;
};

typedef void (*NetworkMonitorCallback)(NetworkAddress, ADDRINT, size_t, void *);

// Function object that implements LESS_THAN for type NetworkAddress
struct NetworkAddress_cmp {
    // return true if networkaddress a is less than networkaddress b
    bool operator()(const NetworkAddress &a, const NetworkAddress &b) {
        return (a.ip < b.ip) || (a.port < b.port);
    }
};

class NetworkMonitor {

    private:
        SyscallMonitor *syscallMonitor;
        bool observeEverything;
        pair<NetworkMonitorCallback, void *> *defaultAddressObserver;
        pair<NetworkMonitorCallback, void *> *allObserver;
        map<NetworkAddress, vector<pair<NetworkMonitorCallback, void *> >, NetworkAddress_cmp > addressObservers;
        map<uint32_t, NetworkAddress> socketToAddress;
        void notifyForRead(syscall_arguments, NetworkAddress& , ADDRINT , size_t );

    public:
        NetworkMonitor(SyscallMonitor *syscallMonitor, bool observeAll);
        ~NetworkMonitor();

        void activate();
        void registerAddressDefault(NetworkMonitorCallback, void *);
        void registerCallbackForAll(NetworkMonitorCallback callback, void *v);

        void observeAddress(string, string, NetworkMonitorCallback, void *);

        friend void socketcallNetworkCallback(INT32, syscall_arguments, void *);
        friend void readNetworkCallback(INT32, syscall_arguments, void *);
        friend void closeNetworkCallback(INT32, syscall_arguments, void *);
};

#endif
