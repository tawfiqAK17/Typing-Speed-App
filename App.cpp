#include "App.h"
#include "MainFrame.h"


wxIMPLEMENT_APP(App);

bool App::OnInit() {
    srand(time(0));
    auto *mainFrame = new MainFrame("hello");
    mainFrame->ShowMenu();
    mainFrame->Center();
    mainFrame->Show();
    return true;
}

