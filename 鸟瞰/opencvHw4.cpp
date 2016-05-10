#include "StdAfx.h"
#include <string>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>


//#pragma comment(lib, "ml.lib") //链接进一个库
//#pragma comment(lib, "cv.lib")
//#pragma comment(lib, "cvaux.lib")
//#pragma comment(lib, "cvcam.lib")
//#pragma comment(lib, "cxcore.lib")
//#pragma comment(lib, "cxts.lib")
//#pragma comment(lib, "highgui.lib")
//#pragma comment(lib, "cvhaartraining.lib")

using namespace std;

int main()
{
	int cube_length=12;
	//CvCapture* capture;
	//capture=cvCreateCameraCapture(0); //打开摄像头、捕获图像
	//if(capture==0)
	//{
	//	printf("无法捕获摄像头设备！\n\n");
	//	return 0;
	//}
	//else
	//{
	//	printf("捕获摄像头设备成功！！\n\n");
	//}
	//IplImage* frame = NULL;
	//cvNamedWindow("摄像机帧截取窗口",1);
	//printf("按“C”键截取当前帧并保存为标定图片...\n按“Q”键退出截取帧过程...\n\n");
	//
	//
	////获取进行标定要使用的图片
	//int number_image=1;
	//char *str1=".jpg";
	//char filename[20]="";

	//while(true)
	//{
	//	frame=cvQueryFrame(capture);//在窗口显示捕获到的每一帧
	//	if(!frame)
	//		break;
	//	cvShowImage("摄像机帧截取窗口",frame);
	//	//当按下c键时，截屏并保存
	//	if(cvWaitKey(10)=='c')
	//	{
	//		sprintf_s (filename,"%d.jpg",number_image);
	//		cvSaveImage(filename,frame);
	//		cout<<"成功获取当前帧，并以文件名"<<filename<<"保存...\n\n";
	//		printf("按“C”键截取当前帧并保存为标定图片...\n按“Q”键退出截取帧过程...\n\n");
	//		number_image++;
	//	}
	//	//当按下q键时退出摄像机
	//	else if(cvWaitKey(10)=='q')
	//	{
	//		printf("截取图像帧过程完成...\n\n");
	//		cout<<"共成功截取"<<--number_image<<"帧图像！！\n\n";
	//		break;
	//	}
	//}
	////释放内存
	//cvReleaseImage(&frame);
	//cvDestroyWindow("摄像机帧截取窗口");	
	int number_image=14;
	char filename[14] = "";
	char *str = ".jpg";
	for (int i = 1; i<=number_image;i++)
	{
		sprintf_s (filename,"%d.jpg",i);
	}
	for (int j=0;j<14;j++){
		cvLoadImage(&filename[j]);
	}

	CvSize board_size=cvSize(12,12);// Cvsizes:OpenCV的基本数据类型之一。表示矩阵框大小，以像素为精度。与CvPoint结构类似，但数据成员是integer类型的width和height。
	int board_width=board_size.width;
	int board_height=board_size.height;
	int total_per_image=board_width*board_height;
	CvPoint2D32f * image_points_buf = new CvPoint2D32f[total_per_image];//创建一个单个元素为点的数组
	CvMat * image_points=cvCreateMat(number_image*total_per_image,2,CV_32FC1);
	CvMat * object_points=cvCreateMat(number_image*total_per_image,3,CV_32FC1);
	CvMat * point_counts=cvCreateMat(number_image,1,CV_32SC1);//存储每张图片的某个信息
	CvMat * intrinsic_matrix=cvCreateMat(3,3,CV_32FC1);//摄像机的内参数矩阵
	CvMat * distortion_coeffs=cvCreateMat(5,1,CV_32FC1);//摄像机的而畸变系数

	int count;
	int found;
	int step;
	int successes=0;
	int a=1;
	int number_image_copy = number_image;
	IplImage * show;
	cvNamedWindow("RePlay",1);

	while(a<=number_image_copy)//针对每张图片做处理
	{
		sprintf_s (filename,"%d.jpg",a);
		show=cvLoadImage(filename,-1);

		found=cvFindChessboardCorners(show,board_size,image_points_buf,&count,
			CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_FILTER_QUADS);//对于当前的图片进行寻找角点
		if(found==0)//没有找到所有角点，失败
		{		
			cout<<"第"<<a<<"帧图片无法找到棋盘格所有角点!\n\n";
			cvNamedWindow("RePlay",1);
			cvShowImage("RePlay",show);
			cvWaitKey(0);

		}
		else
		{	//成功
			//找到图片中的角点并且展示出来
			cout<<"第"<<a<<"帧图像成功获得"<<count<<"个角点...\n";
			cvNamedWindow("RePlay",1);
			IplImage * gray_image= cvCreateImage(cvGetSize(show),8,1);
			cvCvtColor(show,gray_image,CV_BGR2GRAY);//将当前图片变为灰度图片
			cout<<"获取源图像灰度图过程完成...\n";
			cvFindCornerSubPix(gray_image,image_points_buf,count,cvSize(11,11),cvSize(-1,-1),
				cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1));//对得到的灰度图像进行亚像素精度的处理
			cout<<"灰度图亚像素化过程完成...\n";
			cvDrawChessboardCorners(show,board_size,image_points_buf,count,found);
			cout<<"在源图像上绘制角点过程完成...\n\n";
			cvShowImage("RePlay",show);
			cvWaitKey(0);
		}
		//标定图片？？？？？？？？？？？？
		if(total_per_image==count)
		{
			step=successes*total_per_image;
			for(int i=step,j=0;j<total_per_image;++i,++j)
			{
				CV_MAT_ELEM(*image_points,float,i,0)=image_points_buf[j].x;
				CV_MAT_ELEM(*image_points,float,i,1)=image_points_buf[j].y;
				CV_MAT_ELEM(*object_points,float,i,0)=(float)(j/cube_length);
				CV_MAT_ELEM(*object_points,float,i,1)=(float)(j%cube_length);
				CV_MAT_ELEM(*object_points,float,i,2)=0.0f;
			}
			CV_MAT_ELEM(*point_counts,int,successes,0)=total_per_image;
			successes++;
		}
		a++;
	}

	cvReleaseImage(&show);
	cvDestroyWindow("RePlay");


	cout<<"*********************************************\n";
	cout<<number_image<<"帧图片中，标定成功的图片为"<<successes<<"帧...\n";
	cout<<number_image<<"帧图片中，标定失败的图片为"<<number_image-successes<<"帧...\n\n";
	cout<<"*********************************************\n\n";

	cout<<"按任意键开始计算摄像机内参数...\n\n";

	//CvCapture* capture1;
	//capture1=cvCreateCameraCapture(0);
	IplImage * show_colie;
	show_colie=cvLoadImage("1.jpg");

	CvMat * object_points2=cvCreateMat(successes*total_per_image,3,CV_32FC1);
	CvMat * image_points2=cvCreateMat(successes*total_per_image,2,CV_32FC1);
	CvMat * point_counts2=cvCreateMat(successes,1,CV_32SC1);
	//摄像机标定
	for(int i=0;i<successes*total_per_image;++i)
	{
		CV_MAT_ELEM(*image_points2,float,i,0)=CV_MAT_ELEM(*image_points,float,i,0);
		CV_MAT_ELEM(*image_points2,float,i,1)=CV_MAT_ELEM(*image_points,float,i,1);
		CV_MAT_ELEM(*object_points2,float,i,0)=CV_MAT_ELEM(*object_points,float,i,0);
		CV_MAT_ELEM(*object_points2,float,i,1)=CV_MAT_ELEM(*object_points,float,i,1);
		CV_MAT_ELEM(*object_points2,float,i,2)=CV_MAT_ELEM(*object_points,float,i,2);
	}

	for(int i=0;i<successes;++i)
	{
		CV_MAT_ELEM(*point_counts2,int,i,0)=CV_MAT_ELEM(*point_counts,int,i,0);
	}

	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points);
	cvReleaseMat(&point_counts);

	CV_MAT_ELEM(*intrinsic_matrix,float,0,0)=1.0f;
	CV_MAT_ELEM(*intrinsic_matrix,float,1,1)=1.0f;

	cvCalibrateCamera2(object_points2,image_points2,point_counts2,cvGetSize(show_colie),
		intrinsic_matrix,distortion_coeffs,NULL,NULL,0);
	//输出得到的摄像机参数
	cout<<"摄像机内参数矩阵为：\n";
	cout<<CV_MAT_ELEM(*intrinsic_matrix,float,0,0)<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,0,1)
		<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,0,2)
		<<"\n\n";
	cout<<CV_MAT_ELEM(*intrinsic_matrix,float,1,0)<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,1,1)
		<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,1,2)
		<<"\n\n";
	cout<<CV_MAT_ELEM(*intrinsic_matrix,float,2,0)<<"    "<<CV_MAT_ELEM(*intrinsic_matrix,float,2,1)
		<<"          "<<CV_MAT_ELEM(*intrinsic_matrix,float,2,2)
		<<"\n\n";

	cout<<"畸变系数矩阵为：\n";
	cout<<CV_MAT_ELEM(*distortion_coeffs,float,0,0)<<"    "<<CV_MAT_ELEM(*distortion_coeffs,float,1,0)
		<<"    "<<CV_MAT_ELEM(*distortion_coeffs,float,2,0)
		<<"    "<<CV_MAT_ELEM(*distortion_coeffs,float,3,0)
		<<"    "<<CV_MAT_ELEM(*distortion_coeffs,float,4,0)
		<<"\n\n";
	//将得到的摄像机参数保存为xml文件
	cvSave("Intrinsics.xml",intrinsic_matrix);
	cvSave("Distortion.xml",distortion_coeffs);
	cout<<"摄像机矩阵、畸变系数向量已经分别存储在名为Intrinsics.xml、Distortion.xml文档中\n\n";

	CvMat * intrinsic=(CvMat *)cvLoad("Intrinsics.xml");
	CvMat * distortion=(CvMat *)cvLoad("Distortion.xml");

	IplImage * mapx=cvCreateImage(cvGetSize(show_colie),IPL_DEPTH_32F,1);
	IplImage * mapy=cvCreateImage(cvGetSize(show_colie),IPL_DEPTH_32F,1);

	cvInitUndistortMap(intrinsic,distortion,mapx,mapy);

	cvNamedWindow("原始图像",1);
	cvNamedWindow("畸变矫正图像",1);

	cout<<"摄像机标定部分完成\n\n";
	//对读到的每张图片进行畸变校正
	//while(show_colie)
	//{
		IplImage * clone=cvCloneImage(show_colie);
		cvShowImage("原始图像",show_colie);
		cvRemap(clone,show_colie,mapx,mapy);
		cvReleaseImage(&clone);
		cvShowImage("非畸变图像",show_colie);

		//if(cvWaitKey(10)=='e')
		//{
		//	break;
		//}

		//show_colie=cvQueryFrame(capture1);
	//}


	int board_w = board_width;//角点数
	int board_h = board_height;
	int board_n  = board_w * board_h;
	CvSize board_sz = cvSize( board_w, board_h );
	//加载事先做好的内参数和畸变系数
	CvMat *intrinsic2 = intrinsic_matrix;
    CvMat *distortion2 = distortion_coeffs;
	IplImage *image2 = 0, *gray_image2 = 0;
	//读入待转换的图片
	if((image2 = cvLoadImage("before.jpg"))== 0){
		printf("Error: Couldn't load the picture!\n");
		return -1;
	}
	//产生一张该图片的灰度图片
	gray_image2 = cvCreateImage(cvGetSize(image2),8,1);
    cvCvtColor(image2, gray_image2, CV_BGR2GRAY);
	cout<<"灰度图像转换完成\n";

	//矫正畸变
    //IplImage* mapx2 = cvCreateImage( cvGetSize(image2), IPL_DEPTH_32F, 1 );
    //IplImage* mapy2 = cvCreateImage( cvGetSize(image2), IPL_DEPTH_32F, 1 );
    //cvInitUndistortMap(
     // intrinsic2,
    //  distortion2,
     // mapx2,
     // mapy2
    //);
	//IplImage *t = cvCloneImage(image2);
    //cvRemap( t, image2, mapx2, mapy2 );//得到矫正后的图片image;t作为输出图像，image作为输出图像
	//cout<<"图片矫正完成\n";

	cvNamedWindow("Checkers");
    CvPoint2D32f* corners2 = new CvPoint2D32f[ board_n ];
    int corner_count2 = 0;
	//寻找角点
    int found2 = cvFindChessboardCorners(
        image2,
        board_sz,
        corners2,
        &corner_count2, 
        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
      );
	if(!found){
		printf("Couldn't aquire checkerboard on the picture, only found %d of %d corners\n",
				corner_count2,board_n);
		return -1;
	}
	//得到角点的次精度位置
	cvFindCornerSubPix(gray_image2, corners2, corner_count2, 
			  cvSize(11,11),cvSize(-1,-1), 
			  cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

	//GET THE IMAGE AND OBJECT POINTS:
	//Object points are at (r,c): (0,0), (board_w-1,0), (0,board_h-1), (board_w-1,board_h-1)
	//That means corners are at: corners[r*board_w + c]
	CvPoint2D32f objPts[4], imgPts[4];//都是4个点组成的数组    四个角上的点
	objPts[0].x = 0;         objPts[0].y = 0; 
	objPts[1].x = board_w-1; objPts[1].y = 0; 
	objPts[2].x = 0;         objPts[2].y = board_h-1;
	objPts[3].x = board_w-1; objPts[3].y = board_h-1; 
	imgPts[0] = corners2[0];
	imgPts[1] = corners2[board_w-1];
	imgPts[2] = corners2[(board_h-1)*board_w];
	imgPts[3] = corners2[(board_h-1)*board_w + board_w-1];

	//将这些点用不同颜色的圆圈表示
	cvCircle(image2,cvPointFrom32f(imgPts[0]),9,CV_RGB(0,0,255),3);
	cvCircle(image2,cvPointFrom32f(imgPts[1]),9,CV_RGB(0,255,0),3);
	cvCircle(image2,cvPointFrom32f(imgPts[2]),9,CV_RGB(255,0,0),3);
	cvCircle(image2,cvPointFrom32f(imgPts[3]),9,CV_RGB(255,255,0),3);
	cout<<"寻找角点并画出来\n";
	//画出所有角点并且展示出来
	cvDrawChessboardCorners(image2, board_sz, corners2, corner_count2, found2);
    cvShowImage( "Checkers", image2 );
	cvSaveImage("Checkers.jpg",image2);

	//FIND THE HOMOGRAPHY
	CvMat *H = cvCreateMat( 3, 3, CV_32F);
	CvMat *H_invt = cvCreateMat(3,3,CV_32F);
	cvGetPerspectiveTransform(objPts,imgPts,H);//得到透视变换矩阵参数 H为3*3矩阵；从平面到平面是单应性变换

	float Z = 25;
	int key = 0;
	IplImage *birds_image = cvCloneImage(image2);
	cvNamedWindow("Birds_Eye");
	cout<<"开始进行鸟瞰图变换\n";
    while(key != 27) {
	   CV_MAT_ELEM(*H,float,2,2) = Z;
	   cout<<'1';
	   cvWarpPerspective(image2,birds_image,H,    //cvWarpPerspective：用于整个图像的透视变换
			CV_INTER_LINEAR+CV_WARP_INVERSE_MAP+CV_WARP_FILL_OUTLIERS );//对图像进行透视变换得到变换后的图片birds_image；
			//CV_INTER_LINEAR+CV_WARP_INVERSE_MAP+CV_WARP_FILL_OUTLIERS：确定进行的是鸟瞰图转换
	   cout<<'2';
	   cvShowImage("Birds_Eye", birds_image);
	   cout<<"3";
	   cvSaveImage("Birds_Eye.jpg",birds_image);
	   cout<<"鸟瞰图变换成功";
	   key = cvWaitKey();
	   if(key == 'u') Z += 0.5;
	   if(key == 'd') Z -= 0.5;
	  
	}

	//SHOW ROTATION AND TRANSLATION VECTORS
	CvMat* image_points3  = cvCreateMat(4,1,CV_32FC2);
	CvMat* object_points3 = cvCreateMat(4,1,CV_32FC3);
	for(int i=0;i<4;++i){
		CV_MAT_ELEM(*image_points2,CvPoint2D32f,i,0) = imgPts[i];
		CV_MAT_ELEM(*object_points2,CvPoint3D32f,i,0) = cvPoint3D32f(objPts[i].x,objPts[i].y,0);
	}

	CvMat *RotRodrigues   = cvCreateMat(3,1,CV_32F);
	CvMat *Rot   = cvCreateMat(3,3,CV_32F);
	CvMat *Trans = cvCreateMat(3,1,CV_32F);
	cvFindExtrinsicCameraParams2(object_points,image_points,
			intrinsic,distortion,
			RotRodrigues,Trans);
	cvRodrigues2(RotRodrigues,Rot);//三维旋转

	//保存相关文件
	cvSave("Rot.xml",Rot);
	cvSave("Trans.xml",Trans);
	cvSave("H.xml",H);
	cvInvert(H,H_invt);
    cvSave("H_invt.xml",H_invt); //Bottom row of H invert is horizon line
	return 0;
}
