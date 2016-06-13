#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>

using namespace std;


std::string formatTime(){
    char buf[32];
    struct timeval tv;
    if(gettimeofday(&tv, NULL)!=0){
    }

    struct tm tm_time;
    if(localtime_r(&tv.tv_sec, &tm_time)!=0){
        sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d.%d",
                tm_time.tm_year+1900,
                tm_time.tm_mon+1,
                tm_time.tm_mday,
                tm_time.tm_hour,
                tm_time.tm_min,
                tm_time.tm_sec,
                tv.tv_usec  );
    }
    std::string s(buf);
    return s;
}

