//
// Created by DeathWind on 2023/5/14.
//

#include <iostream>

#include "util/OutputSwitcher.h"

void OutputSwitcher::switchOutputToFile(const std::string& path) {
    std::cout << "Output Switched to: " << path << std::endl;

    this->fOut = std::ofstream(path);//文件输出流对象
    this->pOld = std::cout.rdbuf();//old保存cout的标准输出
    std::streambuf *fi = this->fOut.rdbuf();//文件的输出buf
    std::cout.rdbuf(fi);//将cout输出buf设置为文件的输出buf
}

OutputSwitcher::~OutputSwitcher() {
    std::cout << "OutputSwitcher Destructed" << std::endl;
    this->fOut.close();
    std::cout.rdbuf(this->pOld);
}


