/*
 * Camera.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: Win7
 */

#include <Camera.h>
#include <iostream>
using namespace std;

Camera::Camera() :
	m_grip(NetworkTable::GetTable("GRIP"))
{

}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

void Camera::CameraPeriodic() {
	int centerX = -99, centerY = -99;
	auto centerXs = m_grip->GetNumberArray("targets/centerx", llvm::ArrayRef<double>());
	auto centerYs = m_grip->GetNumberArray("targets/centery", llvm::ArrayRef<double>());
	centerX = centerXs[0];
	centerY = centerYs[0];
	cout << centerX << ", " << centerY << endl;
}

void AutoTrack(bool) {

}
