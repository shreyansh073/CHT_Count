# CHT_Count
Counts the number of circles in an image and geneartes a csv report along with the circles numbered on the input image.

## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites and installation
OpenCV 2.4: You can follow [these instructions](https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html?) for installation.

### Compilation
Copy the following command in your bash terminal for compilation
`g++ -std=c++11 cht.cpp -o output pkg-config --cflags --libs opencv`

## Running the tests
Enter code in the following format:
1) ./output <input_image.jpg>
2) <minimum_radius> <maximum_radius>
3) Adjust the Canny threshold and the Accumulator threshold values
4) Press `q` or `Q` when all circles are detected

### For example
This is the sample input image ![input image](http://tatainnoverse.com/challangeImages/1517393608StrawHead.jpg)

1)`./output sample.jpg`

Enter minimum and maximum radius

2)`20 30`

3)Adjust the threshold values

4)Press `q` or `Q` to quit

5)Output image output.jpg and report.csv is generated.
