#pragma once

#include <stdio.h>	
#include <stdbool.h>

bool setup_dump_file();

void write_to_dump_file(const char* format, ...);

void close_dump_file();
