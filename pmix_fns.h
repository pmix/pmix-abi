/*
 * Copyright (c) 2013-2020 Intel, Inc.  All rights reserved.
 * Copyright (c) 2016-2019 Research Organization for Information Science
 *                         and Technology (RIST).  All rights reserved.
 * Copyright (c) 2016      IBM Corporation.  All rights reserved.
 * Copyright (c) 2016-2019 Mellanox Technologies, Inc.
 *                         All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer listed
 *   in this license in the documentation and/or other materials
 *   provided with the distribution.
 *
 * - Neither the name of the copyright holders nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * The copyright holders provide no reassurances that the source code
 * provided does not infringe any patent, copyright, or any other
 * intellectual property rights of third parties.  The copyright holders
 * disclaim any liability to any recipient for claims brought against
 * recipient by any third party for infringement of that parties
 * intellectual property rights.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Copyright (c) 2020      Cisco Systems, Inc.  All rights reserved
 * Copyright (c) 2021      Nanook Consulting.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#ifndef PMIX_FNS_H
#define PMIX_FNS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h> /* for struct timeval */
#include <unistd.h> /* for uid_t and gid_t */
#include <sys/types.h> /* for uid_t and gid_t */

#include <pmix_constants.h>
#include <pmix_types.h>

/* Initialize the PMIx client, returning the process identifier assigned
 * to this client's application in the provided pmix_proc_t struct.
 * Passing a parameter of _NULL_ for this parameter is allowed if the user
 * wishes solely to initialize the PMIx system and does not require
 * return of the identifier at that time.
 *
 * When called the PMIx client will check for the required connection
 * information of the local PMIx server and will establish the connection.
 * If the information is not found, or the server connection fails, then
 * an appropriate error constant will be returned.
 *
 * If successful, the function will return PMIX_SUCCESS and will fill the
 * provided structure with the server-assigned namespace and rank of the
 * process within the application.
 *
 * Note that the PMIx client library is referenced counted, and so multiple
 * calls to PMIx_Init are allowed. Thus, one way to obtain the namespace and
 * rank of the process is to simply call PMIx_Init with a non-NULL parameter.
 *
 * The info array is used to pass user requests pertaining to the init
 * and subsequent operations. Pass a _NULL_ value for the array pointer
 * is supported if no directives are desired.
 */
typedef pmix_status_t (*pmix_init_fn_t)(pmix_proc_t *proc, pmix_info_t info[], size_t ninfo);

/* Finalize the PMIx client, closing the connection to the local server.
 * An error code will be returned if, for some reason, the connection
 * cannot be closed.
 *
 * The info array is used to pass user requests regarding the finalize
 * operation. This can include:
 *
 * (a) PMIX_EMBED_BARRIER - By default, PMIx_Finalize does not include an
 * internal barrier operation. This attribute directs PMIx_Finalize to
 * execute a barrier as part of the finalize operation.
 */
typedef pmix_status_t (*pmix_finalize_fn_t)(const pmix_info_t info[], size_t ninfo);

/* Returns _true_ if the PMIx client has been successfully initialized,
 * returns _false_ otherwise. Note that the function only reports the
 * internal state of the PMIx client - it does not verify an active
 * connection with the server, nor that the server is functional. */
typedef int (*pmix_initialized_fn_t)(void);

/* Request that the provided array of procs be aborted, returning the
 * provided _status_ and printing the provided message. A _NULL_
 * for the proc array indicates that all processes in the caller's
 * nspace are to be aborted.
 *
 * The response to this request is somewhat dependent on the specific resource
 * manager and its configuration (e.g., some resource managers will
 * not abort the application if the provided _status_ is zero unless
 * specifically configured to do so), and thus lies outside the control
 * of PMIx itself. However, the client will inform the RM of
 * the request that the application be aborted, regardless of the
 * value of the provided _status_.
 *
 * Passing a _NULL_ msg parameter is allowed. Note that race conditions
 * caused by multiple processes calling PMIx_Abort are left to the
 * server implementation to resolve with regard to which status is
 * returned and what messages (if any) are printed. */
typedef pmix_status_t (*pmix_abort_fn_t)(int status, const char msg[],
                                         pmix_proc_t procs[], size_t nprocs);


#endif /* PMIX_FNS_H */
