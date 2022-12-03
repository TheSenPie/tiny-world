#ifndef UTIL_H
#define UTIL_H

#define TICKRATE 60

#define PACKED __attribute__((__packed__))

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"

#include <cglm/cglm.h>
#include <cglm/struct.h>

#pragma GCC diagnostic pop

#include "fmath.h"
#include "time.h"
#include "direction.h"
#include "types.h"
#include "color.h"

#endif