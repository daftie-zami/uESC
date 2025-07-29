# uESC

**uESC** is an open-source motor ESC (Electronic Speed Controller) firmware project, released under the GNU General Public License (GPL).

This project is currently in the **beta stage** and is intended primarily for development and early testing.

---

## Features

- Open-source, community-driven ESC firmware
- Modular codebase for easy customization
- Currently supports development and building on **Linux** _(esp. Ubuntu)_ systems only
- Simple setup and build process

---

## Getting Started

### Prerequisites

- Linux-based operating system
- Standard build tools (`make`, `arm-none-eabi-gcc`)
- [libopencm3](https://libopencm3.org/) (handled by the setup script)
<!-- - Other dependencies will be installed automatically -->

### Setup

Clone the repository and run the setup script:

```sh
git clone https://github.com/daftie-zami/uESC.git
cd uESC
./tools/env_setup/setup_linux.sh
