#ifndef INCLUDE_RANK_HOT_CACHE
#define INCLUDE_RANK_HOT_CACHE

#include <pthread.h>
#include <map>
#include <string>
#define RANK_KEY "rank"
namespace rank {
class HotCache {
 public:
  bool GetCache(std::string key, std::string *value) {
    pthread_rwlock_rdlock(&lock_);
    std::map<std::string, std::string>::iterator it
        = value_map_.find(key);
    if (it != value_map_.end()) {
      *value = it->second;
    }
    pthread_rwlock_unlock(&lock_);
    return true;
  }
  
  bool InvalidCache(std::string key) {
    pthread_rwlock_wrlock(&lock_);
    value_map_.erase(key);
    pthread_rwlock_unlock(&lock_);
  }

  bool UpdateCache(std::string key, const std::string &value) {
    pthread_rwlock_wrlock(&lock_);
    value_map_[key] = value;
    pthread_rwlock_unlock(&lock_);
    return true;
  } 

  static HotCache* GetInstance() {
    if (instance_ == NULL) {
      instance_ = new HotCache();
    }
    return instance_;
  }

 private:
  HotCache() {
    pthread_rwlock_init(&lock_, NULL);
  }
  static HotCache *instance_;
  std::map<std::string, std::string> value_map_;
  pthread_rwlock_t lock_;
};

}
#endif
