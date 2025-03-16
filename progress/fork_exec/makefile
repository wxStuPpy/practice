CXX=g++
CXXFLAGS=-Wall -g

X_OBJS=x.o
Y_OBJS=y.o

X_TARGET=x
Y_TARGET=y

# 默认目标，依赖于 x 和 y
all: $(X_TARGET) $(Y_TARGET)

# 生成可执行文件 x
$(X_TARGET): $(X_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 生成目标文件 x.o
$(X_OBJS): x.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 生成可执行文件 y
$(Y_TARGET): $(Y_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 生成目标文件 y.o
$(Y_OBJS): y.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理目标文件和可执行文件
clean:
	-rm -r *.o $(X_TARGET) $(Y_TARGET)

# 运行可执行文件
run: $(X_TARGET) 
	./$(X_TARGET)