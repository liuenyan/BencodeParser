#include "Element.h"
#include <iostream>

using namespace std;

ostream &Element::indent(ostream &os, unsigned int indent) const
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
    os << to_string(element);
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
        (*it)->pretty(os, idt+1);
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

ostream &operator<<(ostream &os, Element &e)
{
    e.pretty(os);
    return os;
}

