#include "SimpleLogger.h"
#include "SimpleLogStream.h"
#include <time.h>
#include <sys/time.h>
#include <iostream>

using namespace ylemon;


const char *LogLevelName[SimpleLogger::LogLevel::NUM_LOG_NUMS] = {
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL",
};


void defaultOutput(const char* msg, int len){
    printf("len1=%d, len2=%d\n", strlen(msg), len);
    size_t n = fwrite(msg, len, 1, stdout);
    (void) n;
}

void defaultFlush(){
    fflush(stdout);
}
SimpleLogger::OutputFunc g_output = defaultOutput;
SimpleLogger::FlushFunc  g_flush = defaultFlush;


void setOutputFunc(SimpleLogger::OutputFunc fn){
    g_output = fn;
}

void setFlushFunc(SimpleLogger::FlushFunc fn){
    g_flush = fn;
}



SimpleLogger::LogLevel initLogLevel(){
    if(::getenv("YLEMON_LOG_TRACE"))
        return SimpleLogger::LogLevel::TRACE;
    else if(::getenv("YLEMON_LOG_DEBUG"))
        return SimpleLogger::LogLevel::DEBUG1;
    else
        return SimpleLogger::LogLevel::INFO;
}

extern SimpleLogger::LogLevel g_logLevel = initLogLevel();

void SimpleLogger::setLogLevel(LogLevel logLevel){
    g_logLevel = logLevel;
}

inline SimpleLogger::LogLevel logLevel(){
    return g_logLevel;
}




SimpleLogger::Impl::Impl(LogLevel logLevel, int savedErrno, const SourceFile file, int line):
    level_(logLevel), stream_(), basename_(file), line_(line){
        formatTime();
        stream_ << LogLevelName[level_] << " [" << formatTime() << "] ";
}

SimpleLogger::SimpleLogger(SourceFile file, int line):
impl_(INFO, 0, file, line){}

SimpleLogger::SimpleLogger(SourceFile file, int line, LogLevel logLevel):
impl_(logLevel, 0, file, line){}

SimpleLogger::SimpleLogger(SourceFile file, int line, LogLevel logLevel, const char *func):
impl_(logLevel, 0, file, line){
    impl_.stream_ << func;
}

std::string SimpleLogger::Impl::formatTime() {
    char buf[32];
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0) {
    }

    struct tm tm_time;
    if (localtime_r(&tv.tv_sec, &tm_time) != 0) {
        sprintf(buf, "%04d/%02d/%02d %02d:%02d:%02d.%d",
                tm_time.tm_year + 1900,
                tm_time.tm_mon + 1,
                tm_time.tm_mday,
                tm_time.tm_hour,
                tm_time.tm_min,
                tm_time.tm_sec,
                tv.tv_usec);
    }
    std::string s(buf);
    return s;
}

void SimpleLogger::Impl::finish() {
    stream_ << "-" << line_ << '\n';
    // << basename_ << ':' << line_ << '\n';
}

SimpleLogger::~SimpleLogger() {
    impl_.finish();
    //SimpleLogStream::Buffer buf=stream().buffer();
    //SimpleLogStream::Buffer buf(stream().buffer());
    auto *buf = &(stream().buffer());
    g_output(buf->data(), buf->length());
    if(impl_.level_ == FATAL){
        g_flush();
        abort();
    }
}
