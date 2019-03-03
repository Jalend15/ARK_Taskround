#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <math.h>

using namespace cv;

using namespace std;

/// Global Variables
int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;

Mat src, erosion_dst, dilation_dst;

int erosion_elem = 0;
int erosion_size = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

/** Function Headers */
void Erosion( int, void* );
void Dilation( int, void* ); Mat dst;
char window_name[] = "Filter Demo 1";

/// Function headers
int display_caption( char* caption );
int display_dst( int delay );

int main() 
{
int win=7;
Mat img1=imread("left_image.jpg",0);
Mat img2=imread("right_image.jpg",0);
Mat Dis(img1.rows,img2.cols,CV_8UC1,Scalar(0));
Mat w1,w2,result;
 long int ssd;
 int a=0;
 long int min1;
 //result.create( 1,img1.cols-win+1,CV_32FC1);
 for(int i=win/2; i<img1.rows-win/2;i++)
   {
     for(int j=win/2; j<img1.cols-win/2;j++)
       {
	 min1=999999999999;
	 ssd=0;
	 a=0;
	 for(int m=win/2;m<img1.cols-win/2;m++)
	   {
	     ssd=0;
	 for(int k=-win/2; k<=win/2;k++)
	   {
	     for(int l=-win/2;l<=win/2;l++)
	       {
		 
		
		   ssd=ssd+(img1.at<uchar>(i+k,j+l)-img2.at<uchar>(i+k,l+m))*(img1.at<uchar>(i+k,j+l)-img2.at<uchar>(i+k,l+m));
		  
	       }
	   }
	  
	 if(ssd<=min1)
	   {
	     a=m;
	     min1=ssd;
	       }
	   }
	  Dis.at<uchar>(i,j)=255*((float)abs(j-a)/(float)(img1.cols-win+1));
       }
   }

    /*w1=img1(Range(i-(win-1)/2,win-1+i-(win-1)/2),Range(j-(win-1)/2,win-1+j-(win-1)/2)); //Window around selected pixel in left image
    w2=img2(Range(i-(win-1)/2,win-1+i-(win-1)/2),Range(0,img1.cols/2));  //Checking region in right image
    matchTemplate(w2, w1, result, CV_TM_SQDIFF_NORMED );    //SSD to find closest match*/
    /* int a,b;
    //normalize( result, result, 0 ,255,NORM_MINMAX,-1,CV_32FC1);
    double minVal; double maxVal; Point minLoc; Point maxLoc;    
    Point matchLoc;   //Point where location matches
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    matchLoc=minLoc;*/
   
namedWindow("W",WINDOW_NORMAL);
imshow("W", Dis);
 src=Dis;/*
if( display_caption( "Gaussian Blur" ) != 0 ) { return 0; }

    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
        { GaussianBlur( Dis,dst, Size( i, i ), 0, 0 );
          if( display_dst( DELAY_BLUR ) != 0 ) { return 0; } }
    imshow("W", dst);*/
  /// Create windows
  namedWindow( "Erosion Demo", CV_WINDOW_AUTOSIZE );
  namedWindow( "Dilation Demo", CV_WINDOW_AUTOSIZE );
  cvMoveWindow( "Dilation Demo", src.cols, 0 );

  /// Create Erosion Trackbar
  createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
                  &erosion_elem, max_elem,
                  Erosion );

  createTrackbar( "Kernel size:\n 2n +1", "Erosion Demo",
                  &erosion_size, max_kernel_size,
                  Erosion );

  /// Create Dilation Trackbar
  createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
                  &dilation_elem, max_elem,
                  Dilation );

  createTrackbar( "Kernel size:\n 2n +1", "Dilation Demo",
                  &dilation_size, max_kernel_size,
                  Dilation );

  /// Default start
  Erosion( 0, 0 );
  Dilation( 0, 0 );
waitKey(0);
return 0;

}
 int display_caption( char* caption )
 {
   dst = Mat::zeros( src.size(), src.type() );
   putText( dst, caption,
            Point( src.cols/4, src.rows/2),
            CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );

   imshow( window_name, dst );
   int c = waitKey( DELAY_CAPTION );
   if( c >= 0 ) { return -1; }
   return 0;
  }
void Erosion( int, void* )
{
  int erosion_type;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );

  /// Apply the erosion operation
  erode( src, erosion_dst, element );
  imshow( "Erosion Demo", erosion_dst );
}

/** @function Dilation */
void Dilation( int, void* )
{
  int dilation_type;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  dilate( src, dilation_dst, element );
  imshow( "Dilation Demo", dilation_dst );
}


  int display_dst( int delay )
  {
    imshow( window_name, dst );
    int c = waitKey ( delay );
    if( c >= 0 ) { return -1; }
    return 0;
  }


