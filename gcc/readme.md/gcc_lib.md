在GCC中，动态库（共享库）和静态库是两种常见的代码复用方式，下面分别介绍它们的创建和使用方法。

### 静态库

#### 1. 静态库的创建
静态库以 `.a` 为扩展名，它是多个目标文件的集合。以下是创建静态库的步骤：

**示例代码**：
假设有两个源文件 `add.c` 和 `sub.c`，分别实现加法和减法功能。
```c
// add.c
int add(int a, int b) {
    return a + b;
}
```
```c
// sub.c
int sub(int a, int b) {
    return a - b;
}
```

**步骤如下**：
- **编译源文件生成目标文件**：
```bash
gcc -c add.c sub.c
```
这会生成 `add.o` 和 `sub.o` 两个目标文件。

- **使用 `ar` 工具创建静态库**：
```bash
ar rcs libmath.a add.o sub.o
```
`ar` 是GNU归档工具，`r` 表示将文件插入到归档文件中，`c` 表示创建归档文件，`s` 表示写入一个目标文件索引。

#### 2. 静态库的使用
创建一个主程序 `main.c` 来使用静态库：
```c
// main.c
#include <stdio.h>

int add(int a, int b);
int sub(int a, int b);

int main() {
    int result1 = add(5, 3);
    int result2 = sub(5, 3);
    printf("5 + 3 = %d\n", result1);
    printf("5 - 3 = %d\n", result2);
    return 0;
}
```
使用以下命令编译并链接静态库：
```bash
gcc main.c -L. -lmath -o main
```
- `-L.` 表示在当前目录下查找库文件。
- `-lmath` 表示链接名为 `libmath.a` 的静态库（`lib` 和 `.a` 会自动添加）。
- `-o main` 表示生成可执行文件 `main`。

### 动态库

#### 1. 动态库的创建
动态库以 `.so` 为扩展名，创建动态库的步骤如下：

**示例代码**：
还是使用上面的 `add.c` 和 `sub.c` 文件。

**步骤如下**：
- **编译源文件生成目标文件，同时使用 `-fPIC` 选项**：
```bash
gcc -fPIC -c add.c sub.c
```
`-fPIC` 选项表示生成位置无关代码，这是创建动态库所必需的。

- **使用 `-shared` 选项创建动态库**：
```bash
gcc -shared -o libmath.so add.o sub.o
```
`-shared` 选项表示创建一个共享库。

#### 2. 动态库的使用
同样使用 `main.c` 作为主程序。

**编译并链接动态库**：
```bash
gcc main.c -L. -lmath -o main
```
这里的编译命令和静态库的使用类似。

**运行程序**：
由于动态库在运行时需要被加载，所以需要让系统知道动态库的位置。可以通过以下几种方式实现：
- **临时设置环境变量**：
```bash
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./main
```
`LD_LIBRARY_PATH` 是一个环境变量，用于指定动态库的搜索路径。

- **永久设置环境变量**：
可以将 `export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH` 添加到 `.bashrc` 或 `.bash_profile` 文件中，然后执行 `source ~/.bashrc` 或 `source ~/.bash_profile` 使设置生效。

- **将动态库复制到系统默认的库搜索路径**：
```bash
sudo cp libmath.so /usr/local/lib
sudo ldconfig
```
`ldconfig` 命令用于更新系统的动态链接库缓存。

- **临时设置环境变量**
`export LD_LIBRARY_PATH=库路径`

### 总结
- 静态库在编译时被链接到可执行文件中，会增加可执行文件的大小，但运行时不需要依赖库文件。
- 动态库在运行时被加载，可执行文件的大小较小，多个程序可以共享同一个动态库，但需要确保系统能够找到动态库。