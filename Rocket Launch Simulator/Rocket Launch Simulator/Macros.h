#pragma once
#include <stdlib.h>	

#define SAFE_FREE(ptr) if (ptr != NULL) { free(ptr); }
