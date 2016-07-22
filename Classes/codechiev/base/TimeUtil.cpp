/*
 * File:   Time.cpp
 * Author: root
 *
 * Created on April 7, 2015, 3:50 PM
 */

#include "TimeUtil.h"
#include <time.h>
#include <sys/time.h>
using namespace codechiev::base;

Time::Time(int64_t t):timeMillis_(t){}

Time Time::now() {
    struct timeval tv;
    ::gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Time(seconds * MILLIS_SEC + tv.tv_usec * .001);
}

Time Time::nowTm() {
    time_t rawtime;
    struct tm * ptm;
    ::time(&rawtime);
    ptm = gmtime(&rawtime);
    return Time(static_cast<int64_t> (rawtime) * MILLIS_SEC);//less acurate
}

void
Time::operator=(int64_t t)
{
    this->timeMillis_ = t;
}

int64_t
Time::operator-(Time & t)
{
    return this->timeMillis_-t.getMillis();
}