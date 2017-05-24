// author: panghao
// date: Mon Jun 20 16:50:31 2016

#ifndef __PLUGIN__REALINFO_LOGIC___
#define __PLUGIN__REALINFO_LOGIC___
#include <string>
#include "core/common.h"
#include "net/http_data.h"

namespace rank {
class RankLogic {
 public:
  RankLogic();
  virtual ~RankLogic();
 private:
  static RankLogic *instance_;
 public:
  static RankLogic *GetInstance();
  static void FreeInstance();
 public:
  bool OnRankConnect(struct server *srv, const int socket);
  bool OnRankMessage(struct server *srv, const int socket,
                            const void *msg, const int len);
  bool OnRankClose(struct server *srv, const int socket);
  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);
  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);
  bool OnBroadcastClose(struct server *srv, const int socket);
  bool OnIniTimer(struct server *srv);
  bool OnTimeout(struct server *srv, char* id, int opcode, int time);

 protected:
  bool OnHotRank(struct server *srv,
                          const int socket, NetBase* netbase,
                          const void* msg, const int len); 
  bool OnHotRank(NetBase *kv, struct server *srv, const int socket,
                 const void *msg, const int len);
 private:
  bool Init();
  void UpdateHistory();
  int day_;
};

}  // namespace candlestock

#endif // __PLUGIN__REALINFO_LOGIC___

