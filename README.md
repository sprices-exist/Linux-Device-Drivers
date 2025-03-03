# Linux-Device-Drivers
Character and Block Device Driver Exxercises in Linux Kernel

## Lab 8 task 4
Implements a simple Linux kernel module that creates a character device for Fibonacci number generation. When a user reads from the device, it returns the current Fibonacci number and updates the sequence. The module doesn't handle writes to the device and provides basic functionality to open and release the device.

## Lab 8 task 6
This kernel module exposes a device and a sysfs entry to interact with a test value, allowing both file-based and sysfs-based read/write operations. The sysfs operations are synchronized to ensure consistent access to the shared test_value.

## Lab 9 task 1
This kernel module creates a simple block device driver that simulates a block device backed by virtual memory and supports read and write operations through a custom request handler. It implements basic device management tasks such as memory allocation, block device registration, and request handling.

## Lab 9 task 2
This kernel module creates two virtual block devices, simulates their storage using memory, and provides custom request handlers to process I/O operations. It also handles errors gracefully, ensuring proper cleanup if anything goes wrong during initialization.
