# fred-server

This Elinos App Project is only created to enable debug of fred-server on the board. Somehow, debug does not work in a Elinos System Project.
If one learn how to debug it in the Elinos System Project, it would be easier since we dont need to manage multiple Elinos projects.

Run `make install` to generate these files

```
app.rootfs/
└── usr
    ├── bin
        └── fred-server
```

The same files are also copied to `../zcu102_hwvirt_linux.app`, to add them to the Linux image. 
Make sure the project names keep the same to avoid changing the makefiles.

Run `make distclean` to prepare the project to update the repository. 

The fred-server source code is from [here](https://github.com/fred-framework/fred-linux/commit/dcce87c62b909707bbd3793e45a838cb822d12da).

In the future, use git submodule instead of copying the code.

Note that fred-server is compiled for debug by default. If you want to do performance evaluation, change this in the Makefile:
 - LOG_GLOBAL_LEVEL=LOG_LEV_FULL => LOG_GLOBAL_LEVEL=LOG_LEV_MUTE
 - DEBUG=YES => DEBUG=NO

 A. Amory