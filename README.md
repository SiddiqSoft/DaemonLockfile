DaemonLockfile : A simple lockfile implementation for modern C++
-------------------------------------------

[![CodeQL](https://github.com/SiddiqSoft/DaemonLockfile/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/SiddiqSoft/DaemonLockfile/actions/workflows/codeql-analysis.yml)
[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.DaemonLockfile?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=6&branchName=main)
![](https://img.shields.io/nuget/v/SiddiqSoft.DaemonLockfile)
![](https://img.shields.io/github/v/tag/SiddiqSoft/DaemonLockfileandle)
![](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/6)
![](https://img.shields.io/azure-devops/coverage/siddiqsoft/siddiqsoft/6)

# Objective
Provide for daemons (applications running as services under docker as an example) to limit themselves to a single instance per "name".

# Requirements
- C++20
- Useful for running "daemons" under docker where you may not always have or wish for a true service under the operating system.

# Usage

- Use the nuget [SiddiqSoft.DaemonLockfile](https://www.nuget.org/packages/SiddiqSoft.DaemonLockfile/)
- You can also git submodule: `git submodule add https://github.com/SiddiqSoft/DaemonLockfile.git`
- Copy paste..whatever works.

Example (when using nuget to add the header in the solution)

```cpp
#include "siddiqsoft/DaemonLockfile.hpp"


// Example daemon host structure
struct MyDaemonType
{
	siddiqsoft::DaemonLockfile myLock{"MyDaemonType"};
	..
	..
};

// The "daemon" will hold the lock and when the application exits the lock will be released.
static MyDaemonType daemon;


int main(int argc, char *argv[])
{
	// Bailout immediately if we do not have a lock--this implies some other instance is running
	if(daemon.isLocked) return 1;
}

```

<small align="right">

&copy; 2021 Siddiq Software LLC. All rights reserved.

</small>
