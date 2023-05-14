//
// Created by DeathWind on 2023/5/14.
//

#ifndef GRABCUT_OUTPUTSWITCHER_H
#define GRABCUT_OUTPUTSWITCHER_H

#include "iostream"
#include <fstream>

/**
 * 修改cout的输出目标。
 * 析构时会将cout的输出目标切换会默认状态，
 * 因此可以在main的开始在栈中新建此类的对象。
 */
class OutputSwitcher {
public:
    void switchOutputToFile(const std::string& path);

    ~OutputSwitcher();

private:
    std::ofstream fOut;
    std::streambuf* pOld;
};


#endif //GRABCUT_OUTPUTSWITCHER_H
