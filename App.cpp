#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    auto *mainFrame = new MainFrame("hello");
    std::string string = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse tincidunt pretium sapien id cursus. Nunc quis risus sed tellus commodo.";
    mainFrame->ShowMenu(string);
    mainFrame->Center();
    mainFrame->Show();
    return true;
}
