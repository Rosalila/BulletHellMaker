#include "Utility/Utility.h"

bool writeLogLine(std::string text)
{
    ofstream log_file("log", ios::app);
    log_file<<text<<"\n";
    log_file.close();

    return true;
}

bool clearLog()
{
    ofstream log_file;
    log_file.open ("log");
    log_file<<"";
    log_file.close();

    return true;
}

std::string toString(int number)
{
    if (number == 0)
        return "0";
    std::string temp="";
    std::string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}
