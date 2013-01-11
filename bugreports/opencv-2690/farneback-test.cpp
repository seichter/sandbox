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

template <int a, int b, int c, int d>
struct FourCC
{
    static const unsigned int value = (((((d << 8) | c) << 8) | b) << 8) | a;
};

class DenseOpticalFlow
{
	cv::Mat frames[2];
	bool currentFrame;

	cv::Mat flow;

	double scale;
	
	
	cv::VideoWriter output;

public:

	DenseOpticalFlow(double scale_ = 4)
		: scale(scale_)
		, currentFrame(false)
	{
	}

	void operator()(const cv::Mat& image,bool record)
	{
		// no image no fun
		if (image.empty()) return;

		int cF = currentFrame ? 1 : 0;
		int pF = currentFrame ? 0 : 1;

		// resize the image
		cv::resize(image,frames[cF],cv::Size(image.cols*scale,image.rows*scale),0,0,cv::INTER_LINEAR);

		std::cout << "Current: " << cF << " Previous: " << pF << std::endl;
		std::cout << "Image " << image.size() << " c:" << image.channels() << " > " << frames[cF].size() << std::endl;

		if (record && !output.isOpened())
		{
			// fourcc
			int fourCC = FourCC<'X','V','I','D'>::value;
			// set framerate to 1fps - easier to check in a standard video player
			if (output.open("flow.avi",fourCC,25,frames[cF].size(),false))
				{
					std::cout << "capture file opened" << std::endl;
				}		
		}

		// make a copy for the initial frame
		if (frames[pF].empty())
			frames[cF].copyTo(frames[pF]);

		if (!flow.empty())
			flow = cv::Mat::zeros(flow.size(),flow.type());

		// calculate dense optical flow
		cv::calcOpticalFlowFarneback(frames[pF],frames[cF],flow,.5,2,8,3,7,1.5,0);

		// we can't draw into the frame!
		cv::Mat outImg = frames[cF].clone();

		drawOptFlowMap(flow,outImg,8,cv::Scalar::all(255));

		cv::imshow("Flow",outImg);

		// flip the buffers
		currentFrame = !currentFrame;
		
		// record the frame
		if (record && output.isOpened())
			output.write(outImg);
		
	}
};

int main(int argc,char* argv[])
{
	const char* keys =
		"{ s   | scale  | 1      | scale of video input }"
		"{ r   | record | false  | record frames }"
		"{ v   | video  | 0      | video input }";
	
	cv::CommandLineParser cmd(argc,argv,keys);
	
	std::string videoName = cmd.get<std::string>("v");
	bool recordVideo = cmd.get<bool>("r");
	
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
	
	// please comment in and out -- this is causing the problem
	int someRandomNumber(0);
	
	DenseOpticalFlow dof(cmd.get<double>("s"));

	do {
		
		if (capture->read(frame))
		{
			cv::cvtColor(frame,gray,cv::COLOR_BGR2GRAY);		
			dof(gray,recordVideo);			
		} else {
			done = true;
		}
		
		int k = (int)cv::waitKey(1);
		
		if (27 == k)
			done = true;
		
	} while (!done);
	
	
	return 0;
}