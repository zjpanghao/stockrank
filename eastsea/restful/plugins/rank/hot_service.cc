#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <map>
#include "db_mysql.h"
#include "logic/logic_comm.h"
#include "hot_service.h"
#include "hot_cache.h"

#define UPDATE_HOUR 8
#define UPDATE_MIN 5

#define URL_FREQ_BUF_NUM  64
#define MAX_URL_BUFFER_LEN 80000
#define MAX_DELAY_SECONDS 3600
namespace rank {
RankHotService *RankHotService::service_ = NULL;
RankHotService* RankHotService::GetInstance() {
  if (service_ == NULL) {
    service_ = new RankHotService();
  }
  return service_;
}

bool RankHotService::UpdateData() {
  struct tm current;
  time_t now = time(NULL);
  localtime_r(&now, &current);
  if (current.tm_mday == day_ || 
      current.tm_hour < UPDATE_HOUR || 
     (current.tm_hour == UPDATE_HOUR && current.tm_min < UPDATE_MIN))
    return false;
  current.tm_hour = UPDATE_HOUR;
  current.tm_min = 0;
  current.tm_sec = 0;
  time_t close_end = mktime(&current);
  time_t open_end = close_end - UPDATE_HOUR*3600 - 9*3600;
  time_t open_start = close_end - UPDATE_HOUR * 3600 - 16 * 3600; 
  std::vector<StockHot> open_list;
  std::vector<StockHot> close_list;
  std::vector<StockHot> day_list;
  open_list.reserve(100);
  close_list.reserve(100);
  day_list.reserve(100);
  // [8:00---16:00]
  bool r = DbMysql::LoadTotalHot(open_start + 3600, open_end + 3600, &open_list);
  if (!r)
    return false;
  time_t close_start = open_end + 3600;
  // [16:00 --- 8:00]
  r = DbMysql::LoadTotalHot(close_start + 3600, close_end, &close_list);
  LOG_MSG2("Load rank close data %d", close_list.size());
  if(!r)
    return false;
  time_t day_start = close_end - UPDATE_HOUR*3600 - 24 *3600;
  
  // [0:00 ------8:00]
  r = DbMysql::LoadTotalHot(day_start + 3600, close_end, &day_list);
  pthread_rwlock_wrlock(&lock_);
  open_data_.set_list(open_list);
  open_data_.set_type(0);
    
  close_data_.set_list(close_list);
   close_data_.set_type(1);
  day_data_.set_list(day_list);
  day_data_.set_type(2);
  day_ = current.tm_mday;
  pthread_rwlock_unlock(&lock_);
  HotCache::GetInstance()->InvalidCache(RANK_KEY);
  LOG_MSG2("Update data ok %d", time(NULL));
  return true;
}

static std::string get_time_str(time_t t) {

  struct tm current_time;
  localtime_r(&t, &current_time);
	char buffer[64] = { 0 };
	sprintf(buffer, "%d-%02d-%02d %02d:%02d:%02d",
	        current_time.tm_year + 1900,
	        current_time.tm_mon + 1,
	        current_time.tm_mday,
	        current_time.tm_hour,
	        current_time.tm_min,
	current_time.tm_sec);
	return buffer;
}

}

