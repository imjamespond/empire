/*
 * File:   Time.h
 * Author: root
 *
 * Created on April 7, 2015, 3:50 PM
 */

#ifndef TIME_H
#define	TIME_H
#include <stdint.h>

namespace codechiev {
    namespace base {
        
        static const int64_t MILLIS_SEC = 1000;
        static const int64_t MICROS_SEC = MILLIS_SEC * 1000;
        static const int64_t NANO_SEC_PERSEC = MICROS_SEC * 1000;
        static const float SEC_OF_MILLI = .001;

        class Time{
        public:
            Time(int64_t t = 0);

            static Time now();
            static Time nowTm();
            static Time getClockTime();
            
            int64_t getMillis(){return timeMillis_;}
            void operator=(int64_t);
            int64_t operator-(Time&);
        private:
            int64_t timeMillis_;
        };
    }
}
#endif	/* TIME_H */

