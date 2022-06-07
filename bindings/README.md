pyArborX
======
This is a tailored Python binding of the ArborX library. It consists of a narrow subset of functionality and dataclasses from both ArborX and the underlying Kokkos library.
Be advised, that this is work in progress and will not be stable. Multiple major changes are necessary to support the majority of ArborX in python.

Installation
------------
The build instructions here are for Unix systems. Building pyArborX in Windows has not been tested.
All build requirements for [ArborX](https://github.com/arborx/ArborX/wiki/Build) have to be fulfilled.
In case Cuda is used as a backend to run the computations on Nvidia-GPUs `-DKokkos_ENABLE_CUDA_LAMBDA` must be enabeled.
pyArborX additionally requires [pybind11](https://pybind11.readthedocs.io/en/stable/index.html).

The only toolchain that was tested for this build is [cmake](https://cmake.org/). Cmake must be able to find all above mentioned packages.

The bindings are build with Cmake as usual (out of source build). In this process Cmake will generate several of the source files for the bindings in order to allow for easy binding to capabilities of ArborX.
The compiler used to build the bindings should be the one that kokkos recommends depending on the configuration. E.g for using cuda as backend the nvcc-wapper should be used.
````
cmake -DCMAKE_CXX_COMPILER=<path_to_kokkos_install>/bin/nvcc_wrapper
````
#Installation using python pip
After building the bindings (e.g. make), pip can be used to install the bindings into the python environment. As the state of the binding is far from being a full package, only development install is supported.
For this run `pip install --user -e .` in the Cmake build directory.

Examples
--------
pyArborX in its current state is able to run a specific search query. This can be found in the [protein example](https://github.com/JBludau/ArborX/tree/st-jude-bdg/examples/protein).
The example contains both a cpp and a python version of the same query.

Citing pyArborX
-------------
Please notify bludauj(at)ornl.gov

License
-------
The bindings are considered part of the ArborX package. Refer to the top-level license information.
