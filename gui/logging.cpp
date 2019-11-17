#include "logging.h"
#include <iostream>
#include <wx/colour.h>

wxLogEx::wxLogEx(wxTextCtrl *text) {
    textbox = text;
}

void wxLogEx::setTextDisplayGUI(wxTextCtrl *text) {
    textbox = text;
}

void wxLogEx::log(const char *str, int err) {
    if(textbox == NULL) {
        quietLog(str, err);
        return;
    }
    
    if(err == 0)
        textbox->AppendText(wxString::FromUTF8(str));
    if(err == 1) {
        textbox->SetDefaultStyle(wxTextAttr(*wxYELLOW));
        textbox->AppendText(wxString::FromUTF8(str));
        textbox->SetDefaultStyle(wxTextAttr(*wxBLACK));
    }
    else if(err == 2) {
        textbox->SetDefaultStyle(wxTextAttr(*wxRED));
        textbox->AppendText(wxString::FromUTF8(str));
        textbox->SetDefaultStyle(wxTextAttr(*wxBLACK));
    }
    
    quietLog(str, err);
}

void wxLogEx::quietLog(const char *str, int err) {
    std::cout << str;
}

void wxLogEx::logViaFile(wxEvtHandler *handler, const char *filename) {
    if(readfiles.size() == 0) {
        timer = new wxTimerEx(handler);
        timer->logger = this;
        timer->Start(200);
    }
    readfiles.push_back(std::make_pair(filename, 0));
}

wxLogEx::wxTimerEx::wxTimerEx(wxEvtHandler*& owner)
                   :wxTimer(owner, wxNewId()) {}
        
void wxLogEx::wxTimerEx::Notify() {    
    for(auto it=logger->readfiles.begin(); it!=logger->readfiles.end(); it++)
    {
        const char *filename = it->first;
        std::streampos f_pos = it->second;
        std::ifstream f(filename);
        if(f.is_open()) {
            std::string line;
            f.seekg(f_pos);
            while(std::getline(f, line)) {
                f_pos = f.tellg();
                line += "\n";
                char cstr[line.size()+1];
                strcpy(cstr, line.c_str());
                char *label = strtok(cstr, " ");
                if(strcmp(label, "WARNING:") == 0)
                    logger->log(line.c_str(), 1);
                else if(strcmp(label, "ERROR:") == 0)
                    logger->log(line.c_str(), 2);
                else
                    logger->log(line.c_str());
            }
            f.close();
        }
        it->second = f_pos;
    }
}

wxLogEx logger;
