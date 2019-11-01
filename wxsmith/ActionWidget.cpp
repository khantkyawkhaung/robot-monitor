#include "ActionWidget.h"
#include <wx/string.h>


#include <Python.h>

extern PyObject *pClickCommand;
extern PyObject *pTextBox;


using namespace std;

wxStaticBoxSizer* actionBox;

ActionWidget::ActionWidget(wxWindow *parent, string name) {
    this->parent = parent;
    this->name = name;
    id = wxNewId();
}

void ActionWidget::addTo(wxSizer *sizer) {}


ActionButton::ActionButton(wxWindow *parent, string name, string text)
             :ActionWidget(parent, name),
              wxButton(parent, id, wxString(text), wxDefaultPosition,
                       wxDefaultSize, 0, wxDefaultValidator, wxString(name))
{
    wxButton::Connect(
        id, wxEVT_COMMAND_BUTTON_CLICKED,
        (wxObjectEventFunction)&ActionButton::onClick
    );
}

void ActionButton::addTo(wxSizer *sizer) {
    sizer->Add(this, 0, wxALL|wxEXPAND, 5);
    parent->Layout();
}

void ActionButton::onClick(wxCommandEvent& event) {
    ActionButton *btn = (ActionButton*)event.GetEventObject();

    string s = btn->name;
    char cstr[s.size() + 1];
    copy(s.begin(), s.end(), cstr);
    cstr[s.size()] = '\0';

    PyObject *pArgs;
    pArgs = PyTuple_New(1);
    PyObject *pValue = PyString_FromString(cstr);
    PyTuple_SetItem(pArgs, 0, pValue);
    PyObject_CallObject(pClickCommand, pArgs);
}


ActionTextCtrl::ActionTextCtrl(wxWindow *parent, string name, string text,
                               ActionInputType type)
              :ActionWidget(parent, name),
               wxTextCtrl(parent, id, wxEmptyString, wxDefaultPosition,
                          wxDefaultSize, 0, wxDefaultValidator, wxString(name))
{
    wxTextCtrl::Connect(
        id, wxEVT_COMMAND_TEXT_UPDATED,
        (wxObjectEventFunction)&ActionTextCtrl::onUpdate
    );
    label = new wxStaticText(
                parent, wxNewId(), wxString(text), wxDefaultPosition,
                wxDefaultSize, 0, wxString(name + "Label")
            );
    box = new wxBoxSizer(wxHORIZONTAL);
}

void ActionTextCtrl::addTo(wxSizer *sizer) {
    box->Add(label, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    box->Add(this, 2, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    sizer->Add(box, 0, wxALL|wxEXPAND, 5);
    parent->Layout();
}

void ActionTextCtrl::onUpdate(wxCommandEvent& event) {
    ActionTextCtrl *txt = (ActionTextCtrl*)event.GetEventObject();

    string s1 = txt->name;
    string s2 = txt->GetValue().ToStdString();
    char cstr1[s1.size() + 1];
    char cstr2[s2.size() + 1];
    copy(s1.begin(), s1.end(), cstr1);
    copy(s2.begin(), s2.end(), cstr2);
    cstr1[s1.size()] = '\0';
    cstr2[s2.size()] = '\0';

    PyObject *pArgs;
    pArgs = PyTuple_New(2);
    PyObject *pValue1 = PyString_FromString(cstr1);
    PyObject *pValue2 = PyString_FromString(cstr2);
    PyTuple_SetItem(pArgs, 0, pValue1);
    PyTuple_SetItem(pArgs, 1, pValue2);
    PyObject_CallObject(pTextBox, pArgs);
}
