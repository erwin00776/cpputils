//
// Created by Erwin on 6/12/16.
// Copyright (c) 2016 ___YellowLemon___. All rights reserved.
//

#include <assert.h>
#include <stdio.h>
#include "SimpleLogStream.h"
using namespace ylemon;

const char digits[] = "9876543210123456789";
const char *zero = digits+9;

template<int SIZE>
void FixedBuffer<SIZE>::cookieStart() {
}

template<int SIZE>
void FixedBuffer<SIZE>::cookieEnd() {
}

template<int SIZE>
const char* FixedBuffer<SIZE>::debugString() {
    *cur_ = '\0';
    return data_;
}

template<typename T>
size_t convert(char buf[], T value){
    T i = value;
    char *p = buf;
    do{
        int lsd = static_cast<int>(i%10);
        i /= 10;
        *p++ = zero[lsd];
    }while(i!=0);
    if(value<0)
        *p++ = '-';
    *p = '\0';
    std::reverse(buf, p);
    return p-buf;
}

template<typename T>
void SimpleLogStream::formatInteger(T v){
    if(buffer_.avail()>=kMaxNumbericSize){
        size_t len = convert(buffer_.current(), v);
        buffer_.add(len);
    }
}

SimpleLogStream& SimpleLogStream::operator<<(short v){
    *this << static_cast<int>(v);
    return *this;
}

SimpleLogStream& SimpleLogStream::operator<<(unsigned int v){
    char buf[32];
    sprintf(buf, "%u", v);
    buffer_.append(buf, strlen(buf));
    return *this;
}

SimpleLogStream& SimpleLogStream::operator<<(int v){
    formatInteger<int>(v);
    return *this;
}

SimpleLogStream& SimpleLogStream::operator<<(unsigned long v){
    formatInteger<unsigned long>(v);
    return *this;
}

SimpleLogStream& SimpleLogStream::operator<<(long v){
    formatInteger<long>(v);
    return *this;
}

SimpleLogStream& SimpleLogStream::operator<<(double v){
    if(buffer_.avail()>=kMaxNumbericSize){
        int len = snprintf(buffer_.current(), kMaxNumbericSize, "%.12g", v);
        buffer_.add(len);
    }
    return *this;
}

template<typename T>
Fmt::Fmt(const char *fmt, T val) {
    length_ = snprintf(buf_, sizeof buf_, fmt, val);
    assert(static_cast<size_t>(length_) < sizeof buf_);
}
