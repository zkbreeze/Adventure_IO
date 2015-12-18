/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

/*
  AdvStat.c
*/

#include "AdvStat.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#if defined(linux)
#include <stdlib.h>
#endif

/*
============================================================
 adv_query_hostname
============================================================
*/

#include <unistd.h>
#include <sys/utsname.h>

bool
adv_query_hostname(char* buf, size_t len, bool is_fqdn)
{
  gethostname(buf, len);
  return false;
}

/*
============================================================
 adv_query_load_average
============================================================
*/

#include <sys/time.h>

bool
adv_query_load_average(double* la1, double* la2, double* la3)
{
#if defined(linux)
  char buf[1024];
  int fd, len;
  char *p;
  double la[3];

  fd = open("/proc/loadavg", O_RDONLY);
  if (fd < 0)
    return true;				  /* error */
  len = read(fd, buf, sizeof(buf));
  if (len < 0)
    return true;				  /* error */
  close(fd);
  buf[len] = '\0';
  la[0] = strtod(buf, &p);
  la[1] = strtod(p, &p);
  la[2] = strtod(p, &p);
  if (la1)
    *la1 = la[0];
  if (la2)
    *la1 = la[1];
  if (la3)
    *la1 = la[2];
  return false;
#elif defined(__FreeBSD__)
  double la[3];
  getloadavg(la, 3);
  if (la1)
    *la1 = la[0];
  if (la2)
    *la1 = la[1];
  if (la3)
    *la1 = la[2];
  return false;
#else
  return true;
#endif
}

/*
============================================================
 adv_query_etime
============================================================
*/

#if defined(__FreeBSD__) || defined(linux) || defined(__osf__)
#include <sys/times.h>
#include <time.h>
#endif

bool
adv_query_time(float64* t)
{
#if defined(__linux__) || defined(__osf__)
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (t)
    *t = (float64)tv.tv_sec + (float64)tv.tv_usec / 1000000.0;
  return false;
#elif defined(__FreeBSD__) || defined(__hi_ux__)
   struct tms local;
   times(&local);
   if (t)
     *t = ((float64)local.tms_utime
	   + (float64)local.tms_stime
	   + (float64)local.tms_cutime
	   + (float64)local.tms_cstime)/CLK_TCK;
   return false;
#else
   if (t)
     *t = 0.0;
   return true;
#endif
}

/*
============================================================
 adv_query_freemem
============================================================
*/

#ifdef __FreeBSD__
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/vmmeter.h>
#include <vm/vm_param.h>
#include <nlist.h>
#include <kvm.h>

#elif defined(linux)
#include <string.h>
#include <stdio.h>

#elif defined(__osf__)
#include <sys/vm.h>
#include <mach.h>
#include <mach/mach_types.h>
#include <mach/vm_statistics.h>

#elif defined(__hi_ux__)
#include <mach.h>
#include <mach/mach_types.h>
#include <mach/vm_statistics.h>

#endif

bool
adv_query_freemem(size_t* freemem, size_t* cachemem)
{
  /*============================================================*/
#ifdef __FreeBSD__
#if 0
  static struct nlist nlst = {"_cnt"};

  kvm_t* kvm = kvm_open(NULL, NULL, NULL, O_RDONLY, "kvm_open");
  if (!kvm)
    return false;
  
  vmmeter vmm;
  if (kvm_read (kvm, nlst.n_value,
		&vmm, sizeof(vmm)) != sizeof(vmm))
    {
      kvm_close(kvm);
      return true;
    }
  kvm_close(kvm);

  int pagesize = getpagesize();
  if (freemem)
    *freemem = vmm.v_free_count * pagesize;
  if (cachemem)
    *cachemem = vmm.v_cache_count * pagesize;
  return false;
#else
  int cache_count;
  int free_count;
  int pagesize;
  unsigned int size;
  size = sizeof(int);
  sysctlbyname("vm.stats.vm.v_cache_count", &cache_count, &size, NULL, 0);
  sysctlbyname("vm.stats.vm.v_free_count", &free_count, &size, NULL, 0);
  pagesize = getpagesize();
  if (freemem)
    *freemem = free_count * pagesize;
  if (cachemem)
    *cachemem = cache_count * pagesize;
  return false;
#endif

  /*============================================================*/
#elif defined(linux)
  char buf[1024];
  FILE* fp;
  fp = fopen("/proc/meminfo", "r");
  fgets(buf, sizeof(buf), fp);		  /* skip the 1st line */
  fgets(buf, sizeof(buf), fp);
  strtok(buf, " ");
  strtok(NULL, " ");
  strtok(NULL, " ");
  fclose(fp);
  if (freemem)
    *freemem = atoi(strtok(NULL, " "));
  strtok(NULL, " ");
  strtok(NULL, " ");
  if (cachemem)
    *cachemem = atoi(strtok(NULL, " "));
  return false;

  /*============================================================*/
#elif defined(__osf__)
  struct vm_statistics vmstats;
  (void)vm_statistics(task_self(), &vmstats);
  if (freemem)
    *freemem = vmstats.free_count * vmstats.pagesize;
  if (cachemem)
    *cachemem = 0.0;				  /* cannot obtain cachememory */
  return false;

  /*============================================================*/
#elif defined(__hi_ux__)
  struct vm_statistics vmstats;
  (void)vm_statistics(mach_task_self(), &vmstats);
  if (freemem)
    *freemem = vmstats.free_count * vmstats.pagesize;
  if (cachemem)
    *cachemem = 0.0;				  /* cannot obtain cachememory */
  return false;

  /*============================================================*/
#else
  return true;
#endif
}

