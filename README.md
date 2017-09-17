# ndjson2img

Multithreaded c++ program transforms ndjson to images.

## Before you build

Make sure that openCV is installed in your computer.

If you use Visual Studio to build it, please check OpenCV_Debug.props and OpenCV_Release.props. Set AdditionalIncludeDirectories, AdditionalLibraryDirectories and AdditionalDependencies to your opencv directory.

## Where is the data

You can got ndjson files from Google's [Simplified quickdraw_dataset](https://console.cloud.google.com/storage/browser/quickdraw_dataset/full/simplified)

## How to use (For x64 on Windows)

```
ndjson2img.exe [ndjson_path]
```

## Thank to

Open Source Computer Vision Library [opencv](https://github.com/opencv/opencv)

Documentation on how to access and use the Quick, Draw! Dataset [quickdraw-dataset](https://github.com/googlecreativelab/quickdraw-dataset)

A fast JSON parser/generator for C++ with both SAX/DOM style API [rapidjson](https://github.com/Tencent/rapidjson/)
