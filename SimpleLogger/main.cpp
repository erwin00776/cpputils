//
//  main.cpp
//  SimpleLogger
//
//  Created by Erwin on 6/12/16.
//  Copyright (c) 2016 ___YellowLemon___. All rights reserved.
//

#include "tests/mytest.h"
#include "SimpleLogger.h"
#include "SimpleLogStream.h"
//#include "Condition.h"
#include <iostream>
using namespace ylemon;

void dummyOutput(const char *msg, int len){
    printf("|%s|\n", msg);
}

void testBasic(){
    LOG_INFO << "hello, basic logger.";
    LOG_WARN << "warning.";
    LOG_ERROR << "error message.";
}

void testBuffer(){
    FixedBuffer<64> fixedBuffer;
    fixedBuffer.append("|test buffer.|\n", 15);
    auto *fixedBuffer2 = &fixedBuffer;
    printf("{%d, %d, %s}\n", fixedBuffer.length(),
            fixedBuffer2->length(), fixedBuffer2->data());
}


int main(int argc, const char * argv[]) {
    testBasic();
    //testBuffer();
    // insert code here...
    std::cout << std::endl << "Hello, World!\n";
    string s = formatTime();
    cout << s << endl;

    return 0;
}