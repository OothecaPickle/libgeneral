//
//  exception.cpp
//  liboffsetfinder64
//
//  Created by tihmstar on 09.03.18.
//  Copyright © 2018 tihmstar. All rights reserved.
//

#define _GNU_SOURCE

#include "../include/libgeneral/macros.h"
#include "../include/libgeneral/exception.hpp"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

using namespace tihmstar;

exception::exception(const char *commit_count_str, const char *commit_sha_str, int line, const char *filename, const char *err ...) :
    _commit_count_str(commit_count_str),
    _commit_sha_str(commit_sha_str),
    _line(line),
    _filename(filename),
    _err(NULL)
{
    va_list ap = {};
    va_start(ap, err);
    vasprintf(&_err, err, ap);
    va_end(ap);
};

exception::exception(const exception &e) :    //copy constructor
_commit_count_str(e._commit_count_str),
_commit_sha_str(e._commit_sha_str),
_line(e._line),
_filename(e._filename),
_err(NULL)
{
    if (e._err) {
        size_t len = strlen(e._err);
        _err = (char*)malloc(len+1);
        strncpy(_err, e._err, len);
        _err[len] = '\0';
    }
}
exception::~exception(){
    safeFree(_err);
}


const char *exception::what() const noexcept {
    return _err;
}

int exception::code() const{
    return (_line << 16) | (int)(_filename.size());
}

void exception::dump() const{
    printf("%s",dumpStr().c_str());
}

std::string exception::dumpStr() const{
    char *dumpstr = NULL;
    cleanup([&]{
        safeFree(dumpstr);
    });
    asprintf(&dumpstr, "[exception]:\n"
                        "what=%s\n"
                        "code=%d\n"
                        "line=%d\n"
                        "file=%s\n"
                        "commit count=%s\n"
                        "commit sha  =%s\n"
                        ,_err
                        ,code()
                        ,_line
                        ,_filename.c_str()
                        ,build_commit_count().c_str()
                        ,build_commit_sha().c_str()
            );
    return dumpstr;
}


std::string exception::build_commit_count() const {
    return _commit_count_str;
};

std::string exception::build_commit_sha() const {
    return _commit_sha_str;
};

