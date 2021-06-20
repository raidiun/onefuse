## Overall Approach

Using MSGraph SDK in C# called from C.
C# is compiled with AOT to a static library.

## Dependencies

- cmake >= 3.12
- libfuse-dev
- libc6-dev-i386 (Required by libfuse)
- dotnet-5.0

## Building
```sh
mkdir build
cd build
cmake ..
make
```

