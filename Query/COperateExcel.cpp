#include <cstring>
#include "COperateExcel.h"

COperateExcel::COperateExcel() {
    source_data_array = (SourceData *) (malloc(sizeof(struct SourceData) * MAX_Source_Data_Len));
    if (source_data_array != nullptr) {
        memset(source_data_array, 0, sizeof(struct SourceData) * MAX_Source_Data_Len);
    }
}

COperateExcel::~COperateExcel() {
    Release();
}

const char * COperateExcel::QueryData(int x, int y) {
    if (x <= 0 || y <= 0 || y > max_columns || x > max_row) {
        return const_cast<char *>("null");
    }
    int counts = 0;
    int count_list[1024] = {0};
    for (int i = 0; i < strlen(source_data_array[y - 1].data); i++) {
        if (source_data_array[y - 1].data[i] == ',' || source_data_array[y - 1].data[i] == '\n') {
            count_list[++counts] = i + 1;
            if (counts == x) {
                break;
            }
        }
    }
    for (int j = count_list[x - 1]; j < count_list[x] - 1; j++) {
        query_Data[j - count_list[x - 1]] = source_data_array[y - 1].data[j];
    }
    return query_Data.c_str();
}

bool COperateExcel::OpenFileOnMode(const char *szFilePath, const char *szOpenMode, int mode) {
    if (mode == SAVE_TO_EXCEL) {
        SetFilePath(szFilePath);
        if ((this->save_file = fopen(szFilePath, szOpenMode)) == nullptr) {
            if ((this->save_file = fopen(szFilePath, szOpenMode)) == nullptr) {
                return false;
            }
        }
    } else {
        SetFilePath(szFilePath);
        this->m_pFile = fopen(szFilePath, szOpenMode);
        if ((this->m_pFile = fopen(szFilePath, szOpenMode)) == nullptr) {
            this->m_pFile = fopen(szFilePath, szOpenMode);
            return false;
        }
    }
    return true;
}

int COperateExcel::GetSourceFileDataAndLen(FILE *file, SourceData *sourceData, SourceDataLocation Location) {
    int len = 0;
    int counts = 0;
    int count_list[128] = {0};
    if (file == nullptr) {

        return 0;
    }
    char buf[2048]{};
    memset(buf, 0, 2048);

    //读取每一行,每次读入10000行防止内存爆满。
    while (fgets(buf, 2048, file) != nullptr && this->now_line <= MAX_Source_Data_Len) {
        strcpy_s(sourceData[len].data, buf);
        for (int i = 0; i < strlen(buf); ++i) {
            if (buf[i] == ',' || buf[i] == '\n') {
                count_list[++counts] = i + 1;
            }
            if (counts == Location.mobileNumberColumn) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    sourceData[len].mobilePhone[j - count_list[counts - 1]] = buf[j];
                }

            } else if (counts == Location.ipColumn) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    sourceData[len].ipaddr[j - count_list[counts - 1]] = buf[j];
                }

            } else if (counts == Location.idCardColumn) {
                for (int j = count_list[counts - 1]; j < count_list[counts] - 1; ++j) {
                    sourceData[len].idCard[j - count_list[counts - 1]] = buf[j];
                }
            }
        }
        memset(buf, 0, 2048);
        memset(count_list, 0, sizeof count_list);
        this->max_row = counts;
        counts = 0;
        ++len;
        this->now_line++;
    }
    this->max_columns = len;
    fclose(file);
    return len;
}

void COperateExcel::SetLocation(int ip, int phone, int id) {
    this->location.ipColumn = ip;
    this->location.mobileNumberColumn = phone;
    this->location.idCardColumn = id;
}

SourceData *COperateExcel::GetSourceData() {
    return this->source_data_array;
}

SourceDataLocation COperateExcel::GetLocation() {
    return this->location;
}


FILE *COperateExcel::GetFilePtr() {
    return this->m_pFile;
}

void COperateExcel::SetFilePtr() {
    this->m_pFile = fopen(m_szFilePath.c_str(), "r");
    if (this->m_pFile == nullptr) {
        this->m_pFile = fopen(m_szFilePath.c_str(), "r");
        if (this->m_pFile == nullptr) {
            throw "文件打开失败";
        }
    }
}

const char *COperateExcel::GetFilePath() {
    return m_szFilePath.c_str();
}

void COperateExcel::SetFilePath(const char *path) {
    this->m_szFilePath = const_cast<char *>(path);
}

bool COperateExcel::UpdateData(int x, int y, const char *data, int len) {
    if (x <= 0 || y <= 0 || y > max_columns || x > max_row) {
        return false;
    }
    int counts = 0;
    int count_list[128] = {0};
    std::string string(source_data_array[y - 1].data);
    for (int i = 0; i < strlen(source_data_array[y - 1].data); i++) {
        if (source_data_array[y - 1].data[i] == ',' || source_data_array[y - 1].data[i] == '\n') {
            count_list[++counts] = i + 1;
            if (counts == x) {
                string.erase(count_list[x - 1], count_list[x] - count_list[x - 1] - 1);
                string.insert(count_list[x - 1], data);
                strcpy_s(source_data_array[y - 1].data, string.data());
                break;
            }
        }
    }
    return true;
}

bool COperateExcel::InsertData(int x, int y, const char *data, int len) {
    if (x <= 0 || y <= 0 || y > max_columns || x > max_row) {
        return false;
    }
    rewind(this->m_pFile);
    int counts = 0;
    int count_list[128] = {0};
    std::string string(source_data_array[y - 1].data);
    std::string data_str(data);
    data_str += ",";
    for (int i = 0; i < strlen(source_data_array[y - 1].data); i++) {
        if (source_data_array[y - 1].data[i] == ',' || source_data_array[y - 1].data[i] == '\n') {
            count_list[++counts] = i + 1;
            if (counts == x) {
                string.insert(count_list[counts], data_str);
                strcpy_s(source_data_array[y - 1].data, string.data());
                break;
            }
        }
    }
    return true;
}

bool COperateExcel::DeleteData(int x, int y) {
    if (x <= 0 || y <= 0 || y > max_columns || x > max_row) {
        return false;
    }
    int counts = 0;
    int count_list[128] = {0};
    std::string string(source_data_array[y - 1].data);
    for (int i = 0; i < strlen(source_data_array[y - 1].data); i++) {
        if (source_data_array[y - 1].data[i] == ',' || source_data_array[y - 1].data[i] == '\n') {
            count_list[++counts] = i + 1;
            if (counts == x) {
                string.erase(count_list[x - 1], count_list[x] - count_list[x - 1] - 1);
                strcpy_s(source_data_array[y - 1].data, string.data());
                break;
            }
        }
    }
    return true;
}

bool COperateExcel::SaveDataToExcel(const char *path) {
    if (!OpenFileOnMode(path, "w+", SAVE_TO_EXCEL)) {
        return false;
    }
    for (int i = 0; i < max_columns; ++i) {
        fprintf(save_file, "%s", source_data_array[i].data);
    }
    fclose(this->save_file);
    return true;
}

void COperateExcel::Create(long long max_len) {
    free(source_data_array);
    source_data_array = static_cast<SourceData *>(malloc(sizeof(struct SourceData) * max_len));
}

void COperateExcel::Release() {
//    if (this->query_Data != nullptr) {
//        delete []this->query_Data;
//        this->query_Data = nullptr;
//    }
    if (this->source_data_array != nullptr) {
        free(source_data_array);
        this->source_data_array = nullptr;
    }
    if (this->save_file != nullptr) {
        fclose(this->save_file);
        free(this->save_file);
        //this->save_file = nullptr;
    }
    if (this->m_pFile != nullptr) {
        fclose(this->m_pFile);
        free(this->m_pFile);
        //this->m_pFile = nullptr;
    }
}

void COperateExcel::SetEndLine(int end_line) {
    this->end = end_line;
}

void COperateExcel::SetStartLine(int start_line) {
    this->start = start_line;
}

int COperateExcel::GetEndLine() const {
    return this->end;
}

int COperateExcel::GetStartLine() const {
    return this->start;
}

int COperateExcel::GetNowLine() const {
    return this->now_line;
}
