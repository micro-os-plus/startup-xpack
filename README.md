## The µOS++ portable startup code for bare-metal platforms

This project provides a portable startup code for bare-metal platforms.

The myth that startup code must be written in assembly because 'the C environment is not ready' is plainly wrong.

The 'C environment' is mainly the stack (and maybe other architecture ABI registers, like GP on RISC-V), and this can be set in a short assembly entry code and then the standard `_start()` function can be safely called.

## Developer info

This section is intended to developers who plan to include this library in their own projects.

### Easy install

This package can be installed from the `npm` [registry](https://www.npmjs.com/package/@micro-os-plus/startup).

```console
$ xpm install @micro-os-plus/startup
```

This package is also available from [GitHub](https://github.com/micro-os-plus/startup-xpack):

```console
$ git clone https://github.com/micro-os-plus/startup-xpack.git startup-xpack.git
```

### Prerequisites

Installing from `npm` registry requires a recent [Node.js](https://nodejs.org) (>7.x; the 6.x LTS version is not compatible), and the `xpm` tool (https://www.npmjs.com/package/xpm).

```console
$ sudo npm install xpm --global
```

On Windows, global packages are installed in the user home folder, and do not require `sudo`.

Compiling the source code require a modern C++ compiler, preferably GCC 5 or higher, but was also compiled with GCC 4.8. 

### How to publish

* commit all changes
* update `CHANGELOG.md`; commit with a message like _CHANGELOG: prepare v0.1.2_
* `npm version patch`
* push all changes to GitHub
* `npm publish`

## License

The original content is released under the [MIT License](https://opensource.org/licenses/MIT), with all rights reserved to [Liviu Ionescu](https://github.com/ilg-ul).

