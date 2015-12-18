
#ifndef _advsys_config_h_included_
#define _advsys_config_h_included_

@TOP@

########################################################################
# Copyright (C) 2000,2001,2002,2006                                    #
# Shinobu Yoshimura, University of Tokyo,                              #
# the Japan Society for the Promotion of Science (JSPS)                #
# All Rights Reserved                                                  #
########################################################################

/* Define if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
#undef _ALL_SOURCE
#endif

/* Define if using alloca.c.  */
#undef C_ALLOCA

/* Define if type char is unsigned and you are not using gcc.  */
#ifndef __CHAR_UNSIGNED__
#undef __CHAR_UNSIGNED__
#endif

/* Define if the closedir function returns void instead of int.  */
#undef CLOSEDIR_VOID

/* Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP systems.
   This function is required for alloca.c support on those systems.  */
#undef CRAY_STACKSEG_END

/* Define for DGUX with <sys/dg_sys_info.h>.  */
#undef DGUX

/* Define if you have <dirent.h>.  */
#undef DIRENT

/* Define to the type of elements in the array set by `getgroups'.
   Usually this is either `int' or `gid_t'.  */
#undef GETGROUPS_T

/* Define if the `getloadavg' function needs to be run setuid or setgid.  */
#undef GETLOADAVG_PRIVILEGED

/* Define if the `getpgrp' function takes no argument.  */
#undef GETPGRP_VOID

/* Define to `int' if <sys/types.h> doesn't define.  */
#undef gid_t

/* Define if you have alloca, as a function or macro.  */
#undef HAVE_ALLOCA

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
#undef HAVE_ALLOCA_H

/* Define if you don't have vprintf but do have _doprnt.  */
#undef HAVE_DOPRNT

/* Define if your system has a working fnmatch function.  */
#undef HAVE_FNMATCH

/* Define if your system has its own `getloadavg' function.  */
#undef HAVE_GETLOADAVG

/* Define if you have the getmntent function.  */
#undef HAVE_GETMNTENT

/* Define if the `long double' type works.  */
#undef HAVE_LONG_DOUBLE

/* Define if you support file names longer than 14 characters.  */
#undef HAVE_LONG_FILE_NAMES

/* Define if you have a working `mmap' system call.  */
#undef HAVE_MMAP

/* Define if system calls automatically restart after interruption
   by a signal.  */
#undef HAVE_RESTARTABLE_SYSCALLS

/* Define if your struct stat has st_blksize.  */
#undef HAVE_ST_BLKSIZE

/* Define if your struct stat has st_blocks.  */
#undef HAVE_ST_BLOCKS

/* Define if you have the strcoll function and it is properly defined.  */
#undef HAVE_STRCOLL

/* Define if your struct stat has st_rdev.  */
#undef HAVE_ST_RDEV

/* Define if you have the strftime function.  */
#undef HAVE_STRFTIME

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#undef HAVE_SYS_WAIT_H

/* Define if your struct tm has tm_zone.  */
#undef HAVE_TM_ZONE

/* Define if you don't have tm_zone but do have the external array
   tzname.  */
#undef HAVE_TZNAME

/* Define if you have <unistd.h>.  */
#undef HAVE_UNISTD_H

/* Define if utime(file, NULL) sets file's timestamp to the present.  */
#undef HAVE_UTIME_NULL

/* Define if you have <vfork.h>.  */
#undef HAVE_VFORK_H

/* Define if you have the vprintf function.  */
#undef HAVE_VPRINTF

/* Define if you have the wait3 system call.  */
#undef HAVE_WAIT3

/* Define if int is 16 bits instead of 32.  */
#undef INT_16_BITS

/* Define if long int is 64 bits.  */
#undef LONG_64_BITS

/* Define if major, minor, and makedev are declared in <mkdev.h>.  */
#undef MAJOR_IN_MKDEV

/* Define if major, minor, and makedev are declared in <sysmacros.h>.  */
#undef MAJOR_IN_SYSMACROS

/* Define if on MINIX.  */
#undef _MINIX

/* Define to `int' if <sys/types.h> doesn't define.  */
#undef mode_t

/* Define if you don't have <dirent.h>, but have <ndir.h>.  */
#undef NDIR

/* Define if you have <memory.h>, and <string.h> doesn't declare the
   mem* functions.  */
#undef NEED_MEMORY_H

/* Define if your struct nlist has an n_un member.  */
#undef NLIST_NAME_UNION

/* Define if you have <nlist.h>.  */
#undef NLIST_STRUCT

/* Define if your C compiler doesn't accept -c and -o together.  */
#undef NO_MINUS_C_MINUS_O

/* Define to `int' if <sys/types.h> doesn't define.  */
#undef pid_t

/* Define if the system does not provide POSIX.1 features except
   with this defined.  */
#undef _POSIX_1_SOURCE

/* Define if you need to in order for stat and other things to work.  */
#undef _POSIX_SOURCE

/* Define as the return type of signal handlers (int or void).  */
#undef RETSIGTYPE

/* Define if the `setpgrp' function takes no argument.  */
#undef SETPGRP_VOID

/* Define if the setvbuf function takes the buffering type as its second
   argument and the buffer pointer as the third, as on System V
   before release 3.  */
#undef SETVBUF_REVERSED

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown
 */
#undef STACK_DIRECTION

/* Define if the `S_IS*' macros in <sys/stat.h> do not work properly.  */
#undef STAT_MACROS_BROKEN

/* Define on System V Release 4.  */
#undef SVR4

/* Define if you don't have <dirent.h>, but have <sys/dir.h>.  */
#undef SYSDIR

/* Define if you don't have <dirent.h>, but have <sys/ndir.h>.  */
#undef SYSNDIR

/* Define if `sys_siglist' is declared by <signal.h>.  */
#undef SYS_SIGLIST_DECLARED

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#undef TIME_WITH_SYS_TIME

/* Define if your <sys/time.h> declares struct tm.  */
#undef TM_IN_SYS_TIME

/* Define to `int' if <sys/types.h> doesn't define.  */
#undef uid_t

/* Define for Encore UMAX.  */
#undef UMAX

/* Define for Encore UMAX 4.3 that has <inq_status/cpustats.h>
   instead of <sys/cpustats.h>.  */
#undef UMAX4_3

/* Define if you do not have <strings.h>, index, bzero, etc..  */
#undef USG

/* Define vfork as fork if vfork does not work.  */
#undef vfork

/* Define if the closedir function returns void instead of int.  */
#undef VOID_CLOSEDIR

/* Define if the X Window System is missing or not being used.  */
#undef X_DISPLAY_MISSING

/* Define if lex declares yytext as a char * by default, not a char[].  */
#undef YYTEXT_POINTER


/* Leave that blank line there!!  Autoheader needs it.
   If you're adding to this file, keep in mind:
   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  */

#undef MAJOR_VERSION
#undef MINOR_VERSION
#undef DEBUG
#undef DEBUG_MEMSTAT
#undef HAVE_CXX
#undef CXX_HAVE_EXCEPTION
#undef CXX_HAVE_NAMESPACE
#undef HAVE_SYSTYPES_H
#undef HAVE_SYSTYPE_INT8
#undef HAVE_SYSTYPE_UINT8
#undef HAVE_SYSTYPE_INT16
#undef HAVE_SYSTYPE_UINT16
#undef HAVE_SYSTYPE_INT32
#undef HAVE_SYSTYPE_UINT32
#undef HAVE_SYSTYPE_INT64
#undef HAVE_SYSTYPE_UINT64
#undef HAVE_SYSTYPE_FLOAT32
#undef HAVE_SYSTYPE_FLOAT64
#undef HAVE_SYSTYPE_FLOAT128

#undef HAVE_MPI
#undef socklen_t
#undef USE_ZLIB

#undef HAVE_GTK
#undef HAVE_MICO
#undef HAVE_MICO_2_3_X
#undef HAVE_ORBIT
#undef HAVE_CURSES
#undef HAVE_ORB

#undef HAVE_FUNC_SNPRINTF
#undef HAVE_FUNC_REALPATH

#undef MACHINE_SR

/* Define if <stdarg.h> defines va_copy. */
#undef HAVE_VA_COPY

/* Define if <stdarg.h> defines __va_copy. */
#undef HAVE___VA_COPY

@BOTTOM@
#endif  /*  _advsys_config_h_included_ */
