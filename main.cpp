#include <iostream>

#include "src/include/GrabCutter.h"

using namespace std;

int main() {
    cout << "--- MAIN START ---" << endl;

    auto grabCutter = make_unique<GrabCutter>(R"(D:\Code\C\clionCpp\GrabCut\output\)");
    std::string imageFileName = "ex3.jpg";
    grabCutter->start(R"(D:\Code\C\clionCpp\GrabCut\test_textures\)" + imageFileName);

    cout << "--- MAIN FINISH ---" << endl;

    return 0;
}
