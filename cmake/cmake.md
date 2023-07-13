# CMakeLists.txt

根项目的 CMakeLists.txt 负责处理全局有效的设定。

而子项目的 CMakeLists.txt 则仅考虑该子项目自身的设定，比如他的头文件目录，要链接的库等等。

## 科普：亲 Unix 软件从源码安装的通用套路

```bash
Makefile 构建系统：
./configure --prefix=/usr --with-some-options    # 生成 Makefile（这个 configure 脚本由 Autoconf 生成）
make -j 8                # 8 核心编译，生成 libtest.so
sudo make install   # 安装，拷贝到 /usr/lib/libtest.so

CMake 构建系统：
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DWITH_SOME_OPTIONS=ON  # 生成 Makefile
cmake --build build --parallel 8                  # 8 核心编译，生成 libtest.so
sudo cmake --build build --target install    # 安装，拷贝到 /usr/lib/libtest.so

注：如果 -DCMAKE_INSTALL_PREFIX=/usr/local 则会拷贝到 /usr/local/lib/libtest.so

```

## 如果第三方库发懒，没有提供 Config 文件怎么办？

```
但是，也有少数不听话的库，官方不提供 CMake 支持，即安装时不自带 Config 文件。
恼人的是，这些不听话的库有些竟然是非常热门的库！例如 Python，CUDA，Jemalloc。
为了不影响 CMake 用户体验，CMake 发明了 Find 文件（FindXXX.cmake），你不支持我是吧？我支持你！Find 文件会在 CMake 安装时负责安装到 /usr/share/cmake/Modules。
包搜索文件可以在不知道包具体位置信息的情况下搜索他们（在 /usr/lib 等默认路径搜索）。
这些都是 CMake 自带的包搜索文件：
/usr/share/cmake/Modules/FindCUDAToolkit.cmake
/usr/share/cmake/Modules/FindPython.cmake
那么如果有个不太热门的第三方库没提供包配置文件，CMake 也没提供包搜索文件，我们该如何找到他？这就需要自己提供包搜索文件了！别担心，你不用自己写，GitHub 上有很多志士仁人已经写过了对应的包搜索文件，你搜一下 FindXXX.cmake 就能找到了。

```

**举例：FindJemalloc.cmake**





**注意不论是项目自己的头文件还是外部的系统的头文件，请全部统一采用 <项目名/模块名.h> 的格式。不要用 “模块名.h” 这种相对路径的格式，避免模块名和系统已有头文件名冲突。**

![image-20230712103514686](/home/lyt/2023/cmake/assets/image-20230712103514686.png)



## 一些参数的含义

1. CMAKE_CURRENT_SOURCE_DIR -- 当前CMakeList.txt所在的路径。

2. CMAKE_SOURCE_DIR -- 表示整个项目最外面的CMakeList.txt

3. CMAKE_PREFIX_PATH -- 指定查找外部软件包或库时的前缀路径。 -- Unix 平台默认为 /usr。

   1. ```cmake
      set(CMAKE_PREFIX_PATH "/path/to/library;/another/path/to/library")
      ```

4. 安装库时， --prefix 选项，指定安装的根路径。



## set

```cmake
add_executable(main)
set(source main.cpp other.cpp other.h)
target_sources(main PUBLIC ${sources})
```

- 设置变量

## 构建和链接静态库和动态库 --- add_library

```cmake
add_library(biology STATIC ${srcs})
```

- 创建目标——静态库。库的名称和源码文件名相同，具体代码如下：

  1. ```cmake
     add_library(message
       STATIC
         Message.hpp
         Message.cpp
       )
       #创建hello-world可执行文件的目标部分不需要修改：
       add_executable(hello-world hello-world.cpp)
       #最后，将目标库链接到可执行目标：
       target_link_libraries(hello-world message)
      
      #编译成功后，构建目录包含libmessage.a一个静态库(在GNU/Linux上)和hello-world可执行文件。
     ```

- `add_library(message STATIC Message.hpp Message.cpp)`：生成必要的构建指令，将指定的源码编译到库中。

  - `add_library`的第一个参数是目标名。
  - 整个`CMakeLists.txt`中，可使用相同的名称来引用库。生成的库的实际名称将由CMake通过在前面添加前缀`lib`和适当的扩展名作为后缀来形成。
  - 生成库是根据第二个参数(`STATIC`或`SHARED`)和操作系统确定的。

- `target_link_libraries(hello-world message)`: 将库链接到可执行文件。

  - 此命令还确保`hello-world`可执行文件可以正确地依赖于消息库。
  - 因此，在消息库链接到`hello-world`可执行文件之前，需要完成消息库的构建。

  

  **CMake接受其他值作为`add_library`的第二个参数的有效值**

  - **STATIC**：用于创建静态库，即编译文件的打包存档，以便在链接其他目标时使用，例如：可执行文件。
  - **SHARED**：用于创建动态库，即可以动态链接，并在运行时加载的库。可以在`CMakeLists.txt`中使用`add_library(message SHARED Message.hpp Message.cpp)`从静态库切换到动态共享对象(DSO)。
  - **OBJECT**：可将给定`add_library`的列表中的源码编译到目标文件，不将它们归档到静态库中，也不能将它们链接到共享对象中。如果需要一次性创建静态库和动态库，那么使用对象库尤其有用。我们将在本示例中演示。
  - **MODULE**：又为DSO组。与`SHARED`库不同，它们不链接到项目中的任何目标，不过可以进行动态加载。该参数可以用于构建运行时插件。

## flie

通过 **GLOB_RECRUSE** 为它批量添加了所有位于 src 和 include 下源码和头文件。

![image-20230712103409952](/home/lyt/2023/cmake/assets/image-20230712103409952.png)

```cmake
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(biology STATIC ${srcs})
target_include_directories(biology PUBLIC include)
#因为子项目的 CMakeLists.txt 里指定的路径都是相对路径，所以这里指定 src 实际上是：根/biology/src。
```

这里我们给 biology 批量添加了 src/*.cpp 下的全部源码文件。

明明只有 *.cpp 需要编译，为什么还添加了 include/*.h？为了头文件也能被纳入 VS 的项目资源浏览器，方便编辑。

**GLOB 和 GLOB_RECRUSE 的区别**

```
file (GLOB myvar CONFIGURE_DEPENDS src/*.cpp)
file (GLOB_RECURSE myvar CONFIGURE_DEPENDS src/*.cpp)

疑问1：都是按照通配符批量匹配文件，有什么区别？
GLOB：		src/main.cpp（√）	src/test/main.cpp（×）
GLOB_RECURSE：	src/main.cpp（√）	src/test/main.cpp（√）
区别在于 GLOB_RECURSE 允许 * 匹配嵌套的目录。

疑问2：加了 CONFIGURE_DEPENDS 这个选项有什么区别？
如果不加，在你创建新文件时，myvar 不会自动更新，还是旧的那几个文件，可能出现 undefined symbol，需要重新运行 cmake -B build 才能更新。
加了，则每次 cmake --build 时自动检测目录是否更新，如果目录有新文件了，CMake 会自动帮你重新运行 cmake -B build 更新 myvar 变量。

```



## include

- include 和 add_subdirectory 的区别
  - include 相当于直接把代码粘贴过去，直接访问调用者的作用域。  -- 比如  include 中含有CMAKE_CURRENT_SOURCE_DIR时，此路径为调用include的文件的路径，而不是include内部的文件路径。
  - include命令是在当前CMake脚本中执行，直接将指定文件的内容插入到该位置，并继续执行后续命令；而add_subdirectory命令则进入指定目录，并执行该目录下的CMakeLists.txt文件中定义的内容。
  - include用于导入其他脚本的内容，而add_subdirectory用于处理其他目录中的项目的构建过程。

## list

## add_subdirectory

```cmake
add_subdirectory(pybmain)
add_subdirectory(biology)
```

通过 add_subdirectory 把两个子项目 pybmain 和 biology 添加进来（顺序无关紧要），这会调用 pybmain/CMakeLists.txt 和 biology/CMakeLists.txt。



## add_executable

用于创建一个可执行文件目标。

```cmake
add_executable(target_name [WIN32] [MACOSX_BUNDLE]
    source1 source2 ...
)
# target_name是要创建的可执行文件目标的名称，可以根据需要自定义。
# WIN32和MACOSX_BUNDLE是可选的参数，用于指定目标的属性。
# source1、source2等是构成可执行文件的源文件（源代码文件）。
```



例：

```cmake
add_executable(my_executable main.cpp helper.cpp)
# add_executable命令创建了一个名为my_executable的可执行文件目标，
# 该目标由main.cpp和helper.cpp这两个源文件构成。CMake会自动根据这些源文件生成对应的编译规则，并生成可执行文件。
```

- 可以根据需要在`add_executable`命令中指定不同的源文件，并可以在同一个CMakeLists.txt文件中多次使用`add_executable`命令创建多个可执行文件目标。
- 使用add_executable命令后，您可以使用其他命令（如target_link_libraries、target_include_directories等）为目标添加依赖库、指定包含目录等其他属性和操作。

## find_package

**用于在CMake项目中查找和加载外部软件包或模块。**

```cmake
find_package(<PackageName> [version] [EXACT] [QUIET] [CONFIG] [MODULE]
             [REQUIRED] [[COMPONENTS] [components...]]
             [OPTIONAL_COMPONENTS components...]
)



# package_name是要查找的软件包或模块的名称，如Boost、OpenCV等。
# version是可选的参数，用于指定软件包的版本号。
# EXACT关键字可以用于精确匹配指定版本的软件包。
# QUIET关键字可以用于禁止输出查找过程中的消息。
# MODULE关键字可以用于查找CMake模块。
# REQUIRED关键字可以用于指定软件包是必需的，如果找不到，将报错并停止构建。
```

例:

```cmake
find_package(OpenCV)
# 查找名为 OpenCV 的包，找不到不报错，事后可以通过 ${OpenCV_FOUND} 查询是否找到。
find_package(OpenCV QUIET)
# 查找名为 OpenCV 的包，找不到不报错，也不打印任何信息。
find_package(OpenCV REQUIRED)    # 最常见用法
# 查找名为 OpenCV 的包，找不到就报错（并终止 cmake 进程，不再继续往下执行）。
find_package(OpenCV REQUIRED COMPONENTS core videoio)
# 查找名为 OpenCV 的包，找不到就报错，且必须具有 OpenCV::core 和 OpenCV::videoio 这两个组件，如果没有这两个组件也会报错。
find_package(OpenCV REQUIRED OPTIONAL_COMPONENTS core videoio)
# 查找名为 OpenCV 的包，找不到就报错，可具有 OpenCV::core 和 OpenCV::videoio 这两个组件，没有这两组件不会报错，通过 ${OpenCV_core_FOUND} 查询是否找到 core 组件。

```

### find_package 说是找“包”，到底是在找什么？

```
find_package(OpenCV) 实际上是在找一个名为 OpenCVConfig.cmake 的文件。

注：出于历史兼容性考虑，除了 OpenCVConfig.cmake 以外 OpenCV-config.cmake 这个文件名也会被 CMake 识别到。

同理，find_package(Qt5) 则是会去找名为 Qt5Config.cmake 的文件。
这些形如 包名 + Config.cmake 的文件，我称之为包配置文件。
Qt5Config.cmake 是你安装 Qt5 时，随 libQt5Core.so 等实际的库文件，一起装到你的系统中去的。以我的 Arch Linux 系统为例：
包配置文件位于 /usr/lib/cmake/Qt5/Qt5Config.cmake。
实际的动态库文件位于 /usr/lib/libQt5Core.so。

```

- 因此 find_package 并不是直接去找具体的动态库文件和头文件（例如  libQt5Core.so）。
- 而是去找包配置文件（例如Qt5Config.cmake），这个配置文件里包含了包的具体信息，包括动态库文件的位置，头文件的目录，链接时需要开启的编译选项等等。而且某些库都具有多个子动态库，例如 Qt 就有 libQt5Core.so、libQt5Widgets.so、libQt5Network.so。因此 CMake 要求所有第三方库作者统一包装成一个 Qt5Config.cmake 文件包含所有相关信息（类似于 nodejs 的 package.json），比你单独的一个个去找动态库文件要灵活的多。
- 包配置文件由第三方库的作者（Qt的开发团队）提供，在这个库安装时（Qt的安装程序或apt install等）会自动放到 /usr/lib/cmake/XXX/XXXConfig.cmake 这个路径（其中XXX是包名），供 CMake 用户找到并了解该包的具体信息。
- /usr/lib/cmake 这个位置是 CMake 和第三方库作者约定俗成的，由第三方库的安装程序负责把包配置文件放到这里。如果第三方库的作者比较懒，没提供 CMake 支持（由安装程序提供XXXConfig.cmake），那么得用另外的一套方法（FindXXX.cmake），稍后细谈。

### Windows 系统下的搜索路径

```
<prefix>/
<prefix>/cmake/
<prefix>/<name>*/
<prefix>/<name>*/cmake/
<prefix>/<name>*/(lib/<arch>|lib*|share)/cmake/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/cmake/

其中 <prefix> 是变量 ${CMAKE_PREFIX_PATH}，Windows 平台默认为 C:/Program Files。
<name> 是你在 find_package(<name> REQUIRED) 命令中指定的包名。
<arch> 是系统的架构名。
```

### Unix 类系统下的搜索路径

```cmake
<prefix>/(lib/<arch>|lib*|share)/cmake/<name>*/
<prefix>/(lib/<arch>|lib*|share)/<name>*/
<prefix>/(lib/<arch>|lib*|share)/<name>*/cmake/
<prefix>/<name>*/(lib/<arch>|lib*|share)/cmake/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/
<prefix>/<name>*/(lib/<arch>|lib*|share)/<name>*/cmake/

其中 <prefix> 是变量 ${CMAKE_PREFIX_PATH}，Unix 平台默认为 /usr。
<name> 是你在 find_package(<name> REQUIRED) 命令中指定的包名。
<arch> 是系统的架构，例如 x86_64-linux-gnu 或 i386-linux-gnu。
（用于伺候 Ubuntu 喜欢把库文件套娃在 /usr/lib/x86_64-linux-gnu 目录下）

```

### 举例说明 find_package 搜索路径

```c
// 例如你是 64 位的 Linux 系统，find_package(Qt5 REQUIRED) 会依次搜索：
/usr/lib/cmake/Qt5/Qt5Config.cmake
/usr/lib/x86_64-linux-gnu/cmake/Qt5/Qt5Config.cmake
/usr/share/cmake/Qt5/Qt5Config.cmake
/usr/lib/Qt5/Qt5Config.cmake
/usr/lib/x86_64-linux-gnu/Qt5/Qt5Config.cmake
/usr/share/Qt5/Qt5Config.cmake
/usr/Qt5/lib/cmake/Qt5/Qt5Config.cmake
/usr/Qt5/lib/x86_64-linux-gnu/cmake/Qt5/Qt5Config.cmake
/usr/Qt5/share/cmake/Qt5/Qt5Config.cmake
/usr/Qt5/lib/Qt5/Qt5Config.cmake
/usr/Qt5/lib/x86_64-linux-gnu/Qt5/Qt5Config.cmake
/usr/Qt5/share/Qt5/Qt5Config.cmake


```

```c
// 例如你是 64 位的 Windows 系统，find_package(Qt5 REQUIRED) 会依次搜索：
C:/Program Files/Qt5Config.cmake
C:/Program Files/cmake/Qt5Config.cmake
C:/Program Files/Qt5/Qt5Config.cmake
C:/Program Files/Qt5/cmake/Qt5Config.cmake
C:/Program Files/Qt5/lib/cmake/Qt5/Qt5Config.cmake
C:/Program Files/Qt5/lib/x86_64-windows-gnu/cmake/Qt5/Qt5Config.cmake
C:/Program Files/Qt5/share/cmake/Qt5/Qt5Config.cmake
C:/Program Files/Qt5/lib/Qt5/Qt5Config.cmake
C:/Program Files/Qt5/lib/x86_64-windows-gnu/Qt5/Qt5Config.cmake
C:/Program Files/Qt5/share/Qt5/Qt5Config.cmake


```

```c 
//还有一点，<name> 可以有额外后缀，且不分大小写（无论 Linux 还是 Windows），例如：
C:/Program Files/Qt5/cmake/Qt5Config.cmake
C:/Program Files/Qt5.12.1/cmake/Qt5Config.cmake
C:/Program Files/qt5dnmd/cmake/Qt5Config.cmake
同样都是可以被 find_package(Qt5 REQUIRED) 搜索到的。
/usr/lib/cmake/OpenCV/OpenCVConfig.cmake
/usr/lib/cmake/opencv4/OpenCVConfig.cmake
同样都是可以被 find_package(OpenCV REQUIRED) 搜索到的。

```

### 安装在非标准路径的库

```cmake
# 以 Qt5 为例，如果你安装在下列标准路径，find_package 能够自动找到。
Windows：C:/Program Files/Qt5.12.1/lib/cmake/Qt5/Qt5Config.cmake。
Linux：/usr/lib/cmake/Qt5/Qt5Config.cmake。
但是假如我的库不是装在这些标准路径，而是我自定义的路径，怎么办？
而且即使你不自定义安装路径，Windows 版的 Qt 默认安装就会安装到：
C:/Qt5.12.1/msvc2017_64/lib/cmake/Qt5/Qt5Config.cmake。
何况我们同学有的还喜欢装到 D 盘去，Windows 是非标准路径的重灾区，他就没有一个统一的 /usr/lib 目录。然而你一旦把库安装到非标准路径，find_package 是找不到的。
这时你需要手动指定一个变量告诉他在哪儿，可以是普通变量 ${Qt5_DIR}，也可以是环境变量 $ENV{Qt5_DIR}，两个中只要设置了任何一个 find_package 都可以识别到。
变量一般通过命令行 -DQt5_DIR=”C:/Program Files/Qt5.12.1/lib/cmake/Qt5” 设置。

```

#### 举例，Windows 系统，Qt5

```cmake
例如我把 Qt5 安装到了 D:/Qt5.12.1。
首先找到他里面的 Qt5Config.cmake 文件所在位置（可以用文件管理器的“搜索”功能）。
假如你找到该文件的位置是 D:/Qt5.12.1/msvc2017/lib/cmake/Qt5/Qt5Config.cmake，那么请你设置变量 Qt5_DIR 为 D:/Qt5.12.1/msvc2017/lib/cmake/Qt5。有三种设置方法：

(1) 单次有效。在 configure 阶段，可以从命令行设置（注意要加引号）：
cmake -B build -DQt5_DIR=”D:/Qt5.12.1/msvc2017/lib/cmake/Qt5”
(2) 全局启用。右键“我的电脑”->“管理”->“高级”添加一个环境变量 Qt5_DIR 值为 D:/Qt5.12.1/msvc2017/lib/cmake/Qt5，然后重启 Visual Studio。这样以后你每次构建任何项目，find_package 都能自动找到这个路径的 Qt5 包了。
(3) 单项目有效。直接在你自己项目的 CMakeLists.txt 最开头写一行（注意要加引号）：
set(Qt5_DIR ”D:/Qt5.12.1/msvc2017/lib/cmake/Qt5”)    # 一定要加在最前面！


```

#### 举例，Linux 系统，Qt5

```cmake
例如我把 Qt5 安装到了 /opt/Qt5.12.1。
首先找到他里面的 Qt5Config.cmake 文件所在位置（可以用文件管理器的“搜索”功能）。
假如你找到该文件的位置是 /opt/Qt5.12.1/lib/cmake/Qt5/Qt5Config.cmake，那么请你设置变量 Qt5_DIR 为 /opt/Qt5.12.1/lib/cmake/Qt5。有三种设置方法：
(1) 单次有效。在 configure 阶段，可以从命令行设置：
cmake -B build -DQt5_DIR=”/opt/Qt5.12.1/lib/cmake/Qt5”
(2) 全局启用。修改你的 ~/.bashrc 文件添加环境变量：
export Qt5_DIR=”/opt/Qt5.12.1/lib/cmake/Qt5”，然后重启终端。这样以后你每次构建任何项目，find_package 都能自动找到这个路径的 Qt5 包了。
(3) 单项目有效。直接在你自己项目的 CMakeLists.txt 最开头写一行：
set(Qt5_DIR ”/opt/Qt5.12.1/lib/cmake/Qt5”)    # 一定要加在最前面！


```

#### 三种方案利弊分析

```cmake
单次有效（通过命令行）最安全，小彭老师高度推荐。

全局有效（添加环境变量）可能影响以后其他项目。比如你 A 项目依赖 Qt5.12.1，你设置了环境变量 Qt5_DIR=/opt/Qt5.12.1，后来又搞了个 B 项目依赖 Qt5.10.3，但是你忘了你设置过全局的环境变量指向 5.12.1 了，导致版本冲突。

单项目有效（写死在 CMakeLists.txt）虽然方便了你，但是你的 CMakeLists.txt 拿到别人电脑上（例如你通过 GitHub 开源的），可能你 set(Qt5_DIR D:/Qt5)，而人家却需要 set(Qt5_DIR E:/Qt5) 呢？就冲突了。

所以“单次有效”虽然劳驾您的高抬贵手每次命令行打一下 -DQt5_DIR=”D:/Qt5”，但人家也打一下 -DQt5_DIR=”E:/Qt5”，就没有冲突，各美其美，美美与共，赋能多元化社会，下沉团队合作发力面。

实际上只要你不删 build，不需要每次都 -DQt5_DIR 一下，CMake 具有“记忆”功能。
cmake -B build -DQt5_DIR=D:/Qt5   # 只需要第一次指定好，
cmake -B build                                  # 以后第二次运行可以省略！
rm -rf build                                         # 只有清理了 build 以后，
cmake -B build -DQt5_DIR=D:/Qt5   # 才需要重新指定。

```

### find_package 的两种模式 ---指定使用哪种模式

```cmake
find_package(TBB MODULE REQUIRED)
# 只会寻找 FindTBB.cmake，搜索路径：
${CMAKE_MODULE_PATH}（默认为 /usr/share/cmake/Modules）

find_package(TBB CONFIG REQUIRED)
# 只会寻找 TBBConfig.cmake，搜索路径：
${CMAKE_PREFIX_PATH}/lib/cmake/TBB（默认为 /usr/lib/cmake/TBB）
${TBB_DIR} 或 $ENV{TBB_DIR}

find_package(TBB REQUIRED)
# 不指定则两者都会尝试，先尝试 FindTBB.cmake，再尝试 TBBConfig.cmake。

```

### find_package 命令指定版本

```cmake
find_package(OpenCV REQUIRED)
# 查找名为 OpenCV 的包，不限版本，事后可以通过 ${OpenCV_VERSION} 查询找到的版本。
find_package(OpenCV 2.0.1 REQUIRED)
# 查找版本在 2.0.1 以上的 OpenCV 包（version >= 2.0.1）。
find_package(OpenCV 2.0.1 EXACT REQUIRED)
# 查找版本刚好为 2.0.1 的 OpenCV 包（version == 2.0.1）。

# 如果没写全，则没写的部分默认为 0。例如下列三者等价：
find_package(OpenCV 2 REQUIRED)
find_package(OpenCV 2.0 REQUIRED)
find_package(OpenCV 2.0.0 REQUIRED)
```

### 总结

```bash
# 安装 TBB：
cd tbb
./configure --prefix=/opt/tbbinstalldir
make -j 8
sudo make install

# 在你的项目里使用 TBB：
cd yourapp
cmake -B build -DTBB_DIR=/opt/tbbinstalldir/lib/cmake/TBB
cmake --build build --parallel 8

```

```cmake
# CMakeLists.txt 这样写：

project(yourapp)
add_executable(yourapp yourmain.cpp)
find_package(TBB CONFIG REQUIRED COMPONENTS tbb)
target_link_libraries(yourapp PUBLIC TBB::tbb)
```



## include_directories

- **target_include_directories  和 include_directories 区别**
- target_include_directories命令：
  - target_include_directories命令是为指定的目标（target）添加包含目录。
  - 通过target_include_directories命令，可以为特定目标（如可执行文件、库或自定义目标）指定包含目录，以便在构建目标时能够找到所需的头文件。
  - 该命令允许为不同的目标设置不同的包含目录，并且可以针对不同的可见性级别（INTERFACE、PUBLIC、PRIVATE）指定包含目录。
  - target_include_directories命令通常与target_link_libraries一起使用，以确保链接的库能够找到其所需的头文件。
- `include_directories`命令：
  - `include_directories`命令用于向整个CMake项目添加全局的包含目录。
  - 通过`include_directories`命令，可以为整个项目指定公共的包含目录，以便在构建过程中所有目标都能够找到所需的头文件。
  - 该命令将包含目录应用于整个项目，对所有目标都具有相同的影响。
  - `include_directories`命令通常位于CMakeLists.txt文件的顶层，并在添加目标之前调用。
- 总结
  - target_include_directories命令用于为特定目标添加包含目录，允许为不同目标设置不同的包含目录，并支持可见性级别。
  - include_directories命令用于为整个项目添加全局的包含目录，将包含目录应用于整个项目的所有目标。



## target_include_directories

为指定的目标（target）添加包含目录。

```cmake
target_include_directories(target_name [SYSTEM] [BEFORE]
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
    

SET (EXEC_PREFIX "szy.")
target_include_directories(${EXEC_PREFIX}log_service PRIVATE ${Protobuf_INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS} ${Qt5Sql_INCLUDE_DIRS})

#${EXEC_PREFIX} -- 作用就是字符拼接
```

- target_name是目标的名称，可以是可执行文件、库或自定义目标。
- [SYSTEM]是可选参数，用于指定包含目录是否为系统目录。
- [BEFORE]也是可选参数，用于指定是否将新的包含目录添加到已有的目录之前。

例：

```cmake
add_executable(my_executable main.cpp)
target_include_directories(my_executable
    PRIVATE include/
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common/
    INTERFACE ${Boost_INCLUDE_DIRS})
# target_include_directories命令将include/目录添加为my_executable目标的私有包含目录，
# ${CMAKE_CURRENT_SOURCE_DIR}/common/添加为公共包含目录，
# 以及${Boost_INCLUDE_DIRS}（Boost库的包含目录）添加为接口包含目录。这样，在构建my_executable时，它将能够访问这些包含目录中的头文件。
```



## link_libraries

用于指定目标（target）链接的库。

```cmake
link_libraries(library1 library2 ...)
# 其中，library1、library2等是要链接的库的名称或路径。
```

**link_libraries命令会将指定的库链接到所有后续的目标中，包括在该命令之后创建的目标。**

**这意味着，在调用link_libraries命令后，所有接下来的add_executable、add_library或自定义目标命令都会自动链接指定的库。**

例：

```cmake
link_libraries(my_library)
add_executable(my_executable main.cpp)
# link_libraries命令将名为my_library的库链接到所有后续的目标中。然后，
# add_executable命令创建了一个名为my_executable的可执行文件，并自动链接了my_library库。
```

**请注意，`link_libraries`命令对于多目标项目可能不够灵活，因为它将指定的库链接到所有目标中，而您可能希望只将特定的库链接到特定的目标。**

**在这种情况下，建议使用`target_link_libraries`命令，它允许更精确地指定要链接的库和目标。**

## target_link_libraries

为指定的目标（target）添加链接的库。

```cmake
target_link_libraries(target_name
    <PRIVATE|PUBLIC|INTERFACE> item1 item2 ...
)
# target_name是目标的名称，可以是可执行文件、库或自定义目标。
# PRIVATE、PUBLIC、INTERFACE关键字用于指定后续要链接的库的可见性和属性。

# PRIVATE：链接的库只对目标本身可见。
# PUBLIC：链接的库对目标本身和依赖于该目标的其他目标可见。
# INTERFACE：链接的库不对目标本身可见，但会传递给依赖于该目标的其他目标。
```

例：

```cmake
add_executable(my_executable main.cpp)
target_link_libraries(my_executable
    PRIVATE library1
    PUBLIC library2
    INTERFACE library3
)
# target_link_libraries命令将library1库链接为my_executable目标的私有库，
# 将library2库链接为公共库，
# 将	library3库链接为接口库。这样，在构建my_executable时，它将链接这些库，并与其他目标共享或传递相应的链接库。
```

### INTERFACE

假设我们有目标A和目标B。目标A编译成可执行文件，是我们最终要运行的目标。而目标B则编译成目标A的一个依赖，比如说，静态库。

PUBLIC的意思就是 目标B的属性 不仅自己使用，还传递给依赖它的目标A。

PRIVATE的意思就是 目标B的属性 不会传递，只给目标B自己使用。

而INTERFACE则极为特殊：它的属性都 不会自己使用，只传递给目标A。

INTERFACE就是纯粹的利他主义，我自己不用，但我甘于奉献，让别人用。INTERFACE只做个纯粹的“接口”。这类似于电话接线员。接线员不能听到任何信息，他们只是把信息转发给别人。

```cmake
add_library(Eigen INTERFACE)

target_sources(Eigen INTERFACE
  FILE_SET HEADERS
    BASE_DIRS src
    FILES src/eigen.h src/vector.h src/matrix.h
)

add_executable(exe1 exe1.cpp)
target_link_libraries(exe1 Eigen)

```

这里 Eigen库是个header-only的库。

首先我们声明它的可见性为INTERFACE。这表示它自己不会使用自己的任何属性。它只是个接线员。（注意：源文件也可以看做是一种"属性"，因此，Eigen这个库的所有源文件和头文件都不会被它自己使用，所以它不会编译出任何东西）

使用target_souces来指定它的源文件。

最后两行，我们编译出exe1这个可执行目标，然后把Eigen链接到exe1上。

exe1由于引用了Eigen，所以Eigen的所有属性都传递到exe1身上。也就是说，Eigen的所有源码都作为了exe1的源码。


## target_sources

将源文件（源代码文件）添加到指定的目标。

```cmake
target_sources(target_name
    PRIVATE|PUBLIC|INTERFACE
    [item1 [item2 ...]]
)
```

**target_name是目标的名称，可以是可执行文件、库或自定义目标。**PRIVATE、PUBLIC、INTERFACE关键字用于指定后续源文件的可见性和属性。

PRIVATE：源文件只对目标本身可见。
PUBLIC：源文件对目标本身和依赖于该目标的其他目标可见。
INTERFACE：源文件不对目标本身可见，但会传递给依赖于该目标的其他目标。



例：

```cmake
add_executable(my_executable)
target_sources(my_executable
    PRIVATE main.cpp
    PUBLIC utils.cpp
    INTERFACE common.h
)
# target_sources命令将main.cpp源文件添加为my_executable目标的私有源文件，
# utils.cpp添加为公共源文件，common.h添加为接口源文件。这样，在构建my_executable时，它将包含这些源文件，并与其他目标共享或传递相应的源文件。
```

