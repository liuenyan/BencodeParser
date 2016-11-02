#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <vector>
#include <string>
#include <map>
#include <memory>

enum ElementType {
    element_default,
    element_integer,
    element_string,
    element_list,
    element_dict
};

class Element
{
    public:
        int get_element_type() const {return element_type;};
    protected:
        Element(int t) : element_type(t){};
        virtual ~Element(){};
    private:
        int element_type{element_integer};
};

class IntegerElement : public Element
{
    public:
        IntegerElement(long i) : Element(element_integer), element(i){};
        ~IntegerElement(){};
        long getElement() const {return element;};
        void setElement(long element){this->element = element;};
    private:
        long element;
};

class StringElement : public Element
{
    public:
        StringElement(std::string s) : Element(element_string), element(s) {};
        ~StringElement(){};
        std::string getElement() const {return element;};
        void setElement(std::string element){this->element = element;};
    private:
        std::string element;
};

class ListElement : public Element
{
    public:
        typedef std::vector<std::shared_ptr<Element>>::const_iterator \
            list_iter;
        ListElement():Element(element_list){};
        ~ListElement(){}; 
        void addToList(std::shared_ptr<Element> e)
        {
            element.push_back(e);
        };
        std::pair<list_iter, list_iter> get_iterators() const
        {
            return std::pair<list_iter, list_iter>
                (element.cbegin(), element.cend());
        }

    private:
        std::vector<std::shared_ptr<Element>> element;
};

class DictElement : public Element
{
    public:
        DictElement() : Element(element_dict){};
        ~DictElement(){};
        void addToDict(std::shared_ptr<StringElement> key,
                std::shared_ptr<Element> value)
        {
            element.emplace(key->getElement(), value); 
        };
        std::shared_ptr<Element> findValue(std::string key) const
        {
            auto it = element.find(key);
            return (it != element.end()) ? (it->second) : (nullptr);
        };
    private:
        std::map<std::string, 
            std::shared_ptr<Element>> element;
};

#endif
