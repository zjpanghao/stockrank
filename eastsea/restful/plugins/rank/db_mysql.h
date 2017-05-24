// author: tianyiheng
// date: Mon Jun 20 16:50:31 2016

#ifndef _PLUGINS_HOTRECORD_DB_MYSQL_H_
#define _PLUGINS_HOTRECORD_DB_MYSQL_H_

#include <list>
#include <map>
#include <set>
#include <string>
#include "public/config/config_info.h"
#include "baseconfig/base_config.h" 
#include "sqldbpool/mysql_db_pool.h"
#define MAX_RANK 20
namespace rank {
struct StockHot {
 int hot;
 std::string stock_code;
 std::string name;
};
class DbMysql {
public:
  DbMysql();
  ~DbMysql();

  static void Init(std::list<base::ConnAddr>* const addrlist);
  static void Init(std::string config_file);
  static void Dest();
  // 
  static bool LoadTotalHot(time_t start_stamp, time_t end_stamp,
                    std::vector<StockHot> *record_list);

protected:
  static plugin_pub_db::MySqlMultiPool *s_multi_db_pool_pt_; 
};

}
#endif // PLUGINS_HOTRECORD_DB_MYSQL_H_ 
