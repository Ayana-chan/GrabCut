#include <iostream>

#include "src/include/GrabCutter.h"

using namespace std;

int main() {
    cout << "--- MAIN START ---" << endl;

    auto grabCutter = make_unique<GrabCutter>();
    grabCutter->start();

    cout << "--- MAIN FINISH ---" << endl;

    return 0;
}
