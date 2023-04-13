# CMake

## 入门

### 构建helloworld

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(project LANGUAGES CXX)  #CXX 代表c++  项目名称project
add_executable(hello-world hello-world.cpp)
```

```cmake
$ cmake -H. -B build
```

- `Makefile`: `make`将运行指令来构建项目。
- `CMakefile`：包含临时文件的目录，CMake用于检测操作系统、编译器等。此外，根据所选的生成器，它还包含特定的文件。
- `cmake_install.cmake`：处理安装规则的CMake脚本，在项目安装时使用。
- `CMakeCache.txt`：如文件名所示，CMake缓存。CMake在重新运行配置时使用这个文件。

### cmake --build . --target <target-name>

​		实现如下功能：

- **all**(或Visual Studio generator中的ALL_BUILD)是默认目标，将在项目中构建所有目标。
- **clean**，删除所有生成的文件。
- **rebuild_cache**，将调用CMake为源文件生成依赖(如果有的话)。
- **edit_cache**，这个目标允许直接编辑缓存。

对于更复杂的项目，通过测试阶段和安装规则，CMake将生成额外的目标：

- **test**(或Visual Studio generator中的**RUN_TESTS**)将在CTest的帮助下运行测试套件。我们将在第4章中详细讨论测试和CTest。
- **install**，将执行项目安装规则。我们将在第10章中讨论安装规则。
- **package**，此目标将调用CPack为项目生成可分发的包。打包和CPack将在第11章中讨论。

### 构建和链接静态库和动态库

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



### 用条件句控制编译

​		具体实施：

1. 将`Message.hpp`和`Message.cpp`构建成一个库(静态或动态)，然后将生成库链接到`hello-world`可执行文件中。
2. 将`Message.hpp`，`Message.cpp`和`hello-world.cpp`构建成一个可执行文件，但不生成任何一个库。

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-04 LANGUAGES CXX)	
#引入了一个新变量USE_LIBRARY，这是一个逻辑变量，值为OFF
set(USE_LIBRARY OFF)
message(STATUS "Compile sources into a library? ${USE_LIBRARY}") #打印USE_LIBRARY

#定义BUILD_SHARED_LIBS全局变量，并设置为OFF。调用add_library并省略第二个参数，将构建一个静态库：
set(BUILD_SHARED_LIBS OFF)

#引入一个变量_sources，包括Message.hpp和Message.cpp
list(APPEND _sources Message.hpp Message.cpp)

#一个基于USE_LIBRARY值的if-else语句。如果逻辑为真，则Message.hpp和Message.cpp将打包成一个库：
if(USE_LIBRARY)
    # add_library will create a static library
    # since BUILD_SHARED_LIBS is OFF
    add_library(message ${_sources})
    add_executable(hello-world hello-world.cpp)
    target_link_libraries(hello-world message)
else()
    add_executable(hello-world hello-world.cpp ${_sources})
endif()

#由于USE_LIBRARY为OFF, hello-world可执行文件将使用所有源文件来编译。可以通过在GNU/Linux上，运行objdump -x命令进行验证。
```

- `BUILD_SHARED_LIBS`是CMake的一个全局标志。因为CMake内部要查询`BUILD_SHARED_LIBS`全局变量，所以`add_library`命令可以在不传递`STATIC/SHARED/OBJECT`参数的情况下调用；如果为`false`或未定义，将生成一个静态库。
- **TIPS**:*`_sources`变量是一个局部变量，不应该在当前范围之外使用，可以在名称前加下划线。*

### 向用户显示选项

​		

```cmake
#用一个选项替换上一个示例的set(USE_LIBRARY OFF)命令。该选项将修改USE_LIBRARY的值，并设置其默认值为OFF：

option(USE_LIBRARY "Compile sources into a library" OFF)


```

```
$ mkdir -p build
$ cd build
$ cmake -D USE_LIBRARY=ON ..	
```

#### `option`可接受三个参数：

```
option(<option_variable> "help string" [initial value])
```

- `<option_variable>`表示该选项的变量的名称。
- `"help string"`记录选项的字符串，在CMake的终端或图形用户界面中可见。
- `[initial value]`选项的默认值，可以是`ON`或`OFF`。

### 指定编译器

​		CMake将语言的编译器存储在`CMAKE_<LANG>_COMPILER`变量中，其中`<LANG>`是受支持的任何一种语言，对于我们的目的是`CXX`、`C`或`Fortran`。

1. 使用CLI中的`-D`选项

   1. ```cmake
      $ cmake -D CMAKE_CXX_COMPILER=clang++ ..
      ```

2. 通过导出环境变量`CXX`(C++编译器)、`CC`(C编译器)和`FC`(Fortran编译器)。例如，使用这个命令使用`clang++`作为`C++`编译器：

   1. ```cmake
      $ env CXX=clang++ cmake ..
      ```

- **NOTE**:*CMake了解运行环境，可以通过其CLI的`-D`开关或环境变量设置许多选项。前一种机制覆盖后一种机制，但是我们建议使用`-D`显式设置选项。显式优于隐式，因为环境变量可能被设置为不适合(当前项目)的值。*

### 切换构建类型

CMake可以配置构建类型，例如：Debug、Release等。配置时，可以为Debug或Release构建设置相关的选项或属性，例如：编译器和链接器标志。控制生成构建系统使用的配置变量是`CMAKE_BUILD_TYPE`。该变量默认为空，CMake识别的值为:

1. **Debug**：用于在没有优化的情况下，使用带有调试符号构建库或可执行文件。
2. **Release**：用于构建的优化的库或可执行文件，不包含调试符号。
3. **RelWithDebInfo**：用于构建较少的优化库或可执行文件，包含调试符号。
4. **MinSizeRel**：用于不增加目标代码大小的优化方式，来构建库或可执行文件。

```cmake
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
```

**可见性的含义如下:**

- PRIVATE，编译选项会应用于给定的目标，不会传递给与目标相关的目标。我们的示例中， 即使compute-areas将链接到geometry库，compute-areas也不会继承geometry目标上设置的编译器选项。
- INTERFACE，给定的编译选项将只应用于指定目标，并传递给与目标相关的目标。
- PUBLIC，编译选项将应用于指定目标和使用它的目标。

**NOTE**:*本书中，我们推荐为每个目标设置编译器标志。使用`target_compile_options()`不仅允许对编译选项进行细粒度控制，而且还可以更好地与CMake的更高级特性进行集成。*



```cmake
set_target_properties(animals
  PROPERTIES
    CXX_STANDARD 14
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE 1
)
add_executable(animal-farm animal-farm.cpp)
set_target_properties(animal-farm
  PROPERTIES
    CXX_STANDARD 14
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
  )
```

​		

- **CXX_STANDARD**会设置我们想要的标准。
- **CXX_EXTENSIONS**告诉CMake，只启用`ISO C++`标准的编译器标志，而不使用特定编译器的扩展。
- **CXX_STANDARD_REQUIRED**指定所选标准的版本。如果这个版本不可用，CMake将停止配置并出现错误。当这个属性被设置为`OFF`时，CMake将寻找下一个标准的最新版本，直到一个合适的标志。这意味着，首先查找`C++14`，然后是`C++11`，然后是`C++98`。（目前会从`C++20`或`C++17`开始查找）

**TIPS**:*如果语言标准是所有目标共享的全局属性，那么可以将`CMAKE_<LANG>_STANDARD`、*

*`CMAKE_<LANG>_EXTENSIONS`和*

*`CMAKE_<LANG>_STANDARD_REQUIRED`变量设置为相应的值。所有目标上的对应属性都将使用这些设置。*



### 使用控制流

`foreach endforeach`和`while-endwhile`。

两者都可以与`break`结合使用，以便尽早从循环中跳出。





- `set_source_files_properties(file PROPERTIES property value)`，它将属性设置为给定文件的传递值。与目标非常相似，文件在CMake中也有属性，允许对构建系统进行非常细粒度的控制。源文件的可用属性列表可以在这里找到: https://cmake.org/cmake/help/v3.5/manual/cmake-properties.7.html#source-file-properties 。
- `get_source_file_property(VAR file property)`，检索给定文件所需属性的值，并将其存储在CMake`VAR`变量中。

**NOTE**:*CMake中，列表是用分号分隔的字符串组。列表可以由`list`或`set`命令创建。例如，`set(var a b c d e)`和`list(APPEND a b c d e)`都创建了列表`a;b;c;d;e`。*



**`foreach()`的四种使用方式:**

- `foreach(loop_var arg1 arg2 ...)`: 其中提供循环变量和显式项列表。当为`sources_with_lower_optimization`中的项打印编译器标志集时，使用此表单。注意，如果项目列表位于变量中，则必须显式展开它；也就是说，`${sources_with_lower_optimization}`必须作为参数传递。
- 通过指定一个范围，可以对整数进行循环，例如：`foreach(loop_var range total)`或`foreach(loop_var range start stop [step])`。
- 对列表值变量的循环，例如：`foreach(loop_var IN LISTS [list1[...]])` 。参数解释为列表，其内容就会自动展开。
- 对变量的循环，例如：`foreach(loop_var IN ITEMS [item1 [...]])`。参数的内容没有展开。







## 构建项目

​		

```c
//工程目录下的
PROJECT()
ADD_SUBDIRECTORY(src bin)
    //src  为其子目录 生成的可执行文件放到 bin文件夹下
```

```cmake
#--
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(answer)
#添加子目录
add_subdirectory()
# 添加libanswer 库目标， STATIC指定为静态库
add_library(libanswer STATIC answer.cpp)

add_execuable(answer main.cpp)

#为answer 可执行目标链接 libanswer
target_link_libraries(answer libanswer)

```

```cmake
cmake -B build #生成构建目录
-S <源路径>
要构建的 CMake 项目的根目录路径。

-B <构建路径>
CMake 将用作构建目录根目录的目录路径。

如果该目录不存在，CMake 会创建它。
cmake --build build
```



```cmake
target_include_dirctory(libanswer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
```



## ADD_SUBDIRECTORY

1. 当前工程添加存放源文件的子目录

## set 

ADD_EXECUTABLE()





静态库和动态库的区别

- 静态库的拓展名一般为".a"或".lib"；动态库的拓展名一般为".so"或".dll"
- 静态库在编译时直接整合到目标程序中，编译成功的可执行文件可独立运行
- 动态库在编译时不会放到连接的目标程序中，即可执行文件无法单独运行。

## target_include_dirctory()

```
target_include_dirctory(libanswer PUBLIC ￥{CMAKE_CURRENT_SOURCE_DIR}/include)
```



1. `￥{CMAKE_CURRENT_SOURCE_DIR}`

## find_package()

1. 用于在系统中寻找已经安装的第三方库的头文件和库文件的位置