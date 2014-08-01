/**

Created by Behzad Abghari
DL Group
Department of Electrical and Computer Engineering
University of Toronto
Summer 2014

**/

#ifndef _TAG_MEMSET_H_
#define _TAG_MEMSET_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "pin.H"

typedef struct memlist{
    ADDRINT memAddress;
    int isCleared;
    struct memlist *next;
} memlist;

#endif
