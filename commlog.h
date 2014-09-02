// =====================================================================================
// 
//       Filename:  commlog.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2014-09-01 14时46分05秒
//       Revision:  none
// 
// 
// =====================================================================================

#ifndef __SG_LOG_H__
#define __SG_LOG_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdarg.h>
#include <time.h>

class commlog
{
public:
    int init(std::string path){
        m_path = path;
    }

    void log(const char *fmt, ...){
        //check if exist no_log file
        const std::string stat_file = m_path + "/" + "no_log";
        struct stat stFile;
        int ret = stat(stat_file.c_str(), &stFile);
        if(ret == 0){
            //No need to log any thing
            return; 
        }

        //try to check and rename old file
        const std::string log_file = m_path + "/" + "sg_agent.log";
        struct stat stLog;
        ret = stat(log_file.c_str(), &stLog);
        if(ret == 0 && stLog.st_size > 10 * 1024 * 1024){
            //Log > 10M rename file and create new file
            int curr_time = time(0);
            char szCurrTime[12] = {0};
            snprintf(szCurrTime, sizeof(szCurrTime), "%d", curr_time);
            std::string strTime = szCurrTime;

            std::string new_log_file = log_file + "." + strTime;
            rename(log_file.c_str(), new_log_file.c_str());
        }

        //try to log to file
        FILE *fp = fopen(log_file.c_str(), "a+");
        if(fp == NULL){
            printf("fopen return null\n");
            return;
        }

        va_list args;
        va_start(args, fmt);
        vfprintf(fp, fmt, args);
        va_end(args);
        fclose(fp); 
    }

    static commlog* getInstance(){
        if(m_commlog == NULL){
            m_commlog = new commlog();
        }

        return m_commlog;
    }
    
    struct tm* getTime(){
        time_t now;
        struct tm *tm_now;
        time(&now);
        m_time = localtime(&now);

        return m_time;
    }
private:
    std::string m_path; //日志路径
    static commlog* m_commlog;
    struct tm* m_time;
};

#define LOG_INIT(path) commlog::getInstance()->init(path)
#define LOG(fmt,args...) \
    commlog::getInstance()->log("[%d-%d-%d %d:%d:%d] FILE:%s LN:%d FUN:%s "fmt"\n",\
        commlog::getInstance()->getTime()->tm_year + 1970, commlog::getInstance()->getTime()->tm_mon, commlog::getInstance()->getTime()->tm_mday, commlog::getInstance()->getTime()->tm_hour, commlog::getInstance()->getTime()->tm_min, commlog::getInstance()->getTime()->tm_sec, __FILE__, __LINE__, __func__,##args)
 
            
#endif
