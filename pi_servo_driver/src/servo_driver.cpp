#include <iostream>
#include <ros/ros.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "std_msgs/UInt16.h"

using namespace std;

ros::NodeHandle* nh;
int fd;
char buf[32];

struct ServoMsg{
	unsigned servo:6;
	unsigned position:10;
} __attribute((packed));

void servoCallback(const std_msgs::UInt16& msg);

int main(int argc, char** argv){
	ros::init(argc, argv, "pi_servo_driver");
  	nh = new ros::NodeHandle;
	ros::Subscriber sub = nh->subscribe("/servo", 2, servoCallback);
	fd = open("/dev/servoblaster", O_WRONLY);
	if(fd < 0){
		ROS_ERROR("[Pi servo_driver] error opening servoblaster");
		ros::shutdown();
	}
	ros::spin();
  return(0);
}

void servoCallback(const std_msgs::UInt16& msg){
	ServoMsg servoMsg = *(ServoMsg*)&msg.data;
	int servo = servoMsg.servo;
	int position = servoMsg.position;
	sprintf(buf, "%d=%d\n", servo, position);
	int n = strlen(buf);
	write(fd, buf, n);
}

