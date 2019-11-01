/***************************************************************
 * Name:      RobotMonitorMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Khant Kyaw Khaung (khantkyawkhaung288@gmail.com)
 * Created:   2019-09-20
 * Copyright: Khant Kyaw Khaung ()
 * License:
 **************************************************************/

#include "ActionWidget.h"
#include "RobotMonitorMain.h"
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <wx/msgdlg.h>


#include <Python.h>

PyObject *pName, *pModule, *pDict;
PyObject *pSendCommand, *pClickCommand, *pTextBox, *pReadIdle;

void pyClean() {
    Py_Finalize();
    Py_DECREF(pModule);
    Py_DECREF(pName);
}


//(*InternalHeaders(RobotMonitorFrame)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(RobotMonitorFrame)
const long RobotMonitorFrame::idAttributeText = wxNewId();
const long RobotMonitorFrame::idStaticLine1 = wxNewId();
const long RobotMonitorFrame::idConsoleLabel = wxNewId();
const long RobotMonitorFrame::idConsoleText = wxNewId();
const long RobotMonitorFrame::idCommandText = wxNewId();
const long RobotMonitorFrame::idCommandButton = wxNewId();
const long RobotMonitorFrame::idMenuQuit = wxNewId();
const long RobotMonitorFrame::idMenuPort = wxNewId();
const long RobotMonitorFrame::idMenuAbout = wxNewId();
//*)

BEGIN_EVENT_TABLE(RobotMonitorFrame,wxFrame)
    //(*EventTable(RobotMonitorFrame)
    //*)
END_EVENT_TABLE()

RobotMonitorFrame::RobotMonitorFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(RobotMonitorFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxBoxSizer* ControlBox;
    wxMenu* Menu1;
    wxBoxSizer* ConsoleBox;
    wxBoxSizer* CommandBox;
    wxStaticBoxSizer* AttributeBox;
    wxMenuBar* MenuBar1;
    wxStaticBoxSizer* ActionBox;
    wxMenu* Menu2;
    wxBoxSizer* BaseBox;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    BaseBox = new wxBoxSizer(wxVERTICAL);
    ControlBox = new wxBoxSizer(wxHORIZONTAL);
    AttributeBox = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Attributes"));
    AttributeText = new wxTextCtrl(this, idAttributeText, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, _T("idAttributeText"));
    AttributeText->SetMinSize(wxSize(0,100));
    AttributeBox->Add(AttributeText, 1, wxALL|wxEXPAND, 5);
    ControlBox->Add(AttributeBox, 3, wxALL|wxEXPAND, 5);
    ActionBox = new wxStaticBoxSizer(wxVERTICAL, this, _("Actions"));
    ControlBox->Add(ActionBox, 2, wxALL|wxEXPAND, 5);
    BaseBox->Add(ControlBox, 1, wxALL|wxEXPAND, 5);
    StaticLine1 = new wxStaticLine(this, idStaticLine1, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL, _T("idStaticLine1"));
    StaticLine1->SetMinSize(wxSize(380,-1));
    BaseBox->Add(StaticLine1, 0, wxALL|wxEXPAND, 5);
    ConsoleLabel = new wxStaticText(this, idConsoleLabel, _("Console"), wxDefaultPosition, wxDefaultSize, 0, _T("idConsoleLabel"));
    BaseBox->Add(ConsoleLabel, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    ConsoleBox = new wxBoxSizer(wxVERTICAL);
    ConsoleText = new wxTextCtrl(this, idConsoleText, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, _T("idConsoleText"));
    ConsoleBox->Add(ConsoleText, 1, wxALL|wxEXPAND, 5);
    BaseBox->Add(ConsoleBox, 1, wxALL|wxEXPAND, 5);
    CommandBox = new wxBoxSizer(wxHORIZONTAL);
    CommandText = new wxTextCtrl(this, idCommandText, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, wxDefaultValidator, _T("idCommandText"));
    CommandText->SetFocus();
    CommandBox->Add(CommandText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    CommandButton = new wxButton(this, idCommandButton, _("Send"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("idCommandButton"));
    CommandBox->Add(CommandButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BaseBox->Add(CommandBox, 0, wxALL|wxEXPAND, 5);
    SetSizer(BaseBox);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu3 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu3, idMenuPort, _("Port"), _("Show available serial ports"), wxITEM_NORMAL);
    Menu3->Append(MenuItem3);
    MenuBar1->Append(Menu3, _("&Tool"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    BaseBox->Fit(this);
    BaseBox->SetSizeHints(this);

    Connect(idCommandButton,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RobotMonitorFrame::OnCommandButtonClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RobotMonitorFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&RobotMonitorFrame::OnAbout);
    //*)

    Connect(idCommandText, wxEVT_TEXT_ENTER,
            wxCommandEventHandler(RobotMonitorFrame::OnCommandButtonClick));
    Connect(wxID_ANY, wxEVT_IDLE,
            wxIdleEventHandler(RobotMonitorFrame::OnIdle));

    AttributeText->SetEditable(false);
    ConsoleText->SetEditable(false);
    actionBox = ActionBox;

    Py_Initialize();
    PyRun_SimpleString("import sys\n"
                       "sys.path.append('./python')");
    pName = PyString_FromString("MonitorCommand");
    pModule = PyImport_Import(pName);
    pDict = PyModule_GetDict(pModule);
    pSendCommand = PyDict_GetItemString(pDict, "sendCommand");
    pClickCommand = PyDict_GetItemString(pDict, "sendClickCommand");
    pTextBox = PyDict_GetItemString(pDict, "updateTextBox");
    pReadIdle = PyDict_GetItemString(pDict, "onIdle");
    atexit(pyClean);
}

RobotMonitorFrame::~RobotMonitorFrame()
{
    //(*Destroy(RobotMonitorFrame)
    //*)
}

#include <iostream>
using namespace std;

enum MonitorCommand {
    COMMAND_DEFAULT,
    COMMAND_PRINT,
    COMMAND_SET,
    COMMAND_ADD_WIDGET,
    COMMAND_PLOT_INIT,
    COMMAND_PLOT,
    COMMAND_MODEL_INIT,
    COMMAND_MODEL_COORDINATE,
    COMMAND_MODEL_ROTATION
};

void RobotMonitorFrame::OnIdle(wxIdleEvent& event) {
    PyObject *pTuple = PyObject_CallObject(pReadIdle, NULL);
    PyObject *pCmd = PyTuple_GetItem(pTuple, 0);
    MonitorCommand cmd = static_cast<MonitorCommand>(PyLong_AsLong(pCmd));

    if(cmd == COMMAND_PRINT) {
        PyObject *pValue = PyTuple_GetItem(pTuple, 1);
        char *cstr = PyString_AsString(pValue);
        ConsoleLog(cstr);
    }
    else if(cmd == COMMAND_SET) {
        PyObject *pName = PyTuple_GetItem(pTuple, 1);
        PyObject *pValue = PyTuple_GetItem(pTuple, 2);
        char *name = PyString_AsString(pName);
        char *value = PyString_AsString(pValue);
        SetAttribute(name, value);
    }
    else if(cmd == COMMAND_ADD_WIDGET) {
        PyObject *pCls = PyTuple_GetItem(pTuple, 1);
        PyObject *pName = PyTuple_GetItem(pTuple, 2);
        PyObject *pText = PyTuple_GetItem(pTuple, 3);
        WidgetType cls = static_cast<WidgetType>(PyLong_AsLong(pCls));
        char *name = PyString_AsString(pName);
        char *text = PyString_AsString(pText);
        ActionWidget *widget;
        switch(cls) {
          case WIDGET_BUTTON:
            widget = new ActionButton(this, name, text);
            break;
          case WIDGET_TEXTBOX:
            PyObject *pInput = PyTuple_GetItem(pTuple, 4);
            long ld = PyLong_AsLong(pInput);
            ActionInputType inputType = static_cast<ActionInputType>(ld);
            widget = new ActionTextCtrl(this, name, text, inputType);
            break;
        }
        widget->addTo(actionBox);
    }
    Update();
}

void RobotMonitorFrame::ConsoleLog(const char *str) {
    ConsoleText->AppendText(wxString::FromUTF8(str));
}


static std::vector<std::pair<std::string, std::string>> attributes;

void RobotMonitorFrame::SetAttribute(const char *name, const char *str) {
    std::string n = name;
    bool match = false;
    for(auto it = attributes.begin(); it != attributes.end(); it++) {
        if(n.compare(it->first) == 0) {
            it->second = str;
            match = true;
            break;
        }
    }
    if(match == false) {
        auto p = std::pair<std::string, std::string>(name, str);
        attributes.push_back(p);
    }
    
    AttributeText->Clear();
    for(auto it = attributes.begin(); it != attributes.end(); it++) {
        AttributeText->AppendText(wxString(it->first));
        AttributeText->AppendText(wxString(" : "));
        AttributeText->AppendText(wxString(it->second));
        AttributeText->AppendText(wxString("\n"));
    }
}

void RobotMonitorFrame::ClearAttributes() {
    attributes.clear();
    AttributeText->Clear();
}


void RobotMonitorFrame::OnCommandButtonClick(wxCommandEvent& event)
{
    std::string s = CommandText->GetValue().ToStdString();
    char cstr[s.size() + 2];
    std::copy(s.begin(), s.end(), cstr);
    cstr[s.size()] = '\n';
    cstr[s.size() + 1] = '\0';

    PyObject *pArgs;
    pArgs = PyTuple_New(1);
    PyObject *pValue = PyString_FromString(cstr);
    PyTuple_SetItem(pArgs, 0, pValue);
    PyObject_CallObject(pSendCommand, pArgs);

    CommandText->Clear();
}


void RobotMonitorFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void RobotMonitorFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
