## The µOS++ startup code for bare-metal platforms

This project provides a portable startup code for bare-metal platforms.

The code starts with `_start`, where the reset handler should jump after setting the stack and possibly other registers required by the architecture API (like GP on RISC-V).

## Developer info

This section is intended to developers that plan to include this library in their own projects.

### Easy install

The source files are available from [GitHub](https://github.com/micro-os-plus/startup):

```bash
$ git clone https://github.com/micro-os-plus/startup.git startup.git
```

The library will shortly be available from the npm registry:

```bash
$ npm install @micro-os-plus/startup
```

### Prerequisites

The source code require a modern C++ compiler, preferably GCC 5 or higher, but was also compiled with GCC 4.8. 

## License

The original content is released under the MIT License, with
all rights reserved to Liviu Ionescu.

