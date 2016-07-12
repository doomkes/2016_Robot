/*
 * GoalVision.h
 *
 *  Created on: Apr 22, 2016
 *      Author: Win7
 */

#ifndef SRC_GOALVISION_H_
#define SRC_GOALVISION_H_

#include <WPIlib.h>
class GoalVision {
	Point m_start = {0, 0}, m_end = {0, 0};
	LineProfile *m_profile = nullptr;
	USBCamera m_camera;
	Image *m_frame = nullptr;
	Image *m_binaryFrame = nullptr;
	Image *m_template = nullptr;
	Image *m_shapesImage = nullptr;
	Range m_hue, m_sat, m_val;
public:
	GoalVision();
	virtual ~GoalVision();
	void Init();
	float GetAngleCorrection();
};

#endif /* SRC_GOALVISION_H_ */
