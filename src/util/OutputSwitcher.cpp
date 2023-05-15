//
// Created by DeathWind on 2023/5/14.
//

#include <iostream>

#include "util/OutputSwitcher.h"

OutputSwitcher::OutputSwitcher() {
    this->switched=false;
}

void OutputSwitcher::switchOutputToFile(const std::string& path) {
    std::cout << "Output Switched to: " << path << std::endl;

    this->fOut = std::ofstream(path);//文件输出流对象
    this->pOld = std::cout.rdbuf();//old保存cout的标准输出
    std::streambuf *fi = this->fOut.rdbuf();//文件的输出buf
    std::cout.rdbuf(fi);//将cout输出buf设置为文件的输出buf
    this->switched= true;
}

OutputSwitcher::~OutputSwitcher() {
    if(!this->switched){
        return;
    }
    std::cout << "OutputSwitcher Destructed" << std::endl;
    this->fOut.close();
    std::cout.rdbuf(this->pOld);
}



