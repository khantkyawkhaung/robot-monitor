/***************************************************************
 * Name:      RobotMonitorApp.h
 * Purpose:   Defines Application Class
 * Author:    Khant Kyaw Khaung (khantkyawkhaung288@gmail.com)
 * Created:   2019-09-20
 * Copyright: Khant Kyaw Khaung ()
 * License:
 **************************************************************/

#ifndef RobotMonitorAPP_H
#define RobotMonitorAPP_H

#include <wx/app.h>


#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

extern char pwd[];


class RobotMonitorApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // RobotMonitorAPP_H
