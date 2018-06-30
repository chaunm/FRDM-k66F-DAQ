/**
 * @file os_port_ucos3.h
 * @brief RTOS abstraction layer (Micrium uC/OS-III)
 *
 * @section License
 *
 * Copyright (C) 2010-2018 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Eval.
 *
 * This software is provided in source form for a short-term evaluation only. The
 * evaluation license expires 90 days after the date you first download the software.
 *
 * If you plan to use this software in a commercial product, you are required to
 * purchase a commercial license from Oryx Embedded SARL.
 *
 * After the 90-day evaluation period, you agree to either purchase a commercial
 * license or delete all copies of this software. If you wish to extend the
 * evaluation period, you must contact sales@oryx-embedded.com.
 *
 * This evaluation software is provided "as is" without warranty of any kind.
 * Technical support is available as an option during the evaluation period.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.2
 **/

#ifndef _OS_PORT_UCOS3_H
#define _OS_PORT_UCOS3_H

//Dependencies
#include "os.h"

//Task priority (normal)
#ifndef OS_TASK_PRIORITY_NORMAL
   #define OS_TASK_PRIORITY_NORMAL (OS_CFG_PRIO_MAX - 2)
#endif

//Task priority (high)
#ifndef OS_TASK_PRIORITY_HIGH
   #define OS_TASK_PRIORITY_HIGH (OS_CFG_PRIO_MAX - 3)
#endif

//Maximum number of tasks that can be dynamically created
#ifndef OS_PORT_MAX_TASKS
   #define OS_PORT_MAX_TASKS 16
#elif (OS_PORT_MAX_TASKS < 1)
   #error OS_PORT_MAX_TASKS parameter is not valid
#endif

//Milliseconds to system ticks
#ifndef OS_MS_TO_SYSTICKS
   #define OS_MS_TO_SYSTICKS(n) (n)
#endif

//System ticks to milliseconds
#ifndef OS_SYSTICKS_TO_MS
   #define OS_SYSTICKS_TO_MS(n) (n)
#endif

//Enter interrupt service routine
#define osEnterIsr() OSIntEnter()

//Leave interrupt service routine
#define osExitIsr(flag) OSIntExit()

//C++ guard
#ifdef __cplusplus
   extern "C" {
#endif


/**
 * @brief Task object
 **/

typedef OS_TCB OsTask;


/**
 * @brief Event object
 **/

typedef OS_FLAG_GRP OsEvent;


/**
 * @brief Semaphore object
 **/

typedef OS_SEM OsSemaphore;


/**
 * @brief Mutex object
 **/

typedef OS_MUTEX OsMutex;


/**
 * @brief Task routine
 **/

typedef void (*OsTaskCode)(void *param);


//Kernel management
void osInitKernel(void);
void osStartKernel(void);

//Task management
bool_t osCreateStaticTask(OsTask *task, const char_t *name, OsTaskCode taskCode,
   void *param, void *stack, size_t stackSize, int_t priority);

OsTask *osCreateTask(const char_t *name, OsTaskCode taskCode,
   void *param, size_t stackSize, int_t priority);

void osDeleteTask(OsTask *task);
void osDelayTask(systime_t delay);
void osSwitchTask(void);
void osSuspendAllTasks(void);
void osResumeAllTasks(void);

//Event management
bool_t osCreateEvent(OsEvent *event);
void osDeleteEvent(OsEvent *event);
void osSetEvent(OsEvent *event);
void osResetEvent(OsEvent *event);
bool_t osWaitForEvent(OsEvent *event, systime_t timeout);
bool_t osSetEventFromIsr(OsEvent *event);

//Semaphore management
bool_t osCreateSemaphore(OsSemaphore *semaphore, uint_t count);
void osDeleteSemaphore(OsSemaphore *semaphore);
bool_t osWaitForSemaphore(OsSemaphore *semaphore, systime_t timeout);
void osReleaseSemaphore(OsSemaphore *semaphore);

//Mutex management
bool_t osCreateMutex(OsMutex *mutex);
void osDeleteMutex(OsMutex *mutex);
void osAcquireMutex(OsMutex *mutex);
void osReleaseMutex(OsMutex *mutex);

//System time
systime_t osGetSystemTime(void);

//Memory management
void *osAllocMem(size_t size);
void osFreeMem(void *p);

//Undefine conflicting definitions
#undef TRACE_LEVEL_OFF
#undef TRACE_LEVEL_INFO

//C++ guard
#ifdef __cplusplus
   }
#endif

#endif
