#include <iostream>
#include <fstream>
#include <cstdio>
#include <thread>
#include <ctime>
using namespace std;

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
using namespace rapidjson;

#include "opencv2\core.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
using namespace cv;

const unsigned int WIDTH = 256;
const String SUFFIX = ".png";
const unsigned int JSON_SIZE = 8192;

void DrawImg(const string);
void MyLine(Mat, Point, Point);
void MyPoint(Mat, Point);

int main(int argc, char *argv[])
{
	ifstream file(argv[1]);
	char json[JSON_SIZE];

	int count = 0;

	cout << "Processing..." << endl;

	clock_t start, finish;
	start = clock();
	while (file.getline(json, JSON_SIZE))
	{
		count++;
		char* s = new char[JSON_SIZE];
		for (size_t i = 0; i < JSON_SIZE; i++)
		{
			s[i] = json[i];
		}
		thread t(DrawImg, s);
		t.detach();
		cout << count << endl;
	}
	finish = clock();
	cout << finish - start << " ms" << endl;
	
	return 0;
}

void DrawImg(const string json)
{
	Mat image = Mat(Size(WIDTH, WIDTH), CV_8UC3, Scalar::all(255));

	Document d;
	d.Parse(json.c_str());

	Value& id = d["key_id"];
	Value& drawing = d["drawing"];

	for (SizeType i = 0; i < drawing.Size(); i++)
	{
		Value& stroke = drawing[i];
		Value& xList = stroke[0];
		Value& yList = stroke[1];
		if (xList.Size() <= 1)
		{
			MyPoint(image, Point(xList[0].GetInt(), yList[0].GetInt()));
		}
		else
		{
			for (SizeType j = 0; j < xList.Size() - 1; j++)
			{
				Point start = Point(xList[j].GetInt(), yList[j].GetInt());
				Point end = Point(xList[j + 1].GetInt(), yList[j + 1].GetInt());
				MyLine(image, start, end);
			}
		}
	}
	imwrite(id.GetString() + SUFFIX, image);
}

void MyLine(Mat img, Point start, Point end)
{
	int thickness = 2;
	int lineType = LINE_8;
	line(img,
		start,
		end,
		Scalar(0, 0, 0),
		thickness,
		lineType);
}

void MyPoint(Mat img, Point center)
{
	int thickness = 2;
	int lineType = LINE_8;

	circle(img,
		center,
		thickness,
		Scalar(0, 0, 0),
		-1,
		lineType);
}