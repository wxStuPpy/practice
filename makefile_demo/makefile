# main:add.o main.o
# 	g++ add.o main.o -o main -std=c++11

# main.o:main.cpp ./inc/head.h 
# 	g++ -c main.cpp -o main.o -std=c++11

# add.o:./src/add.cpp ./inc/head.h
# 	g++ -c ./src/add.cpp -o add.o -std=c++11

# clean:
# 	-rm *.o
	

# object:=add.o\
# main.o

# main:${object}
# 	g++ ${object} -o main
# 定义 C++ 编译器，使用 g++ 来编译 C++ 代码
CXX = g++

# 编译选项，指定使用 C++11 标准。
# -std=c++11 明确告诉编译器使用 C++11 版本的语法和特性
CXXFLAGS = -std=c++11

# 定义目标文件列表，OBJS 变量包含了编译过程中生成的所有目标文件
# 这里我们有 add.o 和 main.o 两个目标文件，后续链接操作会用到这些目标文件
OBJS = add.o main.o

# 定义最终要生成的可执行文件的名称
# TARGET 变量指定了我们编译后的可执行文件名为 main
TARGET = main

# 声明伪目标 all。伪目标不对应实际的文件，只是为了方便执行一系列操作
# all 通常作为默认目标，当你直接在命令行输入 make 时，会执行 all 目标下的规则
.PHONY: all
all: $(TARGET)

# 生成可执行文件的规则。目标是 $(TARGET)，即前面定义的 main 可执行文件
# 依赖项是 $(OBJS)，也就是 add.o 和 main.o 两个目标文件
# 规则的命令部分使用 $(CXX)（即 g++）和 $(CXXFLAGS)（即 -std=c++11）进行链接操作
# $@ 是自动化变量，表示当前规则的目标文件，这里就是 main
# $^ 是自动化变量，表示当前规则的所有依赖文件，这里是 add.o 和 main.o
$(TARGET): $(OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^

# 生成 main.o 目标文件的规则。目标是 main.o
# 依赖项是 main.cpp 源文件和 ./inc/head.h 头文件
# 当 main.cpp 或 ./inc/head.h 文件有更新时，会重新编译生成 main.o
# 规则的命令部分使用 $(CXX)（即 g++）和 $(CXXFLAGS)（即 -std=c++11）进行编译操作
# -c 选项表示只进行编译，不进行链接
# $< 是自动化变量，表示当前规则的第一个依赖文件，这里就是 main.cpp
# $@ 是自动化变量，表示当前规则的目标文件，这里就是 main.o
main.o: main.cpp ./inc/head.h
    $(CXX) $(CXXFLAGS) -c $< -o $@

# 生成 add.o 目标文件的规则。目标是 add.o
# 依赖项是 ./src/add.cpp 源文件和 ./inc/head.h 头文件
# 当 ./src/add.cpp 或 ./inc/head.h 文件有更新时，会重新编译生成 add.o
# 规则的命令部分使用 $(CXX)（即 g++）和 $(CXXFLAGS)（即 -std=c++11）进行编译操作
# -c 选项表示只进行编译，不进行链接
# $< 是自动化变量，表示当前规则的第一个依赖文件，这里就是 ./src/add.cpp
# $@ 是自动化变量，表示当前规则的目标文件，这里就是 add.o
add.o: ./src/add.cpp ./inc/head.h
    $(CXX) $(CXXFLAGS) -c $< -o $@

# 声明伪目标 clean。伪目标不对应实际的文件，主要用于执行清理操作
# 当你在命令行输入 make clean 时，会执行 clean 目标下的规则
.PHONY: clean
# clean 目标的规则，用于清理编译生成的中间文件和最终可执行文件
# -rm -f $(OBJS) $(TARGET) 命令尝试强制删除 $(OBJS)（即 add.o 和 main.o）和 $(TARGET)（即 main）
# - 符号表示忽略命令执行过程中可能出现的错误，确保即使某些文件不存在也能继续执行
clean:
    -rm -f $(OBJS) $(TARGET)