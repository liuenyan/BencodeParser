#include "BencodeParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <memory>

using namespace std;

BencodeParser::BencodeParser(const char *filename) 
    : buffer(0), begin(0) ,end(0), ptr(0), length(0)
{
    this->filename = filename;
}

BencodeParser::~BencodeParser()
{
    delete []buffer;
}

int BencodeParser::readToBuffer()
{ 
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
        return -1;

    fseek(fp, 0L, SEEK_END);
    length = ftell(fp);
    rewind(fp);
    buffer = new char[length];
    if(buffer == NULL)
    {
        fclose(fp);
        return -1;
    }
    size_t nread = fread(buffer, length, 1, fp);
    if(nread != 1)
    {
        fclose(fp);
        delete buffer;
        buffer = 0;
        return -1;
    }
    fclose(fp);
    begin = buffer;
    end = buffer + length;
    ptr = buffer;
    return 0;
}

shared_ptr<IntegerElement> BencodeParser::read_integer()
{
    if(++ptr == end)
        return nullptr;
    char *endptr;
    long integer = strtol(ptr, &endptr, 10);
    if((integer == 0 && endptr == ptr) || *endptr != 'e') //没有找到有效的整数
    {
        return nullptr;
    }
    ptr = endptr + 1;
    return make_shared<IntegerElement>(IntegerElement(integer));
}

shared_ptr<StringElement> BencodeParser::read_string()
{
    char *endptr;
    long string_length = strtol(ptr, &endptr, 10);
    if((string_length == 0 && endptr == ptr) || *endptr != ':')
    {
        return nullptr;
    }
    ptr = endptr + 1;
    char *string_buffer = new char[string_length + 1];
    memcpy(string_buffer, ptr, string_length);
    string_buffer[string_length] = 0;
    ptr += string_length;
    auto ret = make_shared<StringElement>(string(string_buffer));
    delete []string_buffer;
    return ret;
}

shared_ptr<ListElement> BencodeParser::read_list()
{
    if(++ptr == end)
        return nullptr;

    auto le = make_shared<ListElement>(ListElement()); 

    while(ptr != end)
    {
        if(*ptr == 'e') //到达列表结束
        {
            ptr++;
            return le;
        }
        if(*ptr == 'i')
        {
            shared_ptr<IntegerElement> ie= read_integer();
            if(!ie)
            {
                return nullptr;
            }
            else
            {
                le->addToList(ie);
            }
        }
        else if(isdigit(*ptr))
        {
            shared_ptr<StringElement> se = read_string();
            if(!se)
            {
                return nullptr;
            }
            else
            {
                le->addToList(se);
            }
        }
        else if(*ptr == 'l')
        {
            shared_ptr<ListElement> le_ = read_list();
            if(!le_)
            {
                return nullptr;
            }
            else
            {
                le->addToList(le_);
            }
        }
        else if(*ptr == 'd')
        {
            shared_ptr<DictElement> de = read_dict();
            if(!de)
            {
                return nullptr;
            }
            else
            {
                le->addToList(de);
            }
        }

        else
        {
            return nullptr;
        }
    }
    return nullptr;
}

shared_ptr<DictElement> BencodeParser::read_dict()
{
    if(++ptr == end)
        return nullptr;

    auto de = make_shared<DictElement>(DictElement());

    while(ptr != end)
    {
        if(*ptr == 'e') //到达字典结束
        {
            ptr++;
            return de;
        }
        //分析key
        if(!isdigit(*ptr))
            return nullptr;
        
        auto key = read_string();
        if(!key)
        {
            return nullptr;
        }
        //分析value
        if(*ptr == 'i')
        {
            auto value =read_integer();
            if(!value)
            {
                return nullptr;
            }
            else
            {
                de->addToDict(key, value);
            }
        }
        else if(isdigit(*ptr))
        {
            auto value = read_string();
            if(!value)
            {
                return nullptr;
            }
            else
            {
                de->addToDict(key, value);
            }
        }
        else if(*ptr == 'l')
        {
            auto value = read_list();
            if(!value)
            {
                return nullptr;
            }
            else
            {
                de->addToDict(key, value);
            }
        }
        else if(*ptr == 'd')
        {
            auto value = read_dict();
            if(!value)
            {
                return nullptr;
            }
            else
            {
                de->addToDict(key, value);
            }
        }

        else
        {
            return nullptr;
        }
    }
    return nullptr;
}


shared_ptr<Element> BencodeParser::parse()
{
    if(readToBuffer() == -1)
        return nullptr;

    if(*ptr == 'i')
        return read_integer();
    
    else if(isdigit(*ptr))
        return read_string();
    
    else if(*ptr == 'l')
        return read_list();
    
    else if(*ptr == 'd')
        return read_dict();
    
    else
        return nullptr;
    
    return nullptr;
}

int BencodeParser::findFiles()
{
    if(readToBuffer() == -1)
        return -1;

    const char *flag = "5:files";
    ptr = (const char *)memmem((const void *)buffer, length, (const void *)flag, 7);
    if(ptr == NULL)
        return -1;

    ptr += 7;
    auto le = read_list();
    if (!le)
    {
        return -1;
    }
    cout << *le << endl;
    return 0;
}

