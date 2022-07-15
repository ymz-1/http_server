#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

//工具类
class Util{
    public:
        static int ReadLine(int sock, std::string &out)
        {
            //统一转化成为按照\n结尾
            char ch = 'X';
            while(ch != '\n'){
                ssize_t s = recv(sock, &ch, 1, 0);
                if(s > 0){
                    if(ch == '\r'){
                        //\r->\n or \r\n->\n
                        recv(sock, &ch, 1, MSG_PEEK);
                        if(ch == '\n'){
                            //把\r\n转化成\n
                            //窥探成功，这个字符一定存在，再直接读走
                            recv(sock, &ch, 1, 0);
                        }
                        else{ //下一个字符不是\n，即行分隔符只是\r
                            ch = '\n';
                        }
                    }
                    //1. 普通字符
                    //2. \n
                    out.push_back(ch);
                }
                else if(s == 0){
                    return 0;
                }
                else{
                    return -1;
                }
            }
            return out.size();
        }
        //切分报头的一行的k和v
        static bool CutString(const std::string &target, std::string &sub1_out, std::string &sub2_out, std::string sep)
        {
            size_t pos = target.find(sep);
            if(pos != std::string::npos){
                sub1_out = target.substr(0, pos);
                sub2_out = target.substr(pos+sep.size());
                return true;
            }
            return false;
        }
};
