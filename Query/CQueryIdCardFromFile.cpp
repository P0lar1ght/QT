//
// Created by 86156 on 2023/4/1.
//

#include "CQueryIdCardFromFile.h"

CQueryIdCardFromFile::CQueryIdCardFromFile() {
    id_card_array = static_cast<IdCardData *>(malloc(sizeof(struct IdCardData) * ID_CARD_DATA_LINES));;
    if (id_card_array != nullptr) {
        memset(id_card_array, 0, sizeof(struct IdCardData) * ID_CARD_DATA_LINES);
    }
}

CQueryIdCardFromFile::~CQueryIdCardFromFile() {
    if (this->IdCardFile != nullptr) {
        fclose(IdCardFile);
        IdCardFile = nullptr;
    }
    if (id_card_array != nullptr) {
        free(id_card_array);
        id_card_array = nullptr;
    }
}

bool CQueryIdCardFromFile::OpenFileOnMode(const char *szFilePath, const char *szOpenMode) {
    SetIdCardFilePath(szFilePath);
    if ((IdCardFile = fopen(szFilePath, szOpenMode)) == nullptr) {
        if ((IdCardFile = fopen(szFilePath, szOpenMode)) == nullptr) {
            return false;
        }
    }
    return true;
}

FILE *CQueryIdCardFromFile::GetIdCardFilePtr() {
    return this->IdCardFile;
}

const char *CQueryIdCardFromFile::GetIdCardFilePath() {
    return this->id_card_path.c_str();
}

void CQueryIdCardFromFile::SetIdCardFilePath(const char *path) {
    this->id_card_path = path;
}

void CQueryIdCardFromFile::SetIdCardDataLocation(int id_card, int city) {
    this->id_card_location.IdCard = id_card;
    this->id_card_location.city = city;
}

IdCardDataLocation CQueryIdCardFromFile::GetIdCardDataLocation() {
    return this->id_card_location;
}

int
CQueryIdCardFromFile::GetIdCardFileDataAndLen(FILE *file, struct IdCardData *idCardData, IdCardDataLocation Location) {
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
            if (counts == Location.IdCard) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    idCardData[len].idData[j - count_list[counts - 1]] = buf[j];
                }
                sscanf_s(idCardData[len].idData, "%6d", &idCardData[len].idNumber);
            } else if (counts == Location.city) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    idCardData[len].cityData[j - count_list[counts - 1]] = buf[j];
                }
            }
        }
        memset(buf, 0, 1024);
        memset(count_list, 0, sizeof count_list);
        counts = 0;

        len++;
    }
    this->id_card_max_line = len;
    return len;
}

char *CQueryIdCardFromFile::QueryIdCardFileData(struct IdCardData *idCardData, char *searchedString) {
    const char *Exception = "IDFileData is NULL!";
    const char *NotFind = "null";
    int findID;
    sscanf_s(searchedString, "%6d", &findID);
    if (this->id_card_max_line == 0) {
        return const_cast<char *>(Exception);
    }
    int left = 0;
    int right = id_card_max_line + 1; //定义target在左闭右开的区间里，即[left, right)
    while (left < right) {    //因为left = right的时候，在[left, right)区间上无意义
        int middle = left + ((right - left) / 2);
        if (idCardData[middle].idNumber > findID) {
            right = middle; //target 在左区间，在[left, middle)中
        } else if (idCardData[middle].idNumber < findID) {
            left = middle + 1;
        } else {
            return idCardData[middle].cityData;
        }
    }
//    for (int i = 0; i < this->id_card_max_line; ++i) {
//        if (idCardData[i].idNumber == findID) {
//            return idCardData[i].cityData;
//        }
//    }
    return const_cast<char *>(NotFind);

}

IdCardData *CQueryIdCardFromFile::GetIdCardData() {
    return this->id_card_array;
}
