#include "Element.h"

ListElement::~ListElement()
{
    for(std::vector<Element*>::iterator i = element.begin(); i != element.end(); ++i)
    {
        Element *e = *i;
        int type = e->getType();
        if(type == TYPE_INT)
        {
            IntegerElement *ie = (IntegerElement *)e;
            delete ie;
        }
        else if(type == TYPE_STRING)
        {
            StringElement *se = (StringElement *)e;
            delete se;
        }
        else if(type == TYPE_LIST)
        {
            ListElement *le = (ListElement *)e;
            delete le;
        }
        else if(type == TYPE_DICT)
        {
            DictElement *de = (DictElement *)e;
            delete de;
        }
    }
}

void ListElement::addToList(Element *e)
{
    element.push_back(e);
}


void ListElement::setBegin()
{
    iter = element.begin();
}

Element *ListElement::getNext()
{
    //static std::vector<Element*>::iterator i = element.begin();
    Element *current;
    if (iter != element.end())
    {
        current = *iter;
        iter++;
        return current;
    }
    else
        return 0;
}

KeyValue::~KeyValue()
{
    delete key;
    int type = value->getType();
    if(type == TYPE_INT)
    {
        IntegerElement *ie = (IntegerElement *)value;
        delete ie;
    }
    else if(type == TYPE_STRING)
    {
        StringElement *se = (StringElement *)value;
        delete se;
    }
    else if(type == TYPE_LIST)
    {
        ListElement *le = (ListElement *)value;
        delete le;
    }
    else if(type == TYPE_DICT)
    {
        DictElement *de = (DictElement *)value;
        delete de;
    }
}

DictElement::~DictElement()
{
    for(std::vector<KeyValue*>::iterator i = element.begin(); i != element.end(); ++i)
    {
        KeyValue *kv = *i;
        delete kv;
    }
}

void DictElement::addToDict(KeyValue *e)
{
    element.push_back(e);
}

Element *DictElement::findValue(const char *key)
{
    for(std::vector<KeyValue*>::iterator i = element.begin(); i != element.end(); ++i)
    {
        KeyValue *kv = *i;
        if(strcmp(kv->getKey()->getElement(), key) == 0)
        {
            return kv->getValue();
        }
    }
    return 0;
}

