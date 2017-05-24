// author: pangh
// date: Mon Jun 20 16:50:31 2016

#include "rank_logic.h"
#include "core/common.h"
#include "base/logic/logic_comm.h"
#include "base/logic/base_values.h"
#include "public/config/config.h"

#include "logic/logic_unit.h"
#include "net/operator_code.h"
#include "rank/rank_pro.h"
#include "db_mysql.h"
#include "hot_cache.h"
#include "hot_service.h"
#include "dic/base_dic_redis_auto.h"
#include <sstream>

#define DEFAULT_CONFIG_PATH                    "./plugins/rank/rank_config.xml"
#define TIME_REALINFO_UPDATE_ALL                          10000
const int UPDATE_HISTORY_STOCK = 1; 
const int UPDATE_MARKET = 2; 
#define SEND_HTTP_ERROR(b) \  
  do {\
  std::string response = b;\
  base_logic::LogicUnit::SendFull(socket, response.c_str(), b.length());\
  }while(0)
#define SEND_ERROR_INFO(c, i) \  
  do {\
  RankHotProtocol error_pro;\
  error_pro.SetErrorState(c, i);\
  std::string response = error_pro.GetJson();\
  base_logic::LogicUnit::SendFull(socket, response.c_str(), response.length());\
  }while(0)
#define SEND_HTTP_INFO(b) \  
  do {\
  std::string response = b;\
  base_logic::LogicUnit::SendFull(socket, response.c_str(), response.length());\
  }while(0)

namespace rank {

RankLogic* RankLogic::instance_ = NULL;

RankLogic::RankLogic() {
  if (!Init())
    assert(0);
}

RankLogic::~RankLogic() {
}

bool RankLogic::Init() {
  bool r = false;
  srand(time(NULL));
  struct tm current;
  time_t now = time(NULL);
  localtime_r(&now, &current);
  day_ = current.tm_mday;
  std::string my_config_file = "./plugins/rank/base_config.txt";
  #ifndef KUN_DB_POOL
      rank::DbMysql::Init(my_config_file);
  #endif
  HotCache::GetInstance();
  RankHotService::GetInstance()->UpdateData();
  return true;
}

RankLogic* RankLogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new RankLogic();
  return instance_;
}

void RankLogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool RankLogic::OnRankConnect(struct server *srv,
                                            const int socket) {
  return true;
}

bool RankLogic::OnRankMessage(struct server *srv,
                                            const int socket, const void *msg,
                                            const int len) {
  bool r = true;
  do {
    const char* packet = reinterpret_cast<const char*>(msg);
    if (NULL == packet) {
      r = false;
      break;
    }
    std::string http_str(packet, len);
    std::string error_str;
    int error_code = 0;
    scoped_ptr <base_logic::ValueSerializer> serializer(
        base_logic::ValueSerializer::Create(base_logic::IMPL_HTTP,
                                            &http_str));
    NetBase* value = (NetBase*) (serializer.get()->Deserialize(&error_code,
                                                               &error_str));
    LOG_DEBUG2("http_str:%s", http_str.c_str());
    if (NULL == value) {
      error_code = STRUCT_ERROR;
      LOG_MSG("error struct ");
      SEND_ERROR_INFO(STRUCT_ERROR, "error struct");
      r = true;
      break;
    }
    scoped_ptr < RecvPacketBase > head_packet(new RecvPacketBase(value));
    int32 type = head_packet->GetType();
    r = OnHotRank(srv, socket, value, msg, len);
  }while(0);
  return r;
}

bool RankLogic::OnRankClose(struct server *srv,
                                          const int socket) {
  return true;
}

bool RankLogic::OnBroadcastConnect(struct server *srv, const int socket,
                                          const void *msg, const int len) {
  return true;
}

bool RankLogic::OnBroadcastMessage(struct server *srv, const int socket,
                                          const void *msg, const int len) {
  return true;
}

bool RankLogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool RankLogic::OnIniTimer(struct server *srv) {
   srv->add_time_task(srv, "rank", UPDATE_HISTORY_STOCK, 100, -1);
  return true;
}

void RankLogic::UpdateHistory() {
}

bool RankLogic::OnTimeout(struct server *srv, char *id, int opcode,
                              int time) {
  switch (opcode) {
    case UPDATE_HISTORY_STOCK:
      RankHotService::GetInstance()->UpdateData();
      break;
    default:
      break;
  }
  return true;
}


bool RankLogic::OnHotRank(struct server *srv,
                          const int socket, NetBase* netbase,
                          const void* msg, const int len) {
  bool r = true;
  int err = 0;
  std::string stock_code;
  int64 record_type;
  
  std::string json = "";
  HotCache::GetInstance()->GetCache("rank", &json);
  if (json.length() != 0) {
     SEND_HTTP_INFO(json);
     return true;
  }
  RankHotProtocol rank_pro;
  long page = 0;
  long num = 10;
  long table = 0;
  netbase->GetBigInteger(L"page", static_cast<int64*>(&page));
  netbase->GetBigInteger(L"num", static_cast<int64*>(&num));
  netbase->GetBigInteger(L"table", static_cast<int64*>(&table));
  rank_pro.Init(RankHotService::GetInstance()->GetRankQuery(page, num, table)); 
  json = rank_pro.GetJson();
  HotCache::GetInstance()->UpdateCache(RANK_KEY, json);
  SEND_HTTP_INFO(json);
  return r;
}

}  // rank

