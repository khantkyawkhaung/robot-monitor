/***************************************************************
 * Name:      RobotMonitorMain.h
 * Purpose:   Defines Application Frame
 * Author:    Khant Kyaw Khaung (khantkyawkhaung288@gmail.com)
 * Created:   2019-09-20
 * Copyright: Khant Kyaw Khaung ()
 * License:
 **************************************************************/

#ifndef RobotMonitorMAIN_H
#define RobotMonitorMAIN_H

//(*Headers(RobotMonitorFrame)
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/statline.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)

class RobotMonitorFrame: public wxFrame
{
    public:

        RobotMonitorFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~RobotMonitorFrame();

    private:

        void OnTimer(wxTimerEvent& event);
        void OnPortSelection(wxCommandEvent& event);
        void OnBaudrateSelection(wxCommandEvent& event);
        void OnConnect(wxCommandEvent& event);
        void ConsoleLog(const char *str);
        void SetAttribute(const char *name, const char *str);
        void ClearAttributes();

        wxMenu *MenuTools;
        wxMenu *MenuPorts;
        wxMenu *MenuBaudrate;
        wxMenuItem *MenuConnect;

        //(*Handlers(RobotMonitorFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnCommandButtonClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(RobotMonitorFrame)
        static const long idAttributeText;
        static const long idStaticLine1;
        static const long idConsoleLabel;
        static const long idConsoleText;
        static const long idCommandText;
        static const long idCommandButton;
        static const long idMenuQuit;
        static const long idUnknownPort;
        static const long idMenuPorts;
        static const long idBR9600;
        static const long idBR19200;
        static const long idBR38400;
        static const long idBR76800;
        static const long idBR153600;
        static const long idMenuBaudrate;
        static const long idMenuConnect;
        static const long idMenuAbout;
        //*)

        //(*Declarations(RobotMonitorFrame)
        wxTextCtrl* AttributeText;
        wxTextCtrl* CommandText;
        wxStaticText* ConsoleLabel;
        wxMenu* Menu3;
        wxMenu* MenuItem4;
        wxButton* CommandButton;
        wxTextCtrl* ConsoleText;
        wxStaticLine* StaticLine1;
        wxMenuItem* MenuItem9;
        wxMenuItem* MenuItem11;
        wxMenuItem* MenuItem5;
        wxMenuItem* MenuItem10;
        wxMenuItem* MenuItem7;
        wxMenuItem* MenuItem6;
        wxMenu* MenuItem3;
        wxMenuItem* MenuItem8;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // RobotMonitorMAIN_H
