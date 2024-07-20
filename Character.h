#pragma once

#include "wx/wx.h"

class Character : public wxStaticText{
public:
    enum class STATUSES {NON, CURRENT, CORRECT, INCORRECT};
public:
    Character(wxWindow *parent,
              wxWindowID id,
              const wxString &label,
              const wxPoint &pos = wxDefaultPosition,
              const wxSize &size = wxDefaultSize,
              long style = 0,
              const wxString &name = wxASCII_STR(wxStaticTextNameStr));

    void ChangeStatus(Character::STATUSES status);

    STATUSES GetStatus();

private:
    STATUSES m_status = STATUSES::NON;
};
