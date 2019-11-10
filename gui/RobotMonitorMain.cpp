/***************************************************************
 * Name:      RobotMonitorMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Khant Kyaw Khaung (khantkyawkhaung288@gmail.com)
 * Created:   2019-09-20
 * Copyright: Khant Kyaw Khaung ()
 * License:
 **************************************************************/

#include "../process/attribute.h"
#include "actionWidget.h"
#include "RobotMonitorMain.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <wx/colour.h>
#include <wx/msgdlg.h>


#include <Python.h>

#define LOGFILE_PATH "data/python_log"
char logfile_path[256];
std::streampos logfile_pos = 0;
extern char pwd[];

PyObject *pName1, *pModule1, *pDict1;
PyObject *pName2, *pModule2, *pDict2;
PyObject *pGetCommand, *pWrite, *pListPorts, *pConnect, *pIsConnected;

void pyClean() {
    Py_Finalize();
    Py_DECREF(pModule1);
    Py_DECREF(pName1);
    Py_DECREF(pModule2);
    Py_DECREF(pName2);
}

void pyInit() {
    Py_Initialize();
    PyRun_SimpleString("import sys\n"
                       "sys.path.append('./python')");
    
    char pyCode[256];
    sprintf(logfile_path, "%s/%s", pwd, LOGFILE_PATH);
    sprintf(
        pyCode,
        "import robotmonitor.logging\n"
        "robotmonitor.logging.setLogFile('%s')",
        logfile_path
    );
    PyRun_SimpleString(pyCode);
    
    pName1 = PyString_FromString("robotmonitor.command");
    pModule1 = PyImport_Import(pName1);
    pDict1 = PyModule_GetDict(pModule1);
    pGetCommand = PyDict_GetItemString(pDict1, "getCommand");
    pName2 = PyString_FromString("robotmonitor.port");
    pModule2 = PyImport_Import(pName2);
    pDict2 = PyModule_GetDict(pModule2);
    pWrite = PyDict_GetItemString(pDict2, "write");
    pListPorts = PyDict_GetItemString(pDict2, "listPorts");
    pConnect = PyDict_GetItemString(pDict2, "connect");
    pIsConnected = PyDict_GetItemString(pDict2, "isConnected");
    
    atexit(pyClean);
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
const long RobotMonitorFrame::idUnknownPort = wxNewId();
const long RobotMonitorFrame::idMenuPorts = wxNewId();
const long RobotMonitorFrame::idBR9600 = wxNewId();
const long RobotMonitorFrame::idBR19200 = wxNewId();
const long RobotMonitorFrame::idBR38400 = wxNewId();
const long RobotMonitorFrame::idBR76800 = wxNewId();
const long RobotMonitorFrame::idBR153600 = wxNewId();
const long RobotMonitorFrame::idMenuBaudrate = wxNewId();
const long RobotMonitorFrame::idMenuConnect = wxNewId();
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
    MenuItem3 = new wxMenu();
    MenuItem11 = new wxMenuItem(MenuItem3, idUnknownPort, _("COM\?"), wxEmptyString, wxITEM_NORMAL);
    MenuItem3->Append(MenuItem11);
    MenuItem11->Enable(false);
    Menu3->Append(idMenuPorts, _("Port"), MenuItem3, _("Show available serial ports"))->Enable(false);
    MenuItem4 = new wxMenu();
    MenuItem5 = new wxMenuItem(MenuItem4, idBR9600, _("9600"), wxEmptyString, wxITEM_RADIO);
    MenuItem4->Append(MenuItem5);
    MenuItem6 = new wxMenuItem(MenuItem4, idBR19200, _("19200"), wxEmptyString, wxITEM_RADIO);
    MenuItem4->Append(MenuItem6);
    MenuItem7 = new wxMenuItem(MenuItem4, idBR38400, _("38400"), wxEmptyString, wxITEM_RADIO);
    MenuItem4->Append(MenuItem7);
    MenuItem8 = new wxMenuItem(MenuItem4, idBR76800, _("76800"), wxEmptyString, wxITEM_RADIO);
    MenuItem4->Append(MenuItem8);
    MenuItem9 = new wxMenuItem(MenuItem4, idBR153600, _("153600"), wxEmptyString, wxITEM_RADIO);
    MenuItem4->Append(MenuItem9);
    Menu3->Append(idMenuBaudrate, _("Baudrate"), MenuItem4, _("Configure baudrate"));
    MenuItem10 = new wxMenuItem(Menu3, idMenuConnect, _("Connect"), _("Connect to a USB device"), wxITEM_NORMAL);
    Menu3->Append(MenuItem10);
    MenuItem10->Enable(false);
    MenuBar1->Append(Menu3, _("&Tools"));
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

    MenuTools = Menu3;
    MenuPorts = MenuItem3;
    MenuPorts->Destroy(idUnknownPort);
    MenuBaudrate = MenuItem4;
    MenuConnect = MenuItem10;

    Connect(idCommandText, wxEVT_TEXT_ENTER,
            wxCommandEventHandler(RobotMonitorFrame::OnCommandButtonClick));

    MenuBaudrate->Check(idBR9600, true);
    wxCommandEvent event_tmp = wxCommandEvent(wxEVT_NULL, idBR9600);
    OnBaudrateSelection(event_tmp);
    Connect(idBR9600, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(RobotMonitorFrame::OnBaudrateSelection));
    Connect(idBR19200, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(RobotMonitorFrame::OnBaudrateSelection));
    Connect(idBR38400, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(RobotMonitorFrame::OnBaudrateSelection));
    Connect(idBR76800, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(RobotMonitorFrame::OnBaudrateSelection));
    Connect(idBR153600, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(RobotMonitorFrame::OnBaudrateSelection));

    Connect(idMenuConnect, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(RobotMonitorFrame::OnConnect));

    AttributeText->Enable(false);
    AttributeText->SetEditable(false);
    attributeTable.setTextDisplayGUI(AttributeText);
    ConsoleText->SetEditable(false);
    actionBox = ActionBox;
    
    pyInit();

    wxTimer *timer = new wxTimer(this);
    Connect(wxID_ANY, wxEVT_TIMER,
            wxTimerEventHandler(RobotMonitorFrame::OnTimer));
    timer->Start(10);
}

RobotMonitorFrame::~RobotMonitorFrame()
{
    //(*Destroy(RobotMonitorFrame)
    //*)
}

static std::vector<wxMenuItem*> ports;
std::string port = "";
int baudrate;
bool connected = false;

void RobotMonitorFrame::OnPortSelection(wxCommandEvent& event) {
    wxMenuItem *item = MenuPorts->FindItem(event.GetId());
    port = item->GetItemLabelText().ToStdString();
    MenuTools->SetLabel(idMenuPorts, wxString("Port: " + port));
}

void RobotMonitorFrame::OnBaudrateSelection(wxCommandEvent& event) {
    wxMenuItem *item = MenuBaudrate->FindItem(event.GetId());
    std::string str = item->GetItemLabelText().ToStdString();
    baudrate = std::stoi(str);
    MenuTools->SetLabel(idMenuBaudrate, wxString("Baudrate: " + str));
}

void RobotMonitorFrame::OnConnect(wxCommandEvent& event) {
    PyObject *pArgs;
    pArgs = PyTuple_New(2);
    PyObject *pPort = PyString_FromString(port.c_str());
    PyTuple_SetItem(pArgs, 0, pPort);
    PyObject *pBaudrate = PyLong_FromLong(baudrate);
    PyTuple_SetItem(pArgs, 1, pBaudrate);
    PyObject_CallObject(pConnect, pArgs);
}


enum MonitorCommand {
    COMMAND_DEFAULT,
    COMMAND_RESET,
    COMMAND_PRINT,
    COMMAND_SET,
    COMMAND_ADD_WIDGET,
    COMMAND_PLOT_INIT,
    COMMAND_PLOT,
    COMMAND_MODEL_INIT,
    COMMAND_MODEL_COORDINATE,
    COMMAND_MODEL_ROTATION
};


void RobotMonitorFrame::OnTimer(wxTimerEvent& event) {
    // Read log
    std::ifstream logfile(logfile_path);
    if(logfile.is_open()) {
        std::string line;
        logfile.seekg(logfile_pos);
        while(std::getline(logfile, line)) {
            logfile_pos = logfile.tellg();
            line += "\n";
            char cstr[line.size()+1];
            strcpy(cstr, line.c_str());
            char *label = strtok(cstr, " ");
            if(strcmp(label, "WARNING:") == 0)
                ConsoleLog(line.c_str(), 1);
            else if(strcmp(label, "ERROR:") == 0)
                ConsoleLog(line.c_str(), 2);
            else
                ConsoleLog(line.c_str());
        }
        logfile.close();
    }

    // Port list
    PyObject *pPortList = PyObject_CallObject(pListPorts, NULL);
    Py_ssize_t portCount = PyList_Size(pPortList);
    if(portCount != ports.size()) {
        for(auto it=ports.begin(); it!=ports.end(); it++) {
            MenuPorts->Destroy(*it);
        }
        ports.clear();
        bool portStillExists = false;
        for(Py_ssize_t i=0; i<portCount; i++) {
            PyObject *pValue = PyList_GetItem(pPortList, i);
            int id = wxNewId();
            wxString str = wxString(PyString_AsString(pValue));
            wxMenuItem *menuItem = new wxMenuItem(
                MenuPorts,
                id,
                str,
                wxEmptyString,
                wxITEM_RADIO
            );
            ports.push_back(menuItem);
            MenuPorts->Append(menuItem);
            Connect(
                id,
                wxEVT_MENU,
                wxCommandEventHandler(RobotMonitorFrame::OnPortSelection)
            );
            if(portStillExists)
                continue;
            if(port.compare(str.ToStdString()) == 0) {
                portStillExists = true;
                MenuPorts->Check(id, true);
                wxCommandEvent event_tmp = wxCommandEvent(wxEVT_NULL, id);
                OnPortSelection(event_tmp);
            }
        }
        if(portStillExists == false && portCount > 0) {
            int id = ports[0]->GetId();
            MenuPorts->Check(id, true);
            wxCommandEvent event_tmp = wxCommandEvent(wxEVT_NULL, id);
            OnPortSelection(event_tmp);
        }
        if(portCount > 0) {
            MenuTools->Enable(idMenuPorts, true);
            MenuTools->Enable(idMenuConnect, true);
        }
        else {
            MenuTools->SetLabel(idMenuPorts, _("Port"));
            MenuTools->Enable(idMenuPorts, false);
            MenuTools->Enable(idMenuConnect, false);
        }
    }

    // Port connection status
    PyObject *pConnected = PyObject_CallObject(pIsConnected, NULL);
    bool b = PyObject_IsTrue(pConnected);
    if(b) {
        if(connected == false) {
            AttributeText->Enable(true);
            attributeTable.clear();
            ConsoleText->Clear();
            actionBox->Clear();
        }
    }
    else {
        if(connected == true) {
            AttributeText->Enable(false);
            setActionWidgetsEnabled(false);
        }
    }
    connected = b;

    // Monitor command process
    if(connected == false)
        return;
    PyObject *pList = PyObject_CallObject(pGetCommand, NULL);
    Py_ssize_t cmdCount = PyList_Size(pList);
    for(Py_ssize_t i=0; i<cmdCount; i++) {
        PyObject *pTuple = PyList_GetItem(pList, i);
        PyObject *pCmd = PyTuple_GetItem(pTuple, 0);
        MonitorCommand cmd = static_cast<MonitorCommand>(PyLong_AsLong(pCmd));

        if(cmd == COMMAND_PRINT) {
            PyObject *pValue = PyTuple_GetItem(pTuple, 1);
            char *cstr = PyString_AsString(pValue);
            ConsoleLog(cstr);
        }
        else if(cmd == COMMAND_SET) {
            Py_ssize_t tupleSize = PyTuple_Size(pTuple);
            if(tupleSize == 2) {
                PyObject *pName = PyTuple_GetItem(pTuple, 1);
                PyObject *pValue = PyTuple_GetItem(pTuple, 2);
                char *name = PyString_AsString(pName);
                char *value = PyString_AsString(pValue);
                attributeTable.addAttribute(name, value);
            }
            else if(tupleSize > 2) {
                PyObject *pName = PyTuple_GetItem(pTuple, 1);
                PyObject *pValue = PyTuple_GetItem(pTuple, 2);
                char *name = PyString_AsString(pName);
                char *value = PyString_AsString(pValue);
                AttributeType type = ATTRIBUTE_STRING;
                bool displayed = false;
                if(tupleSize >= 3) {
                    PyObject *pType = PyTuple_GetItem(pTuple, 3);
                    type = static_cast<AttributeType>(PyLong_AsLong(pType));
                }
                if(tupleSize == 4) {
                    PyObject *pDisplayed = PyTuple_GetItem(pTuple, 4);
                    displayed = PyObject_IsTrue(pDisplayed);
                }
                attributeTable.addAttribute(
                    new Attribute(name, value, type, displayed)
                );
            }
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
              case WIDGET_TEXTCTRL:
                PyObject *pInput = PyTuple_GetItem(pTuple, 4);
                long ld = PyLong_AsLong(pInput);
                AttributeType inputType = static_cast<AttributeType>(ld);
                widget = new ActionTextCtrl(this, name, text, inputType);
                break;
            }
            widget->addTo(actionBox);
        }
    }
}

void RobotMonitorFrame::ConsoleLog(const char *str, int err) {
    if(err == 0)
        ConsoleText->AppendText(wxString::FromUTF8(str));
    if(err == 1) {
        ConsoleText->SetDefaultStyle(wxTextAttr(*wxYELLOW));
        ConsoleText->AppendText(wxString::FromUTF8(str));
        ConsoleText->SetDefaultStyle(wxTextAttr(*wxBLACK));
    }
    else if(err == 2) {
        ConsoleText->SetDefaultStyle(wxTextAttr(*wxRED));
        ConsoleText->AppendText(wxString::FromUTF8(str));
        ConsoleText->SetDefaultStyle(wxTextAttr(*wxBLACK));
    }
}


void RobotMonitorFrame::OnCommandButtonClick(wxCommandEvent& event)
{
    std::string s = CommandText->GetValue().ToStdString();
    char cstr[s.size() + 2];
    std::copy(s.begin(), s.end(), cstr);
    cstr[s.size()] = ' ';
    cstr[s.size() + 1] = '\0';

    PyObject *pArgs;
    pArgs = PyTuple_New(1);
    PyObject *pValue = PyString_FromString(cstr);
    PyTuple_SetItem(pArgs, 0, pValue);
    PyObject_CallObject(pWrite, pArgs);

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
