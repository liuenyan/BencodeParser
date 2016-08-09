#include "BencodeParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

BencodeParser::BencodeParser(const char *filename) : buffer(0), begin(0) ,end(0), ptr(0), length(0)
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

int BencodeParser::readInteger(IntegerElement &ie)
{

    if(++ptr == end)
        return -1;
    char *endptr;
    long integer = strtol(ptr, &endptr, 10);
    if((integer == 0 && endptr == ptr) || *endptr != 'e') //没有找到有效的整数
    {
        return -1;
    }
    ie.setElement(integer);
    //ptr++;
    ptr = endptr + 1;
    return 0;
}

int BencodeParser::readString(StringElement &se)
{
    char *endptr;
    long string_length = strtol(ptr, &endptr, 10);
    if((string_length == 0 && endptr == ptr) || *endptr != ':')
    {
        return -1;
    }
    ptr = endptr + 1;
    char *string_buffer = new char[string_length + 1];
    memcpy(string_buffer, ptr, string_length);
    string_buffer[string_length] = 0;
    ptr += string_length;
    se.setElement(string_buffer);
    return 0;
}

int BencodeParser::readList(ListElement &le)
{
    if(++ptr == end)
        return -1;

    int ret; 

    while(ptr != end)
    {
        if(*ptr == 'e') //到达列表结束
        {
            ptr++;
            return 0;
        }
        if(*ptr == 'i')
        {
            IntegerElement *ie = new IntegerElement;
            ret = readInteger(*ie);
            if(ret == -1)
            {
                delete ie;
                return -1;
            }
            else
            {
                le.addToList(ie);
            }
        }
        else if(isdigit(*ptr))
        {
            StringElement *se = new StringElement;
            ret = readString(*se);
            if(ret == -1)
            {
                delete se;
                return -1;
            }
            else
            {
                le.addToList(se);
            }
        }
        else if(*ptr == 'l')
        {
            ListElement *le_ = new ListElement;
            ret = readList(*le_);
            if(ret == -1)
            {
                delete le_;
                return -1;
            }
            else
            {
                le.addToList(le_);
            }
        }
        else if(*ptr == 'd')
        {
            DictElement *de = new DictElement;
            ret = readDict(*de);
            if(ret == -1)
            {
                delete de;
                return -1;
            }
            else
            {
                le.addToList(de);
            }
        }

        else
        {
            return -1;
        }
    }
    return -1;
}

int BencodeParser::readDict(DictElement &de)
{
    if(++ptr == end)
        return -1;

    int ret;

    while(ptr != end)
    {
        if(*ptr == 'e') //到达字典结束
        {
            ptr++;
            return 0;
        }
        //分析key
        if(!isdigit(*ptr))
            return -1;
        StringElement *key = new StringElement;
        ret = readString(*key);
        if(ret == -1)
        {
            delete key;
            return -1;
        }
        //分析value
        if(*ptr == 'i')
        {
            IntegerElement *ie = new IntegerElement;
            ret = readInteger(*ie);
            if(ret == -1)
            {
                delete key;
                delete ie;
                return -1;
            }
            else
            {
                KeyValue *kv = new KeyValue(key, ie);
                de.addToDict(kv);
            }
        }
        else if(isdigit(*ptr))
        {
            StringElement *se = new StringElement;
            ret = readString(*se);
            if(ret == -1)
            {
                delete key;
                delete se;
                return -1;
            }
            else
            {
                KeyValue *kv = new KeyValue(key, se);
                de.addToDict(kv);
            }
        }
        else if(*ptr == 'l')
        {
            ListElement *le = new ListElement;
            ret = readList(*le);
            if(ret == -1)
            {
                delete key;
                delete le;
                return -1;
            }
            else
            {
                KeyValue *kv = new KeyValue(key, le);
                de.addToDict(kv);
            }
        }
        else if(*ptr == 'd')
        {
            DictElement *de_ = new DictElement;
            ret = readDict(*de_);
            if(ret == -1)
            {
                delete key;
                delete de_;
                return -1;
            }
            else
            {
                KeyValue *kv = new KeyValue(key, de_);
                de.addToDict(kv);
            }
        }

        else
        {
            return -1;
        }
    }
    return 0;
}

int BencodeParser::parse()
{
    if(readToBuffer() == -1)
        return -1;

    int ret;

    //ptr = buffer;
    if(*ptr == 'i')
    {
        IntegerElement *ie = new IntegerElement;
        ret = readInteger(*ie);
        if(ret == -1)
        {
            delete ie;
            return -1;
        }
        else
        {
            cout << ie->getElement() << endl;
            delete ie;
            return 0;
        }
    }
    else if(isdigit(*ptr))
    {
        StringElement *se = new StringElement;
        ret = readString(*se);
        if(ret == -1)
        {
            delete se;
            return -1;
        }
        else
        {
            cout << se ->getElement() << endl;
            delete se;
            return 0;
        }
    }
    else if(*ptr == 'l')
    {
        ListElement *le = new ListElement;
        ret = readList(*le);
        if(ret == -1)
        {
            delete le;
            return -1;
        }
        else
        {
            delete le;
            return 0;
        }
    }
    else if(*ptr == 'd')
    {
        DictElement *de = new DictElement;
        ret = readDict(*de);
        if(ret == -1)
        {
            delete de;
            return -1;
        }
        else
        {
            delete de;
            return 0;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

int BencodeParser::findFiles()
{
    if(readToBuffer() == -1)
        return -1;

    int ret;
    const char *flag = "5:files";
    ptr = (const char *)memmem((const void *)buffer, length, (const void *)flag, 7);
    if(ptr == NULL)
        return -1;

    ptr += 7;
    ListElement *le = new ListElement;
    ret = readList(*le);
    if (ret == -1)
    {
        delete le;
        return -1;
    }

    le->setBegin(); //将位置置为列表的第一个元素
    while(Element *element = le->getNext())
    {
        if(element->getType() == TYPE_DICT)
        {
            DictElement *de = (DictElement *)element;
            Element *length_element = de->findValue("length");
            if(length_element != 0)
            {
                cout << "length:" << ((IntegerElement *)length_element)->getElement() << endl;
            }
            ListElement *path_element = (ListElement *)de->findValue("path");
            if(path_element != 0)
            {
                cout << "path:" << endl;
                path_element->setBegin();
                while(StringElement *path = (StringElement *)path_element->getNext())
                {
                    cout << path->getElement() << endl;
                }
            }
        }
        else
        {
            return -1;
        }
    }
    delete le;
    return 0;
}

