#include "Character.h"

#define NON_FG_COLOR {255, 255, 255, 255}
#define NON_BG_COLOR {0, 0, 0, 0}
#define CURRENT_BG_COLOR {255, 255, 40, 180}
#define CORRECT_BG_COLOR {0, 255, 0, 180}
#define INCORRECT_BG_COLOR {255, 0, 0, 180}

Character::Character(wxWindow *parent,
                     wxWindowID id,
                     const wxString &label,
                     const wxPoint &pos,
                     const wxSize &size,
                     long style,
                     const wxString &name) :
        wxStaticText(parent, id, label, pos, size, style, name) {

}

void Character::ChangeColor(Character::STATUSES status) {
    switch (status) {
        case STATUSES::NON:
            SetForegroundColour(NON_FG_COLOR);
            SetBackgroundColour(NON_BG_COLOR);
            break;
        case STATUSES::CURRENT:
            SetBackgroundColour(CURRENT_BG_COLOR);
            break;
        case STATUSES::CORRECT:
            SetBackgroundColour(CORRECT_BG_COLOR);
            break;
        case STATUSES::INCORRECT:
            SetBackgroundColour(INCORRECT_BG_COLOR);
            break;
    }
}
