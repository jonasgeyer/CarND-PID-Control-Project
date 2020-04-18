# **PID Controller** 

## Writeup Jonas Geyer

### 18th of April, 2020

---

**PID Controller Project**

The goals / steps of this project are the following:
* Write missing C++ code to finish the PID Controller program
* Test with the simulator


[//]: # (Image References)

[image1]: ./Capture_difficult_turn.PNG "Difficult turn"


## Rubric Points
### Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/1972/view) individually and describe how I addressed each point in my implementation.  

---
### Compiling

#### Your code should compile.

My project compiles under "Windows Subsytem for Linux" with Ubuntu and gcc

### Implementation

#### The PID procedure follows what was taught in the lessons.
Yes!

### Reflection

#### Describe the effect each of the P, I, D components had in your implementation.

The P component results in a steering angle proportional to the cross track error (cte).
It makes sure the car doesn't leave the track.

The I component results in a steering angle proportional to the integral of the cte.
It makes sure the car eventually approaches the center line even if there is a bias.

The D component results in a steering angle proportional to the differential of the cte.
It makes sure the car doesn't oversteer too much.

#### Describe how the final hyperparameters were chosen.

I first only optimized P, then added D and finally I.

The car was making very erratic steering movements.
It seemed to be related to the d-error. 
So I added a moving average of the differential error. This helped to make the car move more smoothly.
But I was not able to keep the car on the street at full speed, so I limited the max speed to 20 mph.


### Simulation

#### The vehicle must successfully drive a lap around the track.

It does. But he following curve was difficult, so the car gets very close to the curb:
![alt text][image1]


I'd appreciate any tips how to improve the code so it can handle the track at full speed.