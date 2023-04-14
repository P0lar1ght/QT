#pragma once
#include "cstdio"
#include<iostream>
#include "string"
#define IP_DATA_LINES 684000
struct IPData {
    char ipStartString[15];
    char ipEndString[15];
    char cityData[84];
    long long ipStartNumber;
    long long ipEndNumber;
};
struct IPDataLocation {
    int ip_start = 1;
    int ip_end = 2;
    int city = 3;
};

class CQueryIPFromFile{
public:
    CQueryIPFromFile();

    ~CQueryIPFromFile();

    bool OpenFileOnMode(const char *szFilePath, const char *szOpenMode);

    FILE *GetIPFilePtr();

    const char *GetIPFilePath();

    void SetIPFilePath(const char *path);

    void SetIPDataLocation(int ip_start, int ip_end, int city);

    IPDataLocation GetIPDataLocation();

    int GetIpFileDataAndLen(FILE *file, struct IPData *ipData, IPDataLocation Location);

    char *QueryIpFileData(struct IPData *ipData, char *searchedIpString);

    static long long ipAddressProcessing(char *searchedIpString);

    IPData * GetIPData();

private:
    int ip_max_line{};
    std::string ip_path{};
    FILE *IpFile{};
    IPDataLocation ip_location;
    struct IPData *ip_data_array;
};
