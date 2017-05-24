#ifndef HOT_RANK_HOT_SERVICE_H
#define  HOT_RANK_HOT_SERVICE_H
#include <pthread.h>
#include <string>
#include <list>
#include <vector>
#include "db_mysql.h"
namespace rank {
class RankData {
 public:
  std::string date() const {
    return date_;
  }
  void set_type(int type) {
    type_ = type;
  }
  int type() const {return type_;}

  std::vector<StockHot> data_list() const  {
    return data_list_;
  }
  
  void set_list(std::vector<StockHot> data_list) {
    data_list_ = data_list;
  }

  std::vector<StockHot> GetRank(int index, int num) {
    std::vector<StockHot> vec;
    if (index < data_list_.size()) {
      vec.assign(data_list_.begin() + index, 
          index + num < data_list_.size() ? data_list_.begin() + index + num : data_list_.end());
    }
    return vec;
  }

 private:
  std::string date_;
  int type_;
  std::vector<StockHot> data_list_;
};

struct RankQuery{
  RankData open;
  RankData close;
  RankData day;
};

class RankHotService {
 public:
  RankHotService() {
    pthread_rwlock_init(&lock_, NULL);
  }
  static RankHotService* GetInstance();
  
  bool UpdateData();
  
  RankQuery  GetRankQuery(int page, int num, int table) {
    RankQuery query ;
    pthread_rwlock_rdlock(&lock_);
    switch(table) {
      case 1:
        query.open.set_list(open_data_.GetRank(page * num, num));
        break;
      case 2:
        query.close.set_list(close_data_.GetRank(page * num, num));
        break;
      case 3:
        query.day.set_list(day_data_.GetRank(page * num, num));
        break;
      default:
        query.open = open_data_;
        query.close = close_data_;
        query.day = day_data_;
        break;
    }
    pthread_rwlock_unlock(&lock_);
    return query;
  }

 private:
  pthread_rwlock_t  lock_;
  RankData  open_data_;
  RankData  close_data_;
  RankData  day_data_;
  static RankHotService *service_;
  int day_;
};
}
#endif
