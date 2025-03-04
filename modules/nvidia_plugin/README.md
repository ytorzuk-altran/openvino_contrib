
# [OpenVINO™ Toolkit](https://01.org/openvinotoolkit) - NVIDIA GPU plugin

OpenVINO™ NVIDIA GPU plugin is developed in order to enable deep neural networks inference on NVIDIA GPUs, using OpenVINO™ API.
The plugin uses custom kernels and [cuBLAS, cuDNN, cuTENSOR libraries\*] as a backend.

## Supported Platforms
OpenVINO™ NVIDIA GPU plugin is supported and validated on the following platforms: 

OS                     | GPU
---------------------- | ----------------------
Ubuntu* 18.04 (64-bit) | Geforce 1080 Ti, NVIDIA T4

## Distribution
OpenVINO™ NVIDIA GPU plugin is not included into Intel® Distribution of OpenVINO™. To use the plugin, it should be built from source code.

## How to build

### Prerequisites

NVIDIA GPU plugin uses the following dependencies to be downloaded and installed separately. Upon downloading them the user should agree with license of each component:

1. Install one of the following compilers with support of **C++17**:
- Install **gcc-7** compiler
```bash
sudo apt-get update
sudo apt-get install gcc-7 g++7
```
- Install **clang-8** compiler
```bash
sudo apt-get update
sudo apt-get install clang-8 clang++8
```

2. Install **NVIDIA 460** version of driver from [NVIDIA download drivers](http://www.nvidia.com/Download/index.aspx?lang=en-us)
3. Install **CUDA 11.2** from [How to install CUDA](https://docs.nvidia.com/cuda/cuda-quick-start-guide/index.html)
   
   Do not forget to add `<path_to_cuda>/bin/` in **PATH** variable for example `export PATH="<path_to_cuda>/bin:$PATH"`    

4. Install **cuDNN 8.1.0** from [How to install cuDNN](https://docs.nvidia.com/deeplearning/cudnn/install-guide/index.html)
5. Install **cuTENSOR 1.3.0** from [How to install cuTENSOR](https://docs.nvidia.com/cuda/cutensor/getting_started.html#installation-and-compilation)

### Build with cmake

In order to build the plugin, you must prebuild OpenVINO™ package from source using [this guideline](https://github.com/openvinotoolkit/openvino/wiki/BuildingCode#building-for-different-oses).

Afterwards plugin build procedure is as following:

1. Clone `openvino_contrib` repository:
```bash
git clone --recurse-submodules --single-branch --branch=master https://github.com/openvinotoolkit/openvino_contrib.git 
```
2. Go to plugin directory:
```bash
cd openvino_contrib/modules/nvidia_plugin
git checkout develop
```
3. Prepare a build folder:
```bash
mkdir build && cd build
```
4. Build plugin

    First of all, switch OpenVINO™ to tag _2022.1.0_ and then build it according the instruction [How to build](https://github.com/openvinotoolkit/openvino/wiki#how-to-build)

    Then build CUDA Plugin with one of 2 options:
- Using `build.sh`

  Setup the following environment variables:
  ```bash
  export OPENVINO_HOME=<OpenVINO source directory>
  export OPENVINO_CONTRIB=<OpenVINOContrib packages source directory>
  export OPENVINO_BUILD_PATH=<OpenVINO build directory>
  ```
  
  Then run one of the following commands: 
  ```bash
  # Run cmake configuration (if necessary) and then build
  ../build.sh --build
  
  # Run cmake configuration
  ../build.sh --setup
  
  # For old build delete old configuration, generate new one and then build
  ../build.sh --rebuild
  ```
- Using _OpenVINODeveloperPackage_
  
  Run the following command:
  ```bash
  cmake -DOpenVINODeveloperPackage_DIR=<path to OpenVINO package build folder> -DCMAKE_BUILD_TYPE=Release ..
  cmake --build . --target nvidia_gpu -j `nproc`
  ```

### Build with _setup.py_

If python available the CUDA Plugin could be compiled with setup.py script as following:

1. Clone `openvino_contrib` repository:
```bash
git clone --recurse-submodules --single-branch --branch=master https://github.com/openvinotoolkit/openvino_contrib.git 
```
2. Go to plugin directory:
```bash
cd openvino_contrib/modules/nvidia_plugin
git checkout develop
```
3. Setup `CUDACXX` environment variable to point to the CUDA _nvcc_ compiler like the next (use yours path)
```bash
export CUDACXX=/usr/local/cuda-11.2/bin/nvcc
```

4. Add the path to the cuda libraries to the `LD_LIBRARY_PATH` environment variable like the next (use yours path)
```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda-11.2/bin/nvcc
```

5. Run setup.py build command as follows.
```bash
export NVIDIA_PLUGIN_SRC_ROOT_DIR=</path/to/openvino_contrib>/modules/nvidia_plugin
python3 ${NVIDIA_PLUGIN_SRC_ROOT_DIR}/wheel/setup.py build
```
This will automatically download, build OpenVINO and build CUDA Plugin finally. The location of the resulting library file will be like the next.
```
${NVIDIA_PLUGIN_SRC_ROOT_DIR}/build/temp.linux-x86_64-3.6/deps/openvino/bin/intel64/Debug/lib/libopenvino_nvidia_gpu_plugin.so
```

## Install as python package with `setup.py`

To install CUDA Plugin as python package do all steps except last one from the `Build with setup.py` section.
After that installation could be done by running setup.py install command as follows.
```bash
export OPENVINO_CONTRIB=</path/to/openvino_contrib>
python3 ${OPENVINO_CONTRIB}/modules/nvidia_plugin/wheel/setup.py install
```
This command will install dependent openvino package if needed and update it for using with NVIDIA GPU plugin.


## Docker support
### Build docker container
First build docker container:

1. Install `docker`:
```bash
./docker.sh install
su $USER # Relogin for current user
```
2. Download all `*.deb` packages for CUDA and put them in one folder
3. Build docker container:
```bash
CUDA_PACKAGES_PATH=<path to CUDA pakcages> ./docker.sh build
```

### Build openvino_nvidia_gpu_plugin in docker container
In order to build openvino_nvidia_gpu_plugin in docker, follow the steps:

1. Enter the docker container:
```bash
docker run --gpus all -it openvino/cudaplugin-2022.1 bin/bash
```
2. Build the OpenVINO and openvino_nvidia_gpu_plugin according the steps described in [## How to build](#how-to-build),
   except 3), 4), 5) steps (this packages already installed in image)
3. Commit all your changes in container:
```bash
docker commit openvino/cudaplugin-2022.1 <name of new image>
```

## Supported Configuration Parameters
The plugin supports the configuration parameters listed below. All parameters must be set before calling `ov::Core::compile_model()` in order to take effect. When specifying key values as raw strings (that is, when using Python API), omit the `KEY_` prefix.

Parameter name  | Parameter values  | Default  | Description
------------- | ------------- | ------------- | -------------
`NVIDIA_THROUGHPUT_STREAMS`   | `NVIDIA_THROUGHPUT_AUTO`, or non negative integer values  | 1  | Specifies number of CPU "execution" streams for the throughput mode. Upper bound for the number of inference requests that can be executed simultaneously.
`NVIDIA_OPERATION_BENCHMARK`   | `NVIDIA_YES`, `NVIDIA_NO`  | `NVIDIA_NO`  | Specifies if operation level benchmark should be run for increasing performance of network

During compilation of the openvino_nvidia_gpu_plugin, user could specify two options:
1) `-DCUDA_KERNEL_PRINT_LOG=ON` enables print logs from kernels (WARNING, be careful with this options, could print to many logs)
2) `-DENABLE_CUDNN_BACKEND_API` enables cuDNN backend support that could increase performance of convolutions by 20%

## Supported Layers and Limitations
The plugin supports IRv10 and higher. The list of supported layers and its limitations are defined in [cuda_opset.md](docs/cuda_opset.md).

## Supported Model Formats
* FP32 – Supported
* FP16 – Supported and preferred
* U8 - Not supported
* U16 - Not supported
* I8 - Not supported
* I16 - Not supported

## Supported Input Precision
* FP32 - Supported
* FP16 - Supported
* U8 - Not supported
* U16 - Not supported
* I8 - Not supported
* I16 - Not supported

## Supported Output Precision 
* FP32 – Supported
* FP16 - Not supported

## Supported Input Layout
* NCDHW – Not supported
* NCHW - Supported
* NHWC - Supported
* NC - Supported

## License
OpenVINO™ NVIDIA GPU plugin is licensed under [Apache License Version 2.0](LICENSE).
By contributing to the project, you agree to the license and copyright terms therein
and release your contribution under these terms.

## How to Contribute
We welcome community contributions to `openvino_contrib` repository. 
If you have an idea how to improve the modules, please share it with us. 
All guidelines for contributing to the repository can be found [here](../../CONTRIBUTING.md).

---
\* Other names and brands may be claimed as the property of others.

[extra modules flags]:https://github.com/openvinotoolkit/openvino_contrib#how-to-build-openvino-with-extra-modules
[OpenVINO™ samples]:https://docs.openvinotoolkit.org/latest/openvino_docs_IE_DG_Samples_Overview.html
[build it from source]:https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html
[Object Detection for SSD sample]:https://docs.openvinotoolkit.org/latest/openvino_inference_engine_samples_object_detection_sample_ssd_README.html
[Model Optimizer]:https://github.com/openvinotoolkit/openvino/tree/master/model-optimizer
[model downloader]:https://github.com/openvinotoolkit/open_model_zoo/blob/master/tools/downloader/README.md#model-downloader-usage
[model converter]:https://github.com/openvinotoolkit/open_model_zoo/blob/master/tools/downloader/README.md#model-converter-usage
[this image]:https://github.com/openvinotoolkit/openvino/blob/master/scripts/demo/car_1.bmp
[Intermediate Representation]:https://docs.openvinotoolkit.org/latest/openvino_docs_MO_DG_IR_and_opsets.html#intermediate_representation_used_in_openvino
[the guideline]:https://github.com/openvinotoolkit/openvino/wiki/BuildingForRaspbianStretchOS#cross-compilation-using-docker
[this guideline]:https://github.com/openvinotoolkit/open_model_zoo/blob/master/demos/README.md#build-the-demo-applications
