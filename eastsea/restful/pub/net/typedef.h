//  Copyright (c) 2015-2015 The restful Authors. All rights reserved.
//  Created on: 2015/11/24 Author: jiaoyongqing

#ifndef _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_NET_TYPEDEF_H_
#define _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_NET_TYPEDEF_H_

#include <vector>
#include <map>
#include <string>
#include "basic/basictypes.h"

typedef std::map<std::string, std::string> MapStrToStr;

typedef std::map<std::string, int64> MapStrToInt64;

typedef std::vector<std::string> ContainerStr;

typedef std::vector<int64> ContainerInt;


#define REDIS_TIME  53

#define ALL "all"

#define PAGE_SIZE 10

#define REDIS_VISIT "visit"

#define REDIS_SEARCH "search"

#define REDIS_FOLLOW "add_follow"

#define REDIS_COUNT "count"

enum UnitType {PERCENT, COUNT, PERCENT_COUNT};

enum Rtype {VISIT = 1, SEARCH, FOLLOW};

enum Order {UP, DOWN};

enum OperateType {DELETE = 1, SAVE, GET, MODIFY};

#endif  //  _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_NET_TYPEDEF_H_
