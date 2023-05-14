#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//int main() {
//    Mat srcImage = imread(R"(D:\AAAAA\BGImage\koishi.jpg)");
//    if (!srcImage.data) {
//        std::cout << "Image not loaded";
//        return -1;
//    }
//    imshow("image", srcImage);
//    waitKey(0);
//    return 0;
//}

int main(){
    std::string folder_path = R"(D:\Code\C\clionCpp\GrabCut\test_textures)"; //path of folder, you can replace "*.*" by "*.jpg" or "*.png"
    std::vector<cv::String> file_names;
    cv::glob(folder_path, file_names);   //get file names

    cv::Mat img;   //try show it when file is a picture

    for (auto & file_name : file_names) {

        int pos = file_name.find_last_of('\\');
        std::string img_name(file_name.substr(pos + 1));
        std::cout << img_name << std::endl;

        img = cv::imread(file_name);
        if (!img.data) {
            continue;
        }
        //do some work with the img you readed
        cv::imshow("img", img);
        cv::waitKey(300);
    }
}


