#include "user_interface.hpp"

void UI::Help() {
    std::cout << "Commands:\n";
    std::cout << "help - show commands\n";
    std::cout << "save - save cube into the file\n";
    std::cout << "load - get the current state of cube from the file\n";
    std::cout << "rotate (after on a new line enter 'up/up*; down/down*; right/right*; left/left*; front/front*; back/back*') - rotate side of the cube\n";
    std::cout << "random - get the random state of cube\n";
    std::cout << "show - prints rubik's cube\n";
    std::cout << "solve - prints the solution of the rubik's cube\n";
    std::cout << "stop - finish working\n";
}

void UI::Runtime() {
    std::string command = "", load_name, save_name, face;
    Help();
    while (command != "stop") {
        std::cin >> command;
        if (command == "save")
            Save();
        else if (command == "load")
            Load();
        else if (command == "rotate")
            Rotate();
        else if (command == "random")
            Random();
        else if (command == "show")
            Show();
        else if (command == "solve")
            Solve();
        else if (command == "help")
            Help();
        else if (command != "stop")
            std::cout << "ERROR\n Enter one of the commands\n";
    }
}

void UI::Save() {
    rubic_cube.Print(true);
}

void UI::Load() {
    rubic_cube.ReadCube();
}

void UI::Rotate() {
    std::string rotate;
    std::cin >> rotate;
    if (rotate == "up")
        rubic_cube.Up();
    else if (rotate == "up*")
        rubic_cube.Up(true);
    else if (rotate == "down")
        rubic_cube.Down();
    else if (rotate == "down*")
        rubic_cube.Down(true);
    else if (rotate == "front")
        rubic_cube.Front();
    else if (rotate == "front*")
        rubic_cube.Front(true);
    else if (rotate == "back")
        rubic_cube.Back();
    else if (rotate == "back*")
        rubic_cube.Back(true);
    else if (rotate == "left")
        rubic_cube.Left();
    else if (rotate == "left*")
        rubic_cube.Left(true);
    else if (rotate == "right")
        rubic_cube.Right();
    else if (rotate == "right*")
        rubic_cube.Right(true);
}

void UI::Random() {
    rubic_cube.RandomCombination();
}

void UI::Show() {
    rubic_cube.Print();
}

void UI::Solve() {
    rubic_cube.AssembleRubicCube();
}