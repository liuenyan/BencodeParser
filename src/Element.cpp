#include "Element.h"
#include <iostream>

using namespace std;

std::ostream &Element::indent(std::ostream &os, unsigned int indent) const
{
    for(unsigned int i=0; i!=indent; ++i)
    {
        os << "  ";
    }
    return os;
}

ostream &IntegerElement::pretty(ostream &os, unsigned int idt) const
{
    indent(os, idt);
    os << std::to_string(element);
    return os;
}

ostream &StringElement::pretty(ostream &os, unsigned int idt) const
{
    indent(os, idt);
    os << element;
    return os;
}

ostream &ListElement::pretty(ostream &os, unsigned int idt) const
{
    indent(os, idt);
    os << "[" << endl;

    for(auto it=element.cbegin(); it!=element.cend(); ++it)
    {
        Element *e = it->get();
        e->pretty(os, idt+1);
        os << "," << endl;
    }
    indent(os, idt);
    os << "]";
    return os;
}


ostream &DictElement::pretty(ostream &os, unsigned int idt) const
{
    indent(os, idt);
    os << "{" << endl;
    for(auto it=element.cbegin(); it!=element.cend(); ++it)
    {
        indent(os, idt+1);
        os << it->first << ":" << endl;
        it->second->pretty(os, idt+2);
        os << "," << endl;
    }
    indent(os, idt);
    os << "}";
    return os; 
}

std::ostream &operator<<(std::ostream &os, IntegerElement& e)
{
    e.pretty(os); 
    return os;
}

std::ostream &operator<<(std::ostream &os, StringElement& e)
{
    e.pretty(os);
    return os;
}

std::ostream &operator<<(std::ostream &os, ListElement& e)
{
    e.pretty(os);
    return os;
}

std::ostream &operator<<(std::ostream &os, DictElement& e)
{
    e.pretty(os);
    return os;
}

