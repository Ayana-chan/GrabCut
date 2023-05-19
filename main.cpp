#include <iostream>

#include "src/include/GrabCutter.h"

using namespace std;

int main() {
    cout << "--- MAIN START ---" << endl;

    auto grabCutter = make_unique<GrabCutter>(R"(D:\Code\C\clionCpp\GrabCut\output\)");
    std::string imageFileName = "ex2.jpg";
    grabCutter->start(R"(D:\Code\C\clionCpp\GrabCut\test_textures\)" + imageFileName);

    cout << "--- MAIN FINISH ---" << endl;

    return 0;
}

//int main (int argc,char *argv[]){
//    cout<<argc<<endl;
//    for(int i=0;i<argc;i++){
//        cout<<argv[i]<<endl;
//    }
//    if(argc!=2 && argc!=3){
//        cout<<"Wrong number of parameters ("<<argc<<"). Should be 1 or 2."<<endl;
//        return -1;
//    }
//
//    cout << "--- START ---" << endl;
//
//    auto outputPath="./";
//    if(argc==3){
//        outputPath=argv[2];
//    }
//    auto grabCutter = make_unique<GrabCutter>(outputPath);
//    grabCutter->start(argv[1]);
//
//    cout << "--- FINISH ---" << endl;
//}
