`Makefile` 是一种用于自动化编译和构建程序的文件，它定义了一系列的规则，告诉 `make` 工具如何根据源文件生成目标文件和可执行文件。以下将从不同复杂度和场景为你介绍 `Makefile` 的编写：

### 简单的 `Makefile` 示例
假设你有一个简单的 C++ 项目，包含 `main.cpp` 和 `math.cpp` 两个源文件，以及对应的头文件 `math.h`，你想生成一个名为 `main` 的可执行文件。

```makefile
# 编译器
CXX = g++
# 编译选项
CXXFLAGS = -Wall -g -I./inc -std=c++11

# 目标文件
OBJS = main.o math.o

# 可执行文件
TARGET = main

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^

# 生成目标文件
%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# 清理规则
clean:
    rm -f $(OBJS) $(TARGET)
```

#### 代码解释：
- **变量定义**：
    - `CXX`：指定使用的 C++ 编译器，这里是 `g++`。
    - `CXXFLAGS`：编译选项，`-Wall` 表示显示所有警告信息，`-g` 表示生成调试信息。
    - `OBJS`：目标文件列表。
    - `TARGET`：最终生成的可执行文件名称。
- **规则定义**：
    - `all`：默认目标，当你直接运行 `make` 命令时，会执行该目标下的规则。
    - `$(TARGET)`：生成可执行文件的规则，`$@` 表示目标文件，`$^` 表示所有依赖文件。
    - `%.o: %.cpp`：模式规则，用于生成目标文件，`$<` 表示第一个依赖文件。
    - `clean`：清理规则，用于删除生成的目标文件和可执行文件。

### 更复杂的 `Makefile` 示例
如果项目结构更复杂，包含多个源文件目录和头文件目录，`Makefile` 可以这样写：

```makefile
# 编译器
CXX = g++
# 编译选项
CXXFLAGS = -Wall -g -I./inc

# 源文件目录
SRCDIR = src
# 头文件目录
INCDIR = inc
# 目标文件目录
OBJDIR = obj
# 可执行文件目录
BINDIR = bin

# 查找源文件
SRCS = $(wildcard $(SRCDIR)/*.cpp)
# 生成目标文件列表
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
# 可执行文件
TARGET = $(BINDIR)/main

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^

# 生成目标文件
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
    @mkdir -p $(OBJDIR)
    $(CXX) $(CXXFLAGS) -c $< -o $@

# 清理规则
clean:
    rm -rf $(OBJDIR) $(BINDIR)
```

#### 代码解释：
- **变量定义**：
    - `SRCDIR`、`INCDIR`、`OBJDIR`、`BINDIR`：分别指定源文件目录、头文件目录、目标文件目录和可执行文件目录。
    - `SRCS`：使用 `wildcard` 函数查找所有源文件。
    - `OBJS`：使用 `patsubst` 函数将源文件路径转换为目标文件路径。
- **规则定义**：
    - `$(OBJDIR)/%.o: $(SRCDIR)/%.cpp`：生成目标文件的规则，在生成目标文件之前会先创建目标文件目录。
    - `clean`：清理规则，删除目标文件目录和可执行文件目录。

通过编写 `Makefile`，你可以方便地管理项目的编译过程，提高开发效率。加上伪目标