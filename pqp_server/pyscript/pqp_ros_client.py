#!/usr/bin/env python

import rospy
from pqp_server.srv import *

def pqp_client(T, R):
    if len(T) != 3 or len(R) != 9:
        print "Incorrect list size for pqp request"
        return True
    rospy.wait_for_service('pqp_server')
    try:
        pqp_server = rospy.ServiceProxy('pqp_server', pqpRequest)
        result = pqp_server(T, R)
        return result
    except rospy.ServiceException, e:
        print "Service Call Failed: %s"%e

if __name__ == '__main__':
    T = [3., 4., 2]
    R = [1., 0., 0., 0., 1., 0., 0., 0., 1.]
    result = pqp_client(T, R)
    print result
