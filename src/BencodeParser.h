#ifndef BENCODEPARSER_H
#define BENCODEPARSER_H

#include "Element.h"
#include <memory>

class BencodeParser
{
public:
    BencodeParser(const char *filename);
    ~BencodeParser();
    std::shared_ptr<Element> parse();
    int findFiles();
private:
    const char *filename;
    char *buffer;  //动态数组,缓存文件内容
    const char *begin;
    const char *end; //数组结束位置的下一个位置
    const char *ptr;
    long length;
    int readToBuffer();
    std::shared_ptr<IntegerElement> read_integer();
    std::shared_ptr<StringElement> read_string();
    std::shared_ptr<ListElement> read_list();
    std::shared_ptr<DictElement> read_dict();
};

#endif
