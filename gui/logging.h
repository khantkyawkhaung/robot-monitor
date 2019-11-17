#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

#include <fstream>
#include <utility>
#include <vector>
#include <wx/textctrl.h>
#include <wx/timer.h>

class wxLogEx {
  public:
    wxLogEx(wxTextCtrl *text=NULL);
    void setTextDisplayGUI(wxTextCtrl*);
    void log(const char *str, int err=0);
    void quietLog(const char *str, int err=0);
    
    // Link with logfiles from other program (Used to link with Python module)
    void logViaFile(wxEvtHandler*, const char*);
  private:
    wxTextCtrl *textbox;
    
    // Read files with timer event
    class wxTimerEx: public wxTimer {
        wxTimerEx(wxEvtHandler*&);
        void Notify() override;
        wxLogEx *logger;
        friend class wxLogEx;
    };
    std::vector<std::pair<const char*, std::streampos>> readfiles;
    wxTimerEx *timer;
};

extern wxLogEx logger;

#endif // LOGGING_H_INCLUDED
