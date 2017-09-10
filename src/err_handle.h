#pragma once

void printErr(std::string errInfo) {
    perror(errInfo.c_str());
}

void printErr(const char * errPtr) {
    perror(errPtr);
}