#ifndef ACTIONWIDGET_H_INCLUDED
#define ACTIONWIDGET_H_INCLUDED

#include <wx/button.h>
#include <wx/control.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/window.h>
#include <string>


enum WidgetType {
    WIDGET_DEFAULT,
    WIDGET_BUTTON,
    WIDGET_TEXTCTRL,
    WIDGET_SLIDER
};


class ActionWidget;
extern wxStaticBoxSizer* actionBox;

class ActionWidget {
  public:
    ActionWidget(wxWindow*, std::string);
    virtual void addTo(wxSizer*);
    WidgetType widgetType = WIDGET_DEFAULT;
  protected:
    wxWindow *parent;
    std::string name;
    int id;
};

void setActionWidgetsEnabled(bool);


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
    ActionInputType inputType;
    void onUpdate(wxCommandEvent&);

    friend void setActionWidgetsEnabled(bool);
};


#endif // ACTIONWIDGET_H_INCLUDED
