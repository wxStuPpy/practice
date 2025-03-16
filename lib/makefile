# 编译器选择，这里使用 g++ 作为 C++ 编译器
CXX = g++

# 编译选项设置
# -Wall: 开启所有常见的编译警告，帮助开发者发现代码中的潜在问题
# -std=c++11: 指定使用 C++11 标准进行编译，可使用 C++11 引入的新特性
# -I./inc: 指定头文件的搜索路径为当前目录下的 inc 文件夹
# -fPIC: 生成位置无关代码（Position Independent Code），这是制作动态库必需的选项，
#        确保动态库可以被加载到任意内存地址而不会出现地址冲突问题
CXXFLAGS = -Wall -std=c++11 -I./inc -fPIC

# 生成动态库的选项
# -shared: 告诉编译器生成共享库文件
SHARED = -shared

# 最终要生成的动态库文件名
TARGET = libadd.so

# 源文件列表，这里假设源文件在 src 目录下
SRCS = ./src/add.cpp

# 通过替换源文件的 .cpp 扩展名得到目标文件列表
OBJS = $(SRCS:.cpp=.o)

# 生成动态库的规则
# 目标：$(TARGET) 即 libadd.so
# 依赖：$(OBJS) 所有的目标文件
# 命令：$(CXX) $(SHARED) $(OBJS) -o $@
#       $(CXX): 使用指定的编译器（这里是 g++）
#       $(SHARED): 应用生成共享库的选项
#       $(OBJS): 链接所有的目标文件
#       -o $@: 指定输出文件名为目标名（即 libadd.so）
$(TARGET): $(OBJS)
	$(CXX) $(SHARED) $(OBJS) -o $@

# 生成目标文件的规则
# 目标：$(OBJS) 所有的目标文件
# 依赖：$(SRCS) 所有的源文件
# 命令：$(CXX) $(CXXFLAGS) -c $< -o $@
#       $(CXX): 使用指定的编译器（g++）
#       $(CXXFLAGS): 应用编译选项
#       -c: 只进行编译，不进行链接操作
#       $<: 表示第一个依赖文件（即当前要编译的源文件）
#       -o $@: 指定输出文件名为目标名（对应的 .o 文件）
$(OBJS): $(SRCS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理规则，用于删除生成的目标文件和动态库文件
# 目标：clean
# 命令：rm -f $(OBJS) $(TARGET)
#       rm: 删除文件命令
#       -f: 强制删除，忽略不存在的文件，避免因文件不存在而报错
#       $(OBJS): 要删除的目标文件列表
#       $(TARGET): 要删除的动态库文件
clean:
	rm -f $(OBJS) 