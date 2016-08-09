#ifndef BENCODEPARSER_H
#define BENCODEPARSER_H

#include "Element.h"

class BencodeParser
{
public:
    BencodeParser(const char *filename);
    ~BencodeParser();
    int parse();
    int findFiles();
private:
    const char *filename;
    char *buffer;  //动态数组,缓存文件内容
    const char *begin;
    const char *end; //数组结束位置的下一个位置
    const char *ptr;
    long length;
    int readToBuffer();
    int readInteger(IntegerElement &ie);
    int readString(StringElement &se);
    int readList(ListElement &v);
    int readDict(DictElement &v);
};

#endif
