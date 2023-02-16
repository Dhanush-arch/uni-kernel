# Unikernel

This is a monolithic operating system kernel written from scratch in Rust which aims to be
compatible with the Linux ABI, that is, it runs Linux binaries without any modifications.

- Implements *NIX process concepts: context switching, signals, `fork(2)`, `execve(2)`, `wait4(2)`, etc.
- Supports commonly used system calls like `write(2)`, `stat(2)`, `mmap(2)`, `pipe(2)`, `poll(2)`, ...
- initramfs is mounted as the root file system.
- Pseudo file systems: tmpfs and devfs.
- [smoltcp](https://github.com/smoltcp-rs/smoltcp)-based TCP/IP support.
- Implements tty and pseudo terminal (pty).
- Supports QEMU and Firecracker (with virtio-net device driver).
- Supports x86_64.
