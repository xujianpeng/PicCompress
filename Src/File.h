#ifndef __FILE_H_
#define __FILE_H_

#include <vector>
#include <string>

bool isDirectory(const std::string &path);
bool isRegularFile(const std::string &path);
void getFileList(const std::string &path, std::vector<std::string> &fileList);
int getFileSize(const std::string &path);
std::string getFileName(const std::string &path);
std::string getFileNameExtension(const std::string &path);

#endif