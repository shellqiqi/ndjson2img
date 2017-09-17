#include <iostream>
#include <fstream>
#include <cstdio>
#include <thread>
#include <mutex>
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

static ifstream file;
static int count = 0;

mutex fileMutex;

bool getNextLine(char*);
void DrawImgThread();
bool DrawImg();
void MyLine(Mat, Point, Point);
void MyPoint(Mat, Point);

int main(int argc, char *argv[])
{
	unsigned long hardware_threads = std::thread::hardware_concurrency();
#ifdef _DEBUG
	file = ifstream("book.ndjson");
#else
	file = ifstream(argv[1]);
#endif

	cout << "Processing... " << hardware_threads << " threads." << endl;

	clock_t start, finish;
	start = clock();

	thread* threads = new thread[hardware_threads];
	for (size_t i = 0; i < hardware_threads; ++i)
		threads[i] = thread(DrawImgThread);
	for (size_t i = 0; i < hardware_threads; ++i)
		threads[i].join();

	cout << ::count << endl;

	finish = clock();
	cout << finish - start << " ms" << endl;
	
	return 0;
}

bool getNextLine(char* json)
{
	lock_guard<mutex> lock(fileMutex);
	if (file.getline(json, JSON_SIZE))
	{
		::count++;
		if (::count % 10000 == 0)
		{
			cout << ::count << endl;
		}
		return true;
	}
	else
		return false;
}

void DrawImgThread()
{
	while (DrawImg())
	{

	}
	return;
}

bool DrawImg()
{
	Mat image = Mat(Size(WIDTH, WIDTH), CV_8UC3, Scalar::all(255));

	char json[JSON_SIZE];
	if (getNextLine(json) == false)
	{
		return false;
	}

	Document d;
	d.Parse(json);

	Value& word = d["word"];
	Value& countrycode = d["countrycode"];
	Value& key_id = d["key_id"];
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

	String fileName = word.GetString();
	fileName += "-";
	fileName += countrycode.GetString();
	fileName += "-";
	fileName += key_id.GetString();
	fileName += SUFFIX;

	imwrite(fileName, image);
	return true;
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
