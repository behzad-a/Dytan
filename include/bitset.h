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

#ifndef _BITSET_H_
#define _BITSET_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  size_t *bits;
  size_t nbits;
} bitset;

bitset* bitset_init(size_t nbits);
bitset* bitset_copy(const bitset *set);
void bitset_free(bitset *set);

void bitset_reset(bitset *set);

size_t bitset_size(const bitset *set);

void bitset_clear_bit(bitset *set, size_t pos);
void bitset_set_bit(bitset *set, size_t pos);
void bitset_set_bits(bitset *dest, const bitset *src);
bool bitset_test_bit(const bitset *set, size_t pos);
void bitset_toggle_bit(bitset *set, size_t pos);

void bitset_union(bitset *dest, const bitset *src);
void bitset_union_n(bitset *dest, ...);
void bitset_intersection(bitset *dest, const bitset *src);
void bitset_difference(bitset *dest, const bitset *src);
void bitset_xor(bitset *dest, const bitset * src);

bool bitset_equal(const bitset *a, const bitset *b);
bool bitset_is_subset(const bitset *a, const bitset *b);
bool bitset_is_empty(const bitset *set);

size_t bitset_population(const bitset *a);

void bitset_print(FILE *f, const bitset *set);
char *bitset_str(const bitset *set);
#endif
