# Lab 09 Report
<p>Name: <b>Jace Kline</b></p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Title: Deep Neural Network

## Procedure
1. Plug in the Raspberry Pi to the power and the HDMI 
2. Switch the display to the Raspberry Pi display on the monitor and log in
3. Download the source code and unzip it
4. Read the lab instructions
5. View the video file (epoch-1.avi) that is used as training data for the neural network
6. Observe the source code in the dnn.py file
7. Run timing tests on the dnn.py file while varying the number of CPU cores that the neural network utilizes
8. Record the following data for each test (1 <= CPU cores <= 4):
   * Mean time (average)
   * Max time (worst-case)
9. Plot the test data, one graph for each measure above as a function of CPU cores (see attached sheet)


## Conclusion

In this lab, I learned about the concept of deep neural networks. Deep neural networks are a technique for training an AI entity without explicitly writing the code governing the rules of the AI. Deep neural networks work via feeding an array of input data to an 'Input layer' and receiving an array (or value) of output data from the 'Output layer'. Between these two layers, there are many 'Hidden layers' that act as computational transformation units (matrix multiplication) on the preceding layer's data. The data of each layer is piped into the next layer. Using deep neural networks, the network can be "trained" by feeding a large sets of predetermined data (where the output is already known from the input). Each training data set allows the neural network, through back-propogation and adjustment of the hidden layer transformations, to adjust its algorithm.

Additionally, I observed the effects of running the neural network on varying numbers of CPU cores (1-4). Interestingly, the time decrease from each CPU core to the next appears to be of logarithmic relation. In other words... with each additional CPU that is added to the network, the less performance increase is observed. However, there are not enough data points to make a conclusive statement about this claim. Another interesting observation is that the worst case time didn't appear to follow the same relation. Although increasing the number of CPUs tended to decrease the worst case time, the trend did not appear to follow any sort of obvious distribution. 
