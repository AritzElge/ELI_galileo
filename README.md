# Embedded Linux Modbus Master Controller

This repository presents an isolated master embedded control system implemented on an Intel Galileo Gen 2 (embedded Linux). The project demonstrates a rigorous engineering approach by developing software ranging from high-level scripts to optimized native C code with inline x86 assembly for efficient and deterministic hardware control.

The goal is to control a Modbus TCP/IP network, prioritizing reliability, deterministic performance, and best practices for embedded systems.

## Technical Features

*   **Master Platform:** Intel Galileo Gen 2 (Quark SoC X1000, Linux).
*   **Languages Used:** Ash shell, Python 3, Native C, Modern C++.
*   **Communication Protocol:** Modbus TCP/IP.
*   **Hardware Interfaces:** SPI (for LCD), GPIO (for multiplexer and LED control), Ethernet (for Modbus communication).
*   **Methodology:** Use of native POSIX APIs, BSD Sockets, `libmodbus`, `pthreads`, and POSIX shared memory (IPC).

## Software Architecture

The system consists of several daemons that communicate internally:

*   `modbus_daemon` (C/C++): Manages network communication with simulated slaves.
*   `lcd_daemon` (C/C++): Controls the SPI display and shows system status.
*   `status_manager` (C/C++): Uses POSIX shared memory (`shm_open`, `mmap`) for efficient IPC between daemons.

## Key Engineering Decisions

*   **Multi-Level Approach:** The same functionality (e.g., I/O control) has been implemented in Bash, Python and C to demonstrate the trade-offs of performance and control at each level of abstraction.
*   **Resource Management (RAII):** Use of modern C++ and the RAII pattern for safe and automatic management of file descriptors and shared memory.
*   **Network Security:** Use of static ARP tables (configurable upon deployment) to mitigate ARP spoofing attacks.

## System Requirements

*   Host PC with Linux (for cross-compilation).
*   Intel Galileo Gen 2 board.
*   Necessary libraries (e.g., `libmodbus`).

## Compilation and Deployment Instructions

There are two deployment methods: for development and for production:

### Method 1: Iterative Development (Rapid Deployment)

This method allows compilation on the host PC and rapid deployment of the binaries to the read/write capable Galileo via wget/SSH for quick testing.

1.  Clone the repository: `git clone github.com`
2.  Configure the Buildroot toolchain (see [docs/BUILDROOT_SETUP.md](docs/BUILDROOT_SETUP.md)).
3.  Compile the project:
    ```bash
    cd src/galileo-master/
    cmake .
    make
    ```
4.  Host a simple web server on your host PC (e.g., using Python: `python3 -m http.server 8000`).
5.  Deploy to the Galileo using `wget`: `wget http://host_pc_ip:8000/daemon_name`
6.  Configure the static ARP table manually (instructions in [docs/ARP_SETUP.md](docs/ARP_SETUP.md)).

### Method 2: Production Deployment (Read-Only Image)

This is the industrial-grade method. Binaries are compiled directly into the final Linux image using Buildroot, and the root filesystem (`rootfs`) is mounted in read-only (`ro`) mode.

1.  Ensure that the project packages are integrated into the Buildroot configuration.
2.  Compile the complete Linux image using Buildroot.
3.  Deploy the generated image to the Galileo's SD card.
4.  The static ARP table configuration is included in the read-only `rootfs` startup scripts during compilation.

## Technical Features (Revised)

*   **Master Platform:** Intel Galileo Gen 2 (Quark SoC X1000, Linux).
*   **Languages Used:** Ash shell, Python 3, Native C, Modern C++.
*   **Communication Protocol:** Modbus TCP/IP.
*   **Hardware Interfaces:** SPI (for LCD), GPIO (for multiplexer and LED control), Ethernet (for Modbus communication).
*   **Methodology:** Use of native POSIX APIs, BSD Sockets, `libmodbus`, `pthreads`, and POSIX shared memory (IPC).
*   **Build System Tool:** **Buildroot** (used to generate the toolchain and the final Linux image).


## Contributions and License

This project is under the [MIT License](LICENSE). Contributions are welcome.
