//
// Created by 86156 on 2023/4/1.
//
#include "cstdio"
#include<iostream>
#include "string"
#include <cstring>

#ifndef QT_CQUERYIDCARDFROMFILE_H
#define QT_CQUERYIDCARDFROMFILE_H
#define ID_CARD_DATA_LINES 6000

struct IdCardData {
    char idData[20];
    char cityData[64];
    int idNumber;
};
struct IdCardDataLocation {
    int IdCard = 1;
    int city = 2;
};

class CQueryIdCardFromFile {
public:
    CQueryIdCardFromFile();

    ~CQueryIdCardFromFile();

    bool OpenFileOnMode(const char *szFilePath, const char *szOpenMode);

    FILE *GetIdCardFilePtr();

    const char *GetIdCardFilePath();

    void SetIdCardFilePath(const char *path);

    void SetIdCardDataLocation(int id_card, int city);

    IdCardDataLocation GetIdCardDataLocation();

    int GetIdCardFileDataAndLen(FILE *file, struct IdCardData *idCardData, IdCardDataLocation Location);

    char *QueryIdCardFileData(struct IdCardData *idCardData, char *searchedString);

    IdCardData *GetIdCardData();

private:

    int id_card_max_line{};
    std::string id_card_path{};
    FILE *IdCardFile{};
    IdCardDataLocation id_card_location;
    struct IdCardData *id_card_array;

};


#endif //QT_CQUERYIDCARDFROMFILE_H
