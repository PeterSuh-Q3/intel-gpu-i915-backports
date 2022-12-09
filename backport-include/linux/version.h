/*
 * Copyright © 2015 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 *
 */

#ifndef _BACKPORT_LINUX_VERSION_H
#define _BACKPORT_LINUX_VERSION_H
#include_next <linux/version.h>
#include <linux/osv_version.h>

#ifndef CONFIG_SUSE_KERNEL
#define SUSE_LOCAL_VERSION_IS_GEQ(X,Y) 0
#define SUSE_LOCAL_VERSION_IS_LESS(X,Y) 0
#define SUSE_RELEASE_VERSION_IS_GEQ(X,Y,Z) 0
#define SUSE_RELEASE_VERSION_IS_LESS(X,Y,Z) 0
#endif

#ifndef UBUNTU_BACKPORT_RELEASE_CODE
#define UBUNTU_RELEASE_VERSION_IS_GEQ(X,Y) 0
#define UBUNTU_RELEASE_VERSION_IS_LESS(X,Y) 0
#define UBUNTU_BACKPORT_VERSION_IS_GEQ(X,Y) 0
#define UBUNTU_BACKPORT_VERSION_IS_LESS(X,Y) 0
#endif

#ifndef RHEL_RELEASE_CODE
#define REDHAT_RELEASE_VERSION_IS_GEQ(X,Y) 0
#define REDHAT_RELEASE_VERSION_IS_LESS(X,Y) 0
#define REDHAT_RELEASE_VERSION_IS_EQL(X,Y) 0
#define REDHAT_RELEASE_VERSION_IS_NOT_EQL(X,Y) 0
#define RHEL_RELEASE_CODE 0
#endif

#ifndef RHEL_RELEASE_VERSION
#define RHEL_RELEASE_VERSION(a,b) (((a) << 8) + (b))
#endif

#if defined(CONFIG_SUSE_KERNEL)
/*
* To differentiate between the different minor versions
* X - Major of minor
* Y - Minor of minor
*/
#define SUSE_LOCAL_VERSION_IS_GEQ(X,Y) SUSE_LOCALVERSION_RELEASE_CODE >= SUSE_LOCALVERSION(X,Y)
#define SUSE_LOCAL_VERSION_IS_LESS(X,Y) SUSE_LOCALVERSION_RELEASE_CODE < SUSE_LOCALVERSION(X,Y)
/*
* To differentiate between the different service packs
* X - SUSE Linux Enterprise Server
* Y - Service pack
* Z - Minor of service pack
*/
#define SUSE_RELEASE_VERSION_IS_GEQ(X,Y,Z) SUSE_RELEASE_CODE >= SUSE_RELEASE_VERSION(X,Y,Z)
#define SUSE_RELEASE_VERSION_IS_LESS(X,Y,Z) SUSE_RELEASE_CODE < SUSE_RELEASE_VERSION(X,Y,Z)
#elif defined(UBUNTU_BACKPORT_RELEASE_CODE)
/*
* To differentiate between the different minor versions
* X - Major of minor
* Y - Minor of minor
*/
#define UBUNTU_BACKPORT_VERSION_IS_GEQ(X,Y) UBUNTU_BACKPORT_RELEASE_CODE >= UBUNTU_BACKPORT_RELEASE_VERSION(X,Y)
#define UBUNTU_BACKPORT_VERSION_IS_LESS(X,Y) UBUNTU_BACKPORT_RELEASE_CODE < UBUNTU_BACKPORT_RELEASE_VERSION(X,Y)
/*
 * To differentiate between the different release versions
 * UBUNTU 20.04
 * X - UBUNTU Major release version i.e <20>
 * Y - UBUNTU Minor release version i.e <04>
*/
#define UBUNTU_RELEASE_VERSION_IS_GEQ(X,Y) UBUNTU_RELEASE_CODE >= UBUNTU_RELEASE_VERSION(X,Y)
#define UBUNTU_RELEASE_VERSION_IS_LESS(X,Y) UBUNTU_RELEASE_CODE < UBUNTU_RELEASE_VERSION(X,Y)
#elif defined(RHEL_RELEASE_CODE)
/*
 * To differentiate between the different release versions
 * RHEL 8.6
 * X - RHEL Major release version i.e <8>
 * Y - RHEL Minor release version i.e <6>
*/
#define REDHAT_RELEASE_VERSION_IS_GEQ(X,Y) RHEL_RELEASE_CODE >= RHEL_RELEASE_VERSION(X,Y)
#define REDHAT_RELEASE_VERSION_IS_LESS(X,Y) RHEL_RELEASE_CODE < RHEL_RELEASE_VERSION(X,Y)
#define REDHAT_RELEASE_VERSION_IS_LEQ(X,Y) RHEL_RELEASE_CODE <= RHEL_RELEASE_VERSION(X,Y)
#define REDHAT_RELEASE_VERSION_IS_EQL(X,Y) RHEL_RELEASE_CODE == RHEL_RELEASE_VERSION(X,Y)
#define REDHAT_RELEASE_VERSION_IS_NOT_EQL(X,Y) RHEL_RELEASE_CODE != RHEL_RELEASE_VERSION(X,Y)
#define REDHAT_RELEASE_VERSION_IS_RANGE(x1,x2, y1,y2) \
	(REDHAT_RELEASE_VERSION_IS_GEQ(x1,x2) && REDHAT_RELEASE_VERSION_IS_LEQ(y1,y2))
#endif

#define LINUX_VERSION_IS_LESS(x1,x2,x3) (LINUX_VERSION_CODE < KERNEL_VERSION(x1,x2,x3))
#define LINUX_VERSION_IS_GEQ(x1,x2,x3)  (LINUX_VERSION_CODE >= KERNEL_VERSION(x1,x2,x3))
#define LINUX_VERSION_IN_RANGE(x1,x2,x3, y1,y2,y3) \
        (LINUX_VERSION_IS_GEQ(x1,x2,x3) && LINUX_VERSION_IS_LESS(y1,y2,y3))

#endif /* _BACKPORT_LINUX_VERSION_H */
