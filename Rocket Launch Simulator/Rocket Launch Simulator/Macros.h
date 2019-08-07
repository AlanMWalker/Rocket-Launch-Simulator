#pragma once
#include <stdlib.h>	

#define SAFE_FREE(ptr) if (ptr != NULL) { delete ptr; }
#define SAFE_FREE_ARRAY(ptr) if (ptr != NULL) { delete[] ptr; }
