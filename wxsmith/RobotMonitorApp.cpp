/***************************************************************
 * Name:      RobotMonitorApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Khant Kyaw Khaung (khantkyawkhaung288@gmail.com)
 * Created:   2019-09-20
 * Copyright: Khant Kyaw Khaung ()
 * License:
 **************************************************************/

#include "RobotMonitorApp.h"

//(*AppHeaders
#include "RobotMonitorMain.h"
#include <wx/image.h>
//*)


char pwd[256];


IMPLEMENT_APP(RobotMonitorApp);

bool RobotMonitorApp::OnInit()
{
    GetCurrentDir(pwd, 256);

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	RobotMonitorFrame* Frame = new RobotMonitorFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
