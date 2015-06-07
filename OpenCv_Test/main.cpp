#ifdef _DEBUG
#pragma comment(lib,"opencv_core249d.lib")
#pragma comment(lib,"opencv_imgproc249d.lib")
#pragma comment(lib,"opencv_highgui249d.lib")
#pragma comment(lib,"opencv_video249d.lib")
#pragma comment(lib,"opencv_objdetect249d.lib")
#pragma comment(lib,"opencv_legacy249d.lib")
#pragma comment(lib,"opencv_calib3d249d.lib")
#pragma comment(lib,"opencv_contrib249d.lib")
#pragma comment(lib,"opencv_features2d249d.lib")
#pragma comment(lib,"opencv_flann249d.lib")
#pragma comment(lib,"opencv_ml249d.lib")
#pragma comment(lib,"opencv_gpu249d.lib")
//#pragma comment(lib,"opencv_haartraining_engined.lib")
#else
#pragma comment(lib,"opencv_core249.lib")
#pragma comment(lib,"opencv_imgproc249.lib")
#pragma comment(lib,"opencv_highgui249.lib")
#pragma comment(lib,"opencv_video249.lib")
#pragma comment(lib,"opencv_objdetect249.lib")
#pragma comment(lib,"opencv_legacy249.lib")
#pragma comment(lib,"opencv_calib3d249.lib")
#pragma comment(lib,"opencv_contrib249.lib")
#pragma comment(lib,"opencv_features2d249.lib")
#pragma comment(lib,"opencv_flann249.lib")
#pragma comment(lib,"opencv_ml249.lib")
#pragma comment(lib,"opencv_gpu249.lib")
//#pragma comment(lib,"opencv_haartraining_engine.lib")
#endif


#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

int main(int, char**)
{
	int x = 0, y = 0;

	cv::VideoCapture cap(0); // �f�t�H���g�J�������I�[�v��
	if (!cap.isOpened())  // �����������ǂ������`�F�b�N
		return -1;
	cv::namedWindow("window1", 1);

	cv::Mat img = cv::imread("./euro.jpg");

	// ���ފ�̓ǂݍ���
	std::string cascadeName = "haarcascade_frontalface_default.xml";
	cv::CascadeClassifier cascade;
	if (!cascade.load(cascadeName))
		return -1;

	std::vector<cv::Rect> faces;

	for (;;)
	{
		cv::Mat frame;
		cv::Mat input_image;
		cv::Mat output_image;
		cap >> frame; // �J��������V�����t���[�����擾
		cvtColor(frame, input_image, CV_BGR2GRAY);//�猟�o�̓O���[��ok
		cv::equalizeHist(input_image, input_image);
		output_image = frame;     //output�̓J���[�ŁD

		//(�摜,�o�͒Z�`,�k���X�P�[��,�Œ�Z�`��,�t���O�H,�ŏ��Z�`)
		cascade.detectMultiScale(input_image, faces,
			1.3, 2,
			CV_HAAR_SCALE_IMAGE
			,
			cv::Size(50, 50));

		

		// ���ʂ̕`��
		std::vector<cv::Rect>::const_iterator r = faces.begin();
		for (; r != faces.end(); ++r) {
			cv::Point center;
			int radius;
			center.x = cv::saturate_cast<int>((r->x + r->width*0.5));
			center.y = cv::saturate_cast<int>((r->y + r->height*0.5));
			radius = cv::saturate_cast<int>((r->width + r->height)*0.25);
			//cv::circle(output_image, center, radius, cv::Scalar(80, 80, 255), 3, 8, 0);

			x = center.x - radius;
			y = center.y - radius;
			cv::Mat my_resize(img.rows / img.rows*radius * 2, img.cols / img.cols*radius * 2, img.type());
			cv::resize(img, my_resize, my_resize.size(), cv::INTER_CUBIC);
			cv::Mat Roi(frame, cv::Rect(x, y, my_resize.cols, my_resize.rows));
			my_resize.copyTo(Roi);

			printf("faceID%d,x=%d,y=%d,width=%d,height=%d\n", r, r->x, r->y, r->width, r->height); 
		}

		//���ɂ��邽�ߔ��]������
		flip(output_image, output_image, 1);
		imshow("window1", output_image);

		if (cv::waitKey(30) >= 0) break;

	}
	// VideoCapture �f�X�g���N�^�ɂ��C�J�����͎����I�ɏI����������܂�
	return 0;
}