///*
// * Leddar.cpp
// *
// *  Created on: Feb 3, 2016
// *      Author: Win7
// */
//
//#include <Leddar.h>
//
//Leddar::Leddar() {
//	m_handle = LeddarCreate();
//	LeddarConnect(m_handle, "AG39005");
//}
//
//Leddar::~Leddar() {
//	LeddarDestroy(m_handle);
//}
//
//LdDetection* Leddar::GetRawDetections() {
//	LeddarGetDetections(m_handle, m_detections, 16);
//	for (int i = 0; i< 16; i++){
//		cout << "segment " << m_detections[i].mSegment
//				<< ":"<<m_detections[i].mDistance
//				<< ", " << m_detections[i].mAmplitude << endl;
//	}
//	//return m_detections;
//}
