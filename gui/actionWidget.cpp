#include "actionWidget.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <wx/string.h>

#include <Python.h>

extern PyObject *pWrite;

wxStaticBoxSizer* actionBox;


ActionWidget::ActionWidget(wxWindow *parent, std::string name) {
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


/*
 * Assigns name, text and creates a click event.
 */
ActionButton::ActionButton(wxWindow *parent, std::string name,
                                             std::string text)
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

    std::string s = btn->name;
    char cstr[s.size() + 11 + 1];
    sprintf(cstr, "<<?call %s?>>", s.c_str());

    PyObject *pArgs;
    pArgs = PyTuple_New(1);
    PyObject *pValue = PyString_FromString(cstr);
    PyTuple_SetItem(pArgs, 0, pValue);
    PyObject_CallObject(pWrite, pArgs);
}


/*
 * A textbox with a label beside.
 */
ActionTextCtrl::ActionTextCtrl(wxWindow *parent, std::string name,
                               std::string text, AttributeType type)
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
    Attribute attr = Attribute(txt->name, txt->GetValue().ToStdString(),
                               txt->inputType);
    if(attr.isValid())
        attr.sendToClient();
}
