CXX = g++
TARGET = fork.exe
OBJS = fork.o
# 编译选项，只包含编译相关的选项
CXXFLAGS = -Wall -std=c++11
# 链接选项，包含库搜索路径和要链接的库名
LDFLAGS = -L../../lib/ -ladd
# 定义运行时的环境变量设置
RUN_ENV = LD_LIBRARY_PATH=../../lib

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@

$(OBJS): fork.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm *.o

run: $(TARGET)
	$(RUN_ENV) ./$(TARGET)