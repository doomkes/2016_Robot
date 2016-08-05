/*
 * GoalVision.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: Win7
 */

#include <GoalVision.h>
#include <string>
using namespace std;
GoalVision::GoalVision() :
		m_camera("cam1", false)
{
	// TODO Auto-generated constructor stub

}

GoalVision::~GoalVision() {
	// TODO Auto-generated destructor stub
}
void GoalVision::ShowFrame() {

	if(SmartDashboard::GetBoolean("binary frame", false)) {
		CameraServer::GetInstance()->SetImage(m_binaryFrame);
	} else {
		CameraServer::GetInstance()->SetImage(m_frame);
	}
}
void GoalVision::UpdateSettings() {
	int newExposure = Preferences::GetInstance()->GetInt("cam_exposure", 0);
	int newBrightness = Preferences::GetInstance()->GetInt("cam_brightness", 0);
	static int oldExposure
		= Preferences::GetInstance()->GetInt("cam_exposure", 0);
	static int oldBrightness
		= Preferences::GetInstance()->GetInt("cam_brightness" , 0);
	if(oldExposure != newExposure) {
		m_camera.SetExposureManual(newExposure);
		oldExposure = newExposure;
	}
	if(oldBrightness != newBrightness) {
		m_camera.SetBrightness(newBrightness);
		oldBrightness = newBrightness;
	}
}
 void GoalVision::Init() {
	m_camera.OpenCamera();
	m_camera.StartCapture();
	m_camera.SetSize(640, 480);
	m_camera.SetWhiteBalanceHoldCurrent();

	m_camera.SetExposureManual(Preferences::GetInstance()->GetInt("cam_exposure", 0));
	m_camera.SetBrightness(Preferences::GetInstance()->GetInt("cam_brightness" , 0));

	m_frame = imaqCreateImage(IMAQ_IMAGE_RGB, 7);
	m_binaryFrame = imaqCreateImage(IMAQ_IMAGE_U8, 7);
	m_shapesImage = imaqCreateImage(IMAQ_IMAGE_U8, 7);
	m_template= imaqCreateImage(IMAQ_IMAGE_U8, 7);

	//imaqReadVisionFile(m_template, "//media//sda1//Templates//test_template.png", NULL, NULL);
	//imaqReadVisionFile(m_frame, "//home//lvuser//Templates//BVC Tower Capture 8.jpg", NULL, NULL);
	imaqReadVisionFile(m_template, "//home//lvuser//Templates//2016_tower3.png", NULL, NULL);
	m_hue = {130, 140};
	m_sat = {71, 157};
	m_val = {90, 198};

	if(!Preferences::GetInstance()->ContainsKey("hue_min")) {
		Preferences::GetInstance()->PutInt("hue_min", 0);
		Preferences::GetInstance()->PutInt("hue_max", 255);
		Preferences::GetInstance()->PutInt("sat_min", 0);
		Preferences::GetInstance()->PutInt("sat_max", 255);
		Preferences::GetInstance()->PutInt("val_min", 0);
		Preferences::GetInstance()->PutInt("val_max", 255);
		Preferences::GetInstance()->PutInt("cam_exposure", 50);
		Preferences::GetInstance()->PutInt("cam_brightness", 50);
	}
}

float GoalVision::GetAngleCorrection() {
	m_camera.GetImage(m_frame);


	ShapeReport *sr = nullptr;

	// non-zero if function succeceds, zero if function fails.
	int imaqError = 0;

	m_hue = {Preferences::GetInstance()->GetInt("hue_min"),
			 Preferences::GetInstance()->GetInt("hue_max")};
	m_sat = {Preferences::GetInstance()->GetInt("sat_min"),
			 Preferences::GetInstance()->GetInt("sat_max")};
	m_val = {Preferences::GetInstance()->GetInt("val_min"),
			 Preferences::GetInstance()->GetInt("val_max")};
	imaqError = imaqColorThreshold(m_binaryFrame, m_frame, 255,
					IMAQ_HSV, &m_hue, &m_sat, &m_val);
	if(!imaqError)
		printf("imaq error: %i\n", imaqError);

	int numMatches = -99;
	sr = imaqMatchShape(m_shapesImage, m_binaryFrame, m_template, 1, 8 , 0.5, &numMatches);
	//printf("num matches: %i\n", numMatches);
	if(sr == nullptr) {
		cout << "sr == nullptr" << endl;
		return -99;
	}
	int foundIndex = -1;
	for(int i = 0; i < numMatches; i++) {
		float aspectRatio = (float)sr[i].coordinates.width / (float)sr[i].coordinates.height;
		if(!(fabs(aspectRatio - 1.67) < 0.5) || sr[i].size < 700) {
			continue;
		}

		foundIndex = i;

		printf("found match %i:\n", i);
		printf("\t\t\t• Centroid x:%i, y:%i, Height: %i Width: %i Size: %i Score:%f\n", sr[i].centroid.x,sr[i].centroid.y,sr[i].coordinates.height,sr[i].coordinates.width,sr[i].size,sr[i].score);
		printf("\t\t\t• Aspect Ratio: %f (Ideally 1.66666...)\n ", aspectRatio);
		//break;
	}

	if(sr != nullptr)
		imaqDispose(sr);

	if(foundIndex < 0)
		return -99;

	float pixelError = 0;
	pixelError = (320 - sr[foundIndex].centroid.x);

	return -pixelError*0.0925;
}
