#ifndef _GETIPANDMAC_H_
#define _GETIPANDMAC_H_

#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>

static void staticSleep(int iTime)
{
    _sleep(iTime);
}

static bool GetLocalIP(std::string &ip)
{
	char *cIp;
	WSADATA wsaData;
	char name[155];
	PHOSTENT hostinfo;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				cIp = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			}
			else 
			{
				return false;
			}
		}
		WSACleanup();
	}
	ip = cIp;
	return true;
}

#else//linux部分的代码

#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <list>
#include <unistd.h>

static void staticSleep(int iTime)
{
    sleep(iTime);
}

static int get_ip_linux(int ipv4_6, std::list<std::string>& out_list_ip)
{
	using namespace std;
    int ret_val = 0;

    struct ifaddrs * ifAddrStruct 	= NULL;
    void * tmpAddrPtr 				= NULL;

    // 1.
    ret_val 						= getifaddrs(&ifAddrStruct);
    if (0 != ret_val)
    {
        ret_val = errno;

        return ret_val;
    }

    // 2.
    std::string str_ipvX;

    int padress_buf_len = 0;
    char addressBuffer[INET6_ADDRSTRLEN] = {0};

    if (AF_INET6 == ipv4_6)
            padress_buf_len = INET6_ADDRSTRLEN;
    else
        padress_buf_len = INET_ADDRSTRLEN;
    
    
    while (NULL != ifAddrStruct ) 
    {
        if (ipv4_6 == ifAddrStruct->ifa_addr->sa_family ) 
        { 
            // is a valid IP4 Address
            tmpAddrPtr = &((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            
            inet_ntop(ipv4_6, tmpAddrPtr, addressBuffer, padress_buf_len);
            str_ipvX = std::string(addressBuffer);

            out_list_ip.push_back(str_ipvX);

            memset(addressBuffer, 0, padress_buf_len);
        } 

        ifAddrStruct=ifAddrStruct->ifa_next;
    }

    return ret_val;
}

static int get_ipv4_linux(std::list<std::string>& out_list_ip4)
{
    return get_ip_linux(AF_INET, out_list_ip4); 
}

static int get_ipv6_linux(std::list<std::string>& out_list_ip6)
{
    return get_ip_linux(AF_INET6, out_list_ip6); 
}

static bool GetLocalIP(std::string &Ip)
{
	using namespace std;
	std::list<std::string> ipv4;
	std::list<std::string> ipv6;

	int ret_val = get_ipv4_linux(ipv4);
	if (0 != ret_val)
	{
    	cout << "error, ipv4, id = " << ret_val << endl;
		Ip = "ERROR";
		return false;
	}
	else
	{
    	int index = 0;
    	for (auto item: ipv4)
    	{
        	cout << "index = " << ++index;
        	cout << ", ipv4 = " << item.c_str() << endl;
			Ip = item.c_str();
    	}
	}
//备用于Ipv6，直接注释
/*
	ret_val = get_ipv6_linux(ipv6);
	if (0 != ret_val)
	{
    	cout << "error, ipv6, id = " << ret_val << endl;
	}
	else
	{
    	int index = 0;
    	for (auto item: ipv6)
    	{
        	cout << "index = " << ++index;
        	cout << ", ipv6 = " << item.c_str() << endl;
    	}
	}
*/
	return true;
}

#endif //WIN32

#endif //_GETIPANDMAC_H_