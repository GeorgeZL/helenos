/*
 * Copyright (C) 2005 Jakub Jermar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ia64_CONTEXT_H__
#define __ia64_CONTEXT_H__

#include <arch/types.h>
#include <arch/register.h>
#include <typedefs.h>
#include <align.h>
#include <arch/stack.h>

/*
 * context_save_arch() and context_restore_arch() are both leaf procedures.
 * No need to allocate scratch area.
 *
 * One item is put onto the stack to support get_stack_base().
 */
#define SP_DELTA	(0+ALIGN_UP(STACK_ITEM_SIZE, STACK_ALIGNMENT))

#ifdef context_set
#undef context_set
#endif

/* RSE stack starts at the bottom of memory stack. */
#define context_set(c, _pc, stack, size)								\
	do {												\
		(c)->pc = (__address) _pc;								\
		(c)->bsp = ((__address) stack) + ALIGN_UP((size), REGISTER_STACK_ALIGNMENT);		\
		(c)->ar_pfs &= PFM_MASK; 								\
		(c)->sp = ((__address) stack) + ALIGN_UP((size), STACK_ALIGNMENT) - SP_DELTA;		\
	} while (0);

/*
 * Only save registers that must be preserved across
 * function calls.
 */
struct context {

	/*
	 * Application registers
	 */
	__u64 ar_pfs;
	__u64 ar_unat_caller;
	__u64 ar_unat_callee;
	__u64 ar_rsc;
	__address bsp;		/* ar_bsp */
	__u64 ar_rnat;
	__u64 ar_lc;

	/*
	 * General registers
	 */
	__u64 r1;
	__u64 r4;
	__u64 r5;
	__u64 r6;
	__u64 r7;
	__address sp;		/* r12 */
	__u64 r13;
	
	/*
	 * Branch registers
	 */
	__address pc;		/* b0 */
	__u64 b1;
	__u64 b2;
	__u64 b3;
	__u64 b4;
	__u64 b5;

	/*
	 * Predicate registers
	 */
	__u64 pr;

	__r128 f2 __attribute__ ((aligned(16)));
	__r128 f3;
	__r128 f4;
	__r128 f5;

	__r128 f16;
	__r128 f17;
	__r128 f18;
	__r128 f19;
	__r128 f20;
	__r128 f21;
	__r128 f22;
	__r128 f23;
	__r128 f24;
	__r128 f25;
	__r128 f26;
	__r128 f27;
	__r128 f28;
	__r128 f29;
	__r128 f30;
	__r128 f31;
	
	ipl_t ipl;
};

#endif
