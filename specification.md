
## 项目依赖

Qt 版本： 5.12.0 或更高。

Nowe 使用 [QXMPP](http://www.qxmpp.org/) 库，开发者应该自行编译安装 QXMPP 库至自己电脑中，学习 QXMPP 库的用法。


## 项目架构

Nowe 使用 Qt 编写，遵循 UI 与业务逻辑分离的设计原则。

### 主要模块

- 登录窗口
  对应 loginwindow.ui，是用户登录连接聊天服务器的界面。
- 注册窗口
  对应 registwindow.ui，是用户在相应服务器注册帐号的界面。
- 主窗口
  对应 mainwindow.ui，显示部分用户信息（用户头像、状态等），好友列表和群聊天列表。
- 聊天窗口
  对应 chatdialog.ui，是用户的聊天界面。
- 用户信息窗口
  对应 dataframe.ui，显示完整用户信息，可以在这里编辑用户信息。
- 头像编辑窗口
  对应 ChangeHeaderWnd.ui，是用户使用修改头像功能的界面。

### 程序流程

![](http://image.chirsz.cc/image/jpg/nowe-process.png)

### 文件组织

```plain
项目目录
├── LICENSE
├── README.md
├── specification.md  -- 开发说明文档
└── src
    ├── nowe.pro      -- 项目文件
    ├── *.cpp *.h ... -- 源代码文件和头文件
    ├── *.ui      ... -- UI 布局文件
    ├── resources.qrc -- 资源文件
    └── images        -- 图片资源文件夹
        ├── 1.png
        ├── 2.png
        └── ...
```

### QXMPP 说明

QXMPP 文档见 [https://doc.qxmpp.org](https://doc.qxmpp.org) 或者 [镜像站](http://chirsz.cc/qxmppdoc/index.html)。

使用 QXMPP 库需要了解一定的 XMPP 知识：[XMPP JID 和通信原语有3种：message、presence和iq](https://blog.csdn.net/iteye_5495/article/details/82652037)，[XMPP详解](https://www.jianshu.com/p/84d15683b61e)

Nowe 客户端主要用到了 QXMPP 的客户端部分。QXMPP 使用 `QXmppClient` 类与服务器建立连接，其它消息收发、获取用户状态等都由加载到 `QXmppClient` 对象的*扩展*（Extension）进行。每次有信息节（stanza）收发，`QXmppClient` 对象会逐个寻找是否有合适的扩展处理。其中 `QXmppRosterManager`、`QXmppVCardManager`、`QXmppVersionManager` 三个扩展是默认加载的，其它扩展需要手动使用 `QXmppClient::addExtension` 加载。程序员也可以通过继承 `QXmppClientExtension` 来编写自己的扩展。

Nowe 客户端的 mainwindow 对象有一个 `QXmppClient*` 成员，这就是用来进行通讯的组件；程序运行过程中应当只用这一个 `QXmppClient` 对象与服务器连接。有一个例外是 loginwindow，它需要创建一个独立的  `QXmppClient` 对象以连接服务器进行注册操作。

下面列出客户端各种功能对应的 QXMPP 的函数或类。

| 功能         | 对应函数/类                                   |
| ------------ | --------------------------------------------- |
| 联系人管理   | `QXmppRosterManager`，`QXmppDiscoveryManager` |
| 聊天群组管理 | `QXmppBookmarkManager`，`QXmppMucManager`     |
| 个人信息管理 | `QXmppVCardManager`                           |
| 历史记录     | `QXmppMamManager`                             |
| 文件上传     | `QXmppUploadRequestManager`                   |
| 帐号注册     | `QXmppRegisterIq`                             |
| 发送消息     | `QXmppClient::sendPacket`                     |

图片发送我们采用将图片发送到图片服务器上，消息内容中使用图片 URL 链接的方式。


## 开发规范细节
1. 源文件统一采用 UTF-8 编码。
2. 换行符统一使用 Unix 格式（LF）。
3. 资源文件、UI 文件只能使用对应的编辑器编辑，不能直接编辑其中文本。
4. 缩进、大括号换行等代码风格在同一源文件内保持一致，尽量使用 K&R 格式，最好在提交前用 astyle 格式化一遍。
