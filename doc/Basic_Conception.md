### Docker

Docker is a tool designed to make it easier to create, deploy, and run applications by using containers. 

Containers allow a developer to package up an application with all of the parts int needs, such as libraries and other dependencies, and ship it all out as on package. 

Docker is a bit like a virtual machine. But unlike a virtual machine, rather than creating a whole virtual operating system, Docker allows applications to use the same Linux kernel as the system that they're running on and only requires applications be shipped with things not already running on the host computer.

Image is a file, comprised of multiple layers, used to execute code in a Docker container. An image is essentially built from the instructions for a complete and executable version of an application, which relies on the host OS kernel.

reference(Korean) - https://subicura.com/2017/01/19/docker-guide-for-beginners-1.html



### Compiling

###### parameters

- ARCH : ISA used for compiling. Affects the GCC being used. Default is `ARCH=x86`if undefined

- PLAT : platform to use when compiling modules with platform-specific code. Set based on the value for ARCH if undefined. Ex: for odroid, `PLAT_DEFAULT_arm=exynos5422`
- CROSS_COMPILE_usr : Which GCC to use for user space applications. Set based on the value for ARCH if undefined
- CROSS_COMPILE_Krn : Which GCC to use for kernel modules. Set based on the value for ARCH if undefined.
- EXTRAFLAGS : Extra GCC options. Set based on the value for ARCH if undefined
- KERNEL_SRC : Path to kernel source used when compiling linux kernel modules. Set based on the value for ARCH if undifined.
- MODULE : Linux module to compile when running 'make exp_module'. No default value.
- UBENCH : Specify a specific target when running 'make ubench'
- DAEMONS : Specify specific daemons to build when running 'make daemons'. If blank all daemons are built. Use, "," to separate multiple values
- KERNEL_SRC_DEFAULT : Specify the kernel source code if cross compiling modules.