/*
 * GoalVision.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: Win7
 */

#include <GoalVision.h>

GoalVision::GoalVision()
{
	// TODO Auto-generated constructor stub

}

GoalVision::~GoalVision() {
	// TODO Auto-generated destructor stub
}
void GoalVision::Init() {
	m_camera = new AxisCamera("axis-camera.local");
	m_frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	m_binaryFrame = imaqCreateImage(IMAQ_IMAGE_U8, 0);
	m_hue = {59, 207};
	m_sat = {101, 255};
	m_val = {115, 255};
}
void GoalVision::SetLine(Point start, Point end) {
	m_start = start;
	m_end = end;
}
float GoalVision::GetAngleCorrection() {
	if(!m_camera->IsFreshImage()) {
		return -1;
	}

	static float time = Timer::GetFPGATimestamp();

	m_camera->GetImage(m_frame);
	time = Timer::GetFPGATimestamp();
	imaqColorThreshold(m_binaryFrame, m_frame, 1, IMAQ_HSV, &m_hue, &m_sat, &m_val);
	if(m_start.x < 0 || m_start.y < 0 || m_start.x > 640 || m_start.y > 480 || m_end.x < 0 || m_end.y < 0 || m_end.x > 640 || m_end.y > 480) {
		printf("vertical line out of range\n");
		return -1;
	}
	m_profile = imaqLineProfile(m_binaryFrame, m_start, m_end);
	if(m_profile == nullptr) {
		printf("m_profile == nullptr\n");
		return -1;
	}
	unsigned pixelCount = 0;
	int i = 0;

	for(i = 0; i < m_profile->dataCount && pixelCount < 3; i++) {
		if(m_profile->profileData[i] == 1) {
			pixelCount++;
		} else if(pixelCount > 0) {
			pixelCount--;
		}
	}
	//printf("\n");
	//printf("i: %i, pixelCount %i\n", i, pixelCount);
	Point p1 = {0, 0}, p2 = {0, 0}; // horizontal line.
	if(pixelCount == 3) {
		//printf("bottom edge found\n");
		p1.y = m_start.y + i - 20;
		p1.x = 255;
		p2.y = m_start.y + i - 20;
		p2.x = 455;
	} else {
		//printf("bottom edge not found\n");
		return -1;
	}
	if(p1.x < 0 || p1.y < 0 || p1.x > 639 || p1.y > 479 || p2.x < 0 || p2.y < 0 || p2.x > 639 || p2.y > 479) {
		//printf("horizontal line out of range\n");
		return -1;
	}
	m_profile = imaqLineProfile(m_binaryFrame, p1, p2);

	int rightDist = 0, leftDist = 0;

	// find right edge
	pixelCount = 0;
	i = 0;
	printf("looking for right edge...\n");
	for(i = m_profile->dataCount/2; i < m_profile->dataCount && pixelCount < 3; i++) {
		if(m_profile->profileData[i] == 1) {
			pixelCount++;
		} else if(pixelCount > 0) {
			pixelCount--;
		}
	}
	if(pixelCount == 3) {
		rightDist = i - m_profile->dataCount/2;
		printf("right edge found, dist %i\n", rightDist);
	} else {
		printf("right edge not found\n");
	}
	// find left edge
	pixelCount = 0;
	i = 0;
	printf("looking for left edge...\n");
	for(i = m_profile->dataCount/2; i > 0 && pixelCount < 3; i--) {
		if(m_profile->profileData[i] == 1) {
			pixelCount++;
		} else if(pixelCount > 0) {
			pixelCount--;
		}
	}
	if(pixelCount == 3) {
		leftDist = i - m_profile->dataCount/2;
		printf("left edge found, dist %i\n", leftDist);
	} else {
		printf("left edge not found\n");
	}
	printf("angle correction = %f\n", ((rightDist+leftDist)/2)*0.1112);
	return ((rightDist+leftDist)/2)*0.1112;
	static unsigned count = 0;
	count++;
	if(count % 100 == 0) {
		printf("profile count: %i\n", m_profile->dataCount);
		printf("horizontal line: x1 %i, y1 %i, x2 %i, y2 %i", p1.x, p1.y, p2.x, p2.y );
		printf("time = %f\n", Timer::GetFPGATimestamp() - time);
	}
}
