//
// Created by poilk on 2024/2/9.
//

#ifndef PEMO_LOG_H
#define PEMO_LOG_H

#include <stdio.h>
#include "assert.h"

#define PEMO_LOG_BY_LEVEL(args...) \
  printf(args);                    \
  putchar('\n');
#define PEMO_LOG_DEBUG(args...) PEMO_LOG_BY_LEVEL(args)
#define PEMO_LOG_INFO(args...) PEMO_LOG_BY_LEVEL(args)
#define PEMO_LOG_WARN(args...) PEMO_LOG_BY_LEVEL(args)
#define PEMO_LOG_ERROR(args...) PEMO_LOG_BY_LEVEL(args)

#endif  // PEMO_LOG_H
