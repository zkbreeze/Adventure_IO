/***********************************************************
Copyright (C) 2000,2001,2002,2006 
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

/*
 AdvStat.h
*/

#ifndef _AdvStat_h_included_
#define _AdvStat_h_included_

#include <sys/times.h>
#include <stddef.h>
#include "AdvTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* query elapsed time (from system up) in sec. */
extern bool adv_query_hostname(char* buf, size_t len, bool is_fqdn);

/* query load average */
extern bool adv_query_load_average(double* loadavg1, double* loadavg2, double* loadavg3);

/* query time in sec. */
extern bool adv_query_time(float64* t);

/* query free real(physical) memory in bytes. */
extern bool adv_query_freemem(size_t* freemem, size_t* cachemem);

#ifdef __cplusplus
}
#endif

#endif /* _AdvStat_h_included_ */
