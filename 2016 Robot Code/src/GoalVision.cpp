///*
// * GoalVision.cpp
// *
// *  Created on: Apr 22, 2016
// *      Author: Win7
// */
//
//#include <GoalVision.h>
//#include <string>
//using namespace std;
//GoalVision::GoalVision()
//{
//	// TODO Auto-generated constructor stub
//
//}
//
//GoalVision::~GoalVision() {
//	// TODO Auto-generated destructor stub
//}
//void GoalVision::Init() {
//	m_camera = new AxisCamera("axis-camera.local");
//	m_frame = imaqCreateImage(IMAQ_IMAGE_RGB, 7);
//	m_binaryFrame = imaqCreateImage(IMAQ_IMAGE_U8, 7);
//	m_shapesImage = imaqCreateImage(IMAQ_IMAGE_U8, 7);
//	m_template= imaqCreateImage(IMAQ_IMAGE_U8, 7);
//	//imaqReadVisionFile(m_template, "//media//sda1//Templates//test_template.png", NULL, NULL);
//	//imaqReadVisionFile(m_frame, "//home//lvuser//Templates//BVC Tower Capture 8.jpg", NULL, NULL);
//	imaqReadVisionFile(m_template, "//home//lvuser//Templates//2016_tower3.png", NULL, NULL);
//	m_hue = {59, 207};
//	m_sat = {101, 255};
//	m_val = {115, 255};
//}
//void GoalVision::SetLine(Point start, Point end) {
//	m_start = start;
//	m_end = end;
//}
//float GoalVision::GetAngleCorrection() {
//	if(!m_camera->IsFreshImage()) {
//		return -1;
//	}
//	static float time = Timer::GetFPGATimestamp();
//	ShapeReport *sr = nullptr;
//	int imaqError = 0;
//
//	static int count = 0;
//	count++;
//
//	m_camera->GetImage(m_frame);
//	time = Timer::GetFPGATimestamp();
//
//	imaqError = imaqColorThreshold(m_binaryFrame, m_frame, 255, IMAQ_HSV, &m_hue, &m_sat, &m_val);
//	if(!imaqError) printf("imaq error: %i\n", imaqError);
//
//	int numMatches = -99;
//	sr = imaqMatchShape(m_shapesImage, m_binaryFrame, m_template, 1, 8 , 0.5, &numMatches);
//	printf("num matches: %i\n", numMatches);
//
////	if(count % 20 == 0) {
////		string filename = "//media//sda1//RoboRIO Saves//shapesImage" + to_string(count) + ".jpg";
////		frcWriteImage(m_shapesImage, filename.data());
////		filename = "//media//sda1//RoboRIO Saves//binaryFrame" + to_string(count) + ".jpg";
////		frcWriteImage(m_binaryFrame, filename.data());
////		printf("done writing to file");
////	}
//	bool targetFound = false;
//	int foundIndex = -1;
//	for(int i = 0; i < numMatches; i++) {
//		float aspectRatio = (float)sr[i].coordinates.width / (float)sr[i].coordinates.height;
//		if(!(fabs(aspectRatio - 1.67) < 0.5) || sr[i].size < 700) {
//			continue;
//		}
//		targetFound = true;
//		foundIndex = i;
//		printf("Centroid x:%i, y:%i, Height: %i Width: %i Size: %i Score:%f\n", sr[i].centroid.x,sr[i].centroid.y,sr[i].coordinates.height,sr[i].coordinates.width,sr[i].size,sr[i].score);
//		printf("Aspect Ratio: %f (Ideally 1.67)\n ", aspectRatio);
//		break;
//	}
//	if(!targetFound){
//		imaqDispose(sr);
//		return -99;
//	}
//	float pixelError = 0;
//	pixelError = (320 - sr[foundIndex].centroid.x);
//	if(sr != nullptr) {
//		imaqDispose(sr);
//	}
//	if(foundIndex == -1 || numMatches == 0){
//		imaqDispose(sr);
//		return 0;
//	}
//
//	return pixelError;
//}
//
////float GoalVision::GetAngleCorrection() {
////	if(!m_camera->IsFreshImage()) {
////		return -1;
////	}
////
////	static float time = Timer::GetFPGATimestamp();
////
////	m_camera->GetImage(m_frame);
////	time = Timer::GetFPGATimestamp();
////	imaqColorThreshold(m_binaryFrame, m_frame, 1, IMAQ_HSV, &m_hue, &m_sat, &m_val);
////	if(m_start.x < 0 || m_start.y < 0 || m_start.x > 640 || m_start.y > 480 || m_end.x < 0 || m_end.y < 0 || m_end.x > 640 || m_end.y > 480) {
////		printf("vertical line out of range\n");
////		return -1;
////	}
////	m_profile = imaqLineProfile(m_binaryFrame, m_start, m_end);
////	if(m_profile == nullptr) {
////		printf("m_profile == nullptr\n");
////		return -1;
////	}
////	unsigned pixelCount = 0;
////	int i = 0;
////
////	for(i = 0; i < m_profile->dataCount && pixelCount < 3; i++) {
////		if(m_profile->profileData[i] == 1) {
////			pixelCount++;
////		} else if(pixelCount > 0) {
////			pixelCount--;
////		}
////	}
////	printf("\n");
////	printf("i: %i, pixelCount %i\n", i, pixelCount);
////	Point p1 = {0, 0}, p2 = {0, 0}; // horizontal line.
////	if(pixelCount == 3) {
////		printf("bottom edge found\n");
////		p1.y = m_start.y + i - 20;
////		p1.x = 255;
////		p2.y = m_start.y + i - 20;
////		p2.x = 455;
////	} else {
////		printf("bottom edge not found\n");
////		return -1;
////	}
////	if(p1.x < 0 || p1.y < 0 || p1.x > 639 || p1.y > 479 || p2.x < 0 || p2.y < 0 || p2.x > 639 || p2.y > 479) {
////		printf("horizontal line out of range\n");
////		return -1;
////	}
////	m_profile = imaqLineProfile(m_binaryFrame, p1, p2);
////
////	int rightDist = 0, leftDist = 0;
////
////	// find right edge
////	pixelCount = 0;
////	i = 0;
////	printf("looking for right edge...\n");
////	for(i = m_profile->dataCount/2; i < m_profile->dataCount && pixelCount < 3; i++) {
////		if(m_profile->profileData[i] == 1) {
////			pixelCount++;
////		} else if(pixelCount > 0) {
////			pixelCount--;
////		}
////	}
////	if(pixelCount == 3) {
////		rightDist = i - m_profile->dataCount/2;
////		printf("right edge found, dist %i\n", rightDist);
////	} else {
////		printf("right edge not found\n");
////	}
////	// find left edge
////	pixelCount = 0;
////	i = 0;
////	printf("looking for left edge...\n");
////	for(i = m_profile->dataCount/2; i > 0 && pixelCount < 3; i--) {
////		if(m_profile->profileData[i] == 1) {
////			pixelCount++;
////		} else if(pixelCount > 0) {
////			pixelCount--;
////		}
////	}
////	if(pixelCount == 3) {
////		leftDist = i - m_profile->dataCount/2;
////		printf("left edge found, dist %i\n", leftDist);
////	} else {
////		printf("left edge not found\n");
////	}
////	printf("angle correction = %f\n", ((rightDist+leftDist)/2)*0.1112);
////	return ((rightDist+leftDist)/2)*0.1112;
////	static unsigned count = 0;
////	count++;
////	if(count % 100 == 0) {
////		printf("profile count: %i\n", m_profile->dataCount);
////		printf("horizontal line: x1 %i, y1 %i, x2 %i, y2 %i", p1.x, p1.y, p2.x, p2.y );
////		printf("time = %f\n", Timer::GetFPGATimestamp() - time);
////	}
////}
