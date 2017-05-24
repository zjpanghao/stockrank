//  Copyright (c) 2015-2015 The restful Authors. All rights reserved.
//  Created on: 2015/11/24 Author: jiaoyongqing

/*
 *  stock_comm_head.h
 *
 *  Created on: 2015/10/22
 *  Author: jiaoyongqing
 */

#ifndef _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_NET_RANK_COMM_HEAD_H_
#define _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_NET_RANK_COMM_HEAD_H_

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <sstream>

#include "net/http_data.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "net/error_comm.h"

namespace netcomm_recv {
class RecvTrendMap:public RecvPacketBase{
public:
 explicit RecvTrendMap(NetBase* m)
 :RecvPacketBase(m) {
   init();
 }

 inline void init() {
   bool r = false;
   r = m_->GetBigInteger(L"user_id", static_cast<int64*>(&user_id_));
   if (false == r) {
     error_code_ = USER_ID_LACK;
   }

   r = m_->GetString(L"token", &token_);
   if (false == r) {
     error_code_ = TOKEN_LACK;
   }

   r = m_->GetBigInteger(L"start", static_cast<int64*>(&start_));
   if (false == r) {
     error_code_ = START_LACK;
   }

   r = m_->GetBigInteger(L"end", static_cast<int64*>(&end_));
   if (false == r) {
     error_code_ = END_LACK;
   }
 }

 int64 user_id() const { return user_id_; }

 std::string token() const { return token_; }

 int64 start() const { return start_;}

 int64 end() const { return end_; }

private:
 int64 user_id_;

 std::string token_;

 int64 start_;

 int64 end_;
};

class RecvStockTransfer:public RecvPacketBase{
public:
	explicit RecvStockTransfer(NetBase* m):RecvPacketBase(m)
	{
		init();
	}

	inline void init()
	{
		bool r = false;
		r = m_->GetBigInteger(L"user_id", static_cast<int64*>(&user_id_));
		if (false == r) {
			error_code_ = USER_ID_LACK;
		}

		r = m_->GetString(L"token", &token_);
		if (false == r) {
			error_code_ = TOKEN_LACK;
		}

		r = m_->GetBigInteger(L"position_id", static_cast<int64*>(&position_id_));
		if (false == r) {
			error_code_ = STOCK_POSITION_ID_LACK;
		}
	}

 int64 user_id() const { return user_id_; }

 std::string token() const { return token_; }

 int64 position_id() const {return position_id_;}


private:
 int64 user_id_;

 std::string token_;

 int64 position_id_;
};

// 行业占比
class RecvIndustryPercent : public RecvLoginBase {
 public:
  explicit RecvIndustryPercent(NetBase *m)
    :RecvLoginBase(m) {
    Init(); 
  }
  std::string strategy_name() {return strategy_name_;}
 
 protected:
  inline void Init() {
    bool ret = false;
    std::string tmp;
    ret = m_->GetString(L"strategy", &tmp);
    // url decode
    base::BasicUtil::UrlDecode(tmp, strategy_name_); 
    if (!ret) {
      error_code_ = STRATEGY_LACK;  
    }
    
  }

 private:
   std::string strategy_name_;
};
//策略简介 收包
class RecvStrategyBrief : public RecvPacketBase 
{
public:
  explicit RecvStrategyBrief(NetBase* m):RecvPacketBase(m)
  {
    init();
  }

  inline void init()
  {
    bool r = false;
    r = m_->GetBigInteger(L"user_id", &user_id_);
    if (false == r) {
      error_code_ = USER_ID_LACK;
      return;
    }

    r = m_->GetString(L"token", &token_);
    if (false == r) {
      error_code_ = TOKEN_LACK;
      return;
    }

    r = m_->GetString(L"strategy_name", &strategy_name_);
    if (false == r) {
      error_code_ = STRATEGY_LACK;
      return;
    }
  }

  int64 user_id() const { return user_id_; }

  std::string token() const { return token_; }

  std::string strategy_name() const {return strategy_name_;}


private:
  int64 user_id_;

  std::string token_;

  std::string strategy_name_;
};

// 策略首页 top 6
class RecvStrategyTop : public RecvLoginBase {
 public:
  explicit RecvStrategyTop(NetBase *kv) : RecvLoginBase(kv) {
    Init();
  } 

  inline int64 top_num() {return top_num_;}
  inline int64 start_time() {return start_time_;}
  inline int64 end_time() {return end_time_;}

 protected:
  void Init() {
    bool ret = false;
    ret = m_->GetBigInteger(L"top_num", &top_num_);
    if (6 > top_num_) {
      top_num_ = 6;
    }
    if (!ret) {
      error_code_ = STRUCT_ERROR;
    }
    ret = m_->GetBigInteger(L"start", &start_time_);
    ret = m_->GetBigInteger(L"end", &end_time_);
  }

  int64 top_num_;
  int64 start_time_;
  int64 end_time_;
};

}  //  namespace netcomm_recv

namespace netcomm_send {
class SendTrendMap:public SendPacketBase {
 public:
  SendTrendMap() {
    result_.reset( new base_logic::ListValue() );
  }

  NetBase* release() {
    head_->Set("result", result_.release());
    this->set_status(1);
    return head_.release();
  }

  inline void set_result(base_logic::DictionaryValue* value) {
    result_->Append(value);
  }

 private:
  scoped_ptr<base_logic::ListValue> result_;
};

// 行业占比
class SendIndustryPercent : public SendPacketBase {
 public:
   explicit SendIndustryPercent(){
     val_list_.reset(new base_logic::ListValue);
   }    
   inline NetBase* release() {
     this->set_status(1);
     //head_->Set("result", val_list_.release());
     MapIndustryStock::iterator it = map_indus_list_stock_.begin();
     int idx = 1;
     for (; it != map_indus_list_stock_.end(); ++it, ++idx) {
       std::stringstream os_key;
       NetBase* kv = new NetBase;
       kv->SetString(L"indus_type", it->first);
       kv->Set("stocklist", it->second);
       os_key << "classify" << idx;
       head_->Set(os_key.str().c_str(), kv);  
     }
     return head_.release();
   }
   inline void AddStockPercentage(base_logic::Value* kv) {
     if (NULL == kv) {
       return;
    }
     val_list_->Append(kv);
   }
   typedef scoped_ptr<base_logic::ListValue> ListValuePtr;
   typedef std::map<std::string, base_logic::ListValue*> MapIndustryStock;
   inline void AddIndusStock(std::string indus, \
       base_logic::Value* kv) {
     MapIndustryStock::iterator it = map_indus_list_stock_.find(indus);
     if (it == map_indus_list_stock_.end()) {
       base_logic::ListValue* stock_list_ptr = new base_logic::ListValue;
       stock_list_ptr->Append(kv);
       map_indus_list_stock_.insert(std::make_pair(indus, stock_list_ptr)); 
     }
     else {
       it->second->Append(kv);            
     }
   }
 private:

  scoped_ptr<base_logic::ListValue> val_list_;
  MapIndustryStock map_indus_list_stock_;
};

class SendStockTransfer:public SendPacketBase {
  public:
    SendStockTransfer() {
      result_.reset( new base_logic::ListValue() );
    }

    NetBase* release() {
      head_->Set("result", result_.release());
      this->set_status(1);
      return head_.release();
    }
  
    inline void set_result(base_logic::DictionaryValue* value) {
      result_->Append(value);
    }

 private:
    scoped_ptr<base_logic::ListValue> result_;
};

// 策略简介 发包
class SendStrategyBrief : public SendPacketBase {
public:
  SendStrategyBrief() {
    strategy_brief_.reset(new base_logic::DictionaryValue);
  }

  virtual ~SendStrategyBrief() {
  }
  inline NetBase* release() {
    head_->Set(L"strategy_brief", strategy_brief_.release());
    this->set_status(1);
    return head_.release();
  }

  inline void set_real(const std::string key, double value) {
    strategy_brief_->SetReal(key, value);
  }

  inline void set_string(const std::string key, const std::string& value) {
    strategy_brief_->SetString(key, value);
  }
 private:
  scoped_ptr<base_logic::DictionaryValue> strategy_brief_;
};

// 策略榜 首页 top 6
class SendStrategyTop : public SendPacketBase {
 public:
  SendStrategyTop() {
    val_list_.reset(new base_logic::ListValue);
  }
  virtual ~SendStrategyTop() {
  }
  inline NetBase* release() {
    head_->Set(L"result", val_list_.release());
    this->set_status(1);
    return head_.release();
  }
  void AddStrategyInfo(base_logic::DictionaryValue* kv) {
    val_list_->Append(kv);
  }
 protected:
  scoped_ptr<base_logic::ListValue> val_list_;
};

}  //  namespace netcomm_send
#endif  //  _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_NET_STOCK_COMM_HEAD_H_
