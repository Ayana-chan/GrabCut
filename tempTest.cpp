//
// Created by DeathWind on 2023/5/17.
//

#include <iostream>
#include <random>
#include <ctime>

int main(){
    std::random_device rd; // 用于随机数引擎获得随机种子
    std::mt19937 gen((unsigned int)time(nullptr)); // 以(unsigned int)time(NULL)为种子的标准mersenne_twister_engine，每次都是不同的
    std::uniform_int_distribution<int> distribute(1, 6);

    for (int i = 0; i < 10; ++i) {
        std::cout << distribute(gen) << " ";
    }

}
