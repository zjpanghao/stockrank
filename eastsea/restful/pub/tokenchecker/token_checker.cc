/*=============================================================================
#    Copyright (c) 2015
#    ShanghaiKunyan.  All rights reserved
#
#    Filename     : /home/tyh/project/wookong/northsea/restful/plugins/eventinfo/token_checker.cc
#    Author       : Tianyiheng
#    Email        : tianyiheng@kunyan-inc.com
#    Date         : 2016-07-26 09:29
#    Description  : 
=============================================================================*/
#include <sstream>
#include <list>
#include "./token_checker.h" 
#include "tools/tools.h"
#include "logic/logic_comm.h"
namespace plugin_pub_tool {

TokenChecker::TokenChecker() {
  LOG_MSG2("This token %x", this);  
}

TokenChecker::~TokenChecker() {
  
}

void TokenChecker::InitFromBaseConfig(std::string const& config_file) {
  plugin_pub::BaseConfig ini_config;
  if (!ini_config.LoadFile(config_file)) {                                 
    LOG_DEBUG2("加载配置文件 [%s]失败", config_file.c_str());
  }                                                                        
  std::string host, user, pswd, dbname;                                                           
  int port = 0;
  host = ini_config.GetParam<std::string>("token_redis", "host"); 
  user = ini_config.GetParam<std::string>("token_redis", "user"); 
  pswd = ini_config.GetParam<std::string>("token_redis", "pswd"); 
  dbname = ini_config.GetParam<std::string>("token_redis", "dbname"); 
  port = ini_config.GetParam<int>("token_redis", "port"); 
  size_t pool_size = ini_config.GetParam<size_t>("token_redis", "pool_size");

  base::ConnAddr redis_addr("0", host, port, user, pswd, dbname);
   
  std::list<base::ConnAddr> list_addr;
  list_addr.push_back(redis_addr);
  Init(&list_addr, pool_size);


  LOG_DEBUG2("TokenChecker,加载配置文件[%s]成功,redis,host:%s, user:%s, pswd:%s, port:%d, db_name:%s", \
      config_file.c_str(), host.c_str(), user.c_str(), pswd.c_str(), port, dbname.c_str());
}


void TokenChecker::Init(std::list<base::ConnAddr> *const addrlist, size_t num) {
  LOG_MSG2("%s", "Token initi");
  base_dic::RedisPool::Init(*addrlist, num);
}

std::string TokenChecker::GetUserToken(int64 user_id) {
  std::string token_saved = "0";
  // TODO : 
  // get user's token from redis

  base_dic::AutoDicCommEngine auto_engine;
  base_storage::DictionaryStorageEngine *redis = auto_engine.GetDicEngine();

  if (NULL == redis) {
    return token_saved;
  }
  std::string table_name = "user_token";
  std::string key_name = "0";
  std::stringstream ss;
  ss << user_id;
  ss >> key_name;
  ss.str("");
  ss.clear();

  char * val = NULL;
  size_t val_len = 0;
  redis->GetHashElement(table_name.c_str(), key_name.c_str(), \
      key_name.length(), &val, &val_len);
  if (NULL != val) {
    token_saved = val;
    free(val);
  }
  return token_saved;
}

void TokenChecker::SaveUserToken(int64 user_id, std::string const& user_token) {
  base_dic::AutoDicCommEngine auto_engine;
  base_storage::DictionaryStorageEngine *redis = auto_engine.GetDicEngine();
  if (NULL == redis) {
    return;
  }
  std::string table_name = "user_token";
  std::string key_name;
  std::stringstream ss;
  ss << user_id;
  ss >> key_name;
  ss.clear();
  ss.str("");

  redis->SetHashElement(table_name.c_str(), key_name.c_str(), \
      key_name.length(), user_token.c_str(), user_token.length());
  LOG_DEBUG2("SaveUserToken, user_id:%lld, token:%s", user_id, user_token.c_str());
}

TokenCheckStatus TokenChecker::CheckUserToken(int64 user_id, std::string const& token) {
  std::string token_saved = GetUserToken(user_id);
  if (token != token_saved) {
    LOG_DEBUG2("token被替换,token_saved:%s, cur token:%s", token_saved.c_str(), token.c_str()); 
    return ETOKEN_CHANGED;
  }
  // token一样，验证合法性和实效性
  std::string  now_token = token;
  if (!tools::CheckToken(user_id, now_token)) {
    LOG_DEBUG("token失效或者token格式不合法");
    return ETOKEN_INVALID;
  }

  return ETOKEN_NORMAL;
}

void TokenChecker::Release() {
  base_dic::RedisPool::Dest(); 
}

}  // namespace plugin_pub_tool
