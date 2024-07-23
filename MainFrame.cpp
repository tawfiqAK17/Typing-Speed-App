#include "MainFrame.h"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>

MainFrame::MainFrame(const wxString &title) :
        wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, {1600, 800},
                wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER)) {}

void MainFrame::ShowMenu() {
    auto panel = new wxPanel(this, wxID_ANY, {}, this->GetClientSize());
    panel->SetFocus();

    auto startButton = new wxButton(panel, wxID_ANY, "START", {this->GetClientSize().x / 2 - 100, this->GetClientSize().y / 2},
                                    {200, 100});

    auto *boxSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(panel);

    SetSizerAndFit(boxSizer);

    panel->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event) {
        this->DestroyChildren();
        ShowMainPanel();
//        event.Skip();
    });
}

std::string MainFrame::GenerateString() {

    char numberOfWords = 20 + rand() % 20; // between 20 and 40 word
    std::ifstream file("words");

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        exit(-1);
    }
    std::vector<int> wordsIdx;

    for (int i = 0; i < numberOfWords; i++) {
        wordsIdx.push_back(rand() % 500); // 500 number of words in the file
    }

    std::stringstream stringstream;
    int i = 0;
    std::string word;

    while (std::getline(file, word)) {
        if (std::find(wordsIdx.begin(), wordsIdx.end(),i) != wordsIdx.end())
            stringstream << word << " ";
        i++;
    }
    return stringstream.str().substr(0, stringstream.str().size() - 1);
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
        m_characters.back()->ChangeStatus(Character::STATUSES::NON);

        curXPos += m_characters.back()->GetSize().x;

        if (c == '\n') {
            curYPos += 70;
            curXPos = 20;
        }
    }
    m_characters[0]->ChangeStatus(Character::STATUSES::CURRENT);
}

void MainFrame::ShowMainPanel() {
    std::string string = GenerateString();

    auto *panel = new wxPanel(this, wxID_ANY, wxDefaultPosition,
                              wxSize(this->GetClientSize().x - 200, this->GetClientSize().y - 200));
//    panel->SetBackgroundColour({0, 255, 255, 200});
    panel->SetFocus();

    RenderCharacters(string, panel);


    panel->Bind(wxEVT_CHAR, [this](wxKeyEvent &event) {
        HandelKeyboardEvent(event);
    });

    auto *boxSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(panel, 0, wxTOP | wxLEFT, 200);

    SetSizerAndFit(boxSizer);
}

void MainFrame::HandelKeyboardEvent(wxKeyEvent &event) {
    if (!m_time_started) {
        m_start = std::chrono::steady_clock::now();
        m_time_started = true;
    }
    if (m_currentIdx > 0 && event.GetKeyCode() == 8) {
        m_characters[m_currentIdx]->SetBackgroundColour({0, 0, 0, 0});
        m_characters[m_currentIdx--]->ChangeStatus(Character::STATUSES::NON);
        m_characters[m_currentIdx]->ChangeStatus(Character::STATUSES::CURRENT);
        return;
    }
    if (event.GetKeyCode() == m_characters[m_currentIdx]->GetLabelText() ||
        event.GetKeyCode() == 13 && m_characters[m_currentIdx]->GetLabelText() == '\n') {
        m_characters[m_currentIdx]->SetBackgroundColour({0, 0, 0, 0});
        m_characters[m_currentIdx++]->ChangeStatus(Character::STATUSES::CORRECT);
        if (m_currentIdx < m_characters.size())
            m_characters[m_currentIdx]->ChangeStatus(Character::STATUSES::CURRENT);
        else
            ShowResult();
    } else {
        m_characters[m_currentIdx]->SetBackgroundColour({0, 0, 0, 0});
        m_characters[m_currentIdx++]->ChangeStatus(Character::STATUSES::INCORRECT);
        if (m_currentIdx < m_characters.size())
            m_characters[m_currentIdx]->ChangeStatus(Character::STATUSES::CURRENT);
        else
            ShowResult();
    }
}

void MainFrame::ShowResult() { // time_lapse in ms
    m_end = std::chrono::steady_clock::now();
    auto time_lapse = std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count();
    this->DestroyChildren();

    int faults = 0;
    for (auto character: m_characters) {
        if (character->GetStatus() == Character::STATUSES::INCORRECT) {
            faults++;
        }
    }

    float precision = 100 - 100 * (float) faults / (float)m_characters.size();

    m_characters.clear();
    m_currentIdx = 0;

    auto timePanel = new wxPanel(this, wxID_ANY, {250, 50}, {this->GetSize().x, 200});
    auto wordsNumPanel = new wxPanel(this, wxID_ANY, {300, 200}, {this->GetSize().x, 200});
    auto precisionPanel = new wxPanel(this, wxID_ANY, {350, 350}, {this->GetSize().x, 200});
    auto wpmPanel = new wxPanel(this, wxID_ANY, {400, 500}, {this->GetSize().x, 200});

    std::ostringstream timeTextStream;
    timeTextStream << "Time: " << time_lapse / 1000 << " s " << time_lapse % 1000 << " ms";
    std::ostringstream wordsNumTextStream;
    wordsNumTextStream << "Number of words: " << m_numOfWords;
    std::ostringstream precisionTextStream;
    precisionTextStream << "Precision: " << std::setprecision(2) << precision << "%";
    std::ostringstream wpmTextStream;
    wpmTextStream << "Words per minute : " << 60000 * m_numOfWords / time_lapse << " wpm";

    auto timeText = new wxStaticText(timePanel, wxID_ANY, timeTextStream.str(), {250, 100},{this->GetSize().x, 200});
    auto wordsNumText = new wxStaticText(wordsNumPanel, wxID_ANY, wordsNumTextStream.str(), {250, 100}, {this->GetSize().x, 200});
    auto precisionText = new wxStaticText(precisionPanel, wxID_ANY, precisionTextStream.str(), {250, 100}, {this->GetSize().x, 200});
    auto wpmText = new wxStaticText(wpmPanel, wxID_ANY, wpmTextStream.str(), {250, 100}, {this->GetSize().x, 200});


    timeText->SetFont(timeText->GetFont().Scale(3));
    wordsNumText->SetFont(wordsNumText->GetFont().Scale(3));
    precisionText->SetFont(precisionText->GetFont().Scale(3));
    wpmText->SetFont(wpmText->GetFont().Scale(3));

    auto backButton = new wxButton(this, wxID_ANY, "back to menu", {1200, 700},
                                   {-1, 50});

    backButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event){
        this->DestroyChildren();
        ShowMenu();
    });
}
























