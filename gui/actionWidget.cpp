#include "actionWidget.h"
#include "logging.h"
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <wx/string.h>

#include <Python.h>
#include <iostream>

extern PyObject *pWrite;

wxStaticBoxSizer* actionBox;


ActionWidget::ActionWidget(std::string name) {
    this->name = name;
    id = wxNewId();
}

void ActionWidget::setEnable(bool en) {}

void ActionWidget::addTo(wxSizer *sizer) {}


/*
 * Assigns name, text and creates a click event.
 */
ActionButton::ActionButton(wxWindow *parent, std::string name,
                                             std::string text)
             :ActionWidget(name),
              wxButton(parent, id, wxString(text), wxDefaultPosition,
                       wxDefaultSize, 0, wxDefaultValidator, wxString(name))
{
    /*
     * Data to be retrived from wxSizerItem.
     * This pointer as ActionWidget is to solve multi inheritance pointer
     * offset problem.
     */
    SetClientData((void*)((ActionWidget*)this));
    
    // On click event
    wxButton::Connect(
        id, wxEVT_COMMAND_BUTTON_CLICKED,
        (wxObjectEventFunction)&ActionButton::onClick
    );
}

void ActionButton::setEnable(bool en) {
    Enable(en);
}

void ActionButton::addTo(wxSizer *sizer) {
    sizer->Add(this, 0, wxALL|wxEXPAND, 5);
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
               :ActionWidget(name),
               wxTextCtrl(parent, id, wxEmptyString, wxDefaultPosition,
                          wxDefaultSize, 0, wxDefaultValidator, wxString(name))
{
    // Data to be retrived from wxSizerItem
    SetClientData((void*)((ActionWidget*)this));
    
    wxTextCtrl::Connect(
        id, wxEVT_COMMAND_TEXT_UPDATED,
        (wxObjectEventFunction)&ActionTextCtrl::onUpdate
    );
    label = new wxStaticText(
                parent, wxNewId(), wxString(text), wxDefaultPosition,
                wxDefaultSize, 0, wxString(name + "Label")
            );
    box = new wxBoxSizer(wxHORIZONTAL);
    inputType = type;
}

void ActionTextCtrl::setEnable(bool en) {
    label->Enable(en);
    Enable(en);
}

void ActionTextCtrl::addTo(wxSizer *sizer) {
    box->Add(label, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    box->Add(this, 2, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
    sizer->Add(box, 0, wxALL|wxEXPAND, 5);
}

void ActionTextCtrl::onUpdate(wxCommandEvent& event) {
    ActionTextCtrl *txt = (ActionTextCtrl*)event.GetEventObject();
    Attribute attr = Attribute(txt->name, txt->GetValue().ToStdString(),
                               txt->inputType);
    if(attr.isValid())
        attr.sendToClient();
}


/*
 * The list containing the user-wanted widgets.
 * Control using iterations.
 */
ActionWidgetTable::ActionWidgetTable(wxWindow *win, wxSizer *sizer) {
    parent = win;
    box = sizer;
    if(box != NULL)
        clear();
}

void ActionWidgetTable::setEnable(bool en) {
    for(int i=0; i < box->GetItemCount(); i++) {
        ActionWidget *widget = getWidget(i);
        widget->setEnable(en);
    }
}

void ActionWidgetTable::addWidget(ActionWidget *widget) {
    // Make sure no widgets have the same name
    for(int i=0; i < box->GetItemCount(); i++) {
        ActionWidget *item = getWidget(i);
        if(widget->name == item->name) {
            logger.log(("Action widget '" + widget->name +
                       "' already exists.\n").c_str(), 1);
        }
    }
    widget->addTo(box);
    parent->Layout();
}

ActionWidget *ActionWidgetTable::getWidget(int i) {
    wxWindow *win = box->GetItem(i)->GetWindow();
    if(win == NULL) { // Should be a widget with a label inside a sizer
        wxSizer *sizer = box->GetItem(i)->GetSizer();
        win = sizer->GetItem(1)->GetWindow();
    }
    return (ActionWidget*) win->GetClientData();
}

void ActionWidgetTable::clear() {
    box->Clear(true);
}
