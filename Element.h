#ifndef ELEMENT_H
#define ELEMENT_H

#include "ElementType.h"
#include <vector>
#include <string.h>

class Element
{
public:
    Element(int type){this->type = type;};
    ~Element(){};
    int getType(){return type;};
protected:
   int type;
};

class IntegerElement : public Element
{
public:
    IntegerElement() : Element(TYPE_INT){};
    ~IntegerElement(){};
    long getElement(){return element;};
    void setElement(int element){this->element = element;};
private:
    long element;
};

class StringElement : public Element
{
public:
    StringElement() : Element(TYPE_STRING), element(0){};
    ~StringElement(){delete []element;};
    const char *getElement(){return element;};
    void setElement(const char *element){this->element = element;};
private:
    const char *element;
};

class ListElement : public Element
{
public:
    ListElement() : Element(TYPE_LIST){};
    ~ListElement(); 
    void addToList(Element *e);
    void setBegin();     /*将迭代器指向vector的开始位置*/
    Element *getNext();  /*获得vector的下一个元素*/
private:
    std::vector<Element*> element;
    std::vector<Element*>::iterator iter;
};

class KeyValue
{
public:
    KeyValue() : key(0), value(0){};
    KeyValue(StringElement *key, Element *value)
    {
        this->key = key;
        this->value = value;
    };
    ~KeyValue();
    StringElement *getKey(){return key;};
    Element *getValue(){return value;};
private:
    StringElement *key;
    Element *value;
};

class DictElement : public Element
{
public:
    DictElement() : Element(TYPE_DICT){};
    ~DictElement();
    void addToDict(KeyValue *e);
    Element *findValue(const char *key); 
private:
    std::vector<KeyValue*> element;
};

#endif
