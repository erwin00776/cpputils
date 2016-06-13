#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "SimpleLogStream.h"

#ifndef __SIMPLE_LOGGER__
#define __SIMPLE_LOGGER__

namespace ylemon {
    class SimpleLogger {
    public:
        enum LogLevel {
            TRACE,
            DEBUG1,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_LOG_NUMS,
        };

        class SourceFile {
        private:
            const char *data_;
            size_t size_;
        public:
            template<int N>
            inline SourceFile(const char (&d)[N]) : data_(d), size_(N - 1) {
                const char *slash = strrchr(data_, '/');
                if (slash) {
                    data_ = slash + 1;
                    size_ -= static_cast<int>(data_ - d);
                }
            }

            explicit SourceFile(const char *filename) : data_(filename) {
                const char *slash = strrchr(filename, '/');
                if (slash) {
                    data_ = slash + 1;

                }
                size_ = static_cast<int>(strlen(data_));
            }
        };


        class Impl {
        public:
            Impl(LogLevel logLevel, int savedErrno, const SourceFile file, int line);

            typedef SimpleLogger::LogLevel LogLevel;

            std::string formatTime();

            void finish();

            SimpleLogStream stream_;
            LogLevel level_;
            int line_;
            SourceFile basename_;
        };


    private:
        Impl impl_;

    public:
        SimpleLogger(SourceFile file, int line);

        SimpleLogger(SourceFile file, int line, LogLevel logLevel);

        SimpleLogger(SourceFile file, int line, LogLevel logLevel, const char *func);

        ~SimpleLogger();

        typedef void (*OutputFunc)(const char *msg, int len);

        typedef void (*FlushFunc)();


        static void setLogLevel(LogLevel);

        SimpleLogStream &stream() {
            return impl_.stream_;
        }
    };


#define LOG_TRACE if(ylemon::SimpleLogger::logLevel()<=ylemon::SimpleLogger::TRACE) \
    SimpleLogger(__FILE__, __LINE__, ylemon::SimpleLogger::TRACE).stream()
#define LOG_DEBUG if(ylemon::SimpleLogger::logLevel()<=ylemon::SimpleLogger::DEBUG1) \
    SimpleLogger(__FILE__, __LINE__, ylemon::SimpleLogger::DEBUG).stream()
#define LOG_INFO SimpleLogger(__FILE__, __LINE__, ylemon::SimpleLogger::INFO).stream()
#define LOG_WARN SimpleLogger(__FILE__, __LINE__, ylemon::SimpleLogger::WARN).stream()
#define LOG_ERROR SimpleLogger(__FILE__, __LINE__, ylemon::SimpleLogger::ERROR).stream()
#define LOG_FATAL SimpleLogger(__FILE__, __LINE__, ylemon::SimpleLogger::FATAL).stream()

}

#endif
