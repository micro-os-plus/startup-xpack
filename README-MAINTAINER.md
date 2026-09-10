[![license](https://img.shields.io/github/license/micro-os-plus/startup-xpack)](https://github.com/micro-os-plus/startup-xpack/blob/xpack/LICENSE)
[![CI on Push](https://github.com/micro-os-plus/startup-xpack/actions/workflows/ci.yml/badge.svg)](https://github.com/micro-os-plus/startup-xpack/actions/workflows/ci.yml)
[![GitHub issues](https://img.shields.io/github/issues/micro-os-plus/startup-xpack.svg)](https://github.com/micro-os-plus/startup-xpack/issues/)
[![GitHub pulls](https://img.shields.io/github/issues-pr/micro-os-plus/startup-xpack.svg)](https://github.com/micro-os-plus/startup-xpack/pulls)

# Maintainer info

## Project repository

The project is hosted on GitHub:

- <https://github.com/micro-os-plus/startup-xpack.git>

To clone the stable branch (`xpack`), run the following commands in a
terminal (on Windows use the _Git Bash_ console):

```sh
rm -rf ~/Work/micro-os-plus/startup-xpack.git && \
mkdir -p ~/Work/micro-os-plus && \
git clone \
  https://github.com/micro-os-plus/startup-xpack.git \
  ~/Work/micro-os-plus/startup-xpack.git
```

For development purposes, clone the development branch (`xpack-development`):

```sh
rm -rf ~/Work/micro-os-plus/startup-xpack.git && \
mkdir -p ~/Work/micro-os-plus && \
git clone \
  --branch xpack-development \
  https://github.com/micro-os-plus/startup-xpack.git \
  ~/Work/micro-os-plus/startup-xpack.git
```

## Prerequisites

A recent [xpm](https://xpack.github.io/xpm/), which is a portable
[Node.js](https://nodejs.org/) command line application.

To run the native tests, a C++ development environment is required.
On macOS install Command Line Tools, on Ubuntu `build-essential`.

## Code formatting

Code formatting is done using `clang-format --style=file`, either manually
from a script, or automatically from Visual Studio Code, or the Eclipse
CppStyle plug-in.

Always reformat the source files that were changed.

## How to make new releases

### Release schedule

There are no fixed releases.

### Check Git

In the `micro-os-plus/startup-xpack` Git repo:

- switch to the `xpack-development` branch
- if needed, merge the `xpack` branch

No need to add a tag here, it'll be added when the release is created.

### Increase the version

Determine the upstream version (like `7.1.0`) and eventually update the
`package.json` file; the format is `7.1.0-pre`.

### Fix possible open issues

Check GitHub issues and pull requests:

- <https://github.com/micro-os-plus/startup-xpack/issues/>

and fix them; assign them to a milestone (like `7.1.0`).

### Update `README-MAINTAINER.md`

Update the `README-MAINTAINER.md` file to reflect the changes
related to the new version.

### Update `CHANGELOG.md`

- open the `CHANGELOG.md` file
- check if all previous fixed issues are in
- add a new entry like _\* v7.1.0_
- commit with a message like _prepare v7.1.0_

### Push changes

- reformat the source files that were changed
- commit and push

### Manual tests

To run the tests manually on the local machine:

```sh
xpm run install-all -C ~/Work/micro-os-plus/startup-xpack.git
xpm run test-all -C ~/Work/micro-os-plus/startup-xpack.git
```

## Publish on the npmjs.com server

- select the `xpack-development` branch
- commit all changes
- `npm pack` and check the content of the archive, which should list
  only `package.json`, `README.md`, `LICENSE`, `CHANGELOG.md`,
  the sources and CMake/meson files;
  possibly adjust `.npmignore`
- `npm version 7.1.0`
- push the `xpack-development` branch to GitHub
- the `postversion` npm script should also update tags via `git push origin --tags`
- wait for the CI job to complete
  (<https://github.com/micro-os-plus/startup-xpack/actions/workflows/ci.yml>)

### Test on all platforms

Manually start the **test-all** workflow and wait for it to complete:

- https://github.com/micro-os-plus/startup-xpack/actions/workflows/test-all.yml

### Publish

- `npm login`
- `npm publish --tag test` (use `npm publish --access public` when
  publishing for the first time)

The version is visible at:

- <https://www.npmjs.com/package/@micro-os-plus/startup?activeTab=versions>

## Update the repo

When the package is considered stable:

- with a Git client (VS Code is fine)
- merge `xpack-development` into `xpack`
- push to GitHub
- select `xpack-development`

## Tag the npm package as `latest`

When the release is considered stable, promote it as `latest`:

- `npm dist-tag ls @micro-os-plus/startup`
- `npm dist-tag add @micro-os-plus/startup@7.1.0 latest`
- `npm dist-tag ls @micro-os-plus/startup`
