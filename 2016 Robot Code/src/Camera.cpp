/*
 * Camera.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: Win7
 */

#include <Camera.h>
#include <unistd.h>
#include <iostream>
using namespace std;

Camera::Camera() :
		m_grip(NetworkTable::GetTable("GRIP"))
{
	const char *JAVA = "/usr/local/frc/JRE/bin/java";
	char *GRIP_ARGS[5] =
			{ "java", "-jar", "/home/lvuser/grip.jar",
					"/home/lvuser/project.grip", NULL };

//	if(fork() == 0)
//	{  //Start a new proccess, and exucute grip.
//		if(execv(JAVA, GRIP_ARGS) == -1)
//		{
//			perror("Error running GRIP");
//		}
//	}
}

Camera::~Camera()
{
	// TODO Auto-generated destructor stub
}

void Camera::AutoTrack()
{
//	auto centerXs = m_grip->GetNumberArray("targets/centerX",
//			llvm::ArrayRef<double>());
//	int centerX = centerXs[0]; // assumes only one target found.
//
//	if(centerX < 640 / 2) // assuming camera width is 640 pixels
//		cout << "found target on the left." << endl;
//	else
//		cout << "found target on the right." << endl;
}
