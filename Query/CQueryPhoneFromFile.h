//
// Created by 86156 on 2023/4/1.
//
#include "cstdio"
#include<iostream>
#include "string"
#ifndef QT_CQUERYPHONEFROMFILE_H
#define QT_CQUERYPHONEFROMFILE_H
#define MOBILE_PHONE_DATA_LINES 473700
struct PhoneData {
    char phoneData[12];
    char cityData[36];
    int phoneNumber;
};
struct PhoneDataLocation {
    int phone = 1;
    int city = 2;
};
class CQueryPhoneFromFile {
public:
    CQueryPhoneFromFile();

    ~CQueryPhoneFromFile();

    bool OpenFileOnMode(const char *szFilePath, const char *szOpenMode);

    FILE *GetPhoneFilePtr();

    const char *GetPhoneFilePath();

    void SetPhoneFilePath(const char *path);

    void SetPhoneDataLocation(int phone, int city);

    PhoneDataLocation GetPhoneDataLocation();

    int GetPhoneFileDataAndLen(FILE *file, struct PhoneData *phoneData, PhoneDataLocation Location);

    char *QueryPhoneFileData(struct PhoneData *phoneData, char *searchedString);

    PhoneData *GetPhoneData();

private:
    int phone_max_line{};
    std::string phone_path{};
    FILE *PhoneFile{};
    PhoneDataLocation phone_location;
    struct PhoneData *mobile_phone_array;
};


#endif //QT_CQUERYPHONEFROMFILE_H
