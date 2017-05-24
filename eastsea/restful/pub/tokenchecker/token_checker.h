/*=============================================================================
#    Copyright (c) 2015
#    ShanghaiKunyan.  All rights reserved
#
#    Filename     : /home/tyh/project/wookong/northsea/restful/plugins/eventinfo/token_checker.h
#    Author       : Tianyiheng
#    Email        : tianyiheng@kunyan-inc.com
#    Date         : 2016-07-26 09:24
#    Description  : 
=============================================================================*/

#ifndef __WOOKONG_PLUGIN_PUB_TOKEN_CHECKER_H__
#define __WOOKONG_PLUGIN_PUB_TOKEN_CHECKER_H__

#include <string>

//#include "public/config/config_info.h"
#include "public/config/config_info.h"
#include "base/dic/base_dic_redis_auto.h"
#include "basic/basictypes.h"
#include "logic/logic_comm.h"
#include "baseconfig/base_config.h"
#include "net/error_comm.h"

namespace plugin_pub_tool {

enum TokenCheckStatus {
  ETOKEN_NORMAL = 0,
  ETOKEN_CHANGED = 1,
  ETOKEN_INVALID = 2
};

class TokenChecker {
 protected:
   TokenChecker();
   ~TokenChecker(); 
 public:
  static void InitFromBaseConfig(std::string const& config_file);
  static void Init(std::list<base::ConnAddr> *const addrlist, size_t num = 4);
  static std::string GetUserToken(int64 user_id);
  static void SaveUserToken(int64 user_id, std::string const& user_token);

  static TokenCheckStatus CheckUserToken(int64 user_id, std::string const& token);
  static void Release();
};



} // namespace plugin_pub_tool

#endif  // __WOOKONG_PLUGIN_PUB_TOKEN_CHECKER_H__
