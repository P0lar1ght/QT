#include <cstring>
#include "CQueryIPFromFile.h"

CQueryIPFromFile::CQueryIPFromFile() {
    ip_data_array = (IPData *) (malloc(sizeof(struct IPData) * IP_DATA_LINES));;
    if (ip_data_array != nullptr) {
        memset(ip_data_array, 0, sizeof(struct IPData) * IP_DATA_LINES);
    }
}

CQueryIPFromFile::~CQueryIPFromFile() {
    if (this->IpFile != nullptr) {
        fclose(IpFile);
        IpFile = nullptr;
    }
    if (ip_data_array != nullptr) {
        free(ip_data_array);
        ip_data_array = nullptr;
    }
}

long long CQueryIPFromFile::ipAddressProcessing(char *searchedIpString) {
    int nTmpIP[4] = {0};
    long long nIPAddress = 0;
    sscanf_s(searchedIpString, "%d.%d.%d.%d", &nTmpIP[0], &nTmpIP[1], &nTmpIP[2], &nTmpIP[3]);
    for (int i = 0; i < 4; i++) {
        nIPAddress += (nTmpIP[i] << (24 - (i * 8)) & 0xFFFFFFFF);
    }
    return nIPAddress;
}

int CQueryIPFromFile::GetIpFileDataAndLen(FILE *file, struct IPData *ipData, IPDataLocation Location) {

    int len = 0;
    int counts = 0;
    int count_list[128] = {0};
    if (file == nullptr) {
        return 0;
    }
    char buf[1024]{};
    memset(buf, 0, 1024);
    //读取每一行
    while (fgets(buf, 1024, file) != nullptr) {
        for (int i = 0; i < strlen(buf); ++i) {
            if (buf[i] == ',' || buf[i] == '\n') {
                count_list[++counts] = i + 1;
            }
            if (counts == Location.ip_start) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    ipData[len].ipStartString[j - count_list[counts - 1]] = buf[j];
                }
                ipData[len].ipStartNumber = ipAddressProcessing(ipData[len].ipStartString);
            } else if (counts == Location.ip_end) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    ipData[len].ipEndString[j - count_list[counts - 1]] = buf[j];
                }
                ipData[len].ipEndNumber = ipAddressProcessing(ipData[len].ipEndString);
            } else if (counts == Location.city) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    ipData[len].cityData[j - count_list[counts - 1]] = buf[j];
                }
            }
        }
        memset(buf, 0, 1024);
        memset(count_list, 0, sizeof count_list);
        counts = 0;
        len++;
    }
    this->ip_max_line = len;
    return len;
}

char *CQueryIPFromFile::QueryIpFileData(struct IPData *ipData, char *searchedIpString) {
    const char *Exception = "IPFileData is NULL!";
    const char *NotFind = "null";
    long long findIpaddr = ipAddressProcessing(searchedIpString);
    if (this->ip_max_line == 0) {
        return const_cast<char *>(Exception);
    }
    int left = 0;
    int right = ip_max_line + 1; //定义target在左闭右开的区间里，即[left, right)
    while (left <= right) {    //因为left = right的时候，在[left, right)区间上无意义
        int middle = left + ((right - left) / 2);
        if (ipData[middle].ipStartNumber > findIpaddr) {
            right = middle - 1; //target 在左区间，在[left, middle)中
        } else if (ipData[middle].ipEndNumber < findIpaddr) {
            left = middle + 1;
        } else {
            return ipData[middle].cityData;
        }
    }
//    for (int i = 0; i < this->ip_max_line; ++i) {
//        if (ipData[i].ipStartNumber <= findIpaddr && ipData[i].ipEndNumber >= findIpaddr) {
//            return ipData[i].cityData;
//        }
//    }
    return const_cast<char *>(NotFind);
}

FILE *CQueryIPFromFile::GetIPFilePtr() {
    return this->IpFile;
}
void CQueryIPFromFile::SetIPDataLocation(int ip_start, int ip_end, int city) {
    this->ip_location.ip_start = ip_start;
    this->ip_location.ip_end = ip_end;
    this->ip_location.city = city;
}

IPDataLocation CQueryIPFromFile::GetIPDataLocation() {
    return this->ip_location;
}

const char *CQueryIPFromFile::GetIPFilePath() {
    return this->ip_path.c_str();
}

bool CQueryIPFromFile::OpenFileOnMode(const char *szFilePath, const char *szOpenMode) {
    SetIPFilePath(szFilePath);
    if ((IpFile = fopen(szFilePath, szOpenMode)) == nullptr) {
        if ((IpFile = fopen(szFilePath, szOpenMode)) == nullptr) {
            return false;
        }
    }
    return true;
}

IPData * CQueryIPFromFile::GetIPData() {
    return this->ip_data_array;
}

void CQueryIPFromFile::SetIPFilePath(const char *path) {
    this->ip_path = path;
}




