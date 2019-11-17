#ifndef ACTIONWIDGET_H_INCLUDED
#define ACTIONWIDGET_H_INCLUDED

/*
 * Prepared widgets for customizable machine interface.
 */

#include "../process/attribute.h"
#include <string>
#include <wx/button.h>
#include <wx/control.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/window.h>


enum WidgetType {
    WIDGET_DEFAULT,
    WIDGET_BUTTON,
    WIDGET_TEXTCTRL,
    WIDGET_SLIDER
};

class ActionWidgetTable;


class ActionWidget {
  public:
    ActionWidget(std::string);
    virtual void setEnable(bool);
    virtual void addTo(wxSizer*);
  protected:
    std::string name;
    int id;
    
    friend class ActionWidgetTable;
};


/*
 * The click event send a command to the client.
 */
class ActionButton: public ActionWidget,
                    public wxButton
{
  public:
    ActionButton(wxWindow*, std::string, std::string);
    void setEnable(bool) override;
    void addTo(wxSizer*) override;
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
    wxStaticText *getStaticText();
    void setEnable(bool) override;
    void addTo(wxSizer*) override;
  private:
    wxBoxSizer *box;
    wxStaticText *label;
    AttributeType inputType;
    void onUpdate(wxCommandEvent&);
};


/*
 * The list containing the user-wanted widgets.
 * Control using iterations.
 *
 * Note: The sizer should not contain any components.
 */
class ActionWidgetTable {
  public:
    ActionWidgetTable(wxWindow *win, wxSizer *sizer);
    void setEnable(bool);
    void addWidget(ActionWidget*);
    ActionWidget *getWidget(int i);
    void clear();
  private:
    wxWindow *parent;
    wxSizer *box;
};

#endif // ACTIONWIDGET_H_INCLUDED
