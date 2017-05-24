//  Copyright (c) 2015-2015 The restful Authors. All rights reserved.
//  Created on: 2015/11/24 Author: jiaoyongqing

/*
 *  stock_comm_head.h
 *
 *  Created on: 2015/10/22
 *  Author: jiaoyongqing
 */

#ifndef _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_NET_STOCK_COMM_HEAD_H_
#define _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_NET_STOCK_COMM_HEAD_H_

#include <list>
#include <string>
#include <sstream>
#include <map>

#include "net/http_data.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "net/error_comm.h"
#include "stocksvc/observe.h"

namespace netcomm_recv {

class RecvGrail:public RecvPacketBase {
 public:
  explicit RecvGrail(NetBase*m)
  :RecvPacketBase(m) {
     type_ = 0;
     init();
  }

  void Reset() {
    init();
  }

  void init() {
    bool r = false;
    r = m_->GetBigInteger(L"user_id", static_cast<int64*>(&user_id_));
    if (false == r) error_code_ = USER_ID_LACK;

    r = m_->GetString(L"token", &token_);
    if (false == r) error_code_ = TOKEN_LACK;

    r = m_->GetString(L"code", &code_);
    if (r == false) {
      error_code_ = STOCK_CODE_LACK;
    } else {
      code_ = code_.substr(0, code_.length() - 1);
    }
  }

  int64 user_id() const { return user_id_; }

  std::string token() const { return token_; }

  std::string code() const { return code_; }

 private:
  int64 user_id_;
  std::string token_;
  std::string code_;
};


class RecvCurve:public RecvPacketBase {
 public:
  explicit RecvCurve(NetBase*m)
  :RecvPacketBase(m) {
     type_ = 0;
     init();
  }

  void Reset() {
    init();
  }

  void init() {
    bool r = false;
    r = m_->GetBigInteger(L"user_id", static_cast<int64*>(&user_id_));
    if (false == r) error_code_ = USER_ID_LACK;

    r = m_->GetString(L"token", &token_);
    if (false == r) error_code_ = TOKEN_LACK;

    r = m_->GetString(L"code", &code_);
    if (r == false) {
      error_code_ = STOCK_CODE_LACK;
    } else {
      code_ = code_.substr(0, code_.length() - 1);
    }
  }

  int64 user_id() const { return user_id_; }

  std::string token() const { return token_; }

  std::string code() const { return code_; }

 private:
  int64 user_id_;
  std::string token_;
  std::string code_;
};

class RecvDigest:public RecvPacketBase {
 public:
  explicit RecvDigest(NetBase*m)
  :RecvPacketBase(m) {
     type_ = 0;
     init();
  }

  void Reset() {
    init();
  }

  void init() {
    bool r = false;
    r = m_->GetBigInteger(L"user_id", static_cast<int64*>(&user_id_));
    if (false == r) error_code_ = USER_ID_LACK;

    r = m_->GetString(L"token", &token_);
    if (false == r) error_code_ = TOKEN_LACK;

    std::string out_name;
    r = m_->GetString(L"event_name", &out_name);
    if (r == false) {
      error_code_ = EVENT_NAME_LACK;
    } else {
      base::BasicUtil::UrlDecode(out_name, event_name_);
      event_name_ = event_name_.substr(0, event_name_.length() - 1);
    }
  }

  int64 user_id() const { return user_id_; }

  std::string token() const { return token_; }

  std::string event_name() const { return event_name_; }

 private:
  int64 user_id_;
  std::string token_;
  std::string event_name_;
};

class RecvStockGroup:public RecvPacketBase {
 public:
  explicit RecvStockGroup(NetBase*m)
  :RecvPacketBase(m) {
    type_ = 0;
    init();
  }

  void Reset() {
    init();
  }

  void init() {
    bool r = false;
    r = m_->GetBigInteger(L"user_id", static_cast<int64*>(&user_id_));
    if (false == r) error_code_ = USER_ID_LACK;

    r = m_->GetString(L"token", &token_);
    if (false == r) error_code_ = TOKEN_LACK;

    std::string out_name;
    r = m_->GetString(L"ori_name", &out_name);
    if (r == false) {
      ori_name_ = "";
    } else {
      base::BasicUtil::UrlDecode(out_name, ori_name_);
      ori_name_ = ori_name_.substr(0, ori_name_.length() - 1);
    }

    r = m_->GetString(L"cur_name", &out_name);
    if (r == false) {
      cur_name_ = "";
    } else {
      base::BasicUtil::UrlDecode(out_name, cur_name_);
      cur_name_ = cur_name_.substr(0, cur_name_.length() - 1);
    }

    r = m_->GetString(L"code", &code_);
    if (r == false) {
      code_ = "";
    } else {
      code_ = code_.substr(0, code_.length() - 1);
    }

    r = m_->GetBigInteger(L"operate_code", &operate_code_);
    if (r == false) {
      error_code_ = OPERATE_CODE_LACK;
      return;
    }

    if (operate_code_ < 1 || operate_code_ > 8) {
      error_code_ = OPERATE_CODE_ERROR;
      return;
    }

    if (operate_code_ != 4 && operate_code_ != 8 && ori_name_ == "") {
      error_code_ = ORI_NAME_LACK;
      return;
    }

    if (operate_code_ == 3 && cur_name_ == "") {
      error_code_ = CUR_NAME_LACK;
      return;
    }

    if ((operate_code_ == 5 || operate_code_ == 6) && code_ == "") {
      error_code_ = STOCK_CODE_LACK;
      return;
    }
  }

  int64 user_id() const { return user_id_; }

  std::string token() const { return token_; }

  std::string ori_name() const { return ori_name_; }

  std::string cur_name() const { return cur_name_; }

  std::string code() const { return code_; }

  int64 operate_code() const { return operate_code_; }

 private:
  int64 user_id_;
  std::string token_;
  std::string ori_name_;
  std::string cur_name_;
  std::string code_;
  int64 operate_code_;
};

class RecvTopTwentyStock:public RecvPacketBase {
 public:
  explicit RecvTopTwentyStock(NetBase*m)
  :RecvPacketBase(m) {
    type_ = 0;
    init();
  }

  inline void Reset() {
    init();
  }

  inline void init() {
    bool r = false;
    r = m_->GetBigInteger(L"user_id", static_cast<int64*>(&user_id_));
    if (false == r) error_code_ = USER_ID_LACK;

    r = m_->GetString(L"token", &token_);
    if (false == r) error_code_ = TOKEN_LACK;

    std::string out_name;
    r = m_->GetString(L"gn", &out_name);
    if (r == true) {
      type_ = observer::ENUME_GN;
    }

    if (r == false) {
      r = m_->GetString(L"hy", &out_name);
      if (r == true) {
        type_ = observer::ENUME_HY;
      }
    }

    if (r == false) {
      r = m_->GetString(L"hot_event", &out_name);
      if (r == true) {
        type_ = observer::ENUME_HOT_EVENT;
      }
    }

    if (type_ > 0) {
      base::BasicUtil::UrlDecode(out_name, name_);
    }

    r = m_->GetBigInteger(L"leaf_num", static_cast<int64*>(&leaf_num_));
    if (false == r) leaf_num_ = 0;

    r = m_->GetBigInteger(L"operate_code", static_cast<int64*>(&operate_code_));
    if (false == r) operate_code_ = 0;

    r = m_->GetBigInteger(L"datatype", static_cast<int64*>(&datatype_));
    if (false == r) {
      if (operate_code_ != 0) {
        error_code_ = DATA_TYPE_LACK;
        return;
      }
    }

    r = m_->GetBigInteger(L"hot_type", static_cast<int64*>(&hot_type_));
    if (false == r) {
      if (operate_code_ != 0) {
        error_code_ = HOT_TYPE_LACK;
        return;
      }
    }
  }

  int64 user_id() const { return user_id_; }

  std::string token() const { return token_; }

  int64 type() const { return type_; }

  std::string name() const { return name_;}
  int64 leaf_num() const { return leaf_num_; }
  int64 operate_code() const { return operate_code_; }
  int64 datatype() const { return datatype_; }
  int64 hot_type() const { return hot_type_; }

 private:
  int64 user_id_;
  std::string token_;
  std::string name_;
  int64 leaf_num_;
  int64 type_;
  int64 operate_code_;
  int64 datatype_;
  int64 hot_type_;
};

}  //  namespace netcomm_recv

namespace netcomm_send {

class SendDigest: public SendPacketBase {
 public:
  SendDigest() {
    base_.reset(new NetBase());
  }

  virtual ~SendDigest() {}

  NetBase* release() {
    head_->Set("result", base_.release());
    this->set_status(1);
    return head_.release();
  }

  inline void set_digest(std::wstring name,  const std::string &digest) {
    base_->SetString(name.c_str(), digest);
  }

 private:
  scoped_ptr<NetBase> base_;
};

class SendGrail: public SendPacketBase {
 public:
  SendGrail() {
    base_.reset(new NetBase());
  }

  virtual ~SendGrail() {}

  NetBase* release() {
    head_->Set("result", base_.release());
    this->set_status(1);
    return head_.release();
  }

  inline void set_comment(std::wstring name,  base_logic::Value *comment) {
    base_->Set(name.c_str(), comment);
  }

 private:
  scoped_ptr<NetBase> base_;
};

class SendCurve: public SendPacketBase {
 public:
  SendCurve() {
    base_.reset(new NetBase());
  }

  virtual ~SendCurve() {}

  NetBase* release() {
    head_->Set("result", base_.release());
    this->set_status(1);
    return head_.release();
  }

  inline void set_comment(std::wstring name,  base_logic::ListValue *comment) {
    base_->Set(name.c_str(), comment);
  }

 private:
  scoped_ptr<NetBase> base_;
};

class SendInfo: public SendPacketBase {
 public:
  SendInfo() {
    base_.reset(new NetBase());
    info_.reset(new NetBase());
  }

  virtual ~SendInfo() {}

  NetBase* release() {
    if (info_->size() > 0) {
      this->base_->Set(L"info", info_.release());
    }

    head_->Set("result", base_.release());
    this->set_status(1);
    return head_.release();
  }

  inline void set_comment(std::wstring name,  base_logic::ListValue *comment) {
    info_->Set(name.c_str(), comment);
  }


 private:
  scoped_ptr<NetBase> base_;
  scoped_ptr<NetBase> info_;
};

class SendTopTwentyStock: public SendPacketBase {
 public:
  SendTopTwentyStock() {
    base_.reset(new NetBase());
    code_info_.reset(new NetBase());
  }

  virtual ~SendTopTwentyStock() {}

  NetBase* release() {
    if (code_info_->size() > 0) {
      this->base_->Set(L"code_info", code_info_.release());
    }

    head_->Set("result", base_.release());
    this->set_status(1);
    return head_.release();
  }

  inline void set_comment(std::wstring name,  base_logic::ListValue *comment) {
    code_info_->Set(name.c_str(), comment);
  }


 private:
  scoped_ptr<NetBase> base_;
  scoped_ptr<NetBase> code_info_;
};

}  //  namespace netcomm_send
#endif  //  _HOME_RUNNER_NORTHSEA_RESTFUL_PUB_NET_STOCK_COMM_HEAD_H_
