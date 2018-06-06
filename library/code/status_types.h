#pragma once

#ifndef     BOOLEAN
typedef unsigned __int8 BOOLEAN, *PBOOLEAN;
#endif

#ifndef STATUS
typedef int STATUS;
#endif // !STATUS

#ifndef NULL
#define NULL ((void *)0)
#endif

// Statuses values
// Generic
#define     STATUS_SUCCESS                          0
#define     STATUS_UNSUCCESSFUL                     -1
#define     STATUS_INVALID_PARAMETER_1              -2
#define     STATUS_INVALID_PARAMETER_2              -3
#define     STATUS_INVALID_PARAMETER_3              -4
#define     STATUS_INVALID_PARAMETER_4              -5

// Hash Table


#define     SUCCESS(Status)                         (0 == Status)