#include <iostream> // for basic_ostream::operator<<, operator<<, endl, basic_ostream, basic_ostream<>::__ostream_type, cout, ostream

#include "tui.h"

int main()
{
    using namespace ftxui;
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries = {
        hello_hi.buf,
        "entry 2",
        "entry 3",
    };
    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();
    auto menu       = Menu(&entries, &selected, option);

    screen.Loop(menu);

    std::cout << std::endl;
    std::cout << "element = " << selected << std::endl;
    std::cout << "value   = " << entries[selected] << std::endl;
}
