#include "attribute.h"
#include <cmath>
#include <stdexcept>

// Python function from robotmonitor.port module
//extern PyObject *pWrite;


Attribute::Attribute(std::string n, std::string v,
                     AttributeType t, bool b)
{
    name = n;
    type = t;
    displayed = b;
    setValue(v);
}


/*
 * The setter functions converts the data from one type to another.
 */

void Attribute::setValue(int i) {
    if(type == ATTRIBUTE_CHAR) {
        if(i >= 0 && i <= 9)
            value = std::string(1, (char)(i+48));
        else {
            value = "";
            valid = false;
            return;
        }
    }
    else
        value = std::to_string(i);
    valid = true;
}

void Attribute::setValue(float f) {
    if(type == ATTRIBUTE_INT)
        value = std::to_string((int)f);
    else if(type == ATTRIBUTE_FLOAT || type == ATTRIBUTE_STRING)
        value = std::to_string(f);
    else {
        value = "";
        valid = false;
        return;
    }
    valid = true;
}

void Attribute::setValue(char c) {
    if(type == ATTRIBUTE_INT)
        value = std::to_string((int)c);
    else if(type == ATTRIBUTE_CHAR || type == ATTRIBUTE_STRING)
        value = std::string(1, c);
    else {
        value = "NaN";
        valid = false;
        return;
    }
    valid = true;
}

/*
 * The common setter function.
 * Converts the string to appropriate format.
 * Validation checks with a try catch block.
 */
void Attribute::setValue(std::string s) {
    try {
        if(type == ATTRIBUTE_INT) {
            int i = std::stoi(s);
            value = std::to_string(i);
        }
        else if(type == ATTRIBUTE_FLOAT) {
            float f = stof(s);
            value = std::to_string(f);
        }
        else if(type == ATTRIBUTE_CHAR)
            value = std::string(1, s[0]);
        else
            value = s;
        valid = true;
    }
    catch(std::logic_error e) {
        if(type == ATTRIBUTE_INT)
            value = "0";
        else if(type == ATTRIBUTE_FLOAT)
            value = "NaN";
        else if(type == ATTRIBUTE_CHAR)
            value = "";
        valid = false;
    }
}

void Attribute::setValue(wxString s) {
    setValue(s.ToStdString());
}

int Attribute::getInt() {
    try {
        return std::stoi(value);
    }
    catch(std::logic_error e) {
        return 0;
    }
}

float Attribute::getFloat() {
    try {
        return std::stof(value);
    }
    catch(std::logic_error e) {
        return NAN;
    }
}

char Attribute::getChar() {
    if(value.size() > 0)
        return value[0];
    else
        return 0;
}


std::string Attribute::getName() { return name; }

std::string Attribute::getValue() { return value; }

bool Attribute::isDisplayed() { return displayed; }

bool Attribute::isValid() { return valid; }


// Python object type
PyObject* Attribute::getPyObject() {
    if(type == ATTRIBUTE_INT)
        return PyLong_FromLong(getInt());
    else if(type == ATTRIBUTE_FLOAT)
        return PyFloat_FromDouble(getFloat());
    else
        return PyString_FromString(value.c_str());
}

/*
 * Format a message which is a command line.
 * Send it though a Python function.
 */
void Attribute::sendToClient() {
    char cstr[name.size() + value.size() + 12];
    sprintf(cstr, "<<?set %s %s?>>", name.c_str(), value.c_str());
    
    PyObject *pArgs;
    pArgs = PyTuple_New(1);
    PyObject *pValue = PyString_FromString(cstr);
    PyTuple_SetItem(pArgs, 0, pValue);
    //PyObject_CallObject(pWrite, pArgs);
}


AttributeTable::AttributeTable(wxTextCtrl *txt) {
    textDisplayGUI = txt;
}

void AttributeTable::setTextDisplayGUI(wxTextCtrl *txt) {
    textDisplayGUI = txt;
}


using attribute_iter = std::vector<Attribute*>::iterator;

attribute_iter AttributeTable::searchedPosition;


void AttributeTable::addAttribute(Attribute *attr) {
    Attribute *a = getAttribute(attr->getName());
    if(a != NULL) {
        a->setValue(attr->getValue());
        if(a->isDisplayed())
            dChanged = true;
    }
    else {
        attributes.emplace(searchedPosition, attr);
        if(attr->isDisplayed()) {
            displayAttributes.push_back(attr);
            dChanged = true;
        }
    }
}

void AttributeTable::addAttribute(std::string name, std::string value) {
    Attribute *a = getAttribute(name);
    if(a != NULL) {
        a->setValue(value);
        if(a->isDisplayed())
            dChanged = true;
    }
    else
        attributes.emplace(searchedPosition, new Attribute(name, value));
}

/*
 * Binary search algorithm which is O.log(n).
 * If not found, a null pointer is returned and a location for the new object
 * insertion is saved so that the list is kept sorted.
 */
Attribute *AttributeTable::getAttribute(std::string name) {
    if(attributes.size() == 0) {
        searchedPosition = attributes.begin();
        return NULL;
    }
    auto l = attributes.begin();
    auto r = attributes.end()-1;
    while(l <= r) {
        searchedPosition = l + (r-l)/2;
        if(name == (*searchedPosition)->getName())
            return *searchedPosition;
        else if(name > (*searchedPosition)->getName())
            l = searchedPosition + 1;
        else
            r = searchedPosition - 1;
    }
    if(name > (*searchedPosition)->getName())
        ++searchedPosition;
    return NULL;
}

void AttributeTable::clear() {
    for(auto it=attributes.begin(); it!=attributes.end(); it++)
        delete *it;
    attributes.clear();
    displayAttributes.clear();
    flush();
}

// Write names and values on the textbox
void AttributeTable::flush() {
    if(textDisplayGUI == NULL || dChanged == false)
        return;
    
    textDisplayGUI->Clear();
    for(auto it=displayAttributes.begin(); it!=displayAttributes.end(); it++)
    {
        textDisplayGUI->AppendText(wxString((*it)->getName()));
        textDisplayGUI->AppendText(wxString(" : "));
        textDisplayGUI->AppendText(wxString((*it)->getValue()));
        textDisplayGUI->AppendText(wxString("\n"));
    }
}


AttributeTable attributeTable;

