## The µOS++ portable startup code for bare-metal platforms

This project provides a portable startup code for bare-metal platforms.

The myth that startup code must be written in assembly because 'the C environment is not ready' is plainly wrong.

The 'C environment' is mainly the stack (and maybe other architecture ABI registers, like GP on RISC-V), and this can be set in a short assembly entry code and then the standard `_start()` function can be safely called.

## Developer info

This section is intended to developers who plan to include this library in their own projects.

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

The original content is released under the MIT License, with all rights reserved to Liviu Ionescu.
