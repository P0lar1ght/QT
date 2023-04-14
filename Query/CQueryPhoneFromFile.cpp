#include <cstring>
#include "CQueryPhoneFromFile.h"

CQueryPhoneFromFile::CQueryPhoneFromFile() {
    mobile_phone_array = static_cast<PhoneData *>(malloc(sizeof(struct PhoneData) * MOBILE_PHONE_DATA_LINES));;
    if (mobile_phone_array != nullptr) {
        memset(mobile_phone_array, 0, sizeof(struct PhoneData) * MOBILE_PHONE_DATA_LINES);
    }
}

CQueryPhoneFromFile::~CQueryPhoneFromFile() {
    if (this->PhoneFile != nullptr) {
        fclose(PhoneFile);
        PhoneFile = nullptr;
    }
    if (mobile_phone_array != nullptr) {
        free(mobile_phone_array);
        mobile_phone_array = nullptr;
    }
}

bool CQueryPhoneFromFile::OpenFileOnMode(const char *szFilePath, const char *szOpenMode) {
    SetPhoneFilePath(szFilePath);
    if ((PhoneFile = fopen(szFilePath, szOpenMode)) == nullptr) {
        if ((PhoneFile = fopen(szFilePath, szOpenMode)) == nullptr) {
            return false;
        }
    }
    return true;

}

FILE *CQueryPhoneFromFile::GetPhoneFilePtr() {
    return this->PhoneFile;
}

const char *CQueryPhoneFromFile::GetPhoneFilePath() {
    return this->phone_path.c_str();
}

void CQueryPhoneFromFile::SetPhoneFilePath(const char *path) {
    this->phone_path = path;
}

void CQueryPhoneFromFile::SetPhoneDataLocation(int phone, int city) {
    this->phone_location.phone = phone;
    this->phone_location.city = city;
}

PhoneDataLocation CQueryPhoneFromFile::GetPhoneDataLocation() {
    return this->phone_location;
}

int CQueryPhoneFromFile::GetPhoneFileDataAndLen(FILE *file, struct PhoneData *phoneData, PhoneDataLocation Location) {
    int len = 0;
    int counts = 0;
    int count_list[128] = {0};
    if (file == nullptr) {
        return 0;
    }
    char buf[1024]{};
    //读取每一行
    while (fgets(buf, 1024, file) != nullptr) {
        for (int i = 0; i < strlen(buf); ++i) {
            if (buf[i] == ',' || buf[i] == '\n') {
                count_list[++counts] = i + 1;
            }
            if (counts == Location.phone) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    phoneData[len].phoneData[j - count_list[counts - 1]] = buf[j];
                }
                sscanf_s(phoneData[len].phoneData, "%8d", &phoneData[len].phoneNumber);
            } else if (counts == Location.city) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    phoneData[len].cityData[j - count_list[counts - 1]] = buf[j];
                }

            }
        }
        memset(buf, 0, 1024);
        memset(count_list, 0, sizeof count_list);
        counts = 0;
        ++len;
    }
    this->phone_max_line = len;
    return len;
}

char *CQueryPhoneFromFile::QueryPhoneFileData(struct PhoneData *phoneData, char *searchedString) {
    const char *Exception = "PhoneFileData is NULL!";
    const char *NotFind = "null";
    int findPhone;
    sscanf_s(searchedString, "%7d", &findPhone);
    if (this->phone_max_line == 0) {
        return const_cast<char *>(Exception);
    }
    int left = 0;
    int right = this->phone_max_line; //定义target在左闭右开的区间里，即[left, right)
    while (left <= right) {    //因为left = right的时候，在[left, right)区间上无意义
        int middle = left + ((right - left) / 2);
        if (phoneData[middle].phoneNumber > findPhone) {
            right = middle - 1; //target 在左区间，在[left, middle)中
        } else if (phoneData[middle].phoneNumber < findPhone) {
            left = middle + 1;
        } else {
            return phoneData[middle].cityData;
        }
    }
    //二分未找到时执行下方
//    for (int i = 0; i < this->phone_max_line; i++) {
//        if (phoneData[i].phoneNumber == findPhone)
//            return phoneData[i].cityData;
//    }
    return const_cast<char *>(NotFind);
}

PhoneData *CQueryPhoneFromFile::GetPhoneData() {
    return this->mobile_phone_array;
}


