/*
 * Camera.h
 *
 *  Created on: Feb 4, 2016
 *      Author: Win7
 */

#ifndef SRC_CAMERA_H_
#define SRC_CAMERA_H_
#include <wpilib.h>

class Camera
{
private:
	std::shared_ptr<NetworkTable> m_grip;
public:
	void AutoTrack();

	Camera();
	virtual ~Camera();
};

#endif /* SRC_CAMERA_H_ */
