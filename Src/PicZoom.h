#ifndef __PICZOOM_H_
#define __PICZOOM_H_

#include <vector>
#include <string>

typedef struct 
{
    int srcSize;            //压缩前大小
    int dstSize;            //压缩后大小
    int error;              //压缩前与目标大小差值
    int resolution;         //分辨率
    int quality;            //压缩质量
}AttributeRecord;


class PicZoom
{
public:
    enum
    {
        MINQUALITY = 10,
        MAXQUALITY = 80,
        RESOLUTIONTHRESHOLD = 960 * 540
    };
    PicZoom(const std::string& srcPath, const std::string& resultPath);
    ~PicZoom() { }

    void start();

    std::vector<std::string> getList();
    std::vector<std::vector<AttributeRecord>> getCompressRecord();
private:

    void getPicList();

private:
    std::string m_srcFilePath;      //带测试图片文件夹路径
    std::string m_resultFilePath;   //压缩图片保存路径

    std::vector<std::string> m_fileList; //带测试图片路径

    std::vector<std::vector<AttributeRecord>> m_testRecord; //压缩结果
};

#endif