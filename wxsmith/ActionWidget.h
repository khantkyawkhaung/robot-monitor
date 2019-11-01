#ifndef ACTIONWIDGET_H_INCLUDED
#define ACTIONWIDGET_H_INCLUDED

#include <wx/button.h>
#include <wx/control.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/window.h>
#include <string>


class ActionWidget;
extern wxStaticBoxSizer* actionBox;

class ActionWidget {
  public:
    ActionWidget(wxWindow*, std::string);
    virtual void addTo(wxSizer*);
  protected:
    wxWindow *parent;
    std::string name;
    int id;
};


class ActionButton: public ActionWidget,
                    public wxButton
{
  public:
    ActionButton(wxWindow*, std::string, std::string);
    void addTo(wxSizer*);
  private:
    void onClick(wxCommandEvent&);
};


enum ActionInputType {
    ACTION_INPUT_CHAR,
    ACTION_INPUT_INT,
    ACTION_INPUT_FLOAT,
    ACTION_INPUT_STRING
};


class ActionTextCtrl: public ActionWidget,
                      public wxTextCtrl
{
  public:
    ActionTextCtrl(wxWindow*, std::string, std::string, ActionInputType);
    void addTo(wxSizer*);
  private:
    wxBoxSizer *box;
    wxStaticText *label;
    ActionInputType type;
    void onUpdate(wxCommandEvent&);
};


#endif // ACTIONWIDGET_H_INCLUDED
