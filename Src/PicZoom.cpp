#include "PicZoom.h"
#include "File.h"
#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <string.h>

PicZoom::PicZoom(const std::string &srcPath, const std::string &resultPath) :
     m_srcFilePath(srcPath),
     m_resultFilePath(resultPath)
{
    
}

void PicZoom::start()
{
    //获取带测试文件列表
    getPicList();
    //待测试文件数量
    size_t size = m_fileList.size();
    size_t total = size * (MAXQUALITY - MINQUALITY);
    printf("total = %lu\n" , total);
    //进度
    int progressTmp = 0;
    //进度百分比
    int progress = 0;

    for(size_t i = 0 ; i < size ; ++i)
    {
        std::string tmpPath = m_fileList[i];       

        //压缩过程图片信息
        AttributeRecord record;
        memset((void *)&record, 0, sizeof(record));
        record.srcSize = getFileSize(tmpPath.c_str());

        //读取图片
        CommPicInfo picInfo;
        memset((void *)&picInfo, 0, sizeof(picInfo));
        // printf("readPic path : %s\n", tmpPath.c_str());
        picInfo.data = stbi_load(tmpPath.c_str(), &picInfo.width, &picInfo.height, &picInfo.components, 3);
        if(picInfo.data == NULL)
        {
            printf("readPic error\n");
            return;
        }
        picInfo.scale_denom = picInfo.width * picInfo.height > RESOLUTIONTHRESHOLD ? 2 : 1;
        record.resolution = picInfo.width * picInfo.height;

        std::vector<AttributeRecord> oneFileLoopRecord{};
        //从minQuality 到maxQuality循环压缩
        std::string fileName = getFileName(tmpPath);
        for(int j = MINQUALITY ; j < MAXQUALITY; ++j)
        {
            progressTmp += 1;

            record.quality = j;
            picInfo.quality = j;
            //TODO 压缩文件命名 fileName + _ + quality _ width*height.jpg
            std::string result = m_resultFilePath 
                                    + "/" 
                                    + fileName 
                                    + "_" 
                                    + std::to_string(j)
                                    + "_"
                                    + std::to_string(picInfo.width)
                                    + "*"
                                    + std::to_string(picInfo.height)
                                    + ".jpg";
            // printf("result path : %s\n", result.c_str());
            if(record.srcSize > 60 * 1024)
            {
                writeJpg(result.c_str(), &picInfo);
            }
            record.dstSize = getFileSize(result);

            oneFileLoopRecord.emplace_back(record);
            if(progress != (int)(progressTmp * 100) / total)
            {
                progress = progressTmp;
                printf("progress : %d\n", progress);
            }
        }
        m_testRecord.emplace_back(oneFileLoopRecord);

        stbi_image_free(picInfo.data);
    }
}

void PicZoom::getPicList()
{
    ::getFileList(m_srcFilePath, m_fileList);
}

std::vector<std::string> PicZoom::getList() 
{
    return m_fileList;
}

std::vector<std::vector<AttributeRecord>> PicZoom::getCompressRecord()
{
    return m_testRecord;
}