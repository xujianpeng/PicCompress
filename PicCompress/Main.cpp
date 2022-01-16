#include <iostream>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "File.h"

using namespace std;
int main(int argc, char *argv[])
{
    vector<string> fileList;

    string path = "/home/jianpengsama/Code/PicCompress/Src";
    getFileList(path, fileList);

    for(auto &i : fileList)
    {
        cout << i << endl;
    }

    return 0;
}