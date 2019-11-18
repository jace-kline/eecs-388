#!/usr/bin/env python
from __future__ import division

# Imports
import tensorflow as tf
model = __import__("model")
import cv2
import sys
import os
import time
import math
import numpy as np
import serial
import struct
import local_common as cm

font = cv2.FONT_HERSHEY_SIMPLEX

ser1 = serial.Serial(port='/dev/ttyAMA1', baudrate=115200)
ser2 = serial.Serial(port='/dev/ttyAMA2', baudrate=115200)

# Radian <-> Degree conversion functions
def deg2rad(deg):
        return deg * math.pi / 180.0
def rad2deg(rad):
        return 180.0 * rad / math.pi

#Get and set the number of cores to be used by TensorFlow
if(len(sys.argv) > 1):
        NCPU = int(sys.argv[1])
else:
        NCPU = 1
config = tf.ConfigProto(intra_op_parallelism_threads=NCPU, \
                        inter_op_parallelism_threads=NCPU, \
                        allow_soft_placement=True, \
                        device_count = {'CPU': 1})

#The max number of frames to be processed,
#    and the number of frames already processed
NFRAMES = 1000
curFrame = 0

#Periodic task options
period = 50
is_periodic = True

#Load the model
sess = tf.InteractiveSession(config=config)
saver = tf.train.Saver()
model_load_path = "model/model.ckpt"
saver.restore(sess, model_load_path)

#Create lists for tracking operation timings
cap_time_list = []
prep_time_list = []
pred_time_list = []
tot_time_list = []

print('---------- Processing video for epoch 1 ----------')

#Open the video file
vid_path = '../lab09/l9-dnn/epoch-1.avi'
assert os.path.isfile(vid_path)
frame_count = cm.frame_count(vid_path)
cap = cv2.VideoCapture(vid_path)

#Process the video while recording the operation execution times
print('performing inference...')
first_frame = True
while(1):
        if curFrame < NFRAMES:
                cam_start = time.time()

                #Get the next video frame
                ret, img = cap.read()
                if not ret:
                        break

                #Preprocess the input frame
                img = cv2.resize(img, (200, 66))
                img = img / 255.

                #Feed the frame to the model and get the control output
                rad = model.y.eval(feed_dict={model.x: [img]})[0][0]
                deg = rad2deg(rad)

                #Write the angle to the serial 1 port
                ser1.write(struct.pack("lf", deg, deg))
                print(deg)

                text = 'Angle: {}'.format(float(deg))

                cv2.putText(img, text, (5,50), font, 1.0, (255,255,255))
                cv2.imshow('frame', img)
                cv2.waitKey(0)

                #Don't include the timings for the first frame due to cache warmup
                if first_frame:
                        first_frame = False
                else:
                        tot_time_list.append(tot_time)
                        curFrame += 1

                #Wait for next period
                wait_time = (period - tot_time) / 1000
                if is_periodic and wait_time > 0:
                        time.sleep(wait_time)
        else:
                break

cap.release()

ser1.close()
ser2.close()

print("Finished")
