/* test rig for OpenCV #2690 */

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/videostab/global_motion.hpp>


inline static
void drawOptFlowMap(const cv::Mat& flow, cv::Mat& cflowmap, int step, const cv::Scalar& color)
{
	for(int y = 0; y < cflowmap.rows; y += step)
		for(int x = 0; x < cflowmap.cols; x += step)
		{
			const cv::Point2f& fxy = flow.at<cv::Point2f>(y, x);
			cv::line(cflowmap, cv::Point(x,y), cv::Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
				 color);
			cv::circle(cflowmap, cv::Point(x,y), 1, color, -1);
		}
}

class DenseOpticalFlow
{
	cv::Mat frames[2];
	bool currentFrame;

	cv::Mat flow;

	double scale;

public:

	DenseOpticalFlow(double scale_ = 4)
		: scale(scale_)
	{
	}

	void operator()(const cv::Mat& image)
	{
		// no image no fun
		if (image.empty()) return;

		int cF = currentFrame ? 1 : 0;
		int pF = currentFrame ? 0 : 1;

		// resize the image
		cv::resize(image,frames[cF],cv::Size(image.cols*scale,image.rows*scale),0,0,cv::INTER_LINEAR);

		// make a copy for the initial frame
		if (frames[pF].empty())
			frames[cF].copyTo(frames[pF]);

		if (!flow.empty())
			flow = cv::Mat::zeros(flow.size(),flow.type());

		// calculate dense optical flow
		cv::calcOpticalFlowFarneback(frames[pF],frames[cF],flow,.5,2,8,3,7,1.5,0);

		cv::Mat outImg = frames[cF].clone();

		drawOptFlowMap(flow,outImg,8,cv::Scalar::all(255));

		cv::imshow("Flow",outImg);

		// flip the buffers
		currentFrame = !currentFrame;
	}
};

int main(int argc,char* argv[])
{
	const char* keys =
		"{ s   | scale  | 1      | scale of video input }"
		"{ v   | video  | 0      | video input }";
	
	cv::CommandLineParser cmd(argc,argv,keys);
	
	std::string videoName = cmd.get<std::string>("v");
	
	cv::Ptr<cv::VideoCapture> capture = new cv::VideoCapture();

	if (videoName.length() == 1) {
		capture->open(::atoi(videoName.c_str()));
	} else {
		capture->open(videoName);
	}
	
	if (!capture->isOpened())
		return -1;
	
	bool done(false);
	
	cv::Mat frame,gray;
	
	DenseOpticalFlow dof(cmd.get<double>("s"));
	
	do {
		if (capture->read(frame))
		{
			cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);		
			dof(gray);			
		} else {
			done = true;
		}
		
		int k = (int)cv::waitKey(1);
		
		if (27 == k)
			done = true;
		
	} while (!done);
	
	
	return 0;
}