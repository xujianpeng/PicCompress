#include "File.h"
#include <boost/filesystem.hpp>
#include <iostream>

bool isDirectory(const std::string &path)
{
    boost::filesystem::path dirPath(path);
    try
    {
        boost::filesystem::file_status status = boost::filesystem::status(dirPath);
        return boost::filesystem::is_directory(status);
    }
    catch(const boost::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}

bool isRegularFile(const std::string &path)
{
    boost::filesystem::path filePath(path);
    try
    {
        boost::filesystem::file_status status = boost::filesystem::status(filePath);
        return boost::filesystem::is_regular_file(status);
    }
    catch(const boost::filesystem::filesystem_error &e)
    {
        std::cerr << e.what() << '\n';
    }
    return false;
}

void getFileList(const std::string &path, std::vector<std::string> &fileList)
{
    if(!isDirectory(path))
    {
        if(isRegularFile(path))
        {
            fileList.emplace_back(path);
        }
    }
    else
    {
        boost::filesystem::path dirPath = boost::filesystem::path(path);
        boost::filesystem::directory_iterator it{dirPath};
        while(it != boost::filesystem::directory_iterator{})
        {
            fileList.emplace_back((*it).path().string());
            it++;
        }
    }
}    

int getFileSize(const std::string &path)
{
    boost::filesystem::path filePath(path);
    boost::system::error_code ec;
    boost::uintmax_t fileSize = boost::filesystem::file_size(filePath, ec);

    return fileSize;
}

std::string getFileName(const std::string &path)
{
    boost::filesystem::path filePath(path);
    std::string fileName = filePath.stem().string();
    return fileName;
}

std::string getFileNameExtension(const std::string &path)
{
    boost::filesystem::path filePath(path);
    std::string fileName = filePath.extension().string();
    return fileName;
}
