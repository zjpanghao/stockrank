//  Copyright (c) 2015-2015 The restful Authors. All rights reserved.
//  Created on: 2015/11/24 Author: jiaoyongqing
//
#ifndef _NET_OPERATOR_CODE_H_
#define _NET_OPERATOR_CODE_H_
#define RANK_XIAO_PATH "/var/www/tmp/rankxiao"
enum netoperatorcode{
  // user
  USER_LOGIN                      = 1001,
  GET_PLATFORM_INFO               = 1002,
  VERSION_UPDATE_NOTICE           = 1003,

  // stock
  ASK_FOR_TOP_TWENTY_STOCK        = 1004,
  ASK_FOR_REALTIME_HOT            = 1005,
  ASK_FOR_SINGLE_REAL_TIME_HOT    = 1008,
  ASK_FOR_HY_OR_GN                = 1009,
  ASK_FOR_TRADE_PRICE             = 1010,
  STOCK_GROUP                     = 1011,
  STOCK_CURVE                     = 1012,
  STOCK_GRAIL                     = 1013,
  EVENT_DIGEST                    = 1014,

  // strategy
  TREND_MAP                       = 1200,
  STRATEGY_INDUS_PERCET           = 1201,
  STOCK_TRANSFER                  = 1300,
  STRATEGY_BRIEF                  = 1400,
  STRATEGY_TOP                    = 1401,
  // information
  RELATED_INFO                    = 1402,
  INFO_DETAIL                     = 1403,
  STOCK_BASE_RELATED              = 1404,
  RELATE_SHG = 1405,                        // 关联的股票，行业，概念, key 也为 股票，行业，概念

  SEARCH_FUN                      = 2001,

  // hot_event
  HE_VP                           = 3001,

  HOT_RECORD                      = 1501,   // 热度记录数据
  HOT_UPDATE                      = 1502,   // 手动补充数据
  ALL_INFO_PLATFORM               = 15002,  // 获取所有资讯平台列表
  CUSTOM_INFO_PLATFORM            = 15003,  // 自定义资讯平台列表
  QUERY_CUSTOM_COMBINATION_INFO   = 15004,  // 获取自定义组合相关的资讯
  EVENT_RELATE_INFO               = 15005,  // 事件关联的资讯
  INFO_SENTI_TREND                = 15006,  // 资讯情感趋势
};


#endif
