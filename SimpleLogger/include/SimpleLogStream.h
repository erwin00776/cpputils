//
// Created by Erwin on 6/12/16.
// Copyright (c) 2016 ___YellowLemon___. All rights reserved.
//
#include <string.h>
#include <string>

#ifndef SIMPLELOGGER_SIMPLELOGSTREAM_H
#define SIMPLELOGGER_SIMPLELOGSTREAM_H

#define kSmallBuffer 64
#define kMaxNumbericSize 16

namespace ylemon {

    template<int SIZE>
    class FixedBuffer {
    private:
        char data_[SIZE];
        char *cur_;

        static void cookieStart();

        static void cookieEnd();

        void (*cookie_)();

        const char *end() const {
            return data_ + sizeof data_;
        }

    public:
        FixedBuffer() : cur_(data_) {
            memset(data_, 0x0, SIZE);
            //setCookie(cookieStart);
        }

        /*
        FixedBuffer(FixedBuffer &fixedBuffer){
            //data_ = fixedBuffer.data_;
            //cur_ = fixedBuffer.data_ + fixedBuffer.length();
        }

        FixedBuffer& operator=(FixedBuffer &fixedBuffer){
            data_ = fixedBuffer.data_;
            cur_ = fixedBuffer.data_ + fixedBuffer.length();
            return *this;
        }
         */

        ~FixedBuffer() {
            //setCookie(cookieEnd);
        }

        void append(const char *buf, size_t len) {
            if ((avail()) > len) {
                memcpy(cur_, buf, len);
                cur_ += len;
            }
        }

        const char *data() const {
            return data_;
        }

        char *current() {
            return cur_;
        }

        int length() const {
            return static_cast<int>(cur_ - data_);
        }

        int avail() const {
            return static_cast<int>(end() - cur_);
        }

        void add(size_t len) {
            cur_ += len;
        }

        void reset() {
            cur_ = data_;
        }

        void bzero() {
            ::bzero(data_, sizeof(data_));
        }

        const char *debugString();

        void setCookie(void (*cookie)()) {
            cookie_ = cookie;
        }

        std::string asString() const {
            return std::string(data_, length());
        }
    };

    class SimpleLogStream {
    public:
        typedef SimpleLogStream self;
        typedef FixedBuffer<kSmallBuffer> Buffer;
    public:
        self &operator<<(bool v) {
            buffer_.append(v ? "1" : "0", 1);
            return *this;
        }

        self &operator<<(short);

        self &operator<<(unsigned int);

        self &operator<<(int);

        self &operator<<(long);

        self &operator<<(unsigned long);

        self &operator<<(long long);

        self &operator<<(unsigned long long);

        self &operator<<(const void *);

        self &operator<<(float v) {
            *this << static_cast<double>(v);
            return *this;
        }

        self &operator<<(double);

        self &operator<<(char v){
            buffer_.append(&v, 1);
            return *this;
        };

        self &operator<<(const char *v) {
            buffer_.append(v, strlen(v));
            return *this;
        }

        self &operator<<(const std::string &s) {
            return operator<<(s.c_str());
        }

        template<typename T>
        void formatInteger(T t);

        Buffer& buffer() {
            //fwrite(buffer_.data(), buffer_.length(), 1, stdout);
            printf("buffer=%d\n", buffer_.length());
            return buffer_;
        }

        void resetBuffer() {
            buffer_.reset();
        }

    private:
        Buffer buffer_;
    };

    class Fmt {
    private:
        char buf_[32];
        int length_;
    public:
        template<typename T>
        Fmt(const char *fmt, T val);

        const char *data() const {
            return buf_;
        }

        int length() const {
            return length_;
        }
    };

}
#endif //SIMPLELOGGER_SIMPLELOGSTREAM_H
