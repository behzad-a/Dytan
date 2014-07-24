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

#include <limits.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>

#include "include/bitset.h"

#define BITSET_BITS \
	( CHAR_BIT * sizeof(size_t) )

#define BITSET_MASK(pos) \
	( ((size_t)1) << ((pos) % BITSET_BITS) )

#define BITSET_WORD(set, pos) \
	( (set)->bits[(pos) / BITSET_BITS] )

#define BITSET_USED(nbits) \
	( ((nbits) + (BITSET_BITS - 1)) / BITSET_BITS )

bitset *bitset_init(size_t nbits) {
	bitset *set;

	set = (bitset *) malloc(sizeof(*set));
	assert(set);

	set->bits = (size_t *) calloc(BITSET_USED(nbits), sizeof(*set->bits));
	set->nbits = nbits;

	assert(set->bits);

	return set;
}

bitset *bitset_copy(const bitset *set) {
  bitset *clone = bitset_init(set->nbits);

  memcpy(clone->bits, set->bits, BITSET_USED(set->nbits) * sizeof(*set->bits));

  return clone;
}

void bitset_reset(bitset *set) {
  memset(set->bits, 0, BITSET_USED(set->nbits) * sizeof(*set->bits));
}

void bitset_free(bitset *set) {
  free(set->bits);
  free(set);
}

size_t bitset_size(const bitset *set) {
  return set->nbits;
}

void bitset_clear_bit(bitset *set, size_t pos) {
  assert(pos < set->nbits);
  
  BITSET_WORD(set, pos) &= ~BITSET_MASK(pos);
}

void bitset_set_bit(bitset *set, size_t pos) {
  assert(pos < set->nbits);
  
  BITSET_WORD(set, pos) |= BITSET_MASK(pos);
}

void bitset_set_bits(bitset *dest, const bitset *src) {
  assert(dest->nbits >= src->nbits);

  memcpy(dest->bits, src->bits, BITSET_USED(src->nbits) * sizeof(*src->bits));
}

bool bitset_test_bit(const bitset *set, size_t pos) {
  assert(pos < set->nbits);
  
  return (BITSET_WORD(set, pos) & BITSET_MASK(pos)) != 0;
}

void bitset_toggle_bit(bitset *set, size_t pos) {
  assert(pos < set->nbits);
  
  if(bitset_test_bit(set, pos)) {
    bitset_clear_bit(set, pos);
  }
  else {
    bitset_set_bit(set, pos);
  }
}

void bitset_union(bitset *dest, const bitset *src) {
  assert(dest->nbits == src->nbits);
  
  int i = BITSET_USED(dest->nbits);
  
  while(i--)
    dest->bits[i] |= src->bits[i];
}

void bitset_union_n(bitset *dest, ...) {
  va_list ap;
  bitset *src;

  va_start(ap, dest);

  while((src = va_arg(ap, bitset *)) != NULL) {
    bitset_union(dest, src);
  }

  va_end(ap);
} 

void bitset_intersection(bitset *dest, const bitset *src) {
  assert(dest->nbits == src->nbits);

  int i = BITSET_USED(dest->nbits);
  
  while(i--)
    dest->bits[i] &= src->bits[i];
}

void bitset_difference(bitset *dest, const bitset *src) {
  assert(dest->nbits == src->nbits);

  int i = BITSET_USED(dest->nbits);
  
  while(i--)
    dest->bits[i] &= ~src->bits[i];
}

void bitset_xor(bitset *dest, const bitset * src) {
  assert(dest->nbits == src->nbits);
  
  int i = BITSET_USED(dest->nbits);
  
  while(i--)
    dest->bits[i] ^= src->bits[i];
}

bool bitset_equal(const bitset *a, const bitset *b) {
  if(a->nbits != b->nbits) {
    return false;
  }

  int i = BITSET_USED(a->nbits);
  
  while(i-- && (a->bits[i] == b->bits[i]))
    ;

  return i == -1;
}

bool bitset_is_subset(const bitset *a, const bitset *b) {
  assert(a->nbits == b->nbits);

  int i = BITSET_USED(a->nbits);
  
  while(i-- && ((a->bits[i] | b->bits[i]) == a->bits[i]));
    ;

  return i == -1;
}

bool bitset_is_empty(const bitset *set) {
  int i = BITSET_USED(set->nbits);
  
  while(i-- && !set->bits[i])
    ;

  return i == -1;
}

size_t bitset_population(const bitset * s) {
  size_t total = 0;

  unsigned int i;
  for(i = 0; i < s->nbits; i++) {
    if(bitset_test_bit(s, i)) total++;
  }

  return total;
}

void bitset_print(FILE *f, const bitset *set) {
  unsigned int i;
  for(i = 0; i < set->nbits; i++) {
    printf("%d", bitset_test_bit(set, i));    
  }
  printf("\n");
}

char * bitset_str(const bitset *set) {
  unsigned int i;
  char *buf = (char *) malloc(set->nbits + 1);
  buf[0] = '\0';

  for(i = 0; i < set->nbits; i++) {
    if(bitset_test_bit(set, i)) {
      strncat(buf, "1", 1);
    }
    else {
      strncat(buf, "0", 1);
    }
  }
  
  return buf;
}
