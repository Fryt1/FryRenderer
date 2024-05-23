
# FryRenderer

**FryRenderer**是基于opengl的渲染器

引入**Assimp库**实现不同格式模型的导入，引用**stb_image库**导入纹理



## 主要功能

实现了**PBR,IBL,PCSS,Deferred Shading,SSAO**等渲染功能

![image](https://github.com/Fryt1/opengl_games202/blob/main/build/output/1.png)



## 构建



```bash
#本项目使用Makefile进行构建。在项目的根目录下，你可以使用以下命令来构建项目：

make

#这将编译所有的源文件并生成可执行文件。

#如果你想清理构建文件，你可以使用以下命令：
make clean
#如果你想运行项目，你可以使用以下命令：
make run
#此外，你还可以使用以下命令来构建项目的release版本或debug版本：
make release

make debug
```

