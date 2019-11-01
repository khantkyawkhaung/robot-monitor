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
//*)

class RobotMonitorFrame: public wxFrame
{
    public:

        RobotMonitorFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~RobotMonitorFrame();

    private:

        void OnIdle(wxIdleEvent& event);
        void ConsoleLog(const char *str);
        void SetAttribute(const char *name, const char *str);
        void ClearAttributes();
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
        static const long idMenuPort;
        static const long idMenuAbout;
        //*)

        //(*Declarations(RobotMonitorFrame)
        wxTextCtrl* AttributeText;
        wxTextCtrl* CommandText;
        wxStaticText* ConsoleLabel;
        wxMenu* Menu3;
        wxButton* CommandButton;
        wxTextCtrl* ConsoleText;
        wxMenuItem* MenuItem3;
        wxStaticLine* StaticLine1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // RobotMonitorMAIN_H
