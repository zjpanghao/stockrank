/*
 * rank_pro.h
 *
 *  Created on: 2017年1月12日
 *      Author: pangh
 */

#ifndef HOT_RANK_HOT_RANK_PRO_H_
#define HOT_RANK_HOT_RANK_PRO_H_
#include <list>
#include <vector>
#include <string>
#include "net/http_data.h"
#include "hot_service.h"
namespace rank {

class RankHotProtocol {
 public:
  explicit RankHotProtocol() {
    result_.reset(new NetBase());
  }
  ~RankHotProtocol() {
    DeleteGroup(L"open", result_.get());
    DeleteGroup(L"close", result_.get());
    DeleteGroup(L"day", result_.get());
  }

  void DeleteGroup(std::wstring key, base_logic::DictionaryValue* root) {
    base_logic::ListValue* listvalue;
    bool r = root->GetList(key, &listvalue);
    if (!r) {
      return;
    }
    while (listvalue->GetSize()) {
      base_logic::Value* result_value;
      listvalue->Remove(0, &result_value);
      base_logic::DictionaryValue* dvalue =
          static_cast<base_logic::DictionaryValue*>(result_value);
      delete result_value;
    }

  }

  void Init(const RankQuery &query) {

    // LOG_MSG2("close_last:%s", query.close.data_list().back().name.c_str());
    AddGroup(L"open", query.open.data_list(), result_.get());
     AddGroup(L"close",query.close.data_list(), result_.get());
    // LOG_MSG2("day_last:%s", query.day.data_list().back().name.c_str());
     AddGroup(L"day", query.day.data_list(), result_.get());
  }

  void AddGroup(std::wstring group_name,
                const std::vector<StockHot> &result,
                base_logic::DictionaryValue* root) {
     std::vector<StockHot>::const_iterator it = result.begin();
     base_logic::ListValue* list = new base_logic::ListValue();
     while (it != result.end()) {
       base_logic::DictionaryValue* info_value =
           new base_logic::DictionaryValue();
       info_value->SetString(L"code", it->stock_code);
       info_value->SetString(L"name", it->name);
      //  LOG_MSG2("Add name %s", it->name.c_str());
       list->Append((base_logic::Value*) (info_value));
     it++;
     }
 
    root->Set(group_name, list);
  }

  std::string GetJson() {
    std::string json;
    scoped_ptr <base_logic::ValueSerializer> serializer(
        base_logic::ValueSerializer::Create(base_logic::IMPL_JSONP));
    bool r = serializer->Serialize(*result_, &json);
    if (!r)
      return "";
    // LOG_MSG2("%s", json.c_str());
    return json;
  }

  void SetCommonInfo(int status) {
    result_->SetInteger(L"status", status);
    result_->SetBigInteger(L"timestamp", time(NULL));
  }
  
  void SetErrorState(int status, std::string error) {
    SetCommonInfo(status);
    result_->SetString(L"error", error);
  }  

 private:
  scoped_ptr<NetBase> result_;
};

}  // namespace rank

#endif /* HOT_RANK_HOT_RANK_PRO_H_ */
