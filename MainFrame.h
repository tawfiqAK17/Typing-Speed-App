#pragma once

#include <chrono>
#include <wx/wx.h>
#include "Character.h"

class MainFrame : public wxFrame {
public:
    explicit MainFrame(const wxString &title);

    void ShowMenu(std::string& string);

    void ShowMainPanel(const std::string& string);

    void RenderCharacters(const std::string &string, wxPanel *panel);

    void HandelKeyboardEvent(wxKeyEvent &event, wxPanel *panel);

    void ShowResult(long time_lapse);

private:
    std::vector<Character *> m_characters;
    size_t m_numOfWords = 1;
    size_t m_currentIdx = 0;

    std::chrono::time_point<std::chrono::steady_clock> m_start;
    std::chrono::time_point<std::chrono::steady_clock> m_end;
    bool m_time_started = false;
};
