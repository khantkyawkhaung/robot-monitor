#ifndef ACTIONWIDGET_H_INCLUDED
#define ACTIONWIDGET_H_INCLUDED

/*
 * Prepared widgets for customizable machine interface.
 */

#include "../process/attribute.h"
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


/*
 * The click event send a command to the client.
 */
class ActionButton: public ActionWidget,
                    public wxButton
{
  public:
    ActionButton(wxWindow*, std::string, std::string);
    void addTo(wxSizer*);
  private:
    void onClick(wxCommandEvent&);
};


/*
 * A textbox with a label beside.
 * Sends attribute update message on text change event.
 */
class ActionTextCtrl: public ActionWidget,
                      public wxTextCtrl
{
  public:
    ActionTextCtrl(wxWindow*, std::string, std::string, AttributeType);
    void addTo(wxSizer*);
  private:
    wxBoxSizer *box;
    wxStaticText *label;
    AttributeType inputType;
    void onUpdate(wxCommandEvent&);

    friend void setActionWidgetsEnabled(bool);
};


#endif // ACTIONWIDGET_H_INCLUDED
