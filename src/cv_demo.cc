#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    // 1. 打开RGB数据文件
    std::string inputFileName = "/userdata/raw.rgb"; // 你需要提供正确的文件名
    std::ifstream inputFile(inputFileName, std::ios::in | std::ios::binary);

    if (!inputFile.is_open())
    {
        std::cerr << "无法打开输入文件" << std::endl;
        return -1;
    }

    // 2. 读取RGB数据并创建OpenCV图像
    int width = 1920;                       // 图像宽度
    int height = 1080;                      // 图像高度
    cv::Mat image(height, width, CV_8UC3); // 创建一个CV_8UC3类型的图像

    inputFile.read(reinterpret_cast<char *>(image.data), 3 * width * height); // 从文件中读取RGB数据

    // 3. 保存图像为JPEG文件
    std::string outputFileName = "output.jpg"; // 你可以更改输出文件名
    bool success = cv::imwrite(outputFileName, image);

    if (success)
    {
        std::cout << "图像保存成功为 " << outputFileName << std::endl;
    }
    else
    {
        std::cerr << "无法保存图像" << std::endl;
    }

    // 4. 关闭输入文件
    inputFile.close();

    return 0;
}