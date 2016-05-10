#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include <string>//文件操作

// using namespace cvut;
//***********----------class Img-----------***********
#include <vector>
#include "iostream"
#include "stdio.h"
/*IMPORTANT*/
using namespace std;  
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
typedef unsigned char byte ;
//#pragma comment(lib,"cxcore.lib")
//#pragma comment(lib,"cv.lib")
//#pragma comment(lib,"highgui.lib")
//"cvaux.lib" "cvcam.lib"  "ml.lib"
//class Color-----
class Color{
public:
	static	CvScalar red;	
	static	CvScalar green;	
	static	CvScalar blue;
	static	CvScalar black;
	static CvScalar white;
	
};
CvScalar Color::green=CV_RGB(0,255,0);
CvScalar Color::red  =CV_RGB(255,0,0);
CvScalar Color::blue =CV_RGB(0,0,255);
CvScalar Color::black=CV_RGB(0,0,0);
CvScalar Color::white=CV_RGB(255,255,255);
//  --- End Class Color------

// ----  CvMat 数据攻取------
//CImag 类
class CImg{
public:
	IplImage *img;
	IplImage *img_32FC1;
	
	
	CImg(){
		img=NULL;
		img_32FC1=NULL;
	}
	
	//-----------------------------------Function here;
	//---static Functions declaration ---------
	static IplImage*  CreateImg(int h=3,int w=3,int typeOfData=8,int nChannels=1)
	{
		IplImage *retImg=cvCreateImage(cvSize(w,h),typeOfData,nChannels);
		return retImg;
	}//end createImg()
	
	static	void ShowImg(int win_num,IplImage *src){
		
		char windowName[22];
		sprintf_s(windowName,"外部img_%d\0",win_num);
		cvNamedWindow(windowName,1);
		cvShowImage(windowName,src);
		
	}//end showImg;
	
	void ReSizeToFrame(int Frame_size=128*2 )//这里并不传入图片，我如何知道img->height ,对了，做为类的函数就有泪的私有(这里的数据是公开的）变量IplImage *img;IplImage *img_32FC1
	
	{
		int k=Frame_size;
		
		int h=img->height;
		int w=img->width;
		
		int max=h>w?h:w;
		float rate=(float)(max*1.0/k);//rate就是//这里的语法是如何的，为何可以这样声明，有为何申明两次，又为何用的是img ,声明的却是Img_32FC1;
		float w1=w/rate;
		float h1=h/rate;
		CvSize sz=cvSize((int)w1,(int)h1);
		
		IplImage *tempimg=cvCreateImage(sz,8,1);
		cvResize(img,tempimg,CV_INTER_LINEAR); //CV_INTER_CUBIC CV_INTER_LINEAR
		img=tempimg;
		
	}//end Resize();
	
	
	
	void LoadToGrayImg(char *FilePath){
		
		IplImage *openImg=cvLoadImage(FilePath,-1);
		if(openImg->nChannels==3)
		{
			IplImage *retGrayImage=cvCreateImage(cvGetSize(openImg),8,1);
			cvCvtColor(openImg,retGrayImage,CV_BGR2GRAY);//熟练使用cvCvColor
			img=retGrayImage;
			
		}
		else 
			if(openImg->nChannels==1)
				img=openImg;
			else cout<<"Image DataType Not supported!\n";//三通道的灰度值转变 always like that 
			
	}//end LoadToGrayImg()
	
	void ShowImg(int win_num=0){
		
		char windowName[22];
		sprintf_s(windowName,"img_%d\0",win_num);
		cvNamedWindow(windowName,1);
		cvShowImage(windowName,img);
		
	}//end showImg;
	void GetImg_32FC1()
	{
		img_32FC1=cvCreateImage(cvGetSize(img),32,1);
		cvConvertScale(img,img_32FC1,1.0);//这种同义优化的意义是？
	}//end GetImg_32FC1()
	
	void AdaptiveThreshold(void)
	{
		assert(img->nChannels==1);   //没有include assert 为何可以使用
		cvAdaptiveThreshold( img, img, 255);//二值化 
	};
	void Smooth(int Temp_Size=5){//高斯平滑
		cvSmooth( img, img, CV_GAUSSIAN, Temp_Size);
	}// End Smooth
	void Threshold(int thresh=119){
		cvThreshold( img, img, thresh, 255,CV_THRESH_BINARY );
	}//End Threshold
	void EqualizeHist(){
		cvEqualizeHist(img,img);
		
	}
	void Canny(int param1=50,int param2=200){//canny 边缘检测
		IplImage *image0=img;
		cvCanny(  image0, image0, param1,  param1, 3 );
	}
	//
	
	static void ShowMatrixData(CvMat *mat){//在debug 中有利工具
#define CV_MAT_PElem(mat__,mytype,I,J) ((mytype*)cvPtr2D(mat__,I,J))//定义宏 这里的mytype 后期可以直接换吗？
		int type=CV_MAT_TYPE(mat->type);//为何不直接mat->type 
		int i,j;
		int t;
		
		
		int h=mat->height;
		int w=mat->width;
		
		int nchannels=1;
		if(type==CV_64FC3||type==CV_64FC2||type==CV_64FC1)//因为mat没有channels所以要特别指定吗？  干嘛不用switch case 
		{
			if(type==CV_64FC3)
			{
				
				nchannels=3;
			}
			else if(type==CV_64FC2)
			{
				nchannels=2;
			}
			else if(type==CV_64FC1)
			{
				nchannels=1;
			}
			
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					if(nchannels!=1)
						printf("[");
					
					for(t=0;t<nchannels;t++)
					{
						double *dp=CV_MAT_PElem(mat,double,i,j);
						printf(" %4.2f ",dp[t]);
					}
					if(nchannels!=1)
						printf("]");
				}
				cout<<"\n";
			}
		}//end double
		
		else if(type==CV_32FC3||type==CV_32FC2||type==CV_32FC1)
		{
			if(type==CV_32FC3)
			{
				
				nchannels=3;
			}
			else if(type==CV_32FC2)
			{
				nchannels=2;
			}
			else if(type==CV_32FC1)
			{
				nchannels=1;
			}
			
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					if(nchannels!=1)
						printf("[");
					for(t=0;t<nchannels;t++)
					{
						float *dp=CV_MAT_PElem(mat,float,i,j);
						//	cout<<" "<<dp[t]<<" ";
						printf(" %4.2f ",dp[t]);
					}
					if(nchannels!=1)
						printf("]");
				}
				cout<<"\n";
			}
		}//end float
		else if(type==CV_32SC3||type==CV_32SC2||type==CV_32SC1)
		{
			if(type==CV_32SC3)
			{
				
				nchannels=3;
			}
			else if(type==CV_32SC2)
			{
				nchannels=2;
			}
			else if(type==CV_32SC1)
			{
				nchannels=1;
			}
			
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					if(nchannels!=1)
						printf("[");
					for(t=0;t<nchannels;t++)
					{
						int *dp=CV_MAT_PElem(mat,int,i,j);
						printf(" %3d ",dp[t]);
					}
					if(nchannels!=1)
						printf("]");
				}
				cout<<"\n";
			}
		}//end int
		else if(type==CV_8UC3||type==CV_8UC2||type==CV_8UC1)
		{
			if(type==CV_8UC3)
			{
				
				nchannels=3;
			}
			else if(type==CV_8UC2)
			{
				nchannels=2;
			}
			else if(type==CV_8UC1)
			{
				nchannels=1; 
			}
			
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{
					if(nchannels!=1)
						printf("[");
					for(t=0;t<nchannels;t++)
					{
						byte *dp=CV_MAT_PElem(mat,byte,i,j);
						printf(" %3d ",dp[t]);
					}
					if(nchannels!=1)
						printf("]");
				}
				cout<<"\n";
			}
		}//end uchar//支持两种数据
		else cout<<"unsupported dataType!!--\n";
		
}// End SHowMatData----------------------

IplImage *GetImg_8UC3_Gray(){
	IplImage *imgC3=	CreateImg(img->height,img->width,8,3);
	cvZero(imgC3);
	IplImage *imgC1=img;
	cvMerge(imgC1,imgC1,imgC1,NULL,imgC3);//合并三个通道
	return imgC3;
}//End Get Img3Channels()
static	void ShowMatrixData(IplImage *src){
	CvMat header;  //仅仅创建头，不分配数据空间
	CvMat *mat	= cvGetMat(src,&header);// share the data Area
	ShowMatrixData(mat);
}//end show imgData
static CvMat* Transpose(CvMat* _mat){
	int h=_mat->width;
	int w=_mat->height;
	int type=CV_MAT_TYPE(_mat->type);
	CvMat *ret_mat=cvCreateMat(h,w,type);
	cvT(_mat,ret_mat);
	return ret_mat;
}//End Transpose() --
static CvMat* Transpose(IplImage* _img){
	CvMat header;  //仅仅创建头，不分配数据空间
	CvMat *mat	= cvGetMat(_img,&header);// share the data Area
	return Transpose(mat);
	
}//End Transpose() --

void LoadToGrayImg(IplImage*src){


	if(src->nChannels==3)
		cvCvtColor(src,img,CV_BGR2GRAY);
	else img=src;
	

}//End LoadToGrayImg(src)


~CImg(){
	if(img!=NULL)cvReleaseImage(&img);
	img_32FC1=NULL;
	cvDestroyAllWindows();
}
};//****-------- End Class CImg ---------------****** 




// *****----  STRUCT_2D_to_3D-----**********
class  C_2D_to_3D{
public:
	CvMat *_2D_pt; //Matrix 2D Point
	
	CvMat *intrinsic_matrix; //Matrix A
	CvMat *rotation_vectors; //Matrix R  -->(1,1,CV_32FC3)
	CvMat *translation_vectors;//Matrix t
	
	CvMat *_3D_pt; //Matrix 3D point
//	int image_count;
	C_2D_to_3D(){
		_2D_pt=NULL; //Matrix 2D Point
		
		intrinsic_matrix=NULL; //Matrix A
		rotation_vectors=NULL; //Matrix R  -->(1,1,CV_32FC3)
		translation_vectors=NULL;//Matrix t
		
		_3D_pt=NULL; //Matrix 3D point
	//	image_count=-1;
	}
	void showAllVals(int valNum=6){
		CImg idcImg;
		int i,j;
	
		CvMat *p[5]={_2D_pt,intrinsic_matrix,rotation_vectors,translation_vectors,_3D_pt};
		char *pName[5]={"_2D_pt","intrinsic_matrix","rotation_vectors","translation_vectors","_3D_pt"};
		for( i=0;i<5;i++)
			if(p[i]==NULL)
			{
				printf(pName[i] );printf(" is NULL pointer!\n");
				return;
			}//end reg Null 
			
			//---------	show All 6 Vals------------------
			cout<<" image_count-------\n"	;                    cout<<rotation_vectors->width <<endl;
			cout<<"idcImg.ShowMatrixData(_2D_pt)\n";			 idcImg.ShowMatrixData(_2D_pt);cout<<endl;
			cout<<"idcImg.ShowMatrixData(intrinsic_matrix)\n";	 idcImg.ShowMatrixData(intrinsic_matrix);cout<<endl;
			cout<<"idcImg.ShowMatrixData(rotation_vectors)\n";   idcImg.ShowMatrixData(rotation_vectors);cout<<endl;
			cout<<"idcImg.ShowMatrixData(translation_vectors)\n";idcImg.ShowMatrixData(translation_vectors);cout<<endl;
			cout<<"idcImg.ShowMatrixData(_3D_pt)\n";             idcImg.ShowMatrixData(_3D_pt);cout<<endl;
		
			
	}	//end Show(vals= 6);
	void do_2D_to_3D(CvPoint2D32f input_pt2D=cvPoint2D32f(333,24))
	{
		CvPoint2D32f inPt2D=input_pt2D;
		CV_MAT_PElem(_2D_pt,float,0,0)[0]=inPt2D.x;
		CV_MAT_PElem(_2D_pt,float,1,0)[0]=inPt2D.y;
		CV_MAT_PElem(_2D_pt,float,2,0)[0]=1;
		
		int i,j;

		CvMat *p[5]={_2D_pt,intrinsic_matrix,rotation_vectors,translation_vectors,_3D_pt};
		char *pName[5]={"_2D_pt","intrinsic_matrix","rotation_vectors","translation_vectors","_3D_pt"};
		for( i=0;i<5;i++)
			if(p[i]==NULL)
			{
				printf(pName[i] );printf(" is NULL pointer!\n");
				return;
			}//end reg Null 
			
			
			
			// get the right six vals-------------------
			for(i=0;i<1;i++)   //image_count=tr~.width--
			{
				
				CvMat* rotation_vector_per=cvCreateMat(1,1,CV_32FC3);
				CvMat* rotation_matrix=cvCreateMat(3,3,CV_32FC1); // 3 x 3
				
				
				rotation_vector_per->data.ptr=cvPtr2D(rotation_vectors,0,i);
				cvRodrigues2(rotation_vector_per,rotation_matrix);
				
				
				CvMat *A=intrinsic_matrix;
				
				CvMat *R=rotation_matrix;
				
				CvMat *B=cvCreateMat(3,3,CV_32FC1);
				CvMat *t=cvCreateMat(3,1,CV_32FC1);
				
				B=cvCloneMat(R);
				
				
				// Initial [t]--  //获得单个图像的t(3x1)矩阵 以及 [R3+t]
				for(j=0;j<3;j++)
				{				
					float t_i_j=(CV_MAT_PElem(t,float,j,0)[0]=CV_MAT_PElem(translation_vectors,float,0,i)[j]);  //Translat_vectors->(T)
					CV_MAT_PElem(B,float,j,2)[0]+=t_i_j;   ////  ( A B=[R1,R,R3+t] )^(-1) [x1,x2,x3](T)
					
				}//end for(j)
				
				cvGEMM(A,B,1,0,1,B);
				cvInvert(B,B,CV_LU);
				cvGEMM(B,_2D_pt,1,0,0,_3D_pt,0);
				
				float _z;
				_z=CV_MAT_PElem(_3D_pt,float,2,0)[0];
				for(j=0;j<3;j++)
				{
					CV_MAT_PElem(_3D_pt,float,j,0)[0]/=_z;		
				}
				
				//	idCImg.ShowMatrixData(_3D_pt);
				
			}  //end for(i:image_count)---
			
	}//end do2D - > 3D 
	void do_3D_to_2D(CvPoint3D32f intputpt_3d=cvPoint3D32f(0,0,1))
	{
		CvPoint3D32f inPt3D=intputpt_3d;
		CV_MAT_PElem(_3D_pt,float,0,0)[0]=inPt3D.x;
		CV_MAT_PElem(_3D_pt,float,1,0)[0]=inPt3D.y;
		CV_MAT_PElem(_3D_pt,float,2,0)[0]=1;
		
		int i,j;

		CvMat *p[5]={_2D_pt,intrinsic_matrix,rotation_vectors,translation_vectors,_3D_pt};
		char *pName[5]={"_2D_pt","intrinsic_matrix","rotation_vectors","translation_vectors","_3D_pt"};
		for( i=0;i<5;i++)
			if(p[i]==NULL)
			{
				printf(pName[i] );printf(" is NULL pointer!\n");
				return;
			}//end reg Null 
			
			
			
			// get the right six vals
			for(i=0;i<1;i++)   //image_count=1
			{
				// C_2D_to_3D id_2d3d;
				
				
				CvMat* rotation_vector_per=cvCreateMat(1,1,CV_32FC3);
				CvMat* rotation_matrix=cvCreateMat(3,3,CV_32FC1); // 3 x 3
				
				
				rotation_vector_per->data.ptr=cvPtr2D(rotation_vectors,0,i);
				cvRodrigues2(rotation_vector_per,rotation_matrix);
				
				
				CvMat *A=intrinsic_matrix;
				
				CvMat *R=rotation_matrix;
				
				CvMat *B=cvCreateMat(3,3,CV_32FC1);
				CvMat *t=cvCreateMat(3,1,CV_32FC1);
				
				B=cvCloneMat(R);
				
				
				// Initial [t]--  //获得单个图像的t(3x1)矩阵
				for(j=0;j<3;j++)
				{
					
					float t_i_j=(CV_MAT_PElem(t,float,j,0)[0]=CV_MAT_PElem(translation_vectors,float,0,i)[j]);  //Translat_vectors->(T)
					CV_MAT_PElem(B,float,j,2)[0]+=t_i_j;   ////  ( A B=[R1,R,R3+t] )^(-1) [x1,x2,x3](T)
					
				}//end for(j)
				

				cvGEMM(R,_3D_pt,1,t,1,_2D_pt);
				cvGEMM(A,_2D_pt,1,0,1,_2D_pt);
				
				// 3D ->2D _z=1 ---
				
				float _z;
				
				_z=CV_MAT_PElem(_2D_pt,float,2,0)[0];
				
				for(j=0;j<3;j++)
				{
					
					CV_MAT_PElem(_2D_pt,float,j,0)[0]/=_z;
				}
				

				
			}  //end for(i:image_count)---
			
	 }	 //end do_3D_to_2D---
	 
};// End struct C_2D_3D-------



void main(){
	CImg idCImg;



//idCImg.ShowImg(1,idCImg.img);







//----------------------------------------------------------CImg test-------------------------------------	
	CvMat *_3D_pt=cvCreateMat(3,1,CV_32FC1);
	CvMat *_2D_pt=cvCreateMat(3,1,CV_32FC1);
	
	int i,j,t;
	ifstream fin("calibdata.txt"); /* 定标所用图像文件的路径 */
	ofstream fout("caliberation_result.txt");  /* 保存定标结果的文件 */
	
	cout<<"开始提取角点………………\n";
	
	CvSize chess_sz=cvSize(6,4);
	int cw=chess_sz.width;
	int ch=chess_sz.height;
	int chess_WxH=cw*ch;
	CvPoint2D32f* image_pt_perImg = new CvPoint2D32f[chess_WxH];   /* 缓存每幅图像上检测到的角点 */
	vector<CvPoint2D32f> seq_img_pt;
	string filename;
	int count=-1;
	int image_count=0;
	while (getline(fin,filename))
	{
		idCImg.LoadToGrayImg((char*)filename.c_str());
		IplImage *img_3C=idCImg.GetImg_8UC3_Gray();
		IplImage *image=idCImg.img;
		CvSize pattern_size=chess_sz;
		CvPoint2D32f* corners=image_pt_perImg;
		int corner_count;
		int find_retcode=cvFindChessboardCorners(image,pattern_size,corners,&corner_count,CV_CALIB_CB_ADAPTIVE_THRESH);
		if(find_retcode==0) continue ;  //Not Find Corners找下一幅图
		// Find corner All 
		image_count++;  
		
		cvFindCornerSubPix( image, 
			corners, 
			corner_count, 
			cvSize(11,11),
			cvSize(-1,-1), 
			cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 )
			);//对粗提取的角点进行精确化
		// ---- Push to seq----
		for(i=0;i<corner_count;i++)
			seq_img_pt.push_back(corners[i]);
		
		// -----Draw ChessBoard---
		cvDrawChessboardCorners( img_3C, pattern_size, corners, corner_count, find_retcode);//用于在图片中标记角点
		
		//----  show Image()------
		idCImg.ShowImg(0,img_3C);
		cvWaitKey(100/4);//-----------waitTime-----
	}  // End while(getLine())----
	
	delete [] image_pt_perImg;
	cout<<"Corners finded---\n";
	int corner_all=seq_img_pt.size();
	
	//---- calibration----
	CvSize square_size = cvSize(10,10);  
	
	CvMat *object_points=cvCreateMat(1,image_count*chess_WxH,CV_32FC3);
	CvMat *image_points=cvCreateMat(1,image_count*chess_WxH,CV_32FC2);
	
	CvMat *points_counts=cvCreateMat(1,image_count,CV_32SC1); //每幅图像点数量
	
	CvSize image_size=chess_sz;
	
	CvMat *intrinsic_matrix=cvCreateMat(3,3,CV_32FC1);
	CvMat *distortion_coeffs=cvCreateMat(1,4,CV_32FC1);
	
	CvMat *rotation_vectors=cvCreateMat(1,image_count,CV_32FC3);
	CvMat *translation_vectors=cvCreateMat(1,image_count,CV_32FC3);
	// ---- Initial Object_points and Image_points ----
	//1. object_points--
	for(t=0;t<image_count;t++)
	{
		for(i=0;i<ch;i++)
		{
			for(j=0;j<cw;j++)
			{
				CvPoint3D32f &PtTmp3D32f=*((CvPoint3D32f*)CV_MAT_PElem(object_points,float,0,t*chess_WxH+i*cw+j));
				PtTmp3D32f.x=j*square_size.width;
				PtTmp3D32f.y=i*square_size.height;
				PtTmp3D32f.z=1; //with Z = 1;
			}//end for(j)
		}//end for(i)
	}//end for(t)
	idCImg.ShowMatrixData(object_points);
	
	// 2.image_points----
	
	for(i=0;i<corner_all;i++)
	{
		CvPoint2D32f& Pt2D32f=*(CvPoint2D32f*)CV_MAT_PElem(image_points,float,0,i);
		Pt2D32f.x=seq_img_pt[i].x; //序列可以直接寻值
		Pt2D32f.y=seq_img_pt[i].y; 
	}
	
	//idCImg.ShowMatrixData(image_points);
	
	// 3.point_counts 各图的点数量
	for(i=0;i<=image_count;i++)
	{
		int &cnt_perTemp=*CV_MAT_PElem(points_counts,int,0,i);
		cnt_perTemp=chess_WxH;
	}
	
	i=translation_vectors->rows;
	
	CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
  CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0f;

	cvCalibrateCamera2(
		,
		image_pointsobject_points,
		points_counts,
		image_size,
		intrinsic_matrix,
		distortion_coeffs,
		rotation_vectors,
		translation_vectors,
		0);	
	
	// -- END CALIBRATION-----
	
	//---test ---Struct C_2D_to_3D


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


	int board_w = 7;//角点数
	int board_h = 7;
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
		CV_MAT_ELEM(*image_points,CvPoint2D32f,i,0) = imgPts[i];
		CV_MAT_ELEM(*object_points,CvPoint3D32f,i,0) = cvPoint3D32f(objPts[i].x,objPts[i].y,0);
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
