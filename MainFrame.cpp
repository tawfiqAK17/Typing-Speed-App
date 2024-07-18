#include "MainFrame.h"

MainFrame::MainFrame(const wxString &title) :
        wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, {1000, 800},
                wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {

    Center();

    SetClientSize(wxGetDisplaySize());

}

void MainFrame::ShowMenu(std::string& string) {
    auto panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    panel->SetBackgroundColour({0, 0, 0, 255});
    auto startButton = new wxButton(panel, wxID_ANY, "START", {this->GetSize().x / 2 - 100, this->GetSize().y / 2}, {200, 100});
    std::cout << this->GetSize().x << " " << this->GetSize().y << "\n";

    this->Bind(wxEVT_BUTTON, [this, string](wxCommandEvent& event) {
        this->DestroyChildren();
        ShowMainPanel(string);
        event.Skip();
    });
}

void MainFrame::ShowMainPanel(const std::string& string ) {
    auto *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition,
                                 wxSize(this->GetSize().x - 50, this->GetSize().y - 50));
    panel->SetBackgroundColour({0, 255, 255, 200});
    panel->SetFocus();

    RenderCharacters(string, panel);


    panel->Bind(wxEVT_CHAR, [this, panel](wxKeyEvent &event) {
        HandelKeyboardEvent(event, panel);
    });

    auto *boxSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(panel, 1, wxALL, 50);

    SetSizerAndFit(boxSizer);
}

void MainFrame::RenderCharacters(const std::string &string, wxPanel *panel) {
    int curXPos = 20;
    int curYPos = 0;
    for (char c: string) {
        if (c == ' ')
            m_numOfWords++;
        if (curXPos + 40 + 100 + 40 >= panel->GetSize().x) {
            curYPos += 80;
            curXPos = 20;
            auto it = m_characters.end() - 1;
            while ((*it)->GetLabelText() != ' ' && it != m_characters.begin()) {
                it--;
            }
            if (it != m_characters.begin()) {
                m_characters.erase(it);
            }

            for (; it != m_characters.end(); it++) {
                (*it)->SetPosition({curXPos, curYPos});
                curXPos += m_characters.back()->GetSize().x;
            }

        }
        if (curXPos != 20) {
            m_characters.push_back(new Character(panel, wxID_ANY, c, wxPoint(curXPos, curYPos), wxSize(-1, -1),
                                                 wxALIGN_CENTRE_HORIZONTAL));
        } else {
            m_characters.push_back(new Character(panel, wxID_ANY, c, wxPoint(curXPos, curYPos), wxSize(-1, -1),
                                                 wxALIGN_CENTRE_HORIZONTAL));
        }

        m_characters.back()->SetFont(m_characters.back()->GetFont().Scale(2));
        m_characters.back()->SetSize(m_characters.back()->GetFont().GetPointSize(), 60);
        m_characters.back()->ChangeColor(Character::STATUSES::NON);

        curXPos += m_characters.back()->GetSize().x;

        if (c == '\n') {
            curYPos += 70;
            curXPos = 20;
        }
    }
    m_characters[0]->ChangeColor(Character::STATUSES::CURRENT);
}

void MainFrame::HandelKeyboardEvent(wxKeyEvent &event, wxPanel *panel) {
    if (m_currentIdx == m_characters.size() - 1) {
        m_end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count();
        std::cout << duration << "\n";
        this->DestroyChildren();
        ShowResult(duration);
        return;
    }
    if (!m_time_started) {
        m_start = std::chrono::steady_clock::now();
        m_time_started = true;
    }
    if (m_currentIdx > 0 && event.GetKeyCode() == 8) {
        m_characters[m_currentIdx]->SetBackgroundColour({0, 0, 0, 0});
        m_characters[m_currentIdx--]->ChangeColor(Character::STATUSES::NON);
        m_characters[m_currentIdx]->ChangeColor(Character::STATUSES::CURRENT);
        return;
    }
    if (event.GetKeyCode() == m_characters[m_currentIdx]->GetLabelText() ||
        event.GetKeyCode() == 13 && m_characters[m_currentIdx]->GetLabelText() == '\n') {
        m_characters[m_currentIdx]->SetBackgroundColour({0, 0, 0, 0});
        m_characters[m_currentIdx++]->ChangeColor(Character::STATUSES::CORRECT);
        if (m_currentIdx < m_characters.size())
            m_characters[m_currentIdx]->ChangeColor(Character::STATUSES::CURRENT);
    } else {
        m_characters[m_currentIdx]->SetBackgroundColour({0, 0, 0, 0});
        m_characters[m_currentIdx++]->ChangeColor(Character::STATUSES::INCORRECT);
        if (m_currentIdx < m_characters.size())
            m_characters[m_currentIdx]->ChangeColor(Character::STATUSES::CURRENT);
    }
}

void MainFrame::ShowResult(long time_lapse) { // time_lapse in ms

    auto timePanel = new wxPanel(this, wxID_ANY, {250, 100}, {this->GetSize().x, 200});
    auto wordsNumPanel = new wxPanel(this, wxID_ANY, {250, 300}, {this->GetSize().x, 200});
    auto wpmPanel = new wxPanel(this, wxID_ANY, {250, 500}, {this->GetSize().x, 200});

    auto time = new wxStaticText(timePanel, wxID_ANY, "Time : " + std::to_string(time_lapse / 1000) + " s " +  std::to_string(time_lapse % 1000) + " ms", {250, 100}, {this->GetSize().x, 200});
    auto wordsNum = new wxStaticText(wordsNumPanel, wxID_ANY, "Number of words: " + std::to_string(m_numOfWords), {250, 100}, {this->GetSize().x, 200});
    auto wpm = new wxStaticText(wpmPanel, wxID_ANY, "Words per minute : " + std::to_string(60000 * m_numOfWords / time_lapse), {250, 100}, {this->GetSize().x, 200});

    time->SetFont(time->GetFont().Scale(5));
    wordsNum->SetFont(wordsNum->GetFont().Scale(5));
    wpm->SetFont(wpm->GetFont().Scale(5));
}
























