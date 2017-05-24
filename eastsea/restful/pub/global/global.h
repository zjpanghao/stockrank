// Copyright (c) 2015-2015 The restful Authors. All rights reserved.
// Created on: 2015/11/24 Author: jiaoyongqing

#ifndef _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_GLOBAL_GLOBAL_H_
#define _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_GLOBAL_GLOBAL_H_

#include<mysql.h>
#include<string>
#include <algorithm>

#include "net/typedef.h"
#include "db/db_comm.h"
#include "tools/tools.h"
#include "logic/logic_comm.h"

namespace global {
class GetInfo {
 public:
  GetInfo() {}
  virtual ~GetInfo() {}

  void init() {
    db::DbSql sql;
    sql.GetAllNameCode();
    MYSQL_ROW rows;
    while (rows = sql.NextRecord()) {
      Stock stock;
      if (rows[0]) {
        stock.stock_code_ = rows[0];
      }
      if (rows[1]) {
        stock.stock_name_ = rows[1];
      }
      if (rows[2]) {
        stock.jp_ = rows[2];
      }
      if (rows[3]) {
        stock.qp_ = rows[3];
      }
      stock_vec_.push_back(stock);
    }

    sql.GetHy();
    while (rows = sql.NextRecord()) {
      Hy hy;
      if (rows[0]) {
        hy.name_ = rows[0];
      }
      if (rows[1]) {
        hy.jp_ = rows[1];
      }
      if (rows[2]) {
        hy.qp_ = rows[2];
      }
      hy_vec_.push_back(hy);
    }    

    sql.GetGn();
    while (rows = sql.NextRecord()) {
      Gn gn;
      if (rows[0]) {
        gn.name_ = rows[0];
      }
      if (rows[1]) {
        gn.jp_ = rows[1];
      }
      if (rows[2]) {
        gn.qp_ = rows[2];
      }
      gn_vec_.push_back(gn);
    }  

    sql.GetEvent();
    while (rows = sql.NextRecord()) {
      Event event;
      if (rows[0]) {
        event.name_ = rows[0];
      }
      if (rows[1]) {
        event.jp_ = rows[1];
      }
      if (rows[2]) {
        event.qp_ = rows[2];
      }
      event_vec_.push_back(event);
    }

  } // Init
 
  void Rest() { // events_total 每小时更新一次
    LOG_DEBUG("update search memory begin");
    stock_vec_.clear();
    hy_vec_.clear();
    gn_vec_.clear();
    event_vec_.clear();
    init();
    LOG_DEBUG2("update search memory end, result:%d %d %d %d",stock_vec_.size(), hy_vec_.size(), gn_vec_.size(), event_vec_.size());
  }
 
  static GetInfo* GetInstance() {
    if (NULL == instance_) {
      instance_ = new GetInfo();
      instance_->init();
    }
    return instance_;
  }

  struct Stock {
    std::string stock_code_;
    std::string stock_name_;
    std::string qp_;
    std::string jp_;
  };
  struct Hy {
    std::string name_;
    std::string qp_;
    std::string jp_;
  };
  struct Gn {
    std::string name_;
    std::string qp_;
    std::string jp_;
   };
  struct Event{
    std::string name_;
    std::string qp_;
    std::string jp_;
  };
 
  std::vector<Stock> stock_vec_;
  std::vector<Hy> hy_vec_;
  std::vector<Gn> gn_vec_;
  std::vector<Event> event_vec_; 

  inline std::vector<Stock> get_stock() { return stock_vec_; }
  inline std::vector<Hy> get_hy() { return hy_vec_; }
  inline std::vector<Gn> get_gn() { return gn_vec_; }
  inline std::vector<Event> get_event() { return event_vec_; }

 private:
  static GetInfo *instance_;
};
}  // namespace global

#endif  //  _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_GLOBAL_GLOBAL_H_
