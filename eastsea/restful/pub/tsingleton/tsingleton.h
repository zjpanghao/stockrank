/*=============================================================================
#    Copyright (c) 2015
#    ShanghaiKunyan.  All rights reserved
#
#    Filename     : /home/tianyiheng_test/testproj/tsingleton/tsingleton.h
#    Author       : tianyiheng
#    Email        : tianyiheng@kunyan-inc.com
#    Date         : 2016-06-23 14:27
#    Description  : 
=============================================================================*/

#ifndef __PUB_PLUGIN_COMM_TSINGLETON_H__
#define __PUB_PLUGIN_COMM_TSINGLETON_H__

#include <assert.h>
#include <pthread.h>

namespace pub_singleton {

template<typename T>
class TSingleton {
  protected:
    TSingleton() {}
    ~TSingleton(){}

  public:
    static T* GetInstance();
    static void ReleaseInstance();

  private:
    TSingleton(const TSingleton& kObj){}
    TSingleton& operator=(const TSingleton& kObj){}

  private:
    static T *instance_ptr_;
    static pthread_mutex_t s_mutex_;
};

template<typename T>
T* TSingleton<T>::instance_ptr_ = NULL;

template<typename T>
pthread_mutex_t TSingleton<T>:: s_mutex_ = PTHREAD_MUTEX_INITIALIZER;

template<typename T>
T* TSingleton<T>::GetInstance() {
  if (NULL == instance_ptr_) {
    pthread_mutex_lock(&s_mutex_);
    if (NULL == instance_ptr_) {
      instance_ptr_ = new T;
      assert(NULL != instance_ptr_);
    }
    pthread_mutex_unlock(&s_mutex_);
  }
  return instance_ptr_;
}

template<typename T>
void TSingleton<T>::ReleaseInstance() {
  pthread_mutex_lock(&s_mutex_);
  if (NULL != instance_ptr_) {
    delete instance_ptr_;
    instance_ptr_ = NULL;
  }
  pthread_mutex_unlock(&s_mutex_);
}

}  // namespace pub_singleton

#endif  // __PUB_PLUGIN_COMM_TSINGLETON_H__
