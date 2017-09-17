# ndjson2img

Multithreaded c++ program transforms ndjson to images

## Before you build

Make sure that openCV is installed in your computer.

If you use Visual Studio to build it, please check OpenCV_Debug.props and OpenCV_Release.props. Set AdditionalIncludeDirectories, AdditionalLibraryDirectories and AdditionalDependencies to your opencv directory.

## How to use (For x64 on Windows)

```
ndjson2img.exe [ndjson_path]
```