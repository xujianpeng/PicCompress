#include <iostream>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "File.h"
#include "PicZoom.h"

using namespace std;
int main(int argc, char *argv[])
{
    // if(argc < 3)
    // {
    //     std::cout << "Uage error" << endl;
    //     return 0 ;
    // }

    std::string srcFilePath = "/home/jianpengsama/Code/PicCompress/Picture";
    std::string resultPath = "/home/jianpengsama/Code/PicCompress/result";
    
    PicZoom picTest(srcFilePath, resultPath);
    picTest.start();
    vector<string> fileList = picTest.getList();

    for(auto &i : fileList)
    {
        // cout << i << endl;
        // cout << getFileName(i) << endl;
        // cout << getFileNameExtension(i) << endl;
    }

    return 0;
}