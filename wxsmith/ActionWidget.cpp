#include "ActionWidget.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <wx/string.h>

#include <Python.h>

extern PyObject *pWrite;


using namespace std;

wxStaticBoxSizer* actionBox;

ActionWidget::ActionWidget(wxWindow *parent, string name) {
    this->parent = parent;
    this->name = name;
    id = wxNewId();
}

void ActionWidget::addTo(wxSizer *sizer) {}

void setActionWidgetsEnabled(bool en) {
    for(size_t i=0; i<actionBox->GetItemCount(); i++) {
        ActionWidget *widget = (ActionWidget*)actionBox->GetItem(i);
        if(widget->widgetType == WIDGET_BUTTON)
        {
            wxWindow *win = (wxWindow*)widget;
            win->Enable(en);
        }
        else if(widget->widgetType == WIDGET_TEXTCTRL)
        {
            ActionTextCtrl *txt = (ActionTextCtrl*)widget;
            txt->label->Enable(en);
            txt->Enable(en);
        }
    }
}


ActionButton::ActionButton(wxWindow *parent, string name, string text)
             :ActionWidget(parent, name),
              wxButton(parent, id, wxString(text), wxDefaultPosition,
                       wxDefaultSize, 0, wxDefaultValidator, wxString(name))
{
    widgetType = WIDGET_BUTTON;
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
    char cstr[s.size() + 11 + 1];
    sprintf(cstr, "<<?call %s?>>", s.c_str());

    PyObject *pArgs;
    pArgs = PyTuple_New(1);
    PyObject *pValue = PyString_FromString(cstr);
    PyTuple_SetItem(pArgs, 0, pValue);
    PyObject_CallObject(pWrite, pArgs);
}


ActionTextCtrl::ActionTextCtrl(wxWindow *parent, string name, string text,
                               ActionInputType type)
              :ActionWidget(parent, name),
               wxTextCtrl(parent, id, wxEmptyString, wxDefaultPosition,
                          wxDefaultSize, 0, wxDefaultValidator, wxString(name))
{
    widgetType = WIDGET_TEXTCTRL;
    inputType = type;
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
    char cstr1[s1.size() + 1];
    copy(s1.begin(), s1.end(), cstr1);
    cstr1[s1.size()] = '\0';
    string s2 = txt->GetValue().ToStdString();
    char *cstr2;

    try {
        if(txt->inputType == ACTION_INPUT_INT) {
            cstr2 = new char[s2.size() + 1];
            int i = stoi(s2);
            sprintf(cstr2, "%d", i);
        }
        else if(txt->inputType == ACTION_INPUT_FLOAT) {
            cstr2 = new char[s2.size() + 5];
            float f = stof(s2);
            sprintf(cstr2, "%.3f", f);
        }
        else {
            cstr2 = new char[s2.size() + 1];
            copy(s2.begin(), s2.end(), cstr2);
            cstr2[s2.size()] = '\0';
        }
    }
    catch(logic_error e) {
        return;
    }

    char cstr[s1.size() + strlen(cstr2) + 11 + 1];
    sprintf(cstr, "<<?set %s %s?>>", cstr1, cstr2);

    PyObject *pArgs;
    pArgs = PyTuple_New(1);
    PyObject *pValue = PyString_FromString(cstr);
    PyTuple_SetItem(pArgs, 0, pValue);
    PyObject_CallObject(pWrite, pArgs);

    delete[] cstr2;
}
