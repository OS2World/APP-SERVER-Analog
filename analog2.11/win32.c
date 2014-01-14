/*** analog 2.1 ***/
/* Please read Readme.html, or http://www.statslab.cam.ac.uk/~sret1/analog/  */

/*** win32.c; stuff only required for the Win32 port ***/
/* This stuff is due to Magnus Hagander (mha@edu.sollentuna.se) */
#include "analhea2.h"
#ifdef WIN32
#include <windows.h>

/*
 * Initialize the required Win32 structures and routines
 */

void Win32Init(void) {
#ifndef NODNS
  WSADATA wsaData;

  if (WSAStartup(MAKEWORD(1,1),&wsaData)) {
     fprintf(stderr,"Error: Unable to initialize winsock.dll!\n");
     exit(1);
  }
#endif
#ifndef NOPRIORITY
  SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_BELOW_NORMAL);
#endif
}

/*
 * Cleanup Win32 structures and routines
 */
void Win32Cleanup(void) {
#ifndef NODNS
  WSACleanup();
#endif
}
#endif
