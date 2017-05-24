// author: tianyiheng
// date: Mon Jun 20 16:50:31 2016

#include "rank_init.h"
#include <signal.h>
#include <stdlib.h>
#include "core/common.h"
#include "core/plugins.h"
#include "rank/rank_logic.h"

struct Rankplugin {
  char* id;
  char* name;
  char* version;
  char* provider;
};

static void *OnRankStart() {
  signal(SIGPIPE, SIG_IGN);
  struct Rankplugin* rank = (struct Rankplugin*) calloc(
      1, sizeof(struct Rankplugin));
  rank->id = "rank";
  rank->name = "rank";
  rank->version = "1.0.0";
  rank->provider = "pangh";

  rank::RankLogic::GetInstance();
  return rank;
}

static handler_t OnRankShutdown(struct server* srv, void* pd) {
  return HANDLER_GO_ON;
}

static handler_t OnRankConnect(struct server *srv, int fd, void *data,
                                      int len) {
  return HANDLER_GO_ON;
}

static handler_t OnRankMessage(struct server *srv, int fd, void *data,
                                      int len) {
  if (rank::RankLogic::GetInstance()->OnRankMessage(srv,
                                                                         fd,
                                                                         data,
                                                                         len)) {
    return HANDLER_FINISHED;
  }
  return HANDLER_GO_ON;
}

static handler_t OnRankClose(struct server *srv, int fd) {

  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data,
                                    int len) {

  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server* srv, int fd) {

  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data,
                                    int len) {

  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server* srv) {
  rank::RankLogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server* srv, char* id, int opcode, int time) {
  rank::RankLogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                          time);

  return HANDLER_GO_ON;
}

int rank_plugin_init(struct plugin *pl) {
  pl->init = OnRankStart;
  pl->clean_up = OnRankShutdown;
  pl->connection = OnRankConnect;
  pl->connection_close = OnRankClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnRankMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}

