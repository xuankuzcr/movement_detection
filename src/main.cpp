#include <iostream>
#include "opencv2/opencv.hpp"
#include <math.h>
#include <numeric>
#include <chrono>
using namespace std;
using namespace cv;

bool judgemovement(Mat &pre_frame, Mat &curr_frame);
int main()
{   
    int number=0;
    std::vector<cv::String> filenames; 
    cv::String folder = "/home/chunran/sustech_rgbd/rgb"; 
    cv::glob(folder, filenames); 
    for(size_t i = 0; i < filenames.size()-1; ++i)
    {
        //std::cout<<filenames[i]<<std::endl;
        Mat pre_frame = cv::imread(filenames[i],CV_LOAD_IMAGE_UNCHANGED);
        Mat curr_frame = cv::imread(filenames[i+1],CV_LOAD_IMAGE_UNCHANGED);//当前帧


        if((!pre_frame.data)||(!curr_frame.data))
            std::cerr << "Problem loading image!!!" << std::endl;
        bool result = judgemovement(pre_frame, curr_frame);
        if(result==true)
            number++;

    }
    if(number==filenames.size()-1)
        cout<<"All movements are detected!!!"<<endl;
    cout<<number<<"/"<<filenames.size()<<endl;
    //Mat pre_frame = imread("../pic/3.jpg",CV_LOAD_IMAGE_UNCHANGED); //上一帧
    //Mat curr_frame = imread("../pic/4.jpg",CV_LOAD_IMAGE_UNCHANGED);//当前帧
    
    //if(result == true)
        //cout<<"The movement is detected"<<endl;
    waitKey(0);   
}

bool judgemovement(Mat &pre_frame, Mat &curr_frame)
{
    vector<vector<Point>>contours;
    vector<Vec4i>hierarchy;   
    Mat element = getStructuringElement(MORPH_RECT,Size(1,1));
    Mat img_delta;
    int count = 0;
    //灰度化
    cvtColor(pre_frame,pre_frame,CV_BGR2GRAY);
    cvtColor(curr_frame,curr_frame,CV_BGR2GRAY);
    //两张灰度图进行差分
    absdiff(pre_frame,curr_frame,img_delta); 
    //二值化：灰度值超过25赋值为255，小于置0
    threshold(img_delta,img_delta,25,255,CV_THRESH_BINARY);
    //膨胀：弥补孔和缺陷，形成连续轮廓
    dilate(img_delta,img_delta,cv::Mat());
    //寻找轮廓
    findContours(img_delta, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    for(int i=0;i<contours.size();i++) //遍历轮廓
    {
      if(contourArea(contours[i])<1000) //轮廓小于1000不要，认为是噪声
        continue;
      else //否则认为有一个移动轮廓
        count++;

    }    
    cout<<count<<"/"<<contours.size()<<endl; // 移动轮廓的个数/所有有轮廓的个数，注意：所有轮廓包含移动轮廓和噪声轮廓
    if(count>=3) //超过10个移动轮廓认为车在移动, 屏蔽掉车不动，但有移动物体的情况
        return true;
    else
        return false;
    
    
    //imshow("diff",img_delta);
    // imshow("pre_frame",pre_frame);
    // imshow ("curr_frame",curr_frame);
 
}

