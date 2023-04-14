#pragma once

#include "cstdio"
#include<iostream>
#include "string"
#define SAVE_TO_EXCEL 1
#define MAX_Source_Data_Len 10000
struct SourceData {
    char data[2048];
    char mobilePhone[12];
    char idCard[20];
    char ipaddr[16];
};
struct SourceDataLocation {
    int ipColumn=2;
    int mobileNumberColumn=1;
    int idCardColumn=3;
};

class COperateExcel {
public:
    COperateExcel();

    ~COperateExcel();

    void Create(long long max_len);

    void Release();

    bool InsertData(int x, int y, const char *data, int len);//插入
    bool DeleteData(int x, int y);//删除
    bool UpdateData(int x, int y, const char *data, int len);//更新
    const char * QueryData(int x, int y);//查询

    bool SaveDataToExcel(const char *path);

    bool OpenFileOnMode(const char *szFilePath, const char *szOpenMode, int mode = 0);

    int GetSourceFileDataAndLen(FILE *file, struct SourceData *sourceData, SourceDataLocation Location);

    void SetLocation(int ip, int phone, int id);

    SourceData *GetSourceData();

    SourceDataLocation GetLocation();

    FILE *GetFilePtr();
    const char *GetFilePath();

    void SetFilePtr();
    void SetFilePath(const char *path);

    int GetStartLine() const;
    int GetEndLine() const;
    void SetStartLine(int start_line);
    void SetEndLine(int end_line);

    int GetNowLine() const;
private:
    int start{};
    int end{};
    int max_columns{};
    int max_row{};
    int now_line{};
    SourceDataLocation location{};
    std::string m_szFilePath{};
    FILE *m_pFile{};
    FILE *save_file{};
    std::string query_Data ;
    struct SourceData *source_data_array{};
};

