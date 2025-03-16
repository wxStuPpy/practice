`pthread_create` 是 POSIX 线程库中用于创建新线程的函数，其原型如下：

```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);
```

下面详细解释该函数的各个参数：

### 1. `pthread_t *thread`
- **类型**：`pthread_t *`，即指向 `pthread_t` 类型的指针。
- **解释**：这个参数用于存储新创建线程的线程 ID。`pthread_t` 是一个不透明的数据类型，代表线程的唯一标识符。在调用 `pthread_create` 成功后，新线程的 ID 会被存储在 `thread` 所指向的内存位置。后续可以使用这个线程 ID 来对该线程进行其他操作，如等待线程结束（`pthread_join`）、取消线程（`pthread_cancel`）等。

### 2. `const pthread_attr_t *attr`
- **类型**：`const pthread_attr_t *`，即指向 `pthread_attr_t` 类型的常量指针。
- **解释**：该参数用于指定新线程的属性，例如线程的栈大小、调度策略、分离状态等。如果将其设置为 `NULL`，则新线程将使用默认的线程属性。`pthread_attr_t` 是一个结构体类型，通过一系列的函数（如 `pthread_attr_init`、`pthread_attr_setstacksize` 等）可以对其进行初始化和设置。

### 3. `void *(*start_routine) (void *)`
- **类型**：`void *(*)(void *)`，这是一个函数指针类型，指向一个返回值为 `void *` 且接受一个 `void *` 类型参数的函数。
- **解释**：这个参数指定了新线程启动时要执行的函数，也就是线程的入口点。当新线程被创建并开始执行时，会调用 `start_routine` 所指向的函数。该函数执行完毕后，线程也会随之结束。

### 4. `void *arg`
- **类型**：`void *`，即通用指针类型。
- **解释**：该参数用于向新线程的入口函数 `start_routine` 传递参数。`void *` 类型的指针可以指向任意类型的数据，因此可以将任意类型的数据传递给新线程的入口函数。在 `start_routine` 函数内部，需要将 `arg` 指针转换为实际的数据类型。


`pthread_exit`、`pthread_cancel`、`pthread_join` 和 `pthread_detach` 是 POSIX 线程库中用于管理线程生命周期和状态的重要函数。下面将详细介绍它们的功能、使用方法和区别。

### 1. `pthread_exit`
#### 功能
`pthread_exit` 用于终止调用它的线程，并可以返回一个指向某个对象的指针作为该线程的退出状态。该函数会释放线程所占用的部分资源，例如线程栈，但线程的某些资源（如线程 ID）在其他线程调用 `pthread_join` 与之汇合之前不会被完全释放。

#### 函数原型
```c
#include <pthread.h>

void pthread_exit(void *retval);
```

#### 参数
- `retval`：`void *` 类型的指针，指向要作为线程退出状态返回的对象。其他线程可以通过 `pthread_join` 来获取这个值。如果不需要返回状态，可以传入 `NULL`。

#### 示例
```c
#include <pthread.h>
#include <stdio.h>

void *thread_function(void *arg) {
    int result = 42;
    pthread_exit((void *)&result);
}

int main() {
    pthread_t thread_id;
    void *retval;

    pthread_create(&thread_id, NULL, thread_function, NULL);
    pthread_join(thread_id, &retval);
    int *result = (int *)retval;
    printf("Thread returned: %d\n", *result);

    return 0;
}
```

### 2. `pthread_cancel`
#### 功能
`pthread_cancel` 用于向指定线程发送取消请求，请求目标线程终止执行。不过，目标线程是否响应取消请求以及何时响应，取决于该线程的取消状态（可取消或不可取消）和取消类型（异步取消或延迟取消）。

#### 函数原型
```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
```

#### 参数
- `thread`：`pthread_t` 类型的线程 ID，指定要取消的目标线程。

#### 返回值
- 成功时返回 0，失败时返回一个非零的错误码。

#### 示例
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread_function(void *arg) {
    while (1) {
        printf("Thread is running...\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_function, NULL);

    sleep(3);
    if (pthread_cancel(thread_id) != 0) {
        perror("pthread_cancel");
    }

    pthread_join(thread_id, NULL);
    printf("Thread has been cancelled.\n");

    return 0;
}
```

### 3. `pthread_join`
#### 功能
`pthread_join` 用于阻塞调用线程，直到指定的目标线程终止。同时，它可以获取目标线程的退出状态。调用该函数会确保目标线程的所有资源（包括线程 ID）被正确释放。

#### 函数原型
```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

#### 参数
- `thread`：`pthread_t` 类型的线程 ID，指定要等待的目标线程。
- `retval`：`void **` 类型的指针，用于存储目标线程的退出状态。如果不需要获取退出状态，可以传入 `NULL`。

#### 返回值
- 成功时返回 0，失败时返回一个非零的错误码。

#### 示例
```c
#include <pthread.h>
#include <stdio.h>

void *thread_function(void *arg) {
    int result = 100;
    return (void *)&result;
}

int main() {
    pthread_t thread_id;
    void *retval;

    pthread_create(&thread_id, NULL, thread_function, NULL);
    if (pthread_join(thread_id, &retval) != 0) {
        perror("pthread_join");
    }

    int *result = (int *)retval;
    printf("Thread returned: %d\n", *result);

    return 0;
}
```

### 4. `pthread_detach`
#### 功能
`pthread_detach` 用于将指定线程标记为分离状态。处于分离状态的线程在终止时，其资源会自动被系统回收，不需要其他线程调用 `pthread_join` 来释放资源。一旦线程被分离，就不能再使用 `pthread_join` 来等待它结束。

#### 函数原型
```c
#include <pthread.h>

int pthread_detach(pthread_t thread);
```

#### 参数
- `thread`：`pthread_t` 类型的线程 ID，指定要设置为分离状态的线程。

#### 返回值
- 成功时返回 0，失败时返回一个非零的错误码。

#### 示例
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *thread_function(void *arg) {
    sleep(2);
    printf("Thread is done.\n");
    return NULL;
}

int main() {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_function, NULL);

    if (pthread_detach(thread_id) != 0) {
        perror("pthread_detach");
    }

    printf("Main thread continues...\n");
    sleep(3);

    return 0;
}
```

### 区别和联系
- **调用目的和主体**
    - `pthread_exit` 由线程自身调用，用于主动终止自身并返回状态。
    - `pthread_cancel` 由其他线程调用，用于请求终止指定线程。
    - `pthread_join` 由其他线程调用，用于等待指定线程结束并获取其退出状态。
    - `pthread_detach` 由其他线程调用，用于将指定线程设置为分离状态，使其资源自动回收。
- **资源管理**
    - `pthread_exit` 部分释放线程资源，完整释放需 `pthread_join`。
    - `pthread_cancel` 若线程响应取消请求，资源释放取决于线程的取消处理。
    - `pthread_join` 确保目标线程资源完全释放。
    - `pthread_detach` 使线程终止时资源自动回收。
- **使用场景**
    - `pthread_exit` 适用于线程正常完成任务后退出。
    - `pthread_cancel` 适用于需要提前终止某个线程的情况。
    - `pthread_join` 用于主线程等待子线程结束并获取结果。
    - `pthread_detach` 适用于不需要等待线程结束和获取退出状态，希望线程资源自动回收的场景。 