#ifndef __ATTRIBUTE_H__
#define __ATTRIBUTE_H__

#include <Python.h>
#include <string>
#include <vector>
#include <wx/textctrl.h>
#include <wx/string.h>


enum AttributeType {
    ATTRIBUTE_INT,
    ATTRIBUTE_FLOAT,
    ATTRIBUTE_CHAR,
    ATTRIBUTE_STRING
};


class AttributeTable;


/*
 * A key-value pair with some extra datas.
 * The value is a string and can be parsed to any other data types.
 * The purpose is to use in communication with files and external devices.
 */
class Attribute {
  public:
    Attribute(std::string n, std::string v,
              AttributeType t=ATTRIBUTE_STRING, bool b=false);

    // Setters and getters for various data types
    void setValue(int);
    void setValue(float);
    void setValue(char);
    void setValue(std::string);
    void setValue(wxString);
    int getInt();
    float getFloat();
    char getChar();
    std::string getName();
    std::string getValue();
    PyObject* getPyObject();
    
    bool isDisplayed();
    bool isValid();
    void sendToClient();
  private:
    std::string name;
    std::string value;
    AttributeType type;
    bool displayed;
    bool valid;
};


/*
 * Handles a group of attributes. Storage and display.
 * Uses binary search algorithm which is O.log(n).
 */
class AttributeTable {
  public:
    AttributeTable(wxTextCtrl *txt=NULL);
    void setTextDisplayGUI(wxTextCtrl*);
    void addAttribute(Attribute*);
    void addAttribute(std::string, std::string);
    Attribute *getAttribute(std::string);
    void clear();
    void flush();
  private:
    std::vector<Attribute*> attributes;
    std::vector<Attribute*> displayAttributes;
    wxTextCtrl *textDisplayGUI;
    
    using attribute_iter = std::vector<Attribute*>::iterator;
    static attribute_iter searchedPosition;
    
    bool dChanged = false;
};

extern AttributeTable attributeTable;

#endif
