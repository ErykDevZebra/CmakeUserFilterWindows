
<!-- GETTING STARTED -->
## Getting Started
This repository contains ready to use project template for compiling a User Filer for Aurora Vision Studio.
To get a local copy up and running follow these simple steps.

### Prerequisites
Compiling the filter requires MVSC compiler installed and CMake. 
Having [Aurora™ Vision Studio](https://www.adaptive-vision.com/en/user_area/download/) installed is also mandatory for the SDK.

### Usage
1. Clone this repository
   ```bash
   git clone https://github.com/ErykDevZebra/CmakeUserFilterWindows.git
   ```
   
2. Run CMake 
   ```bash
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   ```

3. Run MSBuild Build 
   ```bash
   msbuild ./build/UserFilterLibrary.sln /property:Configuration=Release
   ```
