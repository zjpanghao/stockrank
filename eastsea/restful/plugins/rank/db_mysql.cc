// author: tianyiheng
// date: Mon Jun 20 16:50:31 2016
#include <stdlib.h>
#include "db_mysql.h"
#include <assert.h>
#include <mysql/mysql.h>
#include <map>
#include <sstream>
#include "base/db/base_db_mysql_auto.h"
#include "tools/tools.h"
#include "logic/logic_comm.h"
#include "db/db_config.h"

namespace rank {

// static member
plugin_pub_db::MySqlMultiPool * DbMysql::s_multi_db_pool_pt_ = NULL;

DbMysql::DbMysql() {

}

DbMysql::~DbMysql() {

}

void DbMysql::Dest() {
  // base_db::MysqlDBPool::Dest();
  if (NULL != s_multi_db_pool_pt_) {
    s_multi_db_pool_pt_->Release();
    delete s_multi_db_pool_pt_;     
    s_multi_db_pool_pt_ = NULL;     
  }
}

void DbMysql::Init(std::list<base::ConnAddr> *const addrlist) {
  // base_db::MysqlDBPool::Init(*addrlist);

}

void DbMysql::Init(std::string config_file) {
  plugin_pub::BaseConfig ini_config;
  if (!ini_config.LoadFile(config_file)) {                                 
    LOG_DEBUG2("DbMysql::Init,加载配置文件 [%s]失败", config_file.c_str());
  }                                                                        
  LOG_DEBUG2("DbMysql::Init,加载配置文件[%s]成功", config_file.c_str());   
  if (NULL == s_multi_db_pool_pt_) {                                       
    s_multi_db_pool_pt_ = new plugin_pub_db::MySqlMultiPool;               
    assert(NULL != s_multi_db_pool_pt_);                                   
  }                                                                        
  plugin_pub_db::MySqlConfig mysql_config;                                 
  std::string s;                                                           
  int n = 0;
  // 初始化连接池1
  s = ini_config.GetParam<std::string>("mysql1", "host");                             
  mysql_config.set_host(s);                                                           

  n = ini_config.GetParam<int>("mysql1", "port");                                     
  mysql_config.set_port(n);                                                           

  s = ini_config.GetParam<std::string>("mysql1", "user");                             
  mysql_config.set_user(s);                                                           

  s = ini_config.GetParam<std::string>("mysql1", "pswd");                             
  mysql_config.set_pswd(s);                                                           

  s = ini_config.GetParam<std::string>("mysql1", "dbname");                           
  mysql_config.set_dbname(s);                                                         

  n = ini_config.GetParam<int>("mysql1", "pool_size");                                
  mysql_config.set_pool_size(n);
  LOG_DEBUG2("mysql1，host:%s, port:%d, user:%s, pswd:%s, pool_size:%d", \            
       mysql_config.host().c_str(), mysql_config.port(), mysql_config.user().c_str(),\ 
       mysql_config.pswd().c_str(), mysql_config.pool_size());                         
  // 初始化pool1                                                                      
  s_multi_db_pool_pt_->InitPool(1, mysql_config);

  // 初始化连接池2
  s = ini_config.GetParam<std::string>("mysql2", "host");                             
  mysql_config.set_host(s);                                                           

  n = ini_config.GetParam<int>("mysql2", "port");                                     
  mysql_config.set_port(n);                                                           

  s = ini_config.GetParam<std::string>("mysql2", "user");                             
  mysql_config.set_user(s);                                                           

  s = ini_config.GetParam<std::string>("mysql2", "pswd");                             
  mysql_config.set_pswd(s);                                                           

  s = ini_config.GetParam<std::string>("mysql2", "dbname");                           
  mysql_config.set_dbname(s);                                                         

  n = ini_config.GetParam<int>("mysql2", "pool_size");                                
  mysql_config.set_pool_size(n);
  LOG_DEBUG2("mysql2，host:%s, port:%d, user:%s, pswd:%s, pool_size:%d", \            
       mysql_config.host().c_str(), mysql_config.port(), mysql_config.user().c_str(),\ 
       mysql_config.pswd().c_str(), mysql_config.pool_size());                         
  // 初始化pool2                                                                      
  s_multi_db_pool_pt_->InitPool(2, mysql_config);
}
//
bool DbMysql::LoadTotalHot(time_t start_stamp, time_t end_stamp,
                           std::vector<StockHot> *record_list) {
#ifndef KUN_DB_POOL
  AUTO_MYSQL_POOL_ENGINE_USE(s_multi_db_pool_pt_,2, engine);
#else
  base_db::AutoMysqlCommEngine  auto_engine(base_db::KunDBPool::GetInstance(), KUN_MYSQL_POOL_KEY1);
  base_storage::DBStorageEngine* engine = auto_engine.get_engine();
#endif
  if (NULL == engine) {
    LOG_MSG("DbMysql::QueryHot, NULL == engine");  
    return false;
  }
  std::stringstream os;
  os << "select stock_code, sum(count) as total , b.name from stock_visit_history_hour_month"
     << " as a inner join stock_basic as b on a.stock_code = b.code where timeTamp >= " << start_stamp
     << " and timeTamp <= " << end_stamp << " group by stock_code order by total desc , stock_code limit " << MAX_RANK;
  std::string str = os.str();
  LOG_MSG2("The sql %s", str.c_str());
  StockHot record;
  std::string code;

  bool r = engine->SQLExec(str.c_str());
  if (!r) {
    LOG_ERROR("DbMysql::Queryhottocks failed");
    return false;
  }
  int record_num = engine->RecordCount();
  MYSQL_ROW row_ret;
  for (int i = 0; i < record_num; ++i) {
    row_ret = *(reinterpret_cast<MYSQL_ROW*>(engine->FetchRows()->proc));
    if (NULL != row_ret) {
     if (row_ret[0] == NULL)
       continue;
     record.stock_code = row_ret[0];
     if (row_ret[2] == NULL) 
       continue;
      record.name = row_ret[2];
      // LOG_MSG2("push back (%s,%s)", record.stock_code.c_str(), record.name.c_str());
      record_list->push_back(record);
    }

  }
  return true;
}

}  // namespace rank




