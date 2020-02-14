#pragma once
#include "common.h"
#include <sstream>
#include <fstream>
#include <stdarg.h>
#include "event.h"

namespace Debug
{
	void Log(const std::string msg, ...)
	{
        auto finaln = 0;
        auto n = ((int)msg.size()) * 2; // reserve 2 times as much as the length of the fmt_str
        std::unique_ptr<char[]> formatted;
        va_list ap;
        while (true)
        {
            //wrap the plain char array into the unique_ptr
            formatted.reset(new char[n]);
            strcpy(&formatted[0], msg.c_str());
            va_start(ap, msg);
            finaln = vsnprintf(&formatted[0], n, msg.c_str(), ap);
            va_end(ap);

            if (finaln < 0 || finaln >= n)
            {
                n += abs(finaln - n + 1);
            }
            else
            {
                break;
            }
        }

        auto finalMsg = std::string(formatted.get());
        
        DebugMessageEvent e;
        e.msg = finalMsg + "\n";
        Object::Dispatch(e);

        printf("%s\n", finalMsg.c_str());
	}
};