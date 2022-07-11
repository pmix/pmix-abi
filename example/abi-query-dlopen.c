/*
 * Copyright (c) 2022      IBM Corporation.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 * Test PMIx_Query_info outside of init/finalize by asking for a
 * key (PMIX_QUERY_ABI_VERSION) that is allowed to be accessed
 * outside of the init/finalize region. Use dlopen to pick up
 * the PMIx library.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

// This is the dlopen friendly header from the pmix-abi
#include <pmix_fns.h>

// Temporary until it is added to the PMIx Standard
#define PMIX_QUERY_STABLE_ABI_VERSION       "pmix.qry.stabiver"
#define PMIX_QUERY_PROVISIONAL_ABI_VERSION  "pmix.qry.prabiver"


int main(int argc, char **argv) {
    int rc, i;
    size_t ninfo, nqueries;
    pmix_info_t *info = NULL;
    pmix_query_t *query = NULL;
    char *pmix_root = NULL;
    char *pmix_lib = NULL;

    pmix_root = getenv("PMIX_ROOT");
    if( NULL == pmix_root ) {
        fprintf(stderr, "Error: The envar PMIX_ROOT must be set to a PMIx implementation installation.\n");
        exit(1);
    }

    // append "/lib/libpmix.so\0" = 16 characters
    pmix_lib = (char*)malloc(sizeof(char) * (strlen(pmix_root) + 17));
    pmix_lib[0] = '\0';
    strcat(pmix_lib, pmix_root);
    strcat(pmix_lib, "/lib/libpmix.so");
    printf("Using: PMIX_ROOT: \"%s\"\n", pmix_root);
    printf("Using: PMIX_LIB : \"%s\"\n", pmix_lib);

    /*
     * dlopen the PMIx library using the PMIX_ROOT as the location
     */
    void *lib_handle;
    lib_handle = dlopen(pmix_lib, RTLD_LAZY | RTLD_LOCAL);
    if( NULL == lib_handle ) {
        fprintf(stderr, "Error: Failed to open the library. %s\n", dlerror());
        exit(1);
    }

    /*
     * Find the PMIx_Query_info symbol so we can check the ABI level
     */
    pmix_query_info_fn_t query_fn;
    query_fn = dlsym(lib_handle, "PMIx_Query_info");
    if( NULL == query_fn ) {
        fprintf(stderr, "Error: Failed to find the PMIx_Query_info symbol. %s\n", dlerror());
        exit(1);
    }

    /*
     * Perform the query
     */
    nqueries = 2;

    PMIX_QUERY_CREATE(query, nqueries);
    PMIX_ARGV_APPEND(rc, query[0].keys, PMIX_QUERY_STABLE_ABI_VERSION);
    PMIX_ARGV_APPEND(rc, query[1].keys, PMIX_QUERY_PROVISIONAL_ABI_VERSION);

    //rc = PMIx_Query_info(query, nqueries, &info, &ninfo);
    rc = query_fn(query, nqueries, &info, &ninfo);
    if (PMIX_SUCCESS != rc ) {
        fprintf(stderr, "Error: PMIx_Query_info failed: %d\n", rc);
        return rc;
    }

    printf("--> Query returned (ninfo %lu)\n", ninfo);
    for(i = 0; i < ninfo; ++i) {
        printf("--> KEY: %s\n", info[i].key);
        if (PMIX_CHECK_KEY(&info[i], PMIX_QUERY_STABLE_ABI_VERSION)) {
            printf("----> ABI (Stable): String: %s\n",
                   info[i].value.data.string);
        }
        else if (PMIX_CHECK_KEY(&info[i], PMIX_QUERY_PROVISIONAL_ABI_VERSION)) {
            printf("----> ABI (Provisional: String: %s\n",
                   info[i].value.data.string);
        }
    }

    PMIX_INFO_FREE(info, ninfo);
    PMIX_QUERY_FREE(query, nqueries);

    dlclose(lib_handle);
    query_fn = NULL;

    return 0;
}
