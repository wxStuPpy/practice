Git是一款广泛使用的分布式版本控制系统，下面为你详细介绍一些Git的基础操作。

### 1. 安装与配置
在开始使用Git之前，你需要先安装它，不同操作系统的安装方式不同，安装完成后要进行基本配置。
- **安装**：
    - **Windows**：从 [Git 官方网站](https://git-scm.com/download/win) 下载安装程序，按照提示进行安装。
    - **Linux**：使用包管理器进行安装，例如在 Ubuntu 上可以运行 `sudo apt-get install git`。
    - **macOS**：可以使用 Homebrew 安装，命令为 `brew install git`。
- **配置用户信息**：安装完成后，需要配置你的用户名和邮箱，这些信息会记录在每次提交中。
```bash
git config --global user.name "Your Name"
git config --global user.email "your_email@example.com"
```

### 2. 仓库操作
#### 初始化仓库
在本地创建一个新的 Git 仓库，进入项目目录，然后执行以下命令：
```bash
git init
```
这会在当前目录下创建一个 `.git` 文件夹，用于存储版本控制的相关信息。

#### 克隆远程仓库
如果你想将远程仓库的代码克隆到本地，可以使用 `git clone` 命令。例如，克隆一个 GitHub 上的仓库：
```bash
git clone https://github.com/username/repo.git
```
如果你使用 SSH 协议，命令如下：
```bash
git clone git@github.com:username/repo.git
```

### 3. 文件操作
#### 添加文件到暂存区
在你对文件进行修改后，需要将这些文件添加到暂存区，使用 `git add` 命令。
```bash
# 添加单个文件
git add file.txt
# 添加所有修改的文件
git add .
```

#### 提交暂存区的文件到本地仓库
将暂存区的文件提交到本地仓库，使用 `git commit` 命令，同时要添加有意义的提交信息。
```bash
git commit -m "Add a new feature"
```

#### 查看文件状态
使用 `git status` 命令可以查看当前工作目录和暂存区的文件状态。
```bash
git status
```

#### 查看文件修改内容
使用 `git diff` 命令可以查看文件的具体修改内容。
```bash
# 查看工作目录与暂存区的差异
git diff
# 查看暂存区与最后一次提交的差异
git diff --staged
```

### 4. 分支操作
#### 创建分支
使用 `git branch` 命令可以创建新的分支。
```bash
git branch new-feature
```

#### 切换分支
使用 `git checkout` 命令可以切换到已有的分支。
```bash
git checkout new-feature
```
也可以使用 `git switch` 命令，它是 Git 2.23 及以上版本新增的命令，用于更清晰地切换分支。
```bash
git switch new-feature
```

#### 创建并切换到新分支
可以使用以下命令一次性完成创建并切换到新分支的操作。
```bash
git checkout -b new-feature
# 或者使用 git switch
git switch -c new-feature
```

#### 查看分支列表
使用 `git branch` 命令查看本地分支列表。
```bash
git branch
```
如果要查看远程分支列表，可以使用以下命令。
```bash
git branch -r
```

#### 合并分支
当你在某个分支上完成了开发工作，需要将其合并到主分支（通常是 `main` 或 `master`），可以使用 `git merge` 命令。
```bash
# 先切换到主分支
git checkout main
# 然后合并新特性分支
git merge new-feature
```

#### 删除分支
当某个分支不再需要时，可以使用 `git branch -d` 命令删除它。
```bash
git branch -d new-feature
```

### 5. 远程仓库操作
#### 查看远程仓库信息
使用 `git remote` 命令可以查看已配置的远程仓库信息。
```bash
git remote -v
```

#### 添加远程仓库
如果你克隆的仓库没有关联远程仓库，或者想添加新的远程仓库，可以使用 `git remote add` 命令。
```bash
git remote add origin https://github.com/username/repo.git
```

#### 推送本地分支到远程仓库
使用 `git push` 命令将本地分支的内容推送到远程仓库。
```bash
# 第一次推送某个分支到远程仓库
git push -u origin new-feature
# 后续推送可以直接使用
git push
```

#### 从远程仓库拉取最新代码
使用 `git pull` 命令从远程仓库拉取最新代码并合并到本地分支。
```bash
git pull origin main
```

以上就是 Git 的一些基础操作，掌握这些操作可以满足日常的代码版本控制需求。 

在Git操作中使用SSH（Secure Shell）协议可以更安全地与远程仓库进行交互，避免每次操作都输入用户名和密码。以下是使用SSH进行Git操作的详细步骤：

### 1. 检查是否已有SSH密钥
首先，你需要检查本地是否已经存在SSH密钥。通常，SSH密钥会存储在`~/.ssh`目录下。可以通过以下命令查看：
```bash
ls -al ~/.ssh
```
如果你看到类似`id_rsa.pub`或`id_ed25519.pub`这样的文件，说明你已经有SSH公钥了。

### 2. 生成新的SSH密钥（如果没有）
如果没有找到SSH密钥，你需要生成一个新的。可以使用`ssh-keygen`命令来生成，以下是一个示例：
```bash
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
```
- `-t rsa`：指定密钥类型为RSA。
- `-b 4096`：指定密钥的位数为4096。
- `-C "your_email@example.com"`：添加一个注释，方便识别这个密钥，通常使用你的邮箱地址。

执行命令后，会提示你选择密钥的保存位置和设置密码。如果你想使用默认设置，直接按回车键即可。

### 3. 启动SSH代理并添加私钥
生成密钥后，需要启动SSH代理并将私钥添加到代理中。执行以下命令：
```bash
# 启动SSH代理
eval "$(ssh-agent -s)"

# 添加私钥到SSH代理
ssh-add ~/.ssh/id_rsa
```
如果你使用的是其他类型的密钥，将`id_rsa`替换为你的私钥文件名。

### 4. 将公钥添加到远程仓库
你需要将生成的SSH公钥添加到远程仓库（如GitHub、GitLab等）的账户设置中。可以使用以下命令复制公钥内容：
```bash
cat ~/.ssh/id_rsa.pub
```
然后将输出的公钥内容复制到剪贴板。接着登录到你的远程仓库账户，找到“SSH Keys”或“Deploy Keys”设置项，添加新的SSH密钥，将复制的公钥内容粘贴到相应的输入框中，保存即可。

### 5. 使用SSH进行Git操作
现在你可以使用SSH协议进行Git操作了。在克隆仓库时，使用SSH地址而不是HTTPS地址。例如，克隆一个GitHub仓库：
```bash
git clone git@github.com:username/repo.git
```
如果你已经使用HTTPS地址克隆了仓库，可以将远程仓库地址从HTTPS切换到SSH：
```bash
# 查看当前远程仓库地址
git remote -v

# 修改远程仓库地址为SSH地址
git remote set-url origin git@github.com:username/repo.git
```
之后，你就可以像往常一样进行`git push`、`git pull`等操作了，无需再输入用户名和密码。

### 6. 测试SSH连接
可以使用以下命令测试SSH连接是否正常：
```bash
ssh -T git@github.com
```
如果看到类似以下的提示，说明连接成功：
```
Hi username! You've successfully authenticated, but GitHub does not provide shell access.
```

通过以上步骤，你就可以在Git操作中使用SSH协议了。