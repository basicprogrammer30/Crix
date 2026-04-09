/*
 * Copyright (C) 2026 First Person
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <stdint.h>
#include <cga.h>

void kinit() {
    print("Hello Crix!", CGA_COLOR_BLACK, CGA_COLOR_WHITE);
    while(1);
}