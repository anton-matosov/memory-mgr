# A little bit of history

This project has been been started in November of 2007 as a pet project to see if I can build a better memory manager for one of the commercial projects I was working on.
Memory manager was given a chunk of system memory, shared memory (a memory mapped file) to be specific, and it was supposed to provide API for allocating small objects within it.
Since it had to deal with shared memory, there was a big constraint - no raw pointers can be used. The underlying reason for that is because shared memory can be mapped at different base addresses in different processes.
Having to deal with shared memory meant that there should be a cross process synchronization for allocations and deallocations.

Memory manager is a very time critical peace of software, because memory allocations happen all the time and in case of shared memory, delays in memory allocation will halt multiple processes.
Therefore it was essential to make it as fast as possible.

Since I just finished reading "Modern C++ Design: Generic Programming and Design Patterns Applied" by Andrei Alexandrescu, and was excited about template meta-programming I chose to use them extensively in here to move all the computation upfront to compile time.
Do I have to tell you that it was not as of a great idea? It is indeed fun for a pet project, but this library ended up in production code I've been maintaining for years. While it was fast, compilation was terribly slow and all developers had to pay for it.
The next elephant in the room or rather elephants family were error messages. If normal template errors can take many lines, template metaprogram error messages take pages!

Most of the issues with heavy templates use was still possible to overcome. Facade pattern and offers a great way to hide all the details behind the curtain. Since memory manager was a singleton, it was easy to implement a Facade class that was simply forwarding allocation and deallocation to real memory manager instance in cpp file.
The non singleton manager can be still hidden away, with the PImpl idiom. And since library is stable, compilation cost is paid only once and results are cached for a long time in build cache.

# Bringing project back to life

It is 2024 today and I have not touched this project since 2013. The world of C++ and compiler have evolved significantly since the original implementation. Because it was started in 2007, it is based on C++98. Now we have C++11, C++14, C++17, C++20 and C++23 with tons of language and standard library improvements.

Project was used only on Windows and thus heavily relies on MSVC compiler from Visual Studio 2010.
These days the world of C++ is dominated by clang and cross-platform code is a general standard. Back in a days it was not the case. The only platform I've attempted to port this code to was Linux which was done in a VM on my machine. Open source and free CI was not a thing yet and I didn't host my own as I was the only contributor, it worked on my machine, lol.

With all that knowledge about the library history, its time to set the path forward. The goal of bringing project back to life is to practice essential skills of C++ development:
 - be comfortable with legacy codebase
 - working on the cross-platform codebase
 - define the best setup for modern C++ development
 - migrate to modern build systems
 - modernize the codebase without breaking it
 - refactor system to reach the best runtime performance without sacrificing build times

## The first steps

The first step in bringing such a legacy system to life will be to build it. The project used two independent build systems. VS2010 projects for Windows and Automake for Linux. Neither of those is ideal for cross platform project. VS2010 is Windows only and IDE specific, making it impossible to use with other IDEs. While Automake is more cross platform, it requires GNU tools on Windows and is generally unmaintained. Moreover, it is not easy to integrate it with msvc compatible compilers on Windows.

With the goal of bringing the best modern tools to the project CMake is the natural choice. It is  cross platform, supports cross compilation, can generate IDE projects if need to and is well documented and maintained.

Having the build system in our toolbox its time to choose a compiler. I chose Clang without any hesitation. It is the most actively developed compiler, works on all the platforms, has an extensive ecosystem of tools around it (`clangd` - LSP server, `clang-format` - code formatting, `clang-tide` - linter with automatic migration capabilities).

The next thing would be to bring in all the dependencies. Project heavily relies on `Boost` for `type traits` and `mpl` as well as `unit test` libraries. 2013 was the year of `Boost 1.51` so the first natural choice was to dig it out on the internet and use it as is. However it also got stale and is not compatible with modern clang anymore. Therefore a newer version has to be used and since we are modernizing it all, lets go with the latest and greatest `Boost 1.85`

The second dependency is `generic-stl`, another library of mine that was co-developed. And would need to be ported alongside with the current project.

For the IDE I'll stick to my favorite VSCode. It works on all desktop platforms and supports remote development as well as available on Web via GitHub Codespaces and similar products. It is highly extensible and configurable.


