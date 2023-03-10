
# FRED-lib and sum-vec

This Elinos App Project is only created to enable debug on the board. Somehow, debug does not work in a Elinos System Project.
If one learn how to debug it in the Elinos System Project, it would be easier since we dont need to manage multiple Elinos projects.

Run `make install` to generate these files

```
app.rootfs/
└── usr
    ├── bin
    │   └── sum-vec
    ├── include
    │   └── fred_lib.h
    └── lib
        ├── libfred.a
        └── libfred.so
```

The same binary files are also copied to `../zcu102_hwvirt_linux.app`, to add them to the Linux image. 
Make sure the project names keep the same to avoid changing the makefiles.

Run `make distclean` to prepare the project to update the repository. 

The fred-lib source code is from this [repo/commit](https://github.com/fred-framework/fred-linux-client-lib/commit/bffd5141f86cd3ed1f2626bac84f1ceadfaf8d0c)
while the example app is from [here](https://github.com/fred-framework/meta-fred/tree/main/recipes-example/sum-vec/files) and [here](https://github.com/fred-framework/meta-fred/commit/bd210b3c1966a9e5509a4cd051be84aa96601aaf) and its [hardware design](https://github.com/fred-framework/dart_ips/tree/master/ips/sum_vec/hw).

In the future, use git submodule instead of copying the code.

Note that fred-lib is compiled for debug by default. If you want to do performance evaluation, change this in the Makefile:
 - DEBUG=YES => DEBUG=NO
 
A. Amory
