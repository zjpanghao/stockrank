/*=============================================================================
#    Copyright (c) 2015
#    ShanghaiKunyan.  All rights reserved
#
#    Filename     : error_comm.cc
#    Author       : Sunsolo
#    Email        : wukun@kunyan-inc.com
#    Date         : 2016-2-3 19:56
#    Description  : The file is generated automatically by enum_to_switch.py.
#=============================================================================*/

#include "net/error_comm.h"

const char* buddha_strerror(int error_code) {
  char* error_string = "";
  switch (error_code) {
    // user
    case USER_NAME_LACK:
        error_string = "用户名缺失";
        break;

    case USER_ID_LACK:
        error_string = "用户id缺失";
        break;

    case USER_PASSWORD_LACK:
        error_string = "用户密码缺失";
        break;

    case PLATFORM_ID_LACK:
        error_string = "平台id缺失";
        break;

    case TOKEN_LACK:
        error_string = "用户凭据缺失";
        break;

    case TOKEN_OVER_ERROR:
        error_string = "无效的用户凭据";
        break;

    case USER_PASSWORD_ERROR:
        error_string = "用户密码错误";
        break;

    case PLATFORM_NAME_LACK:
        error_string = "platform_name缺失";
        break;

    case HAVE_NO_RIGHT:
        error_string = "权限不够";
        break;

    case USER_ID_ISNOT_IN_SQL:
        error_string = "数据库中没有对应的id";
        break;

    // sql
    case SQL_EXEC_ERROR:
        error_string = "SQL语句执行错误";
        break;

    case EXEC_REDIS_ERROR:
        error_string = "redis执行出错";
        break;

    case NULL_DATA:
        error_string = "没有数据";
        break;

    case REIDS_CONNECT_ERROR:
        error_string = "redis连接失败";
        break;

    case STRUCT_ERROR:
        error_string = "结构错误";
        break;

    case CONNECT_ERROR:
        error_string = "mysql连接失败";
        break;

    case SQL_DATA_LACK:
        error_string = "数据库里没有对应的数据";
        break;

    case START_LACK:
        error_string = "起始时间缺失";
        break;

    case END_LACK:
        error_string = "结束时间缺失";
        break;

    //  strategy
    case STRATEGY_LACK:
        error_string = "策略名缺失";
        break;

    case STOCK_POSITION_ID_LACK:
        error_string = "仓id错误";
        break;

    //  information
    case INFO_TYPE_LIST_LACK:
        error_string = "咨询类型列表缺失";
        break;

    case STOCK_CODE_LACK:
        error_string = "股票代码缺失";
        break;

    case OPERATE_CODE_LACK:
        error_string = "操作码缺失";
        break;

    case OPERATE_CODE_ERROR:
        error_string = "操作码错误";
        break;

    case ORI_NAME_LACK:
       error_string = "原始名缺失";
       break;

    case CUR_NAME_LACK:
       error_string = "当前名缺失";
       break;

    case MESSAGE_LACK:
       error_string = "搜索信息缺失";
       break;

    case OVER_GROUP_NUM:
       error_string = "最多只能有7个标签";
       break;

    case OVER_STOCK_NUM:
       error_string = "最多只能有20个股票";
       break;

    case ALL_DIGIT:
       error_string = "不能全是数字";
       break;

    case GROUP_SUV:
       error_string = "组合名已经存在";
       break;

    case DATA_TYPE_LACK:
       error_string = "datatype缺失";
       break;

    case EVENT_NAME_LACK:
       error_string = "event_name缺失";
       break;

    case HOT_TYPE_LACK:
       error_string = "hot_type缺失";
       break;

    case IDENTIFY_LACK:
        error_string = "identifier缺失";
        break;

    case TOKEN_CHANGED:
        error_string = "token变故";
        break;

    case TOKEN_INVALID:
        error_string = "token失效或格式不对";
        break;
   default:
        error_string = "未知错误";
        break;
    }
    return error_string;
}
