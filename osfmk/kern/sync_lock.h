/*
 * Copyright (c) 2000-2004 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * @OSF_COPYRIGHT@
 * 
 */
/*
 *	File:	kern/sync_lock.h
 *	Author:	Joseph CaraDonna
 *
 *	Contains RT distributed lock synchronization service definitions.
 */

#ifndef _KERN_SYNC_LOCK_H_
#define _KERN_SYNC_LOCK_H_

#include <mach/mach_types.h>

#ifdef	MACH_KERNEL_PRIVATE

#include <kern/wait_queue.h>
#include <kern/macro_help.h>
#include <kern/queue.h>
#include <kern/locks.h>

extern	void		lock_set_reference	(lock_set_t lock_set);
extern	void		lock_set_dereference	(lock_set_t lock_set);

#endif	/* MACH_KERNEL_PRIVATE */

#endif /* _KERN_SYNC_LOCK_H_ */
