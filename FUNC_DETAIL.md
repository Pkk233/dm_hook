# dm_hook 各函数详细使用文档

> 由根目录各模块下的原版 `.htm` 文档转换而来；函数原型与参数名以 `dm_hook/dm.h` 声明为准。
> 调用约定 `__stdcall`；返回 `const char*` 的函数使用线程安全静态缓冲区。

## 概览

共 **416** 个函数。

| 模块 | 函数数 |
|:-----|:------:|
| 基本设置 | 13 |
| 窗口 | 40 |
| 键鼠 | 34 |
| 图色 | 57 |
| 后台设置 | 28 |
| 内存 | 54 |
| 文件 | 26 |
| 文字识别 | 46 |
| 系统 | 44 |
| 杂项 | 9 |
| 汇编 | 9 |
| Ai | 17 |
| Foobar | 22 |
| 答题 | 9 |
| 算法 | 3 |
| 防护盾 | 5 |

----

## 函数详情

### dm_enablePicCache

**签名**：`long dm_enablePicCache(long enable)`

**简介**：设置是否开启或者关闭插件内部的图片缓存机制. (默认是打开).

**原型**：long EnablePicCache (enable)

**参数定义**

- `enable` (整形数)：0:关闭1:打开

**返回值**：整形数: 0:失败1:成功

**示例**：dm.EnablePicCache 0

**注**：有些时候，系统内存比较吃紧，这时候再打开内部缓存，可能会导致缓存分配在虚拟内存，这样频繁换页，反而导致图色效率下降.这时候就建议关闭图色缓存.所有图色缓存机制都是对本对象的，也就是说，调用图色缓存机制的函数仅仅对本对象生效.每个对象都有一个图色缓存队列.

----

### dm_getBasePath

**签名**：`const char* dm_getBasePath()`

**简介**：获取注册在系统中的dm.dll的路径.

**原型**：string GetBasePath()

**返回值**：字符串:返回dm.dll所在路径

**示例**：base_path=dm.GetBasePath() MessageBox base_path

----

### dm_getDmCount

**签名**：`long dm_getDmCount()`

**简介**：返回当前进程已经创建的dm对象个数.

**原型**：long GetDmCount ()

**返回值**：整形数:个数.

**示例**：TracePrint dm.GetDmCount ()

----

### dm_getID

**签名**：`long dm_getID()`

**简介**：返回当前大漠对象的ID值，这个值对于每个对象是唯一存在的。可以用来判定两个大漠对象是否一致.

**原型**：long GetID()

**返回值**：整形数:当前对象的ID值.

**示例**：TracePrint dm.GetID()

----

### dm_getLastError

**签名**：`long dm_getLastError()`

**简介**：获取插件命令的最后错误

**原型**：long GetLastError()

**返回值**：整形数:返回值表示错误值。0表示无错误. -1:表示你使用了绑定里的收费功能，但是没注册，无法使用. -2:使用模式0 2时出现，因为目标窗口有保护.常见于win7以上系统.或者有安全软件拦截插件.解决办法:关闭所有安全软件，然后再重新尝试.如果还不行就可以肯定是目标窗口有特殊保护. -3:使用模式0 2时出现，可能目标窗口有保护，也可能是异常错误.可以尝试换绑定模式或许可以解决. -4:使用模式101 103时出现，这是异常错误. -5:使用模式101 103时出现,这个错误的解决办法就是关闭目标窗口，重新打开再绑定即可.也可能是运行脚本的进程没有管理员权限. -6:被安全软件拦截。典型的是金山.360等.如果是360关闭即可。如果是金山，必须卸载，关闭是没用的. -7 -9:使用模式101 103时出现,异常错误.还有可能是安全软件的问题，比如360等。尝试卸载360. -8 -10:使用模式101 103时出现,目标进程可能有保护,也可能是插件版本过老，试试新的或许可以解决. -8可以尝试使用DmGuard中的np2盾配合. -11:使用模式101 103时出现,目标进程有保护.告诉我解决。-12:使用模式101 103时出现,目标进程有保护.告诉我解决。-13:使用模式101 103时出现,目标进程有保护.或者是因为上次的绑定没有解绑导致。尝试在绑定前调用ForceUnBindWindow. -37:使用模式101 103时出现,目标进程有保护.告诉我解决。-14:可能系统缺少部分DLL,尝试安装d3d.或者是鼠标或者键盘使用了dx.mouse.api或者dx.keypad.api，但实际系统没有插鼠标和键盘.也有可能是图色中有dx.graphic.3d之类的,但相应的图色被占用,比如全屏D3D程序. -16:可能使用了绑定模式0和101，然后可能指定了一个子窗口.导致不支持.可以换模式2或者103来尝试.另外也可以考虑使用父窗口或者顶级窗口.来避免这个错误。还有可能是目标窗口没有正常解绑然后再次绑定的时候. -17:模式101 103时出现.这个是异常错误.告诉我解决. -18:句柄无效. -19:使用模式0 11 101时出现,这是异常错误,告诉我解决. -20:使用模式101 103时出现,说明目标进程里没有解绑，并且子绑定达到了最大.尝试在返回这个错误时，调用ForceUnBindWindow来强制解除绑定. -21:使用模式任何模式时出现,说明目标进程已经存在了绑定(没有正确解绑就退出了?被其它软件绑定?,或者多个线程同时进行了绑定?).尝试在返回这个错误时，调用ForceUnBindWindow来强制解除绑定.或者检查自己的代码. -22:使用模式0 2,绑定64位进程窗口时出现,因为安全软件拦截插件释放的EXE文件导致. -23:使用模式0 2,绑定64位进程窗口时出现,因为安全软件拦截插件释放的DLL文件导致. -24:使用模式0 2,绑定64位进程窗口时出现,因为安全软件拦截插件运行释放的EXE. -25:使用模式0 2,绑定64位进程窗口时出现,因为安全软件拦截插件运行释放的EXE. -26:使用模式0 2,绑定64位进程窗口时出现,因为目标窗口有保护.常见于win7以上系统.或者有安全软件拦截插件.解决办法:关闭所有安全软件，然后再重新尝试.如果还不行就可以肯定是目标窗口有特殊保护. -27:绑定64位进程窗口时出现，因为使用了不支持的模式，目前暂时只支持模式0 2 11 13 101 103 -28:绑定32位进程窗口时出现，因为使用了不支持的模式，目前暂时只支持模式0 2 11 13 101 103 -38:是用了大于2的绑定模式,并且使用了dx.public.inject.c时，分配内存失败.可以考虑开启memory系列盾来尝试. -39:是用了大于2的绑定模式,并且使用了dx.public.inject.c时的异常错误.可以联系我解决. -40:是用了大于2的绑定模式,并且使用了dx.public.inject.c时,写入内存失败.可以考虑开启memory系列盾来尝试. -41:是用了大于2的绑定模式,并且使用了dx.public.inject.c时的异常错误.可以联系我解决. -42:绑定时,创建映射内存失败.这是个异常错误.一般不会出现.如果出现了，检查下代码是不是有同个对象同时绑定的情况.还有可能是你的进程有句柄泄露导致无法创建句柄会出这个错误. -43:绑定时,映射内存失败.这是个异常错误.一般不会出现.如果出现了，一般是你的进程内存不足,检查下你的进程是不是内存泄漏了. -44:无效的参数,通常是传递了不支持的参数. -45:绑定时,创建互斥信号失败.这个是一场错误.一般不会出现.如果出现了检查进程是否有句柄泄漏的情况. -100:调用读写内存函数后，发现无效的窗口句柄-101:读写内存函数失败-200: AsmCall失败-202: AsmCall平台兼容问题.联系我解决.

**示例**：TracePrint dm.GetLastError()

**注**：此函数必须紧跟上一句函数调用，中间任何的语句调用都会改变这个值.

----

### dm_getPath

**签名**：`const char* dm_getPath()`

**简介**：获取全局路径.(可用于调试)

**原型**：string GetPath()

**返回值**：字符串:以字符串的形式返回当前设置的全局路径

**示例**：path=dm.GetPath() MessageBox path

----

### dm_reg

**签名**：`long dm_reg(const char* reg_code, const char* ver_info)`

**简介**：调用此函数来注册，从而使用插件的高级功能.推荐使用此函数.

**原型**：long Reg(reg_code, ver_info)

**参数定义**

- `reg_code` (字符串)：注册码. (从大漠插件后台获取)

- `ver_info` (字符串)：版本附加信息.可以在后台详细信息查看.可以任意填写.可留空.长度不能超过32.并且只能包含数字和字母以及小数点.这个版本信息不是插件版本.

**返回值**：整形数:-1:无法连接网络,(可能防火墙拦截,如果可以正常访问大漠插件网站，那就可以肯定是被防火墙拦截) -2:进程没有以管理员方式运行. (出现在win7 win8 vista 2008.建议关闭uac) 0:失败(未知错误) 1:成功2:余额不足3:绑定了本机器，但是账户余额不足50元. 4:注册码错误5:你的机器或者IP在黑名单列表中或者不在白名单列表中. 6:非法使用插件.一般出现在定制插件时，使用了和绑定的用户名不同的注册码.也有可能是系统的语言设置不是中文简体,也可能有这个错误. 7:你的帐号因为非法使用被封禁.（如果是在虚拟机中使用插件，必须使用Reg或者RegEx，不能使用RegNoMac或者RegExNoMac,否则可能会造成封号，或者封禁机器）8: ver_info不在你设置的附加白名单中. 77：机器码或者IP因为非法使用，而被封禁.（如果是在虚拟机中使用插件，必须使用Reg或者RegEx，不能使用RegNoMac或者RegExNoMac,否则可能会造成封号，或者封禁机器）封禁是全局的，如果使用了别人的软件导致77，也一样会导致所有注册码均无法注册。解决办法是更换IP，更换MAC. 777：同一个机器码注册次数超过了服务器限制,被暂时封禁.请登录后台，插件今日详细消费记录里，相应的机器码是否有次数异常，并立刻优化解决.如果还有问题，可以联系我来解决. -8:版本附加信息长度超过了32 -9:版本附加信息里包含了非法字母.空:这是不可能返回空的，如果出现空，那肯定是当前使用的版本不对,老的插件里没这个函数导致返回为空.最好参考文档中的标准写法,判断插件版本号.

**示例**：dm_ret=dm.Reg("abcdefg","") if dm_ret<>1 then MessageBox"注册失败,只能使用免费功能"end if

**注**：简单游平台调用此函数，不会扣费.注册码在大漠插件后台可以查看.此函数每个进程调用一次即可，不需要每个DM对象都调用.必须保证此函数在创建完对象以后立即调用，尤其必须在绑定窗口之前调用，否则可能会出现异常.如果有多个进程操作同个窗口，必须保证每个进程要么都调用Reg,要么都不要调用Reg，以免出现异常.

----

### dm_regEx

**签名**：`long dm_regEx(const char* reg_code, const char* ver_info, const char* ip)`

**简介**：调用此函数来注册，从而使用插件的高级功能.可以根据指定的IP列表来注册.新手不建议使用!

**原型**：long RegEx(reg_code, ver_info, ip)

**参数定义**

- `reg_code` (字符串)：注册码. (从大漠插件后台获取)

- `ver_info` (字符串)：版本附加信息.可以在后台详细信息查看.可留空.长度不能超过32.并且只能包含数字和字母以及小数点.这个版本信息不是插件版本.

- `ip` (字符串)：插件注册的ip地址.可以用|来组合,依次对ip中的地址进行注册，直到成功. ip地址列表在VIP群中获取.从7.2111开始,这里也可以使用域名的方式。可以自己解析域名到我的IP.比如"1.xxx.com|2.xxx.com"。1.xxx.com和2.xxx.com是自己的域名,解析到我的IP即可.还可以自定义端口.比如"1.xxx.com: 1234|2.xxx.com: 5678"等.如果不指定端口,那么默认使用我内置的端口

**返回值**：整形数:-1:无法连接网络,(可能防火墙拦截,如果可以正常访问大漠插件网站，那就可以肯定是被防火墙拦截) -2:进程没有以管理员方式运行. (出现在win7 win8 vista 2008.建议关闭uac) 0:失败(未知错误) 1:成功2:余额不足3:绑定了本机器，但是账户余额不足50元. 4:注册码错误5:你的机器或者IP在黑名单列表中或者不在白名单列表中. 6:非法使用插件.一般出现在定制插件时，使用了和绑定的用户名不同的注册码.也有可能是系统的语言设置不是中文简体,也可能有这个错误. 7:你的帐号因为非法使用被封禁.（如果是在虚拟机中使用插件，必须使用Reg或者RegEx，不能使用RegNoMac或者RegExNoMac,否则可能会造成封号，或者封禁机器）8: ver_info不在你设置的附加白名单中. 77：机器码或者IP因为非法使用，而被封禁.（如果是在虚拟机中使用插件，必须使用Reg或者RegEx，不能使用RegNoMac或者RegExNoMac,否则可能会造成封号，或者封禁机器）封禁是全局的，如果使用了别人的软件导致77，也一样会导致所有注册码均无法注册。解决办法是更换IP，更换MAC. 777：同一个机器码注册次数超过了服务器限制,被暂时封禁.请登录后台，插件今日详细消费记录里，相应的机器码是否有次数异常，并立刻优化解决.如果还有问题，可以联系我来解决. -8:版本附加信息长度超过了32 -9:版本附加信息里包含了非法字母. -10:非法的参数ip空:这是不可能返回空的，如果出现空，那肯定是当前使用的版本不对,老的插件里没这个函数导致返回为空.最好参考文档中的标准写法,判断插件版本号.

**示例**：//严重注意,这个例子的IP只是示例，实际并无效。真实IP要去VIP群里获取. dm_ret=dm.RegEx("abcdefg","0001","123.45.4.6: 8088|78.79.26.3") if dm_ret<>1 then MessageBox"注册失败,只能使用免费功能"end if

**注**：简单游平台调用此函数，不会扣费.注册码在大漠插件后台可以查看.此函数每个进程调用一次即可，不需要每个DM对象都调用.必须保证此函数在创建完对象以后立即调用，尤其必须在绑定窗口之前调用，否则可能会出现异常.如果有多个进程操作同个窗口，必须保证每个进程要么都调用RegEx,要么都不要调用RegEx，以免出现异常.

----

### dm_regExNoMac

**签名**：`long dm_regExNoMac(const char* reg_code, const char* ver_info, const char* ip)`

**简介**：调用此函数来注册，从而使用插件的高级功能.可以根据指定的IP列表来注册.新手不建议使用!此函数同RegEx函数的不同在于,此函数用于注册的机器码是不带mac地址的.

**原型**：long RegExNoMac(reg_code, ver_info, ip)

**参数定义**

- `reg_code` (字符串)：注册码. (从大漠插件后台获取)

- `ver_info` (字符串)：版本附加信息.可以在后台详细信息查看.可留空.长度不能超过32.并且只能包含数字和字母以及小数点.这个版本信息不是插件版本.

- `ip` (字符串)：插件注册的ip地址.可以用|来组合,依次对ip中的地址进行注册，直到成功. ip地址列表在VIP群中获取.从7.2111开始,这里也可以使用域名的方式。可以自己解析域名到我的IP.比如"1.xxx.com|2.xxx.com"。1.xxx.com和2.xxx.com是自己的域名,解析到我的IP即可.还可以自定义端口.比如"1.xxx.com: 1234|2.xxx.com: 5678"等.如果不指定端口,那么默认使用我内置的端口

**返回值**：整形数:-1:无法连接网络,(可能防火墙拦截,如果可以正常访问大漠插件网站，那就可以肯定是被防火墙拦截) -2:进程没有以管理员方式运行. (出现在win7 win8 vista 2008.建议关闭uac) 0:失败(未知错误) 1:成功2:余额不足3:绑定了本机器，但是账户余额不足50元. 4:注册码错误5:你的机器或者IP在黑名单列表中或者不在白名单列表中. 6:非法使用插件.一般出现在定制插件时，使用了和绑定的用户名不同的注册码.也有可能是系统的语言设置不是中文简体,也可能有这个错误. 7:你的帐号因为非法使用被封禁.（如果是在虚拟机中使用插件，必须使用Reg或者RegEx，不能使用RegNoMac或者RegExNoMac,否则可能会造成封号，或者封禁机器）8: ver_info不在你设置的附加白名单中. 77：机器码或者IP因为非法使用，而被封禁.（如果是在虚拟机中使用插件，必须使用Reg或者RegEx，不能使用RegNoMac或者RegExNoMac,否则可能会造成封号，或者封禁机器）封禁是全局的，如果使用了别人的软件导致77，也一样会导致所有注册码均无法注册。解决办法是更换IP，更换MAC. 777：同一个机器码注册次数超过了服务器限制,被暂时封禁.请登录后台，插件今日详细消费记录里，相应的机器码是否有次数异常，并立刻优化解决.如果还有问题，可以联系我来解决. -8:版本附加信息长度超过了32 -9:版本附加信息里包含了非法字母. -10:非法的参数ip空:这是不可能返回空的，如果出现空，那肯定是当前使用的版本不对,老的插件里没这个函数导致返回为空.最好参考文档中的标准写法,判断插件版本号.

**示例**：//严重注意,这个例子的IP只是示例，实际并无效。真实IP要去VIP群里获取. dm_ret=dm.RegExNoMac("abcdefg","0001","123.45.4.6: 8088|78.79.26.3") if dm_ret<>1 then MessageBox"注册失败,只能使用免费功能"end if

**注**：简单游平台调用此函数，不会扣费.注册码在大漠插件后台可以查看.此函数每个进程调用一次即可，不需要每个DM对象都调用.必须保证此函数在创建完对象以后立即调用，尤其必须在绑定窗口之前调用，否则可能会出现异常.如果有多个进程操作同个窗口，必须保证每个进程要么都调用RegExNoMac,要么都不要调用RegExNoMac，以免出现异常.

----

### dm_regNoMac

**签名**：`long dm_regNoMac(const char* reg_code, const char* ver_info)`

**简介**：调用此函数来注册，从而使用插件的高级功能.推荐使用此函数.新手不建议使用!此函数同Reg函数的不同在于,此函数用于注册的机器码是不带mac地址的.

**原型**：long RegNoMac(reg_code, ver_info)

**参数定义**

- `reg_code` (字符串)：注册码. (从大漠插件后台获取)

- `ver_info` (字符串)：版本附加信息.可以在后台详细信息查看.可以任意填写.可留空.长度不能超过32.并且只能包含数字和字母以及小数点.这个版本信息不是插件版本.

**返回值**：整形数:-1:无法连接网络,(可能防火墙拦截,如果可以正常访问大漠插件网站，那就可以肯定是被防火墙拦截) -2:进程没有以管理员方式运行. (出现在win7 win8 vista 2008.建议关闭uac) 0:失败(未知错误) 1:成功2:余额不足3:绑定了本机器，但是账户余额不足50元. 4:注册码错误5:你的机器或者IP在黑名单列表中或者不在白名单列表中. 6:非法使用插件.一般出现在定制插件时，使用了和绑定的用户名不同的注册码.也有可能是系统的语言设置不是中文简体,也可能有这个错误. 7:你的帐号因为非法使用被封禁.（如果是在虚拟机中使用插件，必须使用Reg或者RegEx，不能使用RegNoMac或者RegExNoMac,否则可能会造成封号，或者封禁机器）8: ver_info不在你设置的附加白名单中. 77：机器码或者IP因为非法使用，而被封禁.（如果是在虚拟机中使用插件，必须使用Reg或者RegEx，不能使用RegNoMac或者RegExNoMac,否则可能会造成封号，或者封禁机器）封禁是全局的，如果使用了别人的软件导致77，也一样会导致所有注册码均无法注册。解决办法是更换IP，更换MAC. 777：同一个机器码注册次数超过了服务器限制,被暂时封禁.请登录后台，插件今日详细消费记录里，相应的机器码是否有次数异常，并立刻优化解决.如果还有问题，可以联系我来解决. -8:版本附加信息长度超过了32 -9:版本附加信息里包含了非法字母.空:这是不可能返回空的，如果出现空，那肯定是当前使用的版本不对,老的插件里没这个函数导致返回为空.最好参考文档中的标准写法,判断插件版本号.

**示例**：dm_ret=dm.RegNoMac("abcdefg","") if dm_ret<>1 then MessageBox"注册失败,只能使用免费功能"end if

**注**：简单游平台调用此函数，不会扣费.注册码在大漠插件后台可以查看.此函数每个进程调用一次即可，不需要每个DM对象都调用.必须保证此函数在创建完对象以后立即调用，尤其必须在绑定窗口之前调用，否则可能会出现异常.如果有多个进程操作同个窗口，必须保证每个进程要么都调用RegNoMac,要么都不要调用RegNoMac，以免出现异常.

----

### dm_setPath

**签名**：`long dm_setPath(const char* path)`

**简介**：设置全局路径,设置了此路径后,所有接口调用中,相关的文件都相对于此路径.比如图片,字库等.

**原型**：long SetPath(path)

**参数定义**

- `path` (字符串)：路径,可以是相对路径,也可以是绝对路径

**返回值**：整形数: 0:失败1:成功

**示例**：//以下代码把全局路径设置到了c盘根目录dm_ret=dm.SetPath("c:\")//如下是把全局路径设置到了相对于当前exe所在的路径dm.SetPath".\MyData"//以上，如果exe在c:\test\a.exe那么，就相当于把路径设置到了c:\test\MyData

----

### dm_setShowErrorMsg

**签名**：`long dm_setShowErrorMsg(long show)`

**简介**：设置是否弹出错误信息,默认是打开.

**原型**：long SetShowErrorMsg(show)

**参数定义**

- `show` (整形数)：0表示不打开, 1表示打开

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetShowErrorMsg (0)

----

### dm_ver

**签名**：`const char* dm_ver()`

**简介**：返回当前插件版本号

**原型**：string Ver()

**返回值**：字符串:当前插件的版本描述字符串

**示例**：//返回版本号ver=dm.Ver() MessageBox ver

----

### dm_clientToScreen

**签名**：`long dm_clientToScreen(intptr_t hwnd, long* x, long* y)`

**简介**：把窗口坐标转换为屏幕坐标

**原型**：long ClientToScreen(hwnd, x, y)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `x` (变参指针)：窗口X坐标

- `y` (变参指针)：窗口Y坐标

**返回值**：整形数: 0:失败1:成功

**示例**：x=0: y=0 dm_ret=dm.ClientToScreen(hwnd, x, y)

----

### dm_enumProcess

**签名**：`long dm_enumProcess(const char* name)`

**简介**：根据指定进程名,枚举系统中符合条件的进程PID,并且按照进程打开顺序排序.

**原型**：string EnumProcess(name)

**参数定义**

- `name` (字符串)：进程名,比如qq. exe

**返回值**：字符串:返回所有匹配的进程PID,并按打开顺序排序,格式"pid1, pid2, pid3"

**示例**：pids=dm.EnumProcess("notepad.exe") pids=split(pids,",")转换为数组后,就可以处理了这里注意, pids数组里的是字符串,要用于使用,还得强制类型转换,比如clng(pids(0))

----

### dm_enumWindow

**签名**：`long dm_enumWindow(intptr_t parent, const char* title, long filter)`

**简介**：根据指定条件,枚举系统中符合条件的窗口,可以枚举到按键自带的无法枚举到的窗口

**原型**：string EnumWindow(parent, title, class_name, filter)

**参数定义**

- `parent` (整形数)：获得的窗口句柄是该窗口的子窗口的窗口句柄,取0时为获得桌面句柄

- `title` (字符串)：窗口标题.此参数是模糊匹配.

- `class_name` (字符串)：窗口类名.此参数是模糊匹配.

- `filter` (整形数)：取值定义如下1:匹配窗口标题,参数title有效2:匹配窗口类名,参数class_name有效. 4:只匹配指定父窗口的第一层孩子窗口8:匹配父窗口为0的窗口,即顶级窗口16:匹配可见的窗口32:匹配出的窗口按照窗口打开顺序依次排列这些值可以相加,比如4+8+16就是类似于任务管理器中的窗口列表

**返回值**：字符串:返回所有匹配的窗口句柄字符串,格式"hwnd1, hwnd2, hwnd3"

**示例**：hwnds=dm.EnumWindow(0,"QQ三国","", 1+4+8+16)这句是获取到所有标题栏中有QQ三国这个字符串的窗口句柄集合hwnds=split(hwnds,",")转换为数组后,就可以处理了这里注意, hwnds数组里的是字符串,要用于使用,比如BindWindow时,还得强制类型转换,比如int(hwnds(0))

----

### dm_enumWindowByProcess

**签名**：`long dm_enumWindowByProcess(const char* process_name, const char* title, long filter)`

**简介**：根据指定进程以及其它条件,枚举系统中符合条件的窗口,可以枚举到按键自带的无法枚举到的窗口

**原型**：string EnumWindowByProcess (process_name, title, class_name, filter)

**参数定义**

- `process_name` (字符串)：进程映像名.比如(svchost.exe).此参数是精确匹配,但不区分大小写.

- `title` (字符串)：窗口标题.此参数是模糊匹配.

- `class_name` (字符串)：窗口类名.此参数是模糊匹配.

- `filter` (整形数)：取值定义如下1:匹配窗口标题,参数title有效2:匹配窗口类名,参数class_name有效4:只匹配指定映像的所对应的第一个进程.可能有很多同映像名的进程，只匹配第一个进程的. 8:匹配父窗口为0的窗口,即顶级窗口16:匹配可见的窗口32:匹配出的窗口按照窗口打开顺序依次排列这些值可以相加,比如4+8+16

**返回值**：字符串:返回所有匹配的窗口句柄字符串,格式"hwnd1, hwnd2, hwnd3"

**示例**：hwnds=dm.EnumWindowByProcess ("game.exe","天龙八部","", 1+8+16)这句是获取到所有标题栏中有"天龙八部"这个字符串的窗口句柄集合,并且所在进程是"game.exe"指定的进程集合. hwnds=split(hwnds,",")转换为数组后,就可以处理了这里注意, hwnds数组里的是字符串,要用于使用,比如BindWindow时,还得强制类型转换,比如int (hwnds (0))

----

### dm_enumWindowByProcessId

**签名**：`long dm_enumWindowByProcessId(long pid, const char* title, long filter)`

**简介**：根据指定进程pid以及其它条件,枚举系统中符合条件的窗口,可以枚举到按键自带的无法枚举到的窗口

**原型**：string EnumWindowByProcessId (pid, title, class_name, filter)

**参数定义**

- `pid` (整形数)：进程pid .

- `title` (字符串)：窗口标题.此参数是模糊匹配.

- `class_name` (字符串)：窗口类名.此参数是模糊匹配.

- `filter` (整形数)：取值定义如下1:匹配窗口标题,参数title有效2:匹配窗口类名,参数class_name有效8:匹配父窗口为0的窗口,即顶级窗口16:匹配可见的窗口这些值可以相加,比如2+8+16

**返回值**：字符串:返回所有匹配的窗口句柄字符串,格式"hwnd1, hwnd2, hwnd3"

**示例**：hwnds=dm.EnumWindowByProcessId (1124,"天龙八部","", 1+8+16)这句是获取到所有标题栏中有"天龙八部"这个字符串的窗口句柄集合,并且所在进程是1124指定的进程. hwnds=split(hwnds,",")转换为数组后,就可以处理了这里注意, hwnds数组里的是字符串,要用于使用,比如BindWindow时,还得强制类型转换,比如int (hwnds (0))

----

### dm_enumWindowSuper

**签名**：`long dm_enumWindowSuper(const char* spec1, long flag1, const char* spec2, long flag2, long filter)`

**简介**：根据两组设定条件来枚举指定窗口.

**原型**：string EnumWindowSuper(spec1, flag1, type1, spec2, flag2, type2, sort)

**参数定义**

- `spec1` (字符串)：查找串1. (内容取决于flag1的值)

- `flag1` (整形数)：取值如下: 0表示spec1的内容是标题1表示spec1的内容是程序名字. (比如notepad) 2表示spec1的内容是类名3表示spec1的内容是程序路径.(不包含盘符,比如\windows\system32) 4表示spec1的内容是父句柄.(十进制表达的串) 5表示spec1的内容是父窗口标题6表示spec1的内容是父窗口类名7表示spec1的内容是顶级窗口句柄.(十进制表达的串) 8表示spec1的内容是顶级窗口标题9表示spec1的内容是顶级窗口类名

- `type1` (整形数)：取值如下0精确判断1模糊判断

- `spec2` (字符串)：查找串2. (内容取决于flag2的值)

- `flag2` (整形数)：取值如下: 0表示spec2的内容是标题1表示spec2的内容是程序名字. (比如notepad) 2表示spec2的内容是类名3表示spec2的内容是程序路径.(不包含盘符,比如\windows\system32) 4表示spec2的内容是父句柄.(十进制表达的串) 5表示spec2的内容是父窗口标题6表示spec2的内容是父窗口类名7表示spec2的内容是顶级窗口句柄.(十进制表达的串) 8表示spec2的内容是顶级窗口标题9表示spec2的内容是顶级窗口类名

- `type2` (整形数)：取值如下0精确判断1模糊判断

- `sort` (整形数)：取值如下0不排序. 1对枚举出的窗口进行排序,按照窗口打开顺序.

**返回值**：字符串:返回所有匹配的窗口句柄字符串,格式"hwnd1, hwnd2, hwnd3"

**示例**：hwnds=dm.EnumWindowSuper("记事本", 0, 1,"notepad", 1, 0, 0) hwnds=split(hwnds,",")转换为数组后,就可以处理了这里注意, hwnds数组里的是字符串,要用于使用,比如BindWindow时,还得强制类型转换,比如int(hwnds(0))

----

### dm_findWindow

**签名**：`long dm_findWindow(const char* cls, const char* title)`

**简介**：查找符合类名或者标题名的顶层可见窗口

**原型**：long FindWindow (class, title)

**参数定义**

- `class` (字符串)：窗口类名，如果为空，则匹配所有.这里的匹配是模糊匹配.

- `title` (字符串)：窗口标题,如果为空，则匹配所有.这里的匹配是模糊匹配.

**返回值**：整形数:整形数表示的窗口句柄，没找到返回0

**示例**：hwnd=dm.FindWindow ("","记事本")

----

### dm_findWindowByProcess

**签名**：`long dm_findWindowByProcess(const char* process_name, const char* cls, const char* title)`

**简介**：根据指定的进程名字，来查找可见窗口.

**原型**：long FindWindowByProcess (process_name, class, title)

**参数定义**

- `process_name` (字符串)：进程名.比如(notepad.exe).这里是精确匹配,但不区分大小写.

- `class` (字符串)：窗口类名，如果为空，则匹配所有.这里的匹配是模糊匹配.

- `title` (字符串)：窗口标题,如果为空，则匹配所有.这里的匹配是模糊匹配.

**返回值**：整形数:整形数表示的窗口句柄，没找到返回0

**示例**：hwnd=dm.FindWindowByProcess ("noteapd.exe","","记事本")

----

### dm_findWindowByProcessId

**签名**：`long dm_findWindowByProcessId(long process_id, const char* cls, const char* title)`

**简介**：根据指定的进程Id，来查找可见窗口.

**原型**：long FindWindowByProcessId(process_id, class, title)

**参数定义**

- `process_id` (整形数)：进程id.

- `class` (字符串)：窗口类名，如果为空，则匹配所有.这里的匹配是模糊匹配.

- `title` (字符串)：窗口标题,如果为空，则匹配所有.这里的匹配是模糊匹配.

**返回值**：整形数:整形数表示的窗口句柄，没找到返回0

**示例**：hwnd=dm.FindWindowByProcessId(123456,"","记事本")

----

### dm_findWindowEx

**签名**：`long dm_findWindowEx(intptr_t parent, long cls, const char* title)`

**简介**：查找符合类名或者标题名的顶层可见窗口,如果指定了parent,则在parent的第一层子窗口中查找.

**原型**：long FindWindowEx (parent, class, title)

**参数定义**

- `parent` (整形数)：父窗口句柄，如果为空，则匹配所有顶层窗口

- `class` (字符串)：窗口类名，如果为空，则匹配所有.这里的匹配是模糊匹配.

- `title` (字符串)：窗口标题,如果为空，则匹配所有.这里的匹配是模糊匹配.

**返回值**：整形数:整形数表示的窗口句柄，没找到返回0

**示例**：hwnd=dm.FindWindowEx (0,"","记事本")

----

### dm_findWindowSuper

**签名**：`long dm_findWindowSuper(const char* cls, const char* title, long flag)`

**简介**：根据两组设定条件来查找指定窗口.

**原型**：long FindWindowSuper(spec1, flag1, type1, spec2, flag2, type2)

**参数定义**

- `spec1` (字符串)：查找串1. (内容取决于flag1的值)

- `flag1` (整形数)：取值如下: 0表示spec1的内容是标题1表示spec1的内容是程序名字. (比如notepad) 2表示spec1的内容是类名3表示spec1的内容是程序路径.(不包含盘符,比如\windows\system32) 4表示spec1的内容是父句柄.(十进制表达的串) 5表示spec1的内容是父窗口标题6表示spec1的内容是父窗口类名7表示spec1的内容是顶级窗口句柄.(十进制表达的串) 8表示spec1的内容是顶级窗口标题9表示spec1的内容是顶级窗口类名

- `type1` (整形数)：取值如下0精确判断1模糊判断

- `spec2` (字符串)：查找串2. (内容取决于flag2的值)

- `flag2` (整形数)：取值如下: 0表示spec2的内容是标题1表示spec2的内容是程序名字. (比如notepad) 2表示spec2的内容是类名3表示spec2的内容是程序路径.(不包含盘符,比如\windows\system32) 4表示spec2的内容是父句柄.(十进制表达的串) 5表示spec2的内容是父窗口标题6表示spec2的内容是父窗口类名7表示spec2的内容是顶级窗口句柄.(十进制表达的串) 8表示spec2的内容是顶级窗口标题9表示spec2的内容是顶级窗口类名

- `type2` (整形数)：取值如下0精确判断1模糊判断

**返回值**：整形数:整形数表示的窗口句柄，没找到返回0

**示例**：hwnd=dm.FindWindowSuper("记事本", 0, 1,"notepad", 1, 0)

----

### dm_getClientRect

**签名**：`long dm_getClientRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2)`

**简介**：获取窗口客户区域在屏幕上的位置

**原型**：long GetClientRect(hwnd, x1, y1, x2, y2)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `x1` (变参指针)：返回窗口客户区左上角X坐标

- `y1` (变参指针)：返回窗口客户区左上角Y坐标

- `x2` (变参指针)：返回窗口客户区右下角X坐标

- `y2` (变参指针)：返回窗口客户区右下角Y坐标

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.GetClientRect(hwnd, x1, y1, x2, y2)

----

### dm_getClientSize

**签名**：`long dm_getClientSize(intptr_t hwnd, long* width, long* height)`

**简介**：获取窗口客户区域的宽度和高度

**原型**：long GetClientSize(hwnd, width, height)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `width` (变参指针)：宽度

- `height` (变参指针)：高度

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.GetClientSize(hwnd, w, h) TracePrint"宽度:"&w&",高度:"&h

----

### dm_getForegroundFocus

**签名**：`long dm_getForegroundFocus()`

**简介**：获取顶层活动窗口中具有输入焦点的窗口句柄

**原型**：long GetForegroundFocus()

**返回值**：整形数:返回整型表示的窗口句柄

**示例**：hwnd=dm.GetForegroundFocus()

----

### dm_getForegroundWindow

**签名**：`long dm_getForegroundWindow()`

**简介**：获取顶层活动窗口,可以获取到按键自带插件无法获取到的句柄

**原型**：long GetForegroundWindow()

**返回值**：整形数:返回整型表示的窗口句柄

**示例**：hwnd=dm.GetForegroundWindow()

----

### dm_getMousePointWindow

**签名**：`long dm_getMousePointWindow()`

**简介**：获取鼠标指向的可见窗口句柄,可以获取到按键自带的插件无法获取到的句柄

**原型**：long GetMousePointWindow ()

**返回值**：整形数:返回整型表示的窗口句柄

**示例**：hwnd=dm.GetMousePointWindow ()

----

### dm_getPointWindow

**签名**：`long dm_getPointWindow(long x, long y)`

**简介**：获取给定坐标的可见窗口句柄,可以获取到按键自带的插件无法获取到的句柄

**原型**：long GetPointWindow (x, y)

**参数定义**

- `X` (整形数)：屏幕X坐标

- `Y` (整形数)：屏幕Y坐标

**返回值**：整形数:返回整型表示的窗口句柄

**示例**：hwnd=dm.GetPointWindow (100, 100)

----

### dm_getProcessInfo

**签名**：`long dm_getProcessInfo(long pid, const char* type)`

**简介**：根据指定的pid获取进程详细信息,(进程名,进程全路径, CPU占用率(百分比),内存占用量(字节))

**原型**：string GetProcessInfo (pid)

**参数定义**

- `pid` (整形数)：进程pid

**返回值**：字符串:格式"进程名|进程路径|cpu|内存"

**示例**：infos=dm.GetProcessInfo (1348) infos=split(infos,"|") TracePrint"进程名:"&infos (0) TracePrint"进程路径:"&infos (1) TracePrint"进程CPU占用率(百分比):"&infos (2) TracePrint"进程内存占用量(字节):"&infos (3)

**注**：有些时候有保护的时候，此函数返回内容会错误，那么此时可以尝试用memory保护盾来试试看.另外此接口调用会延迟1秒.

----

### dm_getSpecialWindow

**签名**：`long dm_getSpecialWindow(long flag)`

**简介**：获取特殊窗口

**原型**：long GetSpecialWindow(flag)

**参数定义**

- `Flag` (整形数)：取值定义如下0:获取桌面窗口1:获取任务栏窗口

**返回值**：整形数:以整型数表示的窗口句柄

**示例**：desk_win=dm.GetSpecialWindow(0)

----

### dm_getWindow

**签名**：`long dm_getWindow(intptr_t hwnd, long flag)`

**简介**：获取给定窗口相关的窗口句柄

**原型**：long GetWindow(hwnd, flag)

**参数定义**

- `hwnd` (整形数)：窗口句柄

- `flag` (整形数)：取值定义如下0:获取父窗口1:获取第一个儿子窗口2:获取First窗口3:获取Last窗口4:获取下一个窗口5:获取上一个窗口6:获取拥有者窗口7:获取顶层窗口

**返回值**：整形数:返回整型表示的窗口句柄

**示例**：own_hwnd=dm.GetWindow(hwnd, 6)

----

### dm_getWindowClass

**签名**：`const char* dm_getWindowClass(intptr_t hwnd)`

**简介**：获取窗口的类名

**原型**：string GetWindowClass(hwnd)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

**返回值**：字符串:窗口的类名

**示例**：class_name=dm.GetWindowClass(hwnd)

----

### dm_getWindowProcessId

**签名**：`long dm_getWindowProcessId(intptr_t hwnd)`

**简介**：获取指定窗口所在的进程ID.

**原型**：long GetWindowProcessId(hwnd)

**参数定义**

- `hwnd` (整形数)：窗口句柄

**返回值**：整形数:返回整型表示的是进程ID

**示例**：process_id=dm.GetWindowProcessId(hwnd)

----

### dm_getWindowProcessPath

**签名**：`const char* dm_getWindowProcessPath(intptr_t hwnd)`

**简介**：获取指定窗口所在的进程的exe文件全路径.

**原型**：string GetWindowProcessPath(hwnd)

**参数定义**

- `hwnd` (整形数)：窗口句柄

**返回值**：字符串:返回字符串表示的是exe全路径名

**示例**：process_path=dm.GetWindowProcessPath(hwnd)

----

### dm_getWindowRect

**签名**：`long dm_getWindowRect(intptr_t hwnd, long* x1, long* y1, long* x2, long* y2)`

**简介**：获取窗口在屏幕上的位置

**原型**：long GetWindowRect(hwnd, x1, y1, x2, y2)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `x1` (变参指针)：返回窗口左上角X坐标

- `y1` (变参指针)：返回窗口左上角Y坐标

- `x2` (变参指针)：返回窗口右下角X坐标

- `y2` (变参指针)：返回窗口右下角Y坐标

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.GetWindowRect(hwnd, x1, y1, x2, y2)

----

### dm_getWindowState

**签名**：`long dm_getWindowState(intptr_t hwnd, long flag)`

**简介**：获取指定窗口的一些属性

**原型**：long GetWindowState (hwnd, flag)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `flag` (整形数)：取值定义如下0:判断窗口是否存在1:判断窗口是否处于激活2:判断窗口是否可见3:判断窗口是否最小化4:判断窗口是否最大化5:判断窗口是否置顶6:判断窗口是否无响应7:判断窗口是否可用(灰色为不可用) 8:另外的方式判断窗口是否无响应,如果6无效可以尝试这个9:判断窗口所在进程是不是64位

**返回值**：整形数: 0:不满足条件1:满足条件

**示例**：dm_ret=dm.GetWindowState (hwnd, 3) If dm_ret=1 Then MessageBox"窗口已经最小化了"End If

----

### dm_getWindowThreadId

**签名**：`long dm_getWindowThreadId(intptr_t hwnd)`

**简介**：获取指定窗口所在的线程ID.

**原型**：long GetWindowThreadId(hwnd)

**参数定义**

- `hwnd` (整形数)：窗口句柄

**返回值**：整形数:返回整型表示的是线程ID

**示例**：thread_id=dm.GetWindowThreadId(hwnd)

----

### dm_getWindowTitle

**签名**：`const char* dm_getWindowTitle(intptr_t hwnd)`

**简介**：获取窗口的标题

**原型**：string GetWindowTitle(hwnd)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

**返回值**：字符串:窗口的标题

**示例**：title=dm.GetWindowTitle(hwnd)

----

### dm_moveWindow

**签名**：`long dm_moveWindow(intptr_t hwnd, long x, long y)`

**简介**：移动指定窗口到指定位置

**原型**：long MoveWindow(hwnd, x, y)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `x` (整形数)：X坐标

- `y` (整形数)：Y坐标

**返回值**：整形数: 0:失败1:成功

**示例**：dm.MoveWindow hwnd,-10,-10

----

### dm_screenToClient

**签名**：`long dm_screenToClient(intptr_t hwnd, long* x, long* y)`

**简介**：把屏幕坐标转换为窗口坐标

**原型**：long ScreenToClient(hwnd, x, y)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `x` (变参指针)：屏幕X坐标

- `y` (变参指针)：屏幕Y坐标

**返回值**：整形数: 0:失败1:成功

**示例**：x=100: y=100 dm_ret=dm.ScreenToClient(hwnd, x, y)

----

### dm_sendPaste

**签名**：`long dm_sendPaste(intptr_t hwnd)`

**简介**：向指定窗口发送粘贴命令.把剪贴板的内容发送到目标窗口.

**原型**：long SendPaste (hwnd)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄.如果为0,则对当前激活的窗口发送.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetClipboard"abcd"dm.SendPaste hwnd

**注**：剪贴板是公共资源，多个线程同时设置剪贴板时,会产生冲突，必须用互斥信号保护.

----

### dm_sendString

**签名**：`long dm_sendString(intptr_t hwnd, const char* str)`

**简介**：向指定窗口发送文本数据

**原型**：long SendString(hwnd, str)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄.如果为0,则对当前激活的窗口发送.

- `str` (字符串)：发送的文本数据

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SendString hwnd,"我是来测试的"注：有时候发送中文，可能会大部分机器正常，少部分会乱码。这种情况一般有两个可能1.系统编码没有设置为GBK 2.目标程序里可能安装了改变当前编码的软件，比如常见的是输入法.（尝试卸载）

----

### dm_sendString2

**签名**：`long dm_sendString2(intptr_t hwnd, const char* str)`

**简介**：向指定窗口发送文本数据

**原型**：long SendString2(hwnd, str)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄.如果为0,则对当前激活的窗口发送.

- `str` (字符串)：发送的文本数据

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SendString2 hwnd,"我是来测试的"

**注**：此接口为老的SendString，如果新的SendString不能输入，可以尝试此接口.有时候发送中文，可能会大部分机器正常，少部分会乱码。这种情况一般有两个可能1.系统编码没有设置为GBK 2.目标程序里可能安装了改变当前编码的软件，比如常见的是输入法.（尝试卸载）

----

### dm_sendStringIme

**签名**：`long dm_sendStringIme(intptr_t hwnd, const char* str)`

**简介**：向绑定的窗口发送文本数据.必须配合dx.public.input.ime属性.

**原型**：long SendStringIme (str)

**参数定义**

- `str` (字符串)：发送的文本数据

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindowEx (hwnd,"normal","normal","normal","dx.public.input.ime", 0) dm.SendStringIme"我是来测试的"

----

### dm_sendStringIme2

**签名**：`long dm_sendStringIme2(intptr_t hwnd, const char* str)`

**简介**：利用真实的输入法，对指定的窗口输入文字.

**原型**：long SendStringIme2(hwnd, str, mode)

**参数定义**

- `hwnd` (整形数)：窗口句柄

- `str` (字符串)：发送的文本数据

- `mode` (整形数)：取值意义如下: 0:向hwnd的窗口输入文字(前提是必须先用模式200安装了输入法) 1:同模式0,如果由于保护无效，可以尝试此模式.(前提是必须先用模式200安装了输入法) 2:同模式0,如果由于保护无效，可以尝试此模式. (前提是必须先用模式200安装了输入法) 200:向系统中安装输入法,多次调用没问题.全局只用安装一次. 300:卸载系统中的输入法.全局只用卸载一次.多次调用没关系.

**返回值**：整形数: 0:失败1:成功

**示例**：If dm.SendStringIme2(hwnd,"", 200)=1 then dm.SendStringIme2 hwnd,"我是来测试的", 0 dm.SendStringIme2 hwnd,"abc", 0 dm.SendStringIme2 hwnd,"123", 0 dm.SendStringIme2 hwnd,"", 300 end if

**注**：如果要同时对此窗口进行绑定，并且绑定的模式是1 3 5 7 101 103，那么您必须要在绑定之前,先执行加载输入法的操作.否则会造成绑定失败!.卸载时，没有限制.还有，在后台输入时，如果目标窗口有判断是否在激活状态才接受输入文字,那么可以配合绑定窗口中的假激活属性来保证文字正常输入.诸如此类.基本上用这个没有输入不了的文字.比如BindWindow hwnd,"normal","normal","normal","dx.public.active.api|dx.public.active.message", 0 dm.SendStringIme2 hwnd,"哈哈", 0

----

### dm_setClientSize

**签名**：`long dm_setClientSize(intptr_t hwnd, long width, long height)`

**简介**：设置窗口客户区域的宽度和高度

**原型**：long SetClientSize(hwnd, width, height)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `width` (整形数)：宽度

- `height` (整形数)：高度

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetClientSize(hwnd, 800, 600)

----

### dm_setSendStringDelay

**签名**：`long dm_setSendStringDelay(long delay)`

**简介**：设置SendString和SendString2的每个字符之间的发送间隔.有些窗口必须设置延迟才可以正常发送.否则可能会顺序错乱.

**原型**：long SetSendStringDelay(delay)

**参数定义**

- `delay` (整形数)：大于等于0的延迟数值.单位是毫秒.默认是0

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetSendStringDelay 100 dm.SendString hwnd,"abcd"

----

### dm_setWindowSize

**签名**：`long dm_setWindowSize(intptr_t hwnd, long width, long height)`

**简介**：设置窗口的大小

**原型**：long SetWindowSize(hwnd, width, height)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `width` (整形数)：宽度

- `height` (整形数)：高度

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetWindowSize(hwnd, 300, 400)

----

### dm_setWindowState

**签名**：`long dm_setWindowState(intptr_t hwnd, long flag)`

**简介**：设置窗口的状态

**原型**：long SetWindowState(hwnd, flag)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `flag` (整形数)：取值定义如下0:关闭指定窗口1:激活指定窗口2:最小化指定窗口,但不激活3:最小化指定窗口,并释放内存,但同时也会激活窗口.(释放内存可以考虑用FreeProcessMemory函数) 4:最大化指定窗口,同时激活窗口. 5:恢复指定窗口,但不激活6:隐藏指定窗口7:显示指定窗口8:置顶指定窗口9:取消置顶指定窗口10:禁止指定窗口11:取消禁止指定窗口12:恢复并激活指定窗口13:强制结束窗口所在进程. 14:闪烁指定的窗口15:使指定的窗口获取输入焦点

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetWindowState(hwnd, 0)

----

### dm_setWindowText

**签名**：`long dm_setWindowText(intptr_t hwnd, const char* title)`

**简介**：设置窗口的标题

**原型**：long SetWindowText(hwnd, title)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `titie` (字符串)：标题

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetWindowText(hwnd,"test")

----

### dm_setWindowTransparent

**签名**：`long dm_setWindowTransparent(intptr_t hwnd, long trans)`

**简介**：设置窗口的透明度

**原型**：long SetWindowTransparent(hwnd, trans)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `trans` (整形数)：透明度取值(0-255)越小透明度越大0为完全透明(不可见) 255为完全显示(不透明)

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetWindowTransparent(hwnd, 200)

**注**：此接口不支持WIN98

----

### dm_enableMouseAccuracy

**签名**：`long dm_enableMouseAccuracy(long enable)`

**简介**：设置当前系统鼠标的精确度开关.如果所示。此接口仅仅对前台MoveR接口起作用.

**原型**：long EnableMouseAccuracy (enable)

**参数定义**

- `enable` (整形数)：0关闭指针精确度开关. 1打开指针精确度开关.一般推荐关闭.

**返回值**：整形数:设置之前的精确度开关.

**示例**：dm.SetMouseAccuracy 0

----

### dm_getCursorPos

**签名**：`long dm_getCursorPos(long* x, long* y)`

**简介**：获取鼠标位置.

**原型**：long GetCursorPos (x, y)

**参数定义**

- `x` (变参指针)：返回X坐标

- `y` (变参指针)：返回Y坐标

**返回值**：整形数: 0:失败1:成功

**示例**：dm.GetCursorPos x, y MessageBox x&","&y

**注**：此接口在3.1223版本之后，返回的值的定义修改。同大多数接口一样,返回的x, y坐标是根据绑定的鼠标参数来决定.如果绑定了窗口，那么获取的坐标是相对于绑定窗口，否则是屏幕坐标.另外，此函数获取的坐标是真实的鼠标坐标，对于某些自绘鼠标位置不一定准确。请自行测试.

----

### dm_getCursorShape

**签名**：`long dm_getCursorShape()`

**简介**：获取鼠标特征码.当BindWindow或者BindWindowEx中的mouse参数含有dx.mouse.cursor时，获取到的是后台鼠标特征，否则是前台鼠标特征.关于如何识别后台鼠标特征.

**原型**：string GetCursorShape ()

**返回值**：字符串:成功时，返回鼠标特征码.失败时，返回空的串.

**示例**：mouse_tz=dm.GetCursorShape () If mouse_tz="7d7160fe"Then MessageBox"找到特征码"End If

**注**：此接口和GetCursorShapeEx (0)等效.相当于工具里的方式1获取的特征码.当此特征码在某些情况下无法区分鼠标形状时，可以考虑使用GetCursorShapeEx (1).另要特别注意, WIN7以及以上系统，必须在字体显示设置里把文字大小调整为默认(100%),否则特征码会变.如图所示.

----

### dm_getCursorShapeEx

**签名**：`const char* dm_getCursorShapeEx(long type)`

**简介**：获取鼠标特征码.当BindWindow或者BindWindowEx中的mouse参数含有dx.mouse.cursor时，获取到的是后台鼠标特征，否则是前台鼠标特征.关于如何识别后台鼠标特征.

**原型**：string GetCursorShapeEx (int type)

**参数定义**

- `type` (整形数)：获取鼠标特征码的方式.和工具中的方式1方式2对应.方式1此参数值为0.方式2此参数值为1.

**返回值**：字符串:成功时，返回鼠标特征码.失败时，返回空的串.

**示例**：mouse_tz=dm.GetCursorShapeEx (0) If mouse_tz="7d7160fe"Then MessageBox"找到特征码"End If

**注**：当type为0时，和GetCursorShape等效.另要特别注意, WIN7以及以上系统，必须在字体显示设置里把文字大小调整为默认(100%),否则特征码会变.如图所示.

----

### dm_getCursorSpot

**签名**：`long dm_getCursorSpot()`

**简介**：获取鼠标热点位置.(参考工具中抓取鼠标后，那个闪动的点就是热点坐标,不是鼠标坐标)当BindWindow或者BindWindowEx中的mouse参数含有dx.mouse.cursor时，获取到的是后台鼠标热点位置，否则是前台鼠标热点位置.关于如何识别后台鼠标特征.

**原型**：string GetCursorSpot ()

**返回值**：字符串:成功时，返回形如"x, y"的字符串失败时，返回空的串.

**示例**：hot_pos=dm.GetCursorSpot () if len (hot_pos)>0 Then hot_pos=split(hot_pos,",") x=int (hot_pos (0)) y=int (hot_pos (1)) end if

----

### dm_getKeyState

**签名**：`long dm_getKeyState(long vk_code)`

**简介**：获取指定的按键状态.(前台信息,不是后台)

**原型**：long GetKeyState (vk_code)

**参数定义**

- `vk_code` (整形数)：虚拟按键码

**返回值**：整形数: 0:弹起1:按下

**示例**：TracePrint dm.GetKeyState(13)

----

### dm_getMouseSpeed

**签名**：`long dm_getMouseSpeed()`

**简介**：获取系统鼠标的移动速度.如图所示红色区域.一共分为11个级别.从1开始, 11结束.这仅是前台鼠标的速度.后台不用理会这个.

**原型**：long GetMouseSpeed ()

**返回值**：整形数: 0:失败其他值,当前系统鼠标的移动速度

**示例**：TracePrint dm.GetMouseSpeed ()

----

### dm_keyDown

**签名**：`long dm_keyDown(long vk_code)`

**简介**：按住指定的虚拟键码

**原型**：long KeyDown(vk_code)

**参数定义**

- `vk_code` (整形数)：虚拟按键码

**返回值**：整形数: 0:失败1:成功

**示例**：dm.KeyDown 13

----

### dm_keyDownChar

**签名**：`long dm_keyDownChar(const char* key_str)`

**简介**：按住指定的虚拟键码

**原型**：long KeyDownChar(key_str)

**参数定义**

- `key_str` (字符串)：字符串描述的键码.大小写无所谓.点这里查看具体对应关系.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.KeyDownChar"enter"dm.KeyDownChar"1"dm.KeyDownChar"F1"dm.KeyDownChar"a"dm.KeyDownChar"B"

----

### dm_keyPress

**签名**：`long dm_keyPress(long vk_code)`

**简介**：按下指定的虚拟键码

**原型**：long KeyPress(vk_code)

**参数定义**

- `vk_code` (整形数)：虚拟按键码

**返回值**：整形数: 0:失败1:成功

**示例**：dm.KeyPress 13

----

### dm_keyPressChar

**签名**：`long dm_keyPressChar(const char* key_str)`

**简介**：按下指定的虚拟键码

**原型**：long KeyPressChar(key_str)

**参数定义**

- `key_str` (字符串)：字符串描述的键码.大小写无所谓.点这里查看具体对应关系.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.KeyPressChar"enter"dm.KeyPressChar"1"dm.KeyPressChar"F1"dm.KeyPressChar"a"dm.KeyPressChar"B"

----

### dm_keyPressStr

**签名**：`long dm_keyPressStr(const char* key_str, long delay)`

**简介**：根据指定的字符串序列，依次按顺序按下其中的字符.

**原型**：long KeyPressStr(key_str, delay)

**参数定义**

- `key_str` (字符串)：需要按下的字符串序列.比如"1234","abcd","7389, 1462"等.

- `delay` (整形数)：每按下一个按键，需要延时多久.单位毫秒.这个值越大，按的速度越慢。

**返回值**：整形数: 0:失败1:成功

**示例**：dm.KeyPressStr"123, 456", 20

**注**：在某些情况下，SendString和SendString2都无法输入文字时，可以考虑用这个来输入.但这个接口只支持标准ASCII可见字符,其它字符一律不支持.(包括中文)

----

### dm_keyUp

**签名**：`long dm_keyUp(long vk_code)`

**简介**：弹起来虚拟键vk_code

**原型**：long KeyUp(vk_code)

**参数定义**

- `vk_code` (整形数)：虚拟按键码

**返回值**：整形数: 0:失败1:成功

**示例**：dm.KeyUp 13

----

### dm_keyUpChar

**签名**：`long dm_keyUpChar(const char* key_str)`

**简介**：弹起来虚拟键key_str

**原型**：long KeyUpChar(key_str)

**参数定义**

- `key_str` (字符串)：字符串描述的键码.大小写无所谓.点这里查看具体对应关系.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.KeyUpChar"enter"dm.KeyUpChar"1"dm.KeyUpChar"F1"dm.KeyUpChar"a"dm.KeyUpChar"B"

----

### dm_leftClick

**签名**：`long dm_leftClick()`

**简介**：按下鼠标左键

**原型**：long LeftClick()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.LeftClick

----

### dm_leftDoubleClick

**签名**：`long dm_leftDoubleClick()`

**简介**：双击鼠标左键

**原型**：long LeftDoubleClick()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.LeftDoubleClick

----

### dm_leftDown

**签名**：`long dm_leftDown()`

**简介**：按住鼠标左键

**原型**：long LeftDown()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.LeftDown

----

### dm_leftUp

**签名**：`long dm_leftUp()`

**简介**：弹起鼠标左键

**原型**：long LeftUp()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.LeftUp

----

### dm_middleClick

**签名**：`long dm_middleClick()`

**简介**：按下鼠标中键

**原型**：long MiddleClick()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.MiddleClick

----

### dm_middleDown

**签名**：`long dm_middleDown()`

**简介**：按住鼠标中键

**原型**：long MiddleDown()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.MiddleDown

----

### dm_middleUp

**签名**：`long dm_middleUp()`

**简介**：弹起鼠标中键

**原型**：long MiddleUp()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.MiddleUp

----

### dm_moveR

**签名**：`long dm_moveR(long rx, long ry)`

**简介**：鼠标相对于上次的位置移动rx, ry .如果您要使前台鼠标移动的距离和指定的rx, ry一致,最好配合EnableMouseAccuracy函数来使用.

**原型**：long MoveR (rx, ry)

**参数定义**

- `rx` (整形数)：相对于上次的X偏移

- `ry` (整形数)：相对于上次的Y偏移

**返回值**：整形数: 0:失败1:成功

**示例**：dm.MoveR rx, ry

**注**：此函数从6.1550开始，为了兼容某些特殊输入，不再自动设置鼠标的速度和精确度。如果您要使前台鼠标移动的距离和指定的rx, ry一致,那么最好配合=EnableMouseAccuracy函数来使用因为rx和ry的偏移量不一定就是鼠标真实的偏移,而是代表了物理鼠标DPI偏移.如果您需要这个偏移和真实鼠标偏移一致，那么需要如下调用这个函数，如下所示: old_accuracy=dm.EnableMouseAccuracy (0)//关闭精确度开关dm.MoveR 30, 30 dm.EnableMouseAccuracy old_accuracy当然你也可以永久关闭精确度开关.一般来说精确度开关默认都是关闭的.以上这些设置都仅对前台有效.后台是不需要这样设置的.

----

### dm_moveTo

**签名**：`long dm_moveTo(long x, long y)`

**简介**：把鼠标移动到目的点(x, y)

**原型**：long MoveTo(x, y)

**返回值**：整形数: 0:失败1:成功

**示例**：dm.MoveTo x, y

----

### dm_moveToEx

**签名**：`long dm_moveToEx(long x, long y, long w, long h)`

**简介**：把鼠标移动到目的范围内的任意一点

**原型**：string MoveToEx (x, y, w, h)

**参数定义**

- `w` (整形数)：宽度(从x计算起)

- `h` (整形数)：高度(从y计算起)

**返回值**：字符串:返回要移动到的目标点.格式为x, y .比如MoveToEx 100, 100, 10, 10,返回值可能是101, 102

**示例**：//移动鼠标到(100, 100)到(110, 110)这个矩形范围内的任意一点. dm.MoveToEx 100, 100, 10, 10

**注**：此函数的意思是移动鼠标到指定的范围(x, y, x+w, y+h)内的任意随机一点.

----

### dm_rightClick

**签名**：`long dm_rightClick()`

**简介**：按下鼠标右键

**原型**：long RightClick()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.RightClick

----

### dm_rightDown

**签名**：`long dm_rightDown()`

**简介**：按住鼠标右键

**原型**：long RightDown()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.RightDown

----

### dm_rightUp

**签名**：`long dm_rightUp()`

**简介**：弹起鼠标右键

**原型**：long RightUp()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.RightUp

----

### dm_setKeypadDelay

**签名**：`long dm_setKeypadDelay(long type, long delay)`

**简介**：设置按键时,键盘按下和弹起的时间间隔。高级用户使用。某些窗口可能需要调整这个参数才可以正常按键。

**原型**：long SetKeypadDelay(type, delay)

**参数定义**

- `type` (字符串)：键盘类型,取值有以下"normal":对应normal键盘默认内部延时为30ms"windows":对应windows键盘默认内部延时为10ms"dx":对应dx键盘默认内部延时为50ms

- `delay` (整形数)：延时,单位是毫秒

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetKeypadDelay"dx", 10

**注**：此函数影响的接口有KeyPress

----

### dm_setMouseDelay

**签名**：`long dm_setMouseDelay(long type, long delay)`

**简介**：设置鼠标单击或者双击时,鼠标按下和弹起的时间间隔。高级用户使用。某些窗口可能需要调整这个参数才可以正常点击。

**原型**：long SetMouseDelay(type, delay)

**参数定义**

- `type` (字符串)：鼠标类型,取值有以下"normal":对应normal鼠标默认内部延时为30ms"windows":对应windows鼠标默认内部延时为10ms"dx":对应dx鼠标默认内部延时为40ms

- `delay` (整形数)：延时,单位是毫秒

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetMouseDelay"dx", 10

**注**：此函数影响的接口有LeftClick RightClick MiddleClick LeftDoubleClick

----

### dm_setMouseSpeed

**签名**：`long dm_setMouseSpeed(long speed)`

**简介**：设置系统鼠标的移动速度.如图所示红色区域.一共分为11个级别.从1开始, 11结束。此接口仅仅对前台鼠标有效.

**原型**：long SetMouseSpeed (speed)

**参数定义**

- `speed` (整形数)：鼠标移动速度,最小1，最大11.居中为6.推荐设置为6

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetMouseSpeed 6

----

### dm_setSimMode

**签名**：`long dm_setSimMode(long mode)`

**简介**：设置前台键鼠的模拟方式.驱动功能支持的系统版本号为(win7/win8/win8.1/win10(10240)/win10(10586)/win10(14393)/win10(15063)/win10(16299)/win10(17134)/win10(17763)/win10(18362)/win10(18363)/win10(19041)/win10(19042)/win10(19043)/win10(19045)/win11(22000)/win11(22621)/win11(22631)/win11(26100)不支持所有的预览版本,仅仅支持正式版本.除了模式3,其他模式同时支持32位系统和64位系统.

**原型**：long SetSimMode(mode)

**参数定义**

- `mode` (整形数)：0正常模式(默认模式) 1硬件模拟2硬件模拟2(ps2)（仅仅支持标准的3键鼠标，即左键，右键，中键，带滚轮的鼠标, 2键和5键等扩展鼠标不支持）3硬件模拟3

**返回值**：整形数: 0:插件没注册-1: 32位系统不支持-2:驱动释放失败. -3:驱动加载失败.可能是权限不够.参考UAC权限设置.或者是被安全软件拦截.如果是WIN10 1607之后的系统，出现这个错误，可参考这里-10:设置失败-7:系统版本不支持.可以用winver命令查看系统内部版本号.驱动只支持正式发布的版本，所有预览版本都不支持. 1:成功

**示例**：dm.SetSimMode 1…dm_ret=dm.BindWindow(hwnd,"normal","normal","normal", 0) ... dm.UnBindWindow除了模式0,其他方式需要加载驱动，所以调用进程必须有管理员权限,参考如何关闭UAC.加载驱动时，必须让安全软件放行.否则模拟无效.硬件模拟1,没有对键鼠的接口类型有任何限制(PS/2 USB接口)都可以模拟，甚至不插任何键鼠设备都可以模拟.硬件模拟2(ps2),模式下的键盘基本是正常的,但鼠标兼容性很差,建议只适用此模式的键盘.鼠标可以使用别的模式.键盘和鼠标不要求必须插入真实的ps2设备.硬件模拟3,设置以后，必须手动按下需要模拟的键盘和鼠标，否则会卡住.直到按下为止.此后，再次设置不需要重新按下键盘鼠标，直到系统重启.这个模拟要求被指定的键盘和鼠标不可以中途插拔，会造成模拟失效.另外,用模拟3后，最好不要调用MoveTo或者MoveToE接口,改为用MoveR自己实现MoveTo或者MoveToEx,否则可能会造成鼠标移动到屏幕左上角的问题.此接口仅对本对象生效,实际上所有的接口都仅仅对本对象生效,除了DmGuard是全局的.

----

### dm_waitKey

**签名**：`long dm_waitKey(long vk_code, long time_out)`

**简介**：等待指定的按键按下(前台,不是后台)

**原型**：long WaitKey(vk_code, time_out)

**参数定义**

- `vk_code` (整形数)：虚拟按键码,当此值为0，表示等待任意按键。鼠标左键是1,鼠标右键时2,鼠标中键是4.

- `time_out` (整形数)：等待多久,单位毫秒.如果是0，表示一直等待

**返回值**：整形数: 0:超时1:指定的按键按下(当vk_code不为0时)按下的按键码:(当vk_code为0时)

**示例**：dm.WaitKey 66, 0

----

### dm_wheelDown

**签名**：`long dm_wheelDown(long repeat)`

**简介**：滚轮向下滚

**原型**：long WheelDown()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.WheelDown

----

### dm_wheelUp

**签名**：`long dm_wheelUp(long repeat)`

**简介**：滚轮向上滚

**原型**：long WheelUp()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.WheelUp

----

### dm_appendPicAddr

**签名**：`const char* dm_appendPicAddr(const char* pic_info, long addr, long size)`

**简介**：对指定的数据地址和长度，组合成新的参数. FindPicMem FindPicMemE以及FindPicMemEx专用

**原型**：string AppendPicAddr(pic_info, addr, size)

**参数定义**

- `pic_info` (字符串)：老的地址描述串

- `addr` (整形数)：数据地址

- `size` (整形数)：数据长度

**返回值**：字符串:新的地址描述串

**示例**：pic_info=""pic_info=dm.AppendPicAddr(pic_info, 12034, 643) pic_info=dm.AppendPicAddr(pic_info, 328435, 8935) pic_info=dm.AppendPicAddr(pic_info, 809234, 789)

----

### dm_bgr2RGB

**签名**：`long dm_bgr2RGB(const char* bgr_color)`

**简介**：把BGR(按键格式)的颜色格式转换为RGB

**原型**：string BGR2RGB(bgr_color)

**返回值**：字符串: RGB格式的字符串

**示例**：rgb_color=dm.BGR2RGB(bgr_color)

----

### dm_capture

**签名**：`long dm_capture(long x1, long y1, long x2, long y2, const char* file)`

**简介**：抓取指定区域(x1, y1, x2, y2)的图像,保存为file(24位位图)

**原型**：long Capture(x1, y1, x2, y2, file)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `file` (字符串)：保存的文件名,保存的地方一般为SetPath中设置的目录当然这里也可以指定全路径名.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.Capture(0, 0, 2000, 2000,"screen.bmp")

----

### dm_captureGif

**签名**：`long dm_captureGif(long x1, long y1, long x2, long y2, const char* file, long delay, long time)`

**简介**：抓取指定区域(x1, y1, x2, y2)的动画，保存为gif格式

**原型**：long CaptureGif(x1, y1, x2, y2, file, delay, time)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `file` (字符串)：保存的文件名,保存的地方一般为SetPath中设置的目录当然这里也可以指定全路径名.

- `delay` (整形数)：动画间隔，单位毫秒。如果为0，表示只截取静态图片

- `time` (整形数)：总共截取多久的动画，单位毫秒。

**返回值**：整形数: 0:失败1:成功

**示例**：//截取动画dm_ret=dm.CaptureGif(0, 0, 2000, 2000,"screen.gif", 100, 3000)//截取静态dm_ret=dm.CaptureGif(0, 0, 2000, 2000,"screen.gif", 0, 0)

----

### dm_captureJpg

**签名**：`long dm_captureJpg(long x1, long y1, long x2, long y2, const char* file, long quality)`

**简介**：抓取指定区域(x1, y1, x2, y2)的图像,保存为file(JPG压缩格式)

**原型**：long CaptureJpg (x1, y1, x2, y2, file, quality)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `file` (字符串)：保存的文件名,保存的地方一般为SetPath中设置的目录当然这里也可以指定全路径名.

- `quality` (整形数)：jpg压缩比率(1-100)越大图片质量越好

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.CaptureJpg(0, 0, 2000, 2000,"screen.jpg", 50)

----

### dm_capturePng

**签名**：`long dm_capturePng(long x1, long y1, long x2, long y2, const char* file)`

**简介**：同Capture函数，只是保存的格式为PNG.

**原型**：long CapturePng(x1, y1, x2, y2, file)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `file` (字符串)：保存的文件名,保存的地方一般为SetPath中设置的目录当然这里也可以指定全路径名.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.CapturePng(0, 0, 2000, 2000,"screen.png")

----

### dm_capturePre

**签名**：`long dm_capturePre(const char* file)`

**简介**：抓取上次操作的图色区域，保存为file(24位位图)

**原型**：long CapturePre(file)

**参数定义**

- `file` (字符串)：保存的文件名,保存的地方一般为SetPath中设置的目录当然这里也可以指定全路径名.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.CapturePre("screen.bmp")注意，要开启此函数，必须先调用EnableDisplayDebug任何图色或者文字识别函数，都可以通过这个来截取.具体可以查看常见问题中"本机文字识别正常,别的机器为何不正常"这一节.

----

### dm_cmpColor

**签名**：`long dm_cmpColor(long x, long y, const char* color, long sim)`

**简介**：比较指定坐标点(x, y)的颜色

**原型**：long CmpColor (x, y, color, sim)

**参数定义**

- `x` (整形数)：X坐标

- `y` (整形数)：Y坐标

- `color` (字符串)：颜色字符串,可以支持偏色,多色,例如"ffffff-202020|000000-000000"这个表示白色偏色为202020,和黑色偏色为000000.颜色最多支持10种颜色组合.注意，这里只支持RGB颜色.

- `sim` (双精度浮点数)：相似度(0.1-1.0)

**返回值**：整形数: 0:颜色匹配1:颜色不匹配

**示例**：dm_ret=dm.CmpColor (200, 300,"000000-000000|ff00ff-101010", 0.9) If dm_ret=0 Then MessageBox"相等"End If

----

### dm_enableDisplayDebug

**签名**：`long dm_enableDisplayDebug(long enable_debug)`

**简介**：开启图色调试模式，此模式会稍许降低图色和文字识别的速度.默认不开启.

**原型**：long EnableDisplayDebug(enable_debug)

**参数定义**

- `enable_debug` (整形数)：0为关闭1为开启

**返回值**：整形数: 0:失败1:成功

**示例**：dm.EnableDisplayDebug 1 dm_ret=dm.CapturePre("screen.bmp")

----

### dm_enableFindPicMultithread

**签名**：`long dm_enableFindPicMultithread(long enable)`

**简介**：当执行FindPicXXX系列接口时,是否在条件满足下(查找的图片大于等于4,这个值可以根据SetFindPicMultithreadCount来修改),开启多线程查找。默认打开.

**原型**：long EnableFindPicMultithread(enable)

**参数定义**

- `enable` (整形数)：0关闭1打开

**返回值**：整形数: 0:失败1:成功

**示例**：dm.EnableFindPicMultithread 0 dm.FindPicXXX dm.EnableFindPicMultithread 1

**注**：如果担心开启多线程会引发占用大量CPU资源,那么可以考虑关闭此功能.在以往版本,这个功能默认都是打开的.这个只是多线程查找的一个开关,另一个开关是SetFindPicMultithreadCount

----

### dm_enableGetColorByCapture

**签名**：`long dm_enableGetColorByCapture(long enable)`

**简介**：允许调用GetColor GetColorBGR GetColorHSV以及CmpColor时，以截图的方式来获取颜色。默认关闭.

**原型**：long EnableGetColorByCapture(enable)

**参数定义**

- `enable` (整形数)：0关闭1打开

**返回值**：整形数: 0:失败1:成功

**示例**：dm.EnableGetColorByCapture 1 TracePrint dm.GetColor(300, 300)

**注**：某些窗口上，可能GetColor会获取不到颜色，可以尝试此接口.

----

### dm_findColor

**签名**：`long dm_findColor(long x1, long y1, long x2, long y2, const char* color, long sim, long dir, long* intX, long* intY)`

**简介**：查找指定区域内的颜色,颜色格式"RRGGBB-DRDGDB",注意,和按键的颜色格式相反

**原型**：long FindColor (x1, y1, x2, y2, color, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color` (字符串)：颜色格式为"RRGGBB-DRDGDB",比如"123456-000000|aabbcc-202020".也可以支持反色模式.前面加@即可.比如"@123456-000000|aabbcc-202020".具体可以看下放注释.注意，这里只支持RGB颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上4：从中心往外查找5:从上到下,从左到右6:从上到下,从右到左7:从下到上,从左到右8:从下到上,从右到左

- `intX` (变参指针)：返回X坐标

- `intY` (变参指针)：返回Y坐标

**返回值**：整形数: 0:没找到1:找到

**示例**：dm_ret=dm.FindColor (0, 0, 2000, 2000,"123456-000000|aabbcc-030303|ddeeff-202020", 1.0, 0, intX, intY) If intX>=0 and intY>=0 Then MessageBox"找到"End If

**注**：反色模式是指匹配任意一个指定颜色之外的颜色.比如"@123456|333333".在匹配时,会匹配除了123456或者333333之外的颜色.

----

### dm_findColorBlock

**签名**：`long dm_findColorBlock(long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height, long* intX, long* intY)`

**简介**：查找指定区域内的颜色块,颜色格式"RRGGBB-DRDGDB",注意,和按键的颜色格式相反

**原型**：long FindColorBlock (x1, y1, x2, y2, color, sim, count, width, height, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color` (字符串)：颜色格式为"RRGGBB-DRDGDB",比如"123456-000000|aabbcc-202020".也可以支持反色模式.前面加@即可.比如"@123456-000000|aabbcc-202020".具体可以看下放注释.注意，这里只支持RGB颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `count` (整形数)：在宽度为width,高度为height的颜色块中，符合color颜色的最小数量.(注意,这个颜色数量可以在综合工具的二值化区域中看到)

- `width` (整形数)：颜色块的宽度

- `height` (整形数)：颜色块的高度

- `intX` (变参指针)：返回X坐标(指向颜色块的左上角)

- `intY` (变参指针)：返回Y坐标(指向颜色块的左上角)

**返回值**：整形数: 0:没找到1:找到

**示例**：dm_ret=dm.FindColorBlock (0, 0, 2000, 2000,"123456-000000|aabbcc-030303|ddeeff-202020", 1.0, 350, 100, 200, intX, intY) If intX>=0 and intY>=0 Then MessageBox"找到"End If

**注**：反色模式是指匹配任意一个指定颜色之外的颜色.比如"@123456|333333".在匹配时,会匹配除了123456或者333333之外的颜色.

----

### dm_findColorBlockEx

**签名**：`long dm_findColorBlockEx(long x1, long y1, long x2, long y2, const char* color, long sim, long count, long width, long height)`

**简介**：查找指定区域内的所有颜色块,颜色格式"RRGGBB-DRDGDB",注意,和按键的颜色格式相反

**原型**：string FindColorBlockEx (x1, y1, x2, y2, color, sim, count, width, height)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color` (字符串)：颜色格式为"RRGGBB-DRDGDB"比如"aabbcc-000000|123456-202020".也可以支持反色模式.前面加@即可.比如"@123456-000000|aabbcc-202020".具体可以看下放注释.注意，这里只支持RGB颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `count` (整形数)：在宽度为width,高度为height的颜色块中，符合color颜色的最小数量.(注意,这个颜色数量可以在综合工具的二值化区域中看到)

- `width` (整形数)：颜色块的宽度

- `height` (整形数)：颜色块的高度

**返回值**：字符串:返回所有颜色块信息的坐标值,然后通过GetResultCount等接口来解析(由于内存限制,返回的颜色数量最多为1800个左右)

**示例**：s=dm.FindColorBlockEx (0, 0, 2000, 2000,"123456-000000|abcdef-202020", 1.0, 350, 100, 200) count=dm.GetResultCount (s) index=0 Do While index<count dm_ret=dm.GetResultPos (s, index, intX, intY) MessageBox intX&","&intY index=index+1 Loop

**注**：反色模式是指匹配任意一个指定颜色之外的颜色.比如"@123456|333333".在匹配时,会匹配除了123456或者333333之外的颜色.

----

### dm_findColorE

**签名**：`long dm_findColorE(long x1, long y1, long x2, long y2, const char* color, long sim, long dir)`

**简介**：查找指定区域内的颜色,颜色格式"RRGGBB-DRDGDB",注意,和按键的颜色格式相反易语言用不了FindColor可以用此接口来代替

**原型**：string FindColorE (x1, y1, x2, y2, color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color` (字符串)：颜色格式为"RRGGBB-DRDGDB",比如"123456-000000|aabbcc-202020".也可以支持反色模式.前面加@即可.比如"@123456-000000|aabbcc-202020".具体可以看下放注释.注意，这里只支持RGB颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上4：从中心往外查找5:从上到下,从左到右6:从上到下,从右到左7:从下到上,从左到右8:从下到上,从右到左

**返回值**：字符串:返回X和Y坐标形式如"x|y",比如"100|200"

**示例**：pos=dm.FindColorE (0, 0, 2000, 2000,"123456-000000|aabbcc-030303|ddeeff-202020", 1.0, 0) pos=split(pos,"|") If int (pos(0))>0 Then MessageBox"找到"End If

**注**：反色模式是指匹配任意一个指定颜色之外的颜色.比如"@123456|333333".在匹配时,会匹配除了123456或者333333之外的颜色.

----

### dm_findColorEx

**签名**：`long dm_findColorEx(long x1, long y1, long x2, long y2, const char* color, long sim, long dir)`

**简介**：查找指定区域内的所有颜色,颜色格式"RRGGBB-DRDGDB",注意,和按键的颜色格式相反

**原型**：string FindColorEx (x1, y1, x2, y2, color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color` (字符串)：颜色格式为"RRGGBB-DRDGDB"比如"aabbcc-000000|123456-202020".也可以支持反色模式.前面加@即可.比如"@123456-000000|aabbcc-202020".具体可以看下放注释.注意，这里只支持RGB颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上5:从上到下,从左到右6:从上到下,从右到左7:从下到上,从左到右8:从下到上,从右到左

**返回值**：字符串:返回所有颜色信息的坐标值,然后通过GetResultCount等接口来解析(由于内存限制,返回的颜色数量最多为1800个左右)

**示例**：s=dm.FindColorEx (0, 0, 2000, 2000,"123456-000000|abcdef-202020", 1.0, 0) count=dm.GetResultCount (s) index=0 Do While index<count dm_ret=dm.GetResultPos (s, index, intX, intY) MessageBox intX&","&intY index=index+1 Loop

**注**：反色模式是指匹配任意一个指定颜色之外的颜色.比如"@123456|333333".在匹配时,会匹配除了123456或者333333之外的颜色.

----

### dm_findMulColor

**签名**：`long dm_findMulColor(long x1, long y1, long x2, long y2, const char* color, long sim)`

**简介**：查找指定区域内的所有颜色.

**原型**：long FindMulColor (x1, y1, x2, y2, color, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color` (字符串)：颜色格式为"RRGGBB-DRDGDB",比如"123456-000000|aabbcc-202020".也可以支持反色模式.前面加@即可.比如"@123456-000000|aabbcc-202020".具体可以看下放注释.注意，这里只支持RGB颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：整形数: 0:没找到或者部分颜色没找到1:所有颜色都找到

**示例**：dm_ret=dm.FindMulColor (0, 0, 2000, 2000,"123456-000000|aabbcc-030303|ddeeff-202020", 1.0) if dm_ret=1 then MessageBox"找到了"end if

**注**：反色模式是指匹配任意一个指定颜色之外的颜色.比如"@123456|333333".在匹配时,会匹配除了123456或者333333之外的颜色.

----

### dm_findMultiColor

**签名**：`long dm_findMultiColor(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir, long* intX, long* intY)`

**简介**：根据指定的多点查找颜色坐标

**原型**：long FindMultiColor(x1, y1, x2, y2, first_color, offset_color, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `first_color` (字符串)：颜色格式为"RRGGBB-DRDGDB|RRGGBB-DRDGDB|…………",比如"123456-000000"这里的含义和按键自带Color插件的意义相同，只不过我的可以支持偏色和多种颜色组合所有的偏移色坐标都相对于此颜色.注意，这里只支持RGB颜色.

- `offset_color` (字符串)：偏移颜色可以支持任意多个点格式和按键自带的Color插件意义相同,只不过我的可以支持偏色和多种颜色组合格式为"x1|y1|RRGGBB-DRDGDB|RRGGBB-DRDGDB……,……xn|yn|RRGGBB-DRDGDB|RRGGBB-DRDGDB……"比如"1|3|aabbcc|aaffaa-101010,-5|-3|123456-000000|454545-303030|565656"等任意组合都可以，支持偏色还可以支持反色模式，比如"1|3|-aabbcc|-334455-101010,-5|-3|-123456-000000|-353535|454545-101010","-"表示除了指定颜色之外的颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

- `intX` (变参指针)：返回X坐标(坐标为first_color所在坐标)

- `intY` (变参指针)：返回Y坐标(坐标为first_color所在坐标)

**返回值**：整形数: 0:没找到1:找到

**示例**：dm_ret=dm.FindMultiColor(0, 0, 2000, 2000,"cc805b-020202|606060-010101","9|2|-00ff00|-ff0000, 15|2|2dff 1c -010101, 6|11|a0d962|aabbcc, 11|14|-ffffff", 1.0, 1, intX, intY) dm.MoveTo intX, intY

----

### dm_findMultiColorE

**签名**：`long dm_findMultiColorE(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir)`

**简介**：根据指定的多点查找颜色坐标易语言用不了FindMultiColor可以用此接口来代替

**原型**：string FindMultiColorE (x1, y1, x2, y2, first_color, offset_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `first_color` (字符串)：颜色格式为"RRGGBB-DRDGDB|RRGGBB-DRDGDB|…………",比如"123456-000000"这里的含义和按键自带Color插件的意义相同，只不过我的可以支持偏色和多种颜色组合所有的偏移色坐标都相对于此颜色.注意，这里只支持RGB颜色.

- `offset_color` (字符串)：偏移颜色可以支持任意多个点格式和按键自带的Color插件意义相同,只不过我的可以支持偏色和多种颜色组合格式为"x1|y1|RRGGBB-DRDGDB|RRGGBB-DRDGDB……,……xn|yn|RRGGBB -DRDGDB|RRGGBB-DRDGDB……"比如"1|3|aabbcc|aaffaa-101010,-5|-3|123456-000000|454545-303030|565656"等任意组合都可以，支持偏色还可以支持反色模式，比如"1|3|-aabbcc|-334455-101010,-5|-3|-123456-000000|-353535|454545-101010","-"表示除了指定颜色之外的颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回X和Y坐标形式如"x|y",比如"100|200"

**示例**：pos=dm.FindMultiColorE (0, 0, 2000, 2000,"cc805b-020202|606060-010101","9|2|-00ff00|-ff0000, 15|2|2dff 1c -010101, 6|11|a0d962|aabbcc, 11|14|-ffffff", 1.0, 1) pos=split(pos,"|") dm.MoveTo int (pos(0)), int (pos(1))

----

### dm_findMultiColorEx

**签名**：`long dm_findMultiColorEx(long x1, long y1, long x2, long y2, const char* first_color, const char* offset_color, long sim, long dir)`

**简介**：根据指定的多点查找所有颜色坐标

**原型**：string FindMultiColorEx (x1, y1, x2, y2, first_color, offset_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `first_color` (字符串)：颜色格式为"RRGGBB-DRDGDB|RRGGBB-DRDGDB|…………",比如"123456-000000"这里的含义和按键自带Color插件的意义相同，只不过我的可以支持偏色和多种颜色组合所有的偏移色坐标都相对于此颜色.注意，这里只支持RGB颜色.

- `offset_color` (字符串)：偏移颜色可以支持任意多个点格式和按键自带的Color插件意义相同,只不过我的可以支持偏色和多种颜色组合格式为"x1|y1|RRGGBB-DRDGDB|RRGGBB-DRDGDB……,……xn|yn|RRGGBB -DRDGDB|RRGGBB-DRDGDB……"比如"1|3|aabbcc|aaffaa-101010,-5|-3|123456-000000|454545-303030|565656"等任意组合都可以，支持偏色还可以支持反色模式，比如"1|3|-aabbcc|-334455-101010,-5|-3|-123456-000000|-353535|454545-101010","-"表示除了指定颜色之外的颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回所有颜色信息的坐标值,然后通过GetResultCount等接口来解析(由于内存限制,返回的坐标数量最多为1800个左右)坐标是first_color所在的坐标

**示例**：dm_ret=dm.FindMultiColorEx (0, 0, 2000, 2000,"cc805b-020202|606060-010101","9|2|-00ff00|-ff0000, 15|2|2dff 1c -010101, 6|11|a0d962|aabbcc, 11|14|-ffffff", 1.0, 1) count=dm.GetResultCount (dm_ret) index=0 Do While index<count aa=dm.GetResultPos (dm_ret, index, intX, intY) dm.MoveTo intX, intY index=index+1 Delay 1000 Loop

----

### dm_findPic

**签名**：`long dm_findPic(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个找到的X Y坐标.

**原型**：long FindPic(x1, y1, x2, y2, pic_name, delta_color, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示).如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

- `intX` (变参指针)：返回图片左上角的X坐标

- `intY` (变参指针)：返回图片左上角的Y坐标

**返回值**：整形数:返回找到的图片的序号,从0开始索引.如果没找到返回-1

**示例**：dm_ret=dm.FindPic(0, 0, 2000, 2000,"1.bmp|2.bmp|3.bmp","000000", 0.9, 0, intX, intY) If intX>=0 and intY>=0 Then MessageBox"找到"End If

----

### dm_findPicE

**签名**：`const char* dm_findPicE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个找到的X Y坐标.易语言用不了FindPic可以用此接口来代替

**原型**：string FindPicE (x1, y1, x2, y2, pic_name, delta_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回找到的图片序号(从0开始索引)以及X和Y坐标形式如"index|x|y",比如"3|100|200"

**示例**：pos=dm.FindPicE (0, 0, 2000, 2000,"1.bmp|2.bmp|3.bmp","000000", 0.9, 0) pos=split(pos,"|") If int (pos(1))>0 Then MessageBox"找到"End If

----

### dm_findPicEx

**签名**：`const char* dm_findPicEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,并且返回所有找到的图像的坐标.

**原型**：string FindPicEx (x1, y1, x2, y2, pic_name, delta_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回的是所有找到的坐标格式如下:"id, x, y|id, x, y..|id, x, y"(图片左上角的坐标)比如"0, 100, 20|2, 30, 40"表示找到了两个,第一个,对应的图片是图像序号为0的图片,坐标是(100, 20),第二个是序号为2的图片,坐标(30, 40) (由于内存限制,返回的图片数量最多为1500个左右)

**示例**：dm_ret=dm.FindPicEx (0, 0, 2000, 2000,"test.bmp|test2.bmp|test3.bmp|test4.bmp|test5.bmp","020202", 1.0, 0) If len (dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count TracePrint ss (index) sss=split(ss (index),",") id=int (sss (0)) x=int (sss (1)) y=int (sss (2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

----

### dm_findPicExS

**签名**：`const char* dm_findPicExS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,并且返回所有找到的图像的坐标.此函数同FindPicEx .只是返回值不同.

**原型**：string FindPicExS (x1, y1, x2, y2, pic_name, delta_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回的是所有找到的坐标格式如下:"file, x, y|file, x, y ..|file, x, y"(图片左上角的坐标)比如"1.bmp, 100, 20|2.bmp, 30, 40"表示找到了两个,第一个,对应的图片是1.bmp,坐标是(100, 20),第二个是2.bmp,坐标(30, 40) (由于内存限制,返回的图片数量最多为1500个左右)

**示例**：dm_ret=dm.FindPicExS (0, 0, 2000, 2000,"test.bmp|test2.bmp|test3.bmp|test4.bmp|test5.bmp","020202", 1.0, 0) If len (dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count TracePrint ss (index) sss=split(ss (index),",") f=sss (0) x=int (sss (1)) y=int (sss (2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

----

### dm_findPicMem

**签名**：`long dm_findPicMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个找到的X Y坐标.这个函数要求图片是数据地址.

**原型**：long FindPicMem(x1, y1, x2, y2, pic_info, delta_color, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_info` (字符串)：图片数据地址集合.格式为"地址1,长度1|地址2,长度2.....|地址n,长度n".可以用AppendPicAddr来组合.地址表示24位位图资源在内存中的首地址，用十进制的数值表示长度表示位图资源在内存中的长度，用十进制数值表示.

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

- `intX` (变参指针)：返回图片左上角的X坐标

- `intY` (变参指针)：返回图片左上角的Y坐标

**返回值**：整形数:返回找到的图片的序号,从0开始索引.如果没找到返回-1

**示例**：pic_info=""pic_info=dm.AppendPicAddr(pic_info, 12034, 643) pic_info=dm.AppendPicAddr(pic_info, 328435, 8935) pic_info=dm.AppendPicAddr(pic_info, 809234, 789) dm_ret=dm.FindPicMem(0, 0, 2000, 2000, pic_info,"000000", 0.9, 0, intX, intY) If intX>=0 and intY>=0 Then MessageBox"找到"End If

**注**：内存中的图片格式必须是24位色，并且不能加密.

----

### dm_findPicMemE

**签名**：`const char* dm_findPicMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个找到的X Y坐标.这个函数要求图片是数据地址.易语言用不了FindPicMem可以用此接口来代替

**原型**：string FindPicMemE(x1, y1, x2, y2, pic_info, delta_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_info` (字符串)：图片数据地址集合.格式为"地址1,长度1|地址2,长度2.....|地址n,长度n".可以用AppendPicAddr来组合.地址表示24位位图资源在内存中的首地址，用十进制的数值表示长度表示位图资源在内存中的长度，用十进制数值表示.

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回找到的图片序号(从0开始索引)以及X和Y坐标形式如"index|x|y",比如"3|100|200"

**示例**：pic_info=""pic_info=dm.AppendPicAddr(pic_info, 12034, 643) pic_info=dm.AppendPicAddr(pic_info, 328435, 8935) pic_info=dm.AppendPicAddr(pic_info, 809234, 789) pos=dm.FindPicMemE(0, 0, 2000, 2000, pic_info,"000000", 0.9, 0) pos=split(pos,"|") If int(pos(1))>0 Then MessageBox"找到"End If

**注**：内存中的图片格式必须是24位色，并且不能加密.

----

### dm_findPicMemEx

**签名**：`const char* dm_findPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,并且返回所有找到的图像的坐标.这个函数要求图片是数据地址.

**原型**：string FindPicMemEx(x1, y1, x2, y2, pic_info, delta_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_info` (字符串)：图片数据地址集合.格式为"地址1,长度1|地址2,长度2.....|地址n,长度n".可以用AppendPicAddr来组合.地址表示24位位图资源在内存中的首地址，用十进制的数值表示长度表示位图资源在内存中的长度，用十进制数值表示.

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回的是所有找到的坐标格式如下:"id, x, y|id, x, y..|id, x, y"(图片左上角的坐标)比如"0, 100, 20|2, 30, 40"表示找到了两个,第一个,对应的图片是图像序号为0的图片,坐标是(100, 20),第二个是序号为2的图片,坐标(30, 40) (由于内存限制,返回的图片数量最多为1500个左右)

**示例**：pic_info=""pic_info=dm.AppendPicAddr(pic_info, 12034, 643) pic_info=dm.AppendPicAddr(pic_info, 328435, 8935) pic_info=dm.AppendPicAddr(pic_info, 809234, 789) dm_ret=dm.FindPicMemEx(0, 0, 2000, 2000, pic_info,"020202", 1.0, 0) If len(dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound(ss)+1 Do While index<count TracePrint ss(index) sss=split(ss(index),",") id=int(sss(0)) x=int(sss(1)) y=int(sss(2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

**注**：内存中的图片格式必须是24位色，并且不能加密.

----

### dm_findPicS

**签名**：`long dm_findPicS(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个找到的X Y坐标.此函数同FindPic .只是返回值不同.

**原型**：string FindPicS (x1, y1, x2, y2, pic_name, delta_color, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

- `intX` (变参指针)：返回图片左上角的X坐标

- `intY` (变参指针)：返回图片左上角的Y坐标

**返回值**：字符串:返回找到的图片的文件名.没找到返回长度为0的字符串.

**示例**：dm_ret=dm.FindPicS (0, 0, 2000, 2000,"1.bmp|2.bmp|3.bmp","000000", 0.9, 0, intX, intY) If intX>=0 and intY>=0 Then MessageBox"找到"&dm_ret End If

----

### dm_findPicSim

**签名**：`long dm_findPicSim(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir, long* intX, long* intY)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个匹配的X Y坐标.

**原型**：long FindPicSim(x1, y1, x2, y2, pic_name, delta_color, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示).如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (整形数)：最小百分比相似率.表示匹配的颜色占总颜色数的百分比.其中透明色也算作匹配色.取值为0到100. 100表示必须完全匹配. 0表示任意颜色都匹配.只有大于sim的相似率的才会被匹配

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

- `intX` (变参指针)：返回图片左上角的X坐标

- `intY` (变参指针)：返回图片左上角的Y坐标

**返回值**：整形数:返回找到的图片的序号,从0开始索引.如果没找到返回-1

**示例**：dm_ret=dm.FindPicSim(0, 0, 2000, 2000,"1.bmp|2.bmp|3.bmp","000000", 80, 0, intX, intY) If intX>=0 and intY>=0 Then MessageBox"找到"End If

**注**：此接口和FindPic类似.只不过FindPicSim是以颜色百分比来进行匹配.如果待查找区域内有杂色,只要颜色百分比达到要求,也一样可以匹配.这个接口是FindPic的进阶版本.当sim为100时,那么FindPicSim就退化为FindPic此接口速度很慢,因为需要搜索任何一种可能.所以尽可能把搜索范围要小一些.以免耗时太长.

----

### dm_findPicSimE

**签名**：`const char* dm_findPicSimE(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个匹配的X Y坐标.易语言用不了FindPicSim可以用此接口来代替

**原型**：string FindPicSimE(x1, y1, x2, y2, pic_name, delta_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (整形数)：最小百分比相似率.表示匹配的颜色占总颜色数的百分比.其中透明色也算作匹配色.取值为0到100. 100表示必须完全匹配. 0表示任意颜色都匹配.只有大于sim的相似率的才会被匹配

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回找到的图片序号(从0开始索引)以及X和Y坐标形式如"index|x|y",比如"3|100|200"

**示例**：pos=dm.FindPicSimE(0, 0, 2000, 2000,"1.bmp|2.bmp|3.bmp","000000", 80, 1) pos=split(pos,"|") If int(pos(1))>0 Then MessageBox"找到"End If

**注**：此接口和FindPicE类似.只不过FindPicSimE是以颜色百分比来进行匹配.如果待查找区域内有杂色,只要颜色百分比达到要求,也一样可以匹配.这个接口是FindPicE的进阶版本.当sim为100时,那么FindPicSimE就退化为FindPicE此接口速度很慢,因为需要搜索任何一种可能.所以尽可能把搜索范围要小一些.以免耗时太长.

----

### dm_findPicSimEx

**签名**：`const char* dm_findPicSimEx(long x1, long y1, long x2, long y2, const char* pic_name, const char* delta_color, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,并且返回所有找到的图像的坐标.

**原型**：string FindPicSimEx(x1, y1, x2, y2, pic_name, delta_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (整形数)：最小百分比相似率.表示匹配的颜色占总颜色数的百分比.其中透明色也算作匹配色.取值为0到100. 100表示必须完全匹配. 0表示任意颜色都匹配.只有大于sim的相似率的才会被匹配

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回的是所有找到的坐标格式如下:"id, sim, x, y|id, sim, x, y..|id, sim, x, y"(图片左上角的坐标)比如"0, 82, 100, 20|2, 70, 30, 40"表示找到了两个,第一个,对应的图片是图像序号为0的图片,坐标是(100, 20),当前匹配百分比是82,第二个是序号为2的图片,坐标(30, 40),当前匹配百分比是70 (由于内存限制,返回的图片数量最多为1500个左右)

**示例**：dm_ret=dm.FindPicSimEx(0, 0, 2000, 2000,"test.bmp|test2.bmp|test3.bmp|test4.bmp|test5.bmp","020202", 80, 0) If len(dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound(ss)+1 Do While index<count TracePrint ss(index) sss=split(ss(index),",") id=int(sss(0)) sim=int(sss(1)) x=int(sss(2)) y=int(sss(3)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

**注**：此接口和FindPicEx类似.只不过FindPicSimEx是以颜色百分比来进行匹配.如果待查找区域内有杂色,只要颜色百分比达到要求,也一样可以匹配.这个接口是FindPicEx的进阶版本.当sim为100时,那么FindPicSimEx就退化为FindPicEx此接口速度很慢,因为需要搜索任何一种可能.所以尽可能把搜索范围要小一些.以免耗时太长.

----

### dm_findPicSimMem

**签名**：`long dm_findPicSimMem(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir, long* intX, long* intY)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个匹配的X Y坐标.这个函数要求图片是数据地址.

**原型**：long FindPicSimMem(x1, y1, x2, y2, pic_info, delta_color, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_info` (字符串)：图片数据地址集合.格式为"地址1,长度1|地址2,长度2.....|地址n,长度n".可以用AppendPicAddr来组合.地址表示24位位图资源在内存中的首地址，用十进制的数值表示长度表示位图资源在内存中的长度，用十进制数值表示.

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (整形数)：最小百分比相似率.表示匹配的颜色占总颜色数的百分比.其中透明色也算作匹配色.取值为0到100. 100表示必须完全匹配. 0表示任意颜色都匹配.只有大于sim的相似率的才会被匹配

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

- `intX` (变参指针)：返回图片左上角的X坐标

- `intY` (变参指针)：返回图片左上角的Y坐标

**返回值**：整形数:返回找到的图片的序号,从0开始索引.如果没找到返回-1

**示例**：pic_info=""pic_info=dm.AppendPicAddr(pic_info, 12034, 643) pic_info=dm.AppendPicAddr(pic_info, 328435, 8935) pic_info=dm.AppendPicAddr(pic_info, 809234, 789) dm_ret=dm.FindPicSimMem(0, 0, 2000, 2000, pic_info,"000000", 80, 0, intX, intY) If intX>=0 and intY>=0 Then MessageBox"找到"End If

**注**：内存中的图片格式必须是24位色，并且不能加密.此接口和FindPicMem类似.只不过FindPicSimMem是以颜色百分比来进行匹配.如果待查找区域内有杂色,只要颜色百分比达到要求,也一样可以匹配.这个接口是FindPicMem的进阶版本.当sim为100时,那么FindPicSimMem就退化为FindPicMem此接口速度很慢,因为需要搜索任何一种可能.所以尽可能把搜索范围要小一些.以免耗时太长.

----

### dm_findPicSimMemE

**签名**：`const char* dm_findPicSimMemE(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个匹配的X Y坐标.这个函数要求图片是数据地址.易语言用不了FindPicSimMem可以用此接口来代替

**原型**：string FindPicSimMemE(x1, y1, x2, y2, pic_info, delta_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_info` (字符串)：图片数据地址集合.格式为"地址1,长度1|地址2,长度2.....|地址n,长度n".可以用AppendPicAddr来组合.地址表示24位位图资源在内存中的首地址，用十进制的数值表示长度表示位图资源在内存中的长度，用十进制数值表示.

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (整形数)：最小百分比相似率.表示匹配的颜色占总颜色数的百分比.其中透明色也算作匹配色.取值为0到100. 100表示必须完全匹配. 0表示任意颜色都匹配.只有大于sim的相似率的才会被匹配

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回找到的图片序号(从0开始索引)以及X和Y坐标形式如"index|x|y",比如"3|100|200"

**示例**：pic_info=""pic_info=dm.AppendPicAddr(pic_info, 12034, 643) pic_info=dm.AppendPicAddr(pic_info, 328435, 8935) pic_info=dm.AppendPicAddr(pic_info, 809234, 789) pos=dm.FindPicMemE(0, 0, 2000, 2000, pic_info,"000000", 80, 0) pos=split(pos,"|") If int(pos(1))>0 Then MessageBox"找到"End If

**注**：内存中的图片格式必须是24位色，并且不能加密.此接口和FindPicMemE类似.只不过FindPicSimMemE是以颜色百分比来进行匹配.如果待查找区域内有杂色,只要颜色百分比达到要求,也一样可以匹配.这个接口是FindPicMemE的进阶版本.当sim为100时,那么FindPicSimMemE就退化为FindPicMemE此接口速度很慢,因为需要搜索任何一种可能.所以尽可能把搜索范围要小一些.以免耗时太长.

----

### dm_findPicSimMemEx

**签名**：`const char* dm_findPicSimMemEx(long x1, long y1, long x2, long y2, const char* pic_info, const char* delta_color, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,并且返回所有找到的图像的坐标.这个函数要求图片是数据地址.

**原型**：string FindPicSimMemEx(x1, y1, x2, y2, pic_info, delta_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_info` (字符串)：图片数据地址集合.格式为"地址1,长度1|地址2,长度2.....|地址n,长度n".可以用AppendPicAddr来组合.地址表示24位位图资源在内存中的首地址，用十进制的数值表示长度表示位图资源在内存中的长度，用十进制数值表示.

- `delta_color` (字符串)：颜色色偏比如"203040"表示RGB的色偏分别是20 30 40 (这里是16进制表示) .如果这里的色偏是2位，表示使用灰度找图.比如"20"

- `sim` (整形数)：最小百分比相似率.表示匹配的颜色占总颜色数的百分比.其中透明色也算作匹配色.取值为0到100. 100表示必须完全匹配. 0表示任意颜色都匹配.只有大于sim的相似率的才会被匹配

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回的是所有找到的坐标格式如下:"id, sim, x, y|id, sim, x, y..|id, sim, x, y"(图片左上角的坐标)比如"0, 82, 100, 20|2, 70, 30, 40"表示找到了两个,第一个,对应的图片是图像序号为0的图片,坐标是(100, 20),当前匹配百分比是82,第二个是序号为2的图片,坐标(30, 40),当前匹配百分比是70 (由于内存限制,返回的图片数量最多为1500个左右)

**示例**：pic_info=""pic_info=dm.AppendPicAddr(pic_info, 12034, 643) pic_info=dm.AppendPicAddr(pic_info, 328435, 8935) pic_info=dm.AppendPicAddr(pic_info, 809234, 789) dm_ret=dm.FindPicSimMemEx(0, 0, 2000, 2000, pic_info,"020202", 80, 0) If len(dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound(ss)+1 Do While index<count TracePrint ss(index) sss=split(ss(index),",") id=int(sss(0)) sim=int(sss(1)) x=int(sss(2)) y=int(sss(3)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

**注**：内存中的图片格式必须是24位色，并且不能加密.此接口和FindPicMemEx类似.只不过FindPicSimMemEx是以颜色百分比来进行匹配.如果待查找区域内有杂色,只要颜色百分比达到要求,也一样可以匹配.这个接口是FindPicMemEx的进阶版本.当sim为100时,那么FindPicSimMemEx就退化为FindPicMemEx此接口速度很慢,因为需要搜索任何一种可能.所以尽可能把搜索范围要小一些.以免耗时太长.

----

### dm_findShape

**签名**：`long dm_findShape(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir, long* intX, long* intY)`

**简介**：查找指定的形状.形状的描述同按键的抓抓.具体可以参考按键的抓抓.和按键的语法不同，需要用大漠综合工具的颜色转换.

**原型**：long FindShape (x1, y1, x2, y2, offset_color, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `offset_color` (字符串)：坐标偏移描述可以支持任意多个点格式和按键自带的Color插件意义相同格式为"x1|y1|e1,……xn|yn|en"比如"1|3|1,-5|-3|0"等任意组合都可以

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

- `intX` (变参指针)：返回X坐标(坐标为形状(0, 0)所在坐标)

- `intY` (变参指针)：返回Y坐标(坐标为形状(0, 0)所在坐标)

**返回值**：整形数: 0:没找到1:找到

**示例**：dm_ret=dm.FindShape (0, 0, 2000, 2000,"1|1|0, 1|6|1, 0|10|1, 9|10|1, 7|6|1, 7|8|0, 8|9|0, 2|2|1, 3|1|1", 1.0, 0, x, y) dm.MoveTo x, y

----

### dm_findShapeE

**签名**：`const char* dm_findShapeE(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir)`

**简介**：查找指定的形状.形状的描述同按键的抓抓.具体可以参考按键的抓抓.和按键的语法不同，需要用大漠综合工具的颜色转换.易语言用不了FindShape可以用此接口来代替

**原型**：string FindShapeE (x1, y1, x2, y2, offset_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `offset_color` (字符串)：坐标偏移描述可以支持任意多个点格式和按键自带的Color插件意义相同格式为"x1|y1|e1,……xn|yn|en"比如"1|3|1,-5|-3|0"等任意组合都可以

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回X和Y坐标形式如"x|y",比如"100|200"

**示例**：pos=dm.FindShapeE (0, 0, 2000, 2000,"1|1|0, 1|6|1, 0|10|1, 9|10|1, 7|6|1, 7|8|0, 8|9|0, 2|2|1, 3|1|1", 1.0, 0) pos=split(pos,"|") dm.MoveTo int (pos(0)), int (pos(1))

----

### dm_findShapeEx

**签名**：`const char* dm_findShapeEx(long x1, long y1, long x2, long y2, const char* offset_color, long sim, long dir)`

**简介**：查找所有指定的形状的坐标.形状的描述同按键的抓抓.具体可以参考按键的抓抓.和按键的语法不同，需要用大漠综合工具的颜色转换.

**原型**：string FindShapeEx(x1, y1, x2, y2, offset_color, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `offset_color` (字符串)：坐标偏移描述可以支持任意多个点格式和按键自带的Color插件意义相同格式为"x1|y1|e1,……xn|yn|en"比如"1|3|1,-5|-3|0"等任意组合都可以

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回所有形状的坐标值,然后通过GetResultCount等接口来解析(由于内存限制,返回的坐标数量最多为1800个左右)

**示例**：dm_ret=dm.FindShapeEx(0, 0, 2000, 2000,"1|1|0, 1|6|1, 0|10|1, 9|10|1, 7|6|1, 7|8|0, 8|9|0, 2|2|1, 3|1|1", 1.0, 1) count=dm.GetResultCount(dm_ret) index=0 Do While index<count aa=dm.GetResultPos(dm_ret, index, intX, intY) dm.MoveTo intX, intY index=index+1 Delay 1000 Loop

----

### dm_freePic

**签名**：`long dm_freePic(const char* pic_name)`

**简介**：释放指定的图片,此函数不必要调用,除非你想节省内存.

**原型**：long FreePic(pic_name)

**参数定义**

- `pic_name` (字符串)：文件名比如"1.bmp|2.bmp|3.bmp"等,可以使用通配符,比如"*.bmp"这个对应了所有的bmp文件"a?c*.bmp"这个代表了所有第一个字母是a第三个字母是c第二个字母任意的所有bmp文件"abc???.bmp|1.bmp|aa??.bmp"可以这样任意组合.

**返回值**：整形数: 0:失败1:成功

**示例**：PutAttachment"c:\test","*.bmp"dm_ret=dm.SetPath("c:\test") all_pic="1.bmp|2.bmp|3.bmp"dm_ret=dm.LoadPic(all_pic) dm_ret=dm.FreePic(all_pic)

----

### dm_getAveHSV

**签名**：`const char* dm_getAveHSV(long x1, long y1, long x2, long y2)`

**简介**：获取范围(x1, y1, x2, y2)颜色的均值,返回格式"H.S.V"

**原型**：string GetAveHSV(x1, y1, x2, y2)

**参数定义**

- `x1` (整形数)：左上角X

- `y1` (整形数)：左上角Y

- `x2` (整形数)：右下角X

- `y2` (整形数)：右下角Y

**返回值**：字符串:颜色字符串

**示例**：color=dm.GetAveHSV(30, 30, 100, 100) MessageBox color

----

### dm_getAveRGB

**签名**：`const char* dm_getAveRGB(long x1, long y1, long x2, long y2)`

**简介**：获取范围(x1, y1, x2, y2)颜色的均值,返回格式"RRGGBB"

**原型**：string GetAveRGB(x1, y1, x2, y2)

**参数定义**

- `x1` (整形数)：左上角X

- `y1` (整形数)：左上角Y

- `x2` (整形数)：右下角X

- `y2` (整形数)：右下角Y

**返回值**：字符串:颜色字符串

**示例**：color=dm.GetAveRGB(30, 30, 100, 100) MessageBox color

----

### dm_getColor

**签名**：`const char* dm_getColor(long x, long y)`

**简介**：获取(x, y)的颜色,颜色返回格式"RRGGBB",注意,和按键的颜色格式相反

**原型**：string GetColor(x, y)

**返回值**：字符串:颜色字符串(注意这里都是小写字符，和工具相匹配)

**示例**：color=dm.GetColor(30, 30) If color="ffffff"Then MessageBox"是白色"End If

----

### dm_getColorBGR

**签名**：`const char* dm_getColorBGR(long x, long y)`

**简介**：获取(x, y)的颜色,颜色返回格式"BBGGRR"

**原型**：string GetColorBGR(x, y)

**返回值**：字符串:颜色字符串(注意这里都是小写字符，和工具相匹配)

**示例**：color=dm.GetColorBGR(30, 30) If color="0000ff"Then MessageBox"是红色"End If

----

### dm_getColorHSV

**签名**：`const char* dm_getColorHSV(long x, long y)`

**简介**：获取(x, y)的HSV颜色,颜色返回格式"H.S.V"

**原型**：string GetColorHSV(x, y)

**返回值**：字符串:颜色字符串

**示例**：color=dm.GetColorHSV(30, 30) If color="100.20.20"Then MessageBox"ok"End If

----

### dm_getColorNum

**签名**：`long dm_getColorNum(long x1, long y1, long x2, long y2, const char* color, long sim)`

**简介**：获取指定区域的颜色数量,颜色格式"RRGGBB-DRDGDB",注意,和按键的颜色格式相反

**原型**：long GetColorNum (x1, y1, x2, y2, color, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color` (字符串)：颜色格式为"RRGGBB-DRDGDB",比如"123456-000000|aabbcc-202020".也可以支持反色模式.前面加@即可.比如"@123456-000000|aabbcc-202020".具体可以看下放注释.注意，这里只支持RGB颜色.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：整形数:颜色数量

**示例**：TracePrint dm.GetColorNum (0, 0, 2000, 2000,"123456-000000|aabbcc-030303|ddeeff-202020", 1.0)

**注**：反色模式是指匹配任意一个指定颜色之外的颜色.比如"@123456|333333".在匹配时,会匹配除了123456或者333333之外的颜色.

----

### dm_getPicSize

**签名**：`const char* dm_getPicSize(const char* pic_name)`

**简介**：获取指定图片的尺寸，如果指定的图片已经被加入缓存，则从缓存中获取信息.此接口也会把此图片加入缓存.（当图色缓存机制打开时,具体参考EnablePicCache）

**原型**：string GetPicSize (pic_name)

**参数定义**

- `pic_name` (字符串)：文件名比如"1. bmp"

**返回值**：字符串:形式如"w, h"比如"30, 20"

**示例**：PutAttachment"c:\test","*.bmp"dm_ret=dm.SetPath ("c:\test") pic_size=dm.GetPicSize ("1.bmp") pic_size=split(pic_size,",") w=int (pic_size (0)) h=int (pic_size (1)) Trace"宽度:"&w Trace"高度:"&h

----

### dm_getScreenData

**签名**：`long dm_getScreenData(long x1, long y1, long x2, long y2)`

**简介**：获取指定区域的图像,用二进制数据的方式返回,（不适合按键使用）方便二次开发.

**原型**：long GetScreenData(x1, y1, x2, y2)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

**返回值**：整形数:返回的是指定区域的二进制颜色数据地址,每个颜色是4个字节,表示方式为(00RRGGBB)

**示例**：无示例,开发者都懂的。注意,调用完此接口后，返回的数据指针在当前dm对象销毁时，或者再次调用GetScreenData时，会自动释放.从2.1120版本之后，调用完此函数后，没必要再调用FreeScreenData了,插件会自动释放.

----

### dm_getScreenDataBmp

**签名**：`const char* dm_getScreenDataBmp(long x1, long y1, long x2, long y2, long* size)`

**简介**：获取指定区域的图像,用24位位图的数据格式返回,方便二次开发.（或者可以配合SetDisplayInput的mem模式）

**原型**：long GetScreenDataBmp (x1, y1, x2, y2, data, size)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `data` (变参指针)：返回图片的数据指针

- `size` (变参指针)：返回图片的数据长度

**返回值**：整形数: 0:失败1:成功

**示例**：以下是在E语言中的示例. .局部变量data,整数型.局部变量size,整数型dm.数值方法(“GetScreenDataBmp”, 0, 0, 100, 200, data, size)图片框1.图片＝指针到字节集(data, size)需要注意的是,调用此接口获取的数据指针保存在当前对象中,到下次调用此接口时,内部就会释放.哪怕是转成字节集,这个地址也还是在此字节集中使用.如果您要此地址一直有效，那么您需要自己拷贝字节集到自己的字节集中.

----

### dm_imageToBmp

**签名**：`long dm_imageToBmp(const char* pic_name, const char* bmp_name)`

**简介**：转换图片格式为24位BMP格式.

**原型**：long ImageToBmp(pic_name, bmp_name)

**参数定义**

- `pic_name` (字符串)：要转换的图片名

- `bmp_name` (字符串)：要保存的BMP图片名

**返回值**：整形数: 0:失败1:成功

**示例**：dm.ImageToBmp"1.png","1.bmp"dm.ImageToBmp"2.jpg","2.bmp"dm.ImageToBmp"3.gif","3.bmp"

----

### dm_isDisplayDead

**签名**：`long dm_isDisplayDead(long x1, long y1, long x2, long y2, long time)`

**简介**：判断指定的区域，在指定的时间内(秒),图像数据是否一直不变.(卡屏). (或者绑定的窗口不存在也返回1)

**原型**：long IsDisplayDead (x1, y1, x2, y2, t)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `t` (整形数)：需要等待的时间,单位是秒

**返回值**：整形数: 0:没有卡屏，图像数据在变化. 1:卡屏.图像数据在指定的时间内一直没有变化.或者绑定的窗口不见了.

**示例**：TracePrint dm.IsDisplayDead (0, 0, 100, 100, 5)

**注**：此函数的原理是不停的截取指定区域的图像，然后比较，如果改变就立刻返回0,否则等待直到指定的时间到达.

----

### dm_loadPic

**签名**：`long dm_loadPic(const char* pic_name)`

**简介**：预先加载指定的图片,这样在操作任何和图片相关的函数时,将省去了加载图片的时间。调用此函数后,没必要一定要调用FreePic,插件自己会自动释放.另外,此函数不是必须调用的,所有和图形相关的函数只要调用过一次，图片会自动加入缓存.如果想对一个已经加入缓存的图片进行修改，那么必须先用FreePic释放此图片在缓存中占用的内存，然后重新调用图片相关接口，就可以重新加载此图片.（当图色缓存机制打开时,具体参考EnablePicCache）

**原型**：long LoadPic(pic_name)

**参数定义**

- `pic_name` (字符串)：文件名比如"1.bmp|2.bmp|3.bmp"等,可以使用通配符,比如"*.bmp"这个对应了所有的bmp文件"a?c*.bmp"这个代表了所有第一个字母是a第三个字母是c第二个字母任意的所有bmp文件"abc???.bmp|1.bmp|aa??. bmp"可以这样任意组合.

**返回值**：整形数: 0:失败1:成功

**示例**：PutAttachment"c:\test","*.bmp"dm_ret=dm.SetPath("c:\test") all_pic="abc???. bmp|1.bmp|aa??.bmp"dm_ret=dm.LoadPic(all_pic)

**注**：如果在LoadPic后(图片名为相对路径时)，又设置SetPath为别的目录，会导致加入缓存的图片失效，等于没加载.

----

### dm_loadPicByte

**签名**：`long dm_loadPicByte(const char* pic_name, long data, long size)`

**简介**：预先加载指定的图片,这样在操作任何和图片相关的函数时,将省去了加载图片的时间。调用此函数后,没必要一定要调用FreePic,插件自己会自动释放.另外,此函数不是必须调用的,所有和图形相关的函数只要调用过一次，图片会自动加入缓存.如果想对一个已经加入缓存的图片进行修改，那么必须先用FreePic释放此图片在缓存中占用的内存，然后重新调用图片相关接口，就可以重新加载此图片.（当图色缓存机制打开时,具体参考EnablePicCache）此函数同LoadPic，只不过LoadPic是从文件中加载图片,而LoadPicByte从给定的内存中加载.

**原型**：long LoadPicByte (addr, size, pic_name)

**参数定义**

- `addr` (整形数)：BMP图像首地址.(完整的BMP图像，不是经过解析的.和BMP文件里的内容一致)

- `size` (整形数)：BMP图像大小.(和BMP文件大小一致)

- `pic_name` (字符串)：文件名,指定这个地址对应的图片名.用于找图时使用.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.LoadPicByte 123456, 300,"1.bmp"dm_ret=dm.FindPic (0, 0, 2000, 2000,"1.bmp","000000", 0.9, 0, x, y)

**注**：如果在LoadPicByte后(图片名为相对路径时)，又设置SetPath为别的目录，会导致加入缓存的图片失效，等于没加载.

----

### dm_matchPicName

**签名**：`const char* dm_matchPicName(const char* pic_name)`

**简介**：根据通配符获取文件集合.方便用于FindPic和FindPicEx

**原型**：string MatchPicName(pic_name)

**参数定义**

- `pic_name` (字符串)：文件名比如"1.bmp|2.bmp|3.bmp"等,可以使用通配符,比如"*.bmp"这个对应了所有的bmp文件"a?c*.bmp"这个代表了所有第一个字母是a第三个字母是c第二个字母任意的所有bmp文件"abc???.bmp|1.bmp|aa??.bmp"可以这样任意组合.

**返回值**：字符串:返回的是通配符对应的文件集合，每个图片以|分割

**示例**：PutAttachment"c:\test","*.bmp"dm_ret=dm.SetPath("c:\test") all_pic="abc*.bmp"pic_name=dm.MatchPicName(all_pic)//比如c:\test目录下有abc001.bmp abc002.bmp abc003.bmp//那么pic_name的值为abc001.bmp|abc002.bmp|abc003.bmp

----

### dm_rgb2BGR

**签名**：`const char* dm_rgb2BGR(const char* rgb_color)`

**简介**：把RGB的颜色格式转换为BGR(按键格式)

**原型**：string RGB2BGR(rgb_color)

**返回值**：字符串: BGR格式的字符串

**示例**：bgr_color=dm.RGB2BGR(rgb_color)

----

### dm_setExcludeRegion

**签名**：`long dm_setExcludeRegion(long mode, const char* info)`

**简介**：设置图色,以及文字识别时,需要排除的区域.(支持所有图色接口,以及文字相关接口,但对单点取色,或者单点颜色比较的接口不支持)

**原型**：long SetExcludeRegion (mode, info)

**参数定义**

- `mode` (整形数)：模式,取值如下: 0:添加排除区域1:设置排除区域的颜色,默认颜色是FF00FF(此接口的原理是把排除区域设置成此颜色,这样就可以让这块区域失效) 2:清空排除区域

- `info` (字符串)：根据mode的取值来决定当mode为0时,此参数指添加的区域,可以多个区域,用"|"相连.格式为"x1, y1, x2, y2|....."当mode为1时,此参数为排除区域的颜色,"RRGGBB"当mode为2时,此参数无效

**返回值**：整形数: 0:失败1:成功

**示例**：//先清空区域dm.SetExcludeRegion 2,""//添加区域dm.SetExcludeRegion 0,"30, 30, 100, 300|300, 400, 500, 600"dm.SetExcludeRegion 0,"100, 100, 200, 200"至于颜色如果有需要也可以设置比如dm.SetExcludeRegion 1,"FF11FF"

----

### dm_setFindPicMultithreadCount

**签名**：`long dm_setFindPicMultithreadCount(long count)`

**简介**：当执行FindPicXXX系列接口时,当图片个数少于count时,使用单线程查找,否则使用多线程。这个count默认是4.

**原型**：long SetFindPicMultithreadCount(count)

**参数定义**

- `count` (整形数)：图片数量.最小不能小于2.因为1个图片必定是单线程.这个值默认是4.如果你不更改的话.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetFindPicMultithreadCount 2 dm.FindPicXXX

**注**：这个只是设置多线程查找的一个条件.另外一个开关是EnableFindPicMultithread .

----

### dm_setFindPicMultithreadLimit

**签名**：`long dm_setFindPicMultithreadLimit(long limit)`

**简介**：当执行FindPicXXX系列接口时,当触发多线程查找条件时,设置开启的最大线程数量.注意,不可以超过当前CPU核心数.

**原型**：long SetFindPicMultithreadLimit (limit)

**参数定义**

- `limit` (整形数)：最大线程数,不能超过当前CPU核心数.超过无效. 0表示无限制.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetFindPicMultithreadLimie 2 dm.FindPicXXX

----

### dm_setPicPwd

**签名**：`long dm_setPicPwd(const char* pwd)`

**简介**：设置图片密码，如果图片本身没有加密，那么此设置不影响不加密的图片，一样正常使用.

**原型**：long SetPicPwd (pwd)

**参数定义**

- `pwd` (字符串)：图片密码

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetPicPwd ("123")注意,此函数必须在使用图片之前调用.

----

### dm_bindWindow

**签名**：`long dm_bindWindow(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, long mode)`

**简介**：绑定指定的窗口,并指定这个窗口的屏幕颜色获取方式,鼠标仿真模式,键盘仿真模式,以及模式设定,高级用户可以参考BindWindowEx更加灵活强大.

**原型**：long BindWindow(hwnd, display, mouse, keypad, mode)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `display` (字符串)：屏幕颜色获取方式取值有以下几种"normal":正常模式,平常我们用的前台截屏模式"gdi": gdi模式,用于窗口采用GDI方式刷新时.此模式占用CPU较大.参考SetAero win10以上系统使用此模式，如果截图失败，尝试把目标程序重新开启再试试。"gdi2": gdi2模式,此模式兼容性较强,但是速度比gdi模式要慢许多,如果gdi模式发现后台不刷新时,可以考虑用gdi2模式."dx2": dx2模式,用于窗口采用dx模式刷新,如果dx方式会出现窗口所在进程崩溃的状况,可以考虑采用这种.采用这种方式要保证窗口有一部分在屏幕外.win7 win8或者vista不需要移动也可后台.此模式占用CPU较大.参考SetAero . win10以上系统使用此模式，如果截图失败，尝试把目标程序重新开启再试试。"dx3": dx3模式,同dx2模式,但是如果发现有些窗口后台不刷新时,可以考虑用dx3模式,此模式比dx2模式慢许多.此模式占用CPU较大.参考SetAero . win10以上系统使用此模式，如果截图失败，尝试把目标程序重新开启再试试。"dx": dx模式,等同于BindWindowEx中，display设置的"dx.graphic.2d|dx.graphic.3d",具体参考BindWindowEx

- `mouse` (字符串)：鼠标仿真模式取值有以下几种"normal":正常模式,平常我们用的前台鼠标模式"windows": Windows模式,采取模拟windows消息方式同按键自带后台插件."windows2": Windows2模式,采取模拟windows消息方式(锁定鼠标位置)此模式等同于BindWindowEx中的mouse为以下组合"dx.mouse.position.lock.api|dx.mouse.position.lock.message|dx.mouse.state.message""windows3": Windows3模式，采取模拟windows消息方式,可以支持有多个子窗口的窗口后台."dx": dx模式,采用模拟dx后台鼠标模式,这种方式会锁定鼠标输入.有些窗口在此模式下绑定时，需要先激活窗口再绑定(或者绑定以后激活)，否则可能会出现绑定后鼠标无效的情况.此模式等同于BindWindowEx中的mouse为以下组合"dx.public.active.api|dx.public.active.message|dx.mouse.position.lock.api|dx.mouse.position.lock.message|dx.mouse.state.api|dx.mouse.state.message|dx.mouse.api|dx.mouse.focus.input.api|dx.mouse.focus.input.message|dx.mouse.clip.lock.api|dx.mouse.input.lock.api|dx.mouse.cursor""dx2"：dx2模式,这种方式类似于dx模式,但是不会锁定外部鼠标输入.有些窗口在此模式下绑定时，需要先激活窗口再绑定(或者绑定以后手动激活)，否则可能会出现绑定后鼠标无效的情况.此模式等同于BindWindowEx中的mouse为以下组合"dx.public.active.api|dx.public.active.message|dx.mouse.position.lock.api|dx.mouse.state.api|dx.mouse.api|dx.mouse.focus.input.api|dx.mouse.focus.input.message|dx.mouse.clip.lock.api|dx.mouse.input.lock.api|dx.mouse.cursor"

- `keypad` (字符串)：键盘仿真模式取值有以下几种"normal":正常模式,平常我们用的前台键盘模式"windows": Windows模式,采取模拟windows消息方式同按键的后台插件."dx": dx模式,采用模拟dx后台键盘模式。有些窗口在此模式下绑定时，需要先激活窗口再绑定(或者绑定以后激活)，否则可能会出现绑定后键盘无效的情况.此模式等同于BindWindowEx中的keypad为以下组合"dx.public.active.api|dx.public.active.message|dx.keypad.state.api|dx.keypad.api|dx.keypad.input.lock.api"

- `mode` (整形数)：模式。取值有以下几种0:推荐模式此模式比较通用，而且后台效果是最好的. 2:同模式0,如果模式0有崩溃问题，可以尝试此模式.注意0和2模式，当主绑定(第一个绑定同个窗口的对象)绑定成功后，那么调用主绑定的线程必须一致维持,否则线程一旦推出,对应的绑定也会消失. 101:超级绑定模式.可隐藏目标进程中的dm.dll.避免被恶意检测.效果要比dx.public.hide.dll好.推荐使用. 103:同模式101，如果模式101有崩溃问题，可以尝试此模式. 11:需要加载驱动,适合一些特殊的窗口,如果前面的无法绑定，可以尝试此模式.此模式不支持32位系统13:需要加载驱动,适合一些特殊的窗口,如果前面的无法绑定，可以尝试此模式.此模式不支持32位系统需要注意的是:模式101 103在大部分窗口下绑定都没问题。但也有少数特殊的窗口，比如有很多子窗口的窗口，对于这种窗口，在绑定时，一定要把鼠标指向一个可以输入文字的窗口，比如一个文本框，最好能激活这个文本框，这样可以保证绑定的成功.

**返回值**：整形数: 0:失败1:成功如果返回0，可以调用GetLastError来查看具体失败错误码,帮助分析问题.

**示例**：//display:前台鼠标:前台键盘:前台模式0 dm_ret=dm.BindWindow(hwnd,"normal","normal","normal", 0)//display: dx鼠标:前台键盘:前台模式0 dm_ret=dm.BindWindow(hwnd,"dx","normal","normal", 0)//display: dx鼠标: dx后台键盘: dx后台模式1 dm_ret=dm.BindWindow(hwnd,"dx","dx","dx", 1)//display: dx鼠标: windows3后台键盘: windows后台模式101 dm_ret=dm.BindWindow(hwnd,"dx","windows3","windows", 101)注意:绑定之后,所有的坐标都相对于窗口的客户区坐标(不包含窗口边框)另外,绑定窗口后,必须加以下代码,以保证所有资源正常释放这个函数的意思是在脚本结束时,会调用这个函数。需要注意的是，目前的按键版本对于这个函数的执行不是线程级别的，也就是说，这个函数只会在主线程执行，子线程绑定的大漠对象，不保证完全释放。Sub OnScriptExit() dm_ret=dm.UnBindWindow() End Sub另外绑定dx会比较耗时间,请不要频繁调用此函数.还有一点特别要注意的是,有些窗口绑定之后必须加一定的延时,否则后台也无效.一般1秒到2秒的延时就足够.发现绑定失败的几种可能(一般是需要管理员权限的模式才有可能会失败) 1.系统登录的帐号必须有Administrators权限2.一些防火墙会防止插件注入窗口所在进程，比如360防火墙等，必须把dm.dll设置为信任. 3.还有一个比较弱智的可能性，那就是插件没有注册到系统中，这时CreateObject压根就是失败的.检测对象是否创建成功很简单，如下代码set dm=createobject("dm.dmsoft") ver=dm.Ver() If len(ver)=0 Then MessageBox"创建对象失败,检查系统是否禁用了vbs脚本权限"EndScript End If 4.在沙盘中开的窗口进程，绑定一些需要管理员权限的模式，会失败。解决方法是要配置沙盘参数，具体如何配置参考沙盘绑定方法. 5.窗口所在进程有保护，这个我也无能为力.

----

### dm_bindWindowEx

**签名**：`long dm_bindWindowEx(intptr_t hwnd, const char* display, const char* mouse, const char* keypad, const char* pub, long mode)`

**简介**：绑定指定的窗口,并指定这个窗口的屏幕颜色获取方式,鼠标仿真模式,键盘仿真模式高级用户使用.

**原型**：long BindWindowEx(hwnd, display, mouse, keypad, public, mode)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `display` (字符串)：屏幕颜色获取方式取值有以下几种"normal":正常模式,平常我们用的前台截屏模式"gdi": gdi模式,用于窗口采用GDI方式刷新时.此模式占用CPU较大.参考SetAero . win10以上系统使用此模式，如果截图失败，尝试把目标程序重新开启再试试。"gdi2": gdi2模式,此模式兼容性较强,但是速度比gdi模式要慢许多,如果gdi模式发现后台不刷新时,可以考虑用gdi2模式."dx2": dx2模式,用于窗口采用dx模式刷新,如果dx方式会出现窗口进程崩溃的状况,可以考虑采用这种.采用这种方式要保证窗口有一部分在屏幕外.win7 win8或者vista不需要移动也可后台.此模式占用CPU较大.参考SetAero . win10以上系统使用此模式，如果截图失败，尝试把目标程序重新开启再试试。"dx3": dx3模式,同dx2模式,但是如果发现有些窗口后台不刷新时,可以考虑用dx3模式,此模式比dx2模式慢许多.此模式占用CPU较大.参考SetAero . win10以上系统使用此模式，如果截图失败，尝试把目标程序重新开启再试试。dx模式,用于窗口采用dx模式刷新,取值可以是以下任意组合，组合采用"|"符号进行连接.支持BindWindow中的缩写模式.比如dx代表"dx.graphic.2d|dx.graphic.3d"1."dx.graphic.2d"2d窗口的dx图色模式2."dx.graphic.2d.2"2d窗口的dx图色模式是dx.graphic.2d的增强模式.兼容性更好. 3."dx.graphic.3d"3d窗口的dx图色模式, 4."dx.graphic.3d.8"3d窗口的dx8图色模式,此模式对64位进程无效. 5."dx.graphic.opengl"3d窗口的opengl图色模式,极少数窗口采用opengl引擎刷新.此图色模式速度可能较慢.有些窗口采用这个参数截图后,上下是反向的,可以在公共参数里增加dx.public.graphic.revert来反转. 6."dx.graphic.opengl.esv2"3d窗口的opengl_esv2图色模式,极少数窗口采用opengl引擎刷新.此图色模式速度可能较慢.有些窗口采用这个参数截图后,上下是反向的,可以在公共参数里增加dx.public.graphic.revert来反转. 7."dx.graphic.3d.10plus"3d窗口的dx10 dx11 dx12图色模式

- `mouse` (字符串)：鼠标仿真模式取值有以下几种"normal":正常模式,平常我们用的前台鼠标模式"windows": Windows模式,采取模拟windows消息方式同按键的后台插件."windows3": Windows3模式，采取模拟windows消息方式,可以支持有多个子窗口的窗口后台dx模式,取值可以是以下任意组合.组合采用"|"符号进行连接.支持BindWindow中的缩写模式,比如windows2代表"dx.mouse.position.lock.api|dx.mouse.position.lock.message|dx.mouse.state.message"1."dx.mouse.position.lock.api"此模式表示通过封锁系统API，来锁定鼠标位置. 2."dx.mouse.position.lock.message"此模式表示通过封锁系统消息，来锁定鼠标位置. 3."dx.mouse.focus.input.api"此模式表示通过封锁系统API来锁定鼠标输入焦点. 4."dx.mouse.focus.input.message"此模式表示通过封锁系统消息来锁定鼠标输入焦点. 5."dx.mouse.clip.lock.api"此模式表示通过封锁系统API来锁定刷新区域。注意，使用这个模式，在绑定前，必须要让窗口完全显示出来. 6."dx.mouse.input.lock.api"此模式表示通过封锁系统API来锁定鼠标输入接口. 7."dx.mouse.state.api"此模式表示通过封锁系统API来锁定鼠标输入状态. 8."dx.mouse.state.message"此模式表示通过封锁系统消息来锁定鼠标输入状态. 9."dx.mouse.api"此模式表示通过封锁系统API来模拟dx鼠标输入. 10."dx.mouse.cursor"开启此模式，可以后台获取鼠标特征码. 11."dx.mouse.raw.input"有些窗口需要这个才可以正常操作鼠标. 12."dx.mouse.input.lock.api2"部分窗口在后台操作时，前台鼠标会移动,需要这个属性. 13."dx.mouse.input.lock.api3"部分窗口在后台操作时，前台鼠标会移动,需要这个属性. 14."dx.mouse.raw.input.active"这个参数必须配合dx.mouse.raw.input才有意义.有些窗口必须配合这个参数才可以后台.使用这个参数时，必须在绑定前激活目标窗口. (这个参数能不用就不要用,除非非用不可,可能会对前台产生干扰)

- `keypad` (字符串)：键盘仿真模式取值有以下几种"normal":正常模式,平常我们用的前台键盘模式"windows": Windows模式,采取模拟windows消息方式同按键的后台插件. dx模式,取值可以是以下任意组合.组合采用"|"符号进行连接.支持BindWindow中的缩写模式.比如dx代表"dx.public.active.api|dx.public.active.message|dx.keypad.state.api|dx.keypad.api|dx.keypad.input.lock.api"1."dx.keypad.input.lock.api"此模式表示通过封锁系统API来锁定键盘输入接口. 2."dx.keypad.state.api"此模式表示通过封锁系统API来锁定键盘输入状态. 3."dx.keypad.api"此模式表示通过封锁系统API来模拟dx键盘输入. 4."dx.keypad.raw.input"有些窗口需要这个才可以正常操作键盘. 5."dx.keypad.raw.input.active"这个参数必须配合dx.keypad.raw.input才有意义.有些窗口必须配合这个参数才可以后台.使用这个参数时，必须在绑定前激活目标窗口. (这个参数能不用就不要用,除非非用不可,可能会对前台产生干扰)

- `public` (字符串)：公共属性dx模式共有取值可以是以下任意组合.组合采用"|"符号进行连接这个值可以为空1."dx.public.active.api"此模式表示通过封锁系统API来锁定窗口激活状态.注意，部分窗口在此模式下会耗费大量资源慎用. 2."dx.public.active.message"此模式表示通过封锁系统消息来锁定窗口激活状态.注意，部分窗口在此模式下会耗费大量资源慎用.另外如果要让此模式生效，必须在绑定前，让绑定窗口处于激活状态,否则此模式将失效.比如dm.SetWindowState hwnd, 1然后再绑定. 3."dx.public.disable.window.position"此模式将锁定绑定窗口位置.不可与"dx.public.fake.window.min"共用. 4."dx.public.disable.window.size"此模式将锁定绑定窗口,禁止改变大小.不可与"dx.public.fake.window.min"共用. 5."dx.public.disable.window.minmax"此模式将禁止窗口最大化和最小化,但是付出的代价是窗口同时也会被置顶.不可与"dx.public.fake.window.min"共用. 6."dx.public.fake.window.min"此模式将允许目标窗口在最小化状态时，仍然能够像非最小化一样操作..另注意，此模式会导致任务栏顺序重排，所以如果是多开模式下，会看起来比较混乱，建议单开使用，多开不建议使用.同时此模式不是万能的,有些情况下最小化以后图色会不刷新或者黑屏. 7."dx.public.hide.dll"此模式将会隐藏目标进程的大漠插件，避免被检测..另外使用此模式前，请仔细做过测试，此模式可能会造成目标进程不稳定，出现崩溃。8."dx.public.active.api2"此模式表示通过封锁系统API来锁定窗口激活状态.部分窗口遮挡无法后台,需要这个属性. 9."dx.public.input.ime"此模式是配合SendStringIme使用.具体可以查看SendStringIme接口. 10"dx.public.graphic.protect"此模式可以保护dx图色不被恶意检测.同时对dx.keypad.api和dx.mouse.api也有保护效果.这个参数可能会导致某些情况下目标图色失效.一般出现在场景重新加载的时候.重新绑定会恢复. 11"dx.public.disable.window.show"禁止目标窗口显示,这个一般用来配合dx.public.fake.window.min来使用. 12"dx.public.anti.api"此模式可以突破部分窗口对后台的保护. 13"dx.public.km.protect"此模式可以保护dx键鼠不被恶意检测.最好配合dx.public.anti.api一起使用.此属性可能会导致部分后台功能失效. 14"dx.public.prevent.block"绑定模式1 3 5 7 101 103下，可能会导致部分窗口卡死.这个属性可以避免卡死. 15"dx.public.ori.proc"此属性只能用在模式0 1 2 3和101下.有些窗口在不同的界面下(比如登录界面和登录进以后的界面)，键鼠的控制效果不相同.那可以用这个属性来尝试让保持一致.注意的是，这个属性不可以滥用，确保测试无问题才可以使用.否则可能会导致后台失效. 16"dx.public.down.cpu"此模式可以配合DownCpu来降低目标进程CPU占用.当图色方式降低CPU无效时，可以尝试此种方式.需要注意的是，当使用此方式降低CPU时，会让图色方式降低CPU失效17"dx.public.focus.message"当后台绑定后,后台无法正常在焦点窗口输入文字时,可以尝试加入此属性.此属性会强制键盘消息发送到焦点窗口.慎用此模式,此模式有可能会导致后台键盘在某些情况下失灵. 18"dx.public.graphic.speed"只针对图色中的dx模式有效.此模式会牺牲目标窗口的性能，来提高DX图色速度，尤其是目标窗口刷新很慢时，这个参数就很有用了. 19"dx.public.memory"让本对象突破目标进程防护,可以正常使用内存接口.当用此方式使用内存接口时，内存接口的速度会取决于目标窗口的刷新率. 20"dx.public.inject.super"突破某些难以绑定的窗口.此属性仅对除了模式0和2的其他模式有效. 21"dx.public.hack.speed"类似变速齿轮，配合接口HackSpeed使用22"dx.public.inject.c"突破某些难以绑定的窗口.此属性仅对除了模式0和2的其他模式有效. 23"dx.public.graphic.revert"此模式将截图后的内容上下反向.仅对图色模式为dx.graphic.opengl和dx.graphic.opengl.esv2生效.

- `mode` (整形数)：模式。取值有以下几种0:推荐模式此模式比较通用，而且后台效果是最好的. 2:同模式0,如果模式0有崩溃问题，可以尝试此模式.注意0和2模式，当主绑定(第一个绑定同个窗口的对象)绑定成功后，那么调用主绑定的线程必须一直维持,否则线程一旦推出,对应的绑定也会消失. 101:超级绑定模式.可隐藏目标进程中的dm.dll.避免被恶意检测.效果要比dx.public.hide.dll好.推荐使用. 103:同模式101，如果模式101有崩溃问题，可以尝试此模式. 11:需要加载驱动,适合一些特殊的窗口,如果前面的无法绑定，可以尝试此模式.此模式不支持32位系统13:需要加载驱动,适合一些特殊的窗口,如果前面的无法绑定，可以尝试此模式.此模式不支持32位系统需要注意的是:模式101 103在大部分窗口下绑定都没问题。但也有少数特殊的窗口，比如有很多子窗口的窗口，对于这种窗口，在绑定时，一定要把鼠标指向一个可以输入文字的窗口，比如一个文本框，最好能激活这个文本框，这样可以保证绑定的成功.

**返回值**：整形数: 0:失败1:成功如果返回0，可以调用GetLastError来查看具体失败错误码,帮助分析问题.

**示例**：比如dm_ret=dm.BindWindowEx(hwnd,"normal","dx.mouse.position.lock.api|dx.mouse.position.lock.message","windows","dx.public.active.api", 0) dm_ret=dm.BindWindowEx(hwnd,"dx2","windows","normal","dx.public.active.api", 0) dm_ret=dm.BindWindowEx(hwnd,"dx.graphic.2d","dx.mouse.position.lock.api|dx.mouse.position.lock.message","dx.keypad.state.api|dx.keypad.api","", 0) dm_ret=dm.BindWindowEx(hwnd,"dx2","windows","windows","", 0) dm_ret=dm.BindWindowEx(hwnd,"dx2","windows","windows","dx.public.disable.window.size|dx.public.disable.window.minmax", 0) dm_ret=dm.BindWindowEx(hwnd,"dx2","windows3","windows","dx.mouse.position.lock.api", 0)等等.注意:绑定之后,所有的坐标都相对于窗口的客户区坐标(不包含窗口边框)另外,绑定窗口后,必须加以下代码,以保证所有资源正常释放这个函数的意思是在脚本结束时,会调用这个函数。需要注意的是，目前的按键版本对于这个函数的执行不是线程级别的，也就是说，这个函数只会在主线程执行，子线程绑定的大漠对象，不保证完全释放。高级语言中则需要自己控制在适当的时候解除绑定. Sub OnScriptExit() dm_ret=dm.UnBindWindow() End Sub另外绑定dx会比较耗时间,请不要频繁调用此函数.还有一点特别要注意的是,有些窗口绑定之后必须加一定的延时,否则后台也无效.一般1秒到2秒的延时就足够.发现绑定失败的几种可能(一般是需要管理员权限的模式才有可能会失败) 1.系统登录的帐号必须有Administrators权限2.一些防火墙会防止插件注入窗口所在进程，比如360防火墙等，必须把dm.dll设置为信任. 3.还有一个比较弱智的可能性，那就是插件没有注册到系统中，这时CreateObject压根就是失败的.检测对象是否创建成功很简单，如下代码set dm=createobject("dm.dmsoft") ver=dm.Ver() If len(ver)=0 Then MessageBox"创建对象失败,检查系统是否禁用了vbs脚本权限"EndScript End If 4.在沙盘中开的窗口，绑定一些需要管理员权限的模式，会失败。解决方法是要配置沙盘参数，参考如何配置沙盘参数. 5.窗口所在进程有保护，这个我也无能为力.

----

### dm_downCpu

**签名**：`long dm_downCpu(long enable, long rate)`

**简介**：降低目标窗口所在进程的CPU占用.

**原型**：long DownCpu (type, rate)

**参数定义**

- `type` (整形数)：当取值为0时, rate取值范围大于等于0,这个值越大表示降低CPU效果越好当取值为1时, rate取值范围大于等于0,表示以固定的FPS来降低CPU. rate表示FPS.并且这时不能有dx.public.down.cpu .

- `rate` (整形数)：取值取决于type.为0表示关闭

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindowEx (hwnd,"dx.graphic.3d","normal","normal","", 0) dm.DownCpu 1, 10 dm_ret=dm.BindWindowEx (hwnd,"normal","normal","normal","dx.public.down.cpu", 101) dm.DownCpu 0, 50注意:此接口必须在绑定窗口成功以后调用，而且必须保证目标窗口可以支持dx.graphic.3d或者dx.graphic.3d.8或者dx.graphic.2d或者dx.graphic.2d.2或者dx.graphic.opengl或者dx.graphic.opengl.esv2方式截图，或者使用dx.public.down.cpu (仅限type为0).否则降低CPU无效.因为降低CPU是通过降低窗口刷新速度或者在系统消息循环增加延时来实现，所以注意，开启此功能以后会导致窗口刷新速度变慢.

----

### dm_enableBind

**签名**：`long dm_enableBind(long enable)`

**简介**：设置是否暂时关闭或者开启后台功能.默认是开启.一般用在前台切换，或者脚本暂停和恢复时，可以让用户操作窗口.

**原型**：long EnableBind (enable)

**参数定义**

- `enable` (整形数)：0全部关闭(图色键鼠都关闭,也就是说图色,键鼠都是前台,但是如果有指定dx.public.active.message时，在窗口前后台切换时，这个属性会失效.) -1只关闭图色.(也就是说图色是normal前台.键鼠不变) 1开启(恢复原始状态) 5同0，也是全部关闭，但是这个模式下，就算窗口在前后台切换时，属性dx.public.active.message的效果也一样不会失效.

**返回值**：整形数: 0:失败1:成功

**示例**：//绑定为后台dm_ret=dm.BindWindow (hwnd,"dx","dx","dx", 101)//后台操作…//切换到前台dm.EnableBind 0//前台操作…//再切换回后台dm.EnableBind 1

**注**：注意切换到前台以后,相当于dm_ret=dm.BindWindow (hwnd,"normal","normal","normal", 0),图色键鼠全部是前台.如果你经常有频繁切换后台和前台的操作，推荐使用这个函数.同时要注意,如果有多个对象绑定了同个窗口，其中任何一个对象禁止了后台,那么其他对象后台也同样失效.

----

### dm_enableFakeActive

**签名**：`long dm_enableFakeActive(long enable)`

**简介**：设置是否开启后台假激活功能.默认是关闭.一般用不到.除非有人有特殊需求.注意看注释.

**原型**：long EnableFakeActive (enable)

**参数定义**

- `enable` (整形数)：0关闭1开启

**返回值**：整形数: 0:失败1:成功

**示例**：//绑定以后再调用此函数dm.EnableFakeActive 1//这里做需要在窗口非激活状态下,可以操作的接口或者第三方函数…//恢复dm.EnableFakeActive 0

**注**：此接口的含义并不是关闭或者开启窗口假激活功能(dx.public.active.api或者dx.public.active.message).而是说有些时候，本来窗口没有激活并且在没有绑定的状态下，可以正常使用的功能，而在窗口绑定以后,并且窗口在非激活状态下,此时由于绑定的锁定导致无法使用.那么，你就需要把你的部分代码用EnableFakeActive来保护起来。这样就让插件认为你的这段代码是在窗口激活状态下执行.另外，此函数开启以后，有可能会让前台影响到后台.所以如果不是特殊情况，最好是关闭.开启这个还会把已经锁定的键盘鼠标(LockInput)强制解锁.有些时候，有人会故意利用这个前台影响后台的作用，做类似同步器的软件，那这个函数就很有作用了.另外,还有一些窗口对消息检测比较严格,那么需要开启这个接口才可以后台操作,或者组合键操作.

----

### dm_enableIme

**签名**：`long dm_enableIme(long enable)`

**简介**：设置是否关闭绑定窗口所在进程的输入法.

**原型**：long EnableIme(enable)

**参数定义**

- `enable` (整形数)：1开启0关闭

**返回值**：整形数: 0:失败1:成功

**示例**：//绑定为后台dm_ret=dm.BindWindow(hwnd,"dx","dx","dx", 101)…//关闭输入法dm.EnableIme 0…//再开启输入法dm.EnableIme 1

**注**：此函数必须在绑定后调用才有效果.

----

### dm_enableKeypadMsg

**签名**：`long dm_enableKeypadMsg(long enable)`

**简介**：是否在使用dx键盘时开启windows消息.默认开启.

**原型**：long EnableKeypadMsg (enable)

**参数定义**

- `enable` (整形数)：0禁止1开启

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindow (hwnd,"dx","dx2","dx", 0) dm.EnableKeypadMsg 0

**注**：此接口必须在绑定之后才能调用。特殊时候使用.

----

### dm_enableKeypadPatch

**签名**：`long dm_enableKeypadPatch(long enable)`

**简介**：键盘消息发送补丁.默认是关闭.

**原型**：long EnableKeypadPatch (enable)

**参数定义**

- `enable` (整形数)：0禁止1开启

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindow (hwnd,"dx","dx2","dx", 0) dm.EnableKeypadPatch 1

**注**：此接口必须在绑定之后才能调用。

----

### dm_enableKeypadSync

**签名**：`long dm_enableKeypadSync(long enable)`

**简介**：键盘消息采用同步发送模式.默认异步.

**原型**：long EnableKeypadSync (enable, time_out)

**参数定义**

- `enable` (整形数)：0禁止同步1开启同步

- `time_out` (整形数)：单位是毫秒,表示同步等待的最大时间.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindow (hwnd,"dx","dx2","dx", 0) dm.EnableKeypadSync 1, 200

**注**：此接口必须在绑定之后才能调用。有些时候，如果是异步发送，如果发送动作太快,中间没有延时,有可能下个动作会影响前面的.而用同步就没有这个担心.

----

### dm_enableMouseMsg

**签名**：`long dm_enableMouseMsg(long enable)`

**简介**：是否在使用dx鼠标时开启windows消息.默认开启.

**原型**：long EnableMouseMsg(enable)

**参数定义**

- `enable` (整形数)：0禁止1开启

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindow(hwnd,"dx","dx2","dx", 0) dm.EnableMouseMsg 0

**注**：此接口必须在绑定之后才能调用。特殊时候使用.

----

### dm_enableMouseSync

**签名**：`long dm_enableMouseSync(long enable, long time_out)`

**简介**：鼠标消息采用同步发送模式.默认异步.

**原型**：long EnableMouseSync(enable, time_out)

**参数定义**

- `enable` (整形数)：0禁止同步1开启同步

- `time_out` (整形数)：单位是毫秒,表示同步等待的最大时间.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindow(hwnd,"dx","dx2","dx", 0) dm.EnableMouseSync 1, 200

**注**：此接口必须在绑定之后才能调用。有些时候，如果是异步发送，如果发送动作太快,中间没有延时,有可能下个动作会影响前面的.而用同步就没有这个担心.

----

### dm_enableRealKeypad

**签名**：`long dm_enableRealKeypad(long enable)`

**简介**：键盘动作模拟真实操作,点击延时随机.

**原型**：long EnableRealKeypad (enable)

**参数定义**

- `enable` (整形数)：0关闭模拟1开启模拟

**返回值**：整形数: 0:失败1:成功

**示例**：dm.EnableRealKeypad 1 dm.KeyPressChar"E"

**注**：此接口对KeyPress KeyPressChar KeyPressStr起作用。具体表现是键盘按下和弹起的间隔会在当前设定延时的基础上,上下随机浮动50%.假如设定的键盘延时是100,那么这个延时可能就是50-150之间的一个值.设定延时的函数是SetKeypadDelay

----

### dm_enableRealMouse

**签名**：`long dm_enableRealMouse(long enable, long mousedelay, long mousestep)`

**简介**：鼠标动作模拟真实操作,带移动轨迹,以及点击延时随机.

**原型**：long EnableRealMouse(enable, mousedelay, mousestep)

**参数定义**

- `enable` (整形数)：0关闭模拟1开启模拟(直线模拟) 2开启模拟(随机曲线,更接近真实) 3开启模拟(小弧度曲线,弧度随机) 4开启模拟(大弧度曲线,弧度随机)

- `mousedelay` (整形数)：单位是毫秒.表示在模拟鼠标移动轨迹时,每移动一次的时间间隔.这个值越大,鼠标移动越慢.必须大于0,否则会失败.

- `Mousestep` (整形数)：表示在模拟鼠标移动轨迹时,每移动一次的距离.这个值越大，鼠标移动越快速.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.EnableRealMouse 1, 20, 30 dm.MoveTo 100, 100 dm.MoveTo 500, 500

**注**：此接口同样对LeftClick RightClick MiddleClick LeftDoubleClick起作用。具体表现是鼠标按下和弹起的间隔会在当前设定延时的基础上,上下随机浮动50%.假如设定的鼠标延时是100,那么这个延时可能就是50-150之间的一个值.设定延时的函数是SetMouseDelay

----

### dm_enableSpeedDx

**签名**：`long dm_enableSpeedDx(long enable)`

**简介**：设置是否开启高速dx键鼠模式。默认是关闭.

**原型**：long EnableSpeedDx (enable)

**参数定义**

- `enable` (整形数)：0关闭1开启

**返回值**：整形数: 0:失败1:成功

**示例**：dm.EnableSpeedDx 1

**注**：此函数开启的后果就是，所有dx键鼠操作将不会等待，适用于某些特殊的场合(比如避免窗口无响应导致宿主进程也卡死的问题). EnableMouseSync和EnableKeyboardSync开启以后，此函数就无效了.此函数可能在部分窗口下会有副作用，谨慎使用!!

----

### dm_forceUnBindWindow

**签名**：`long dm_forceUnBindWindow()`

**简介**：强制解除绑定窗口,并释放系统资源.

**原型**：long ForceUnBindWindow (hwnd)

**参数定义**

- `hwnd` (整形数)：需要强制解除绑定的窗口句柄.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.ForceUnBindWindow (hwnd)

**注**：此接口一般用在BindWindow和BindWindowEx中，使用了模式1 3 5 7或者属性dx.public.hide.dll后，在线程或者进程结束后，没有正确调用UnBindWindow而导致下次绑定无法成功时，可以先调用这个函数强制解除绑定，并释放资源，再进行绑定.此接口不可替代UnBindWindow .只是用在非常时刻.切记.一般情况下可以无条件的在BindWindow或者BindWindowEx之前调用一次此函数。保证此刻窗口处于非绑定状态.另外，需要注意的是,此函数只可以强制解绑在同进程绑定的窗口.不可在不同的进程解绑别的进程绑定的窗口.(会产生异常)

----

### dm_getBindWindow

**签名**：`long dm_getBindWindow()`

**简介**：获取当前对象已经绑定的窗口句柄.无绑定返回0

**原型**：long GetBindWindow ()

**返回值**：整形数:窗口句柄

**示例**：bind_hwnd=dm.GetBindWindow ()

----

### dm_getFps

**签名**：`long dm_getFps()`

**简介**：获取绑定窗口的fps. (即时fps,不是平均fps).要想获取fps,那么图色模式必须是dx模式的其中一种.比如dx.graphic.3d dx.graphic.opengl等.

**原型**：long GetFps ()

**返回值**：整形数: fps

**示例**：fps=dm.GetFps ()

----

### dm_hackSpeed

**签名**：`long dm_hackSpeed(long rate)`

**简介**：对目标窗口设置加速功能(类似变速齿轮),必须在绑定参数中有dx.public.hack.speed时才会生效.

**原型**：long HackSpeed (rate)

**参数定义**

- `rate` (双精度浮点数)：取值范围大于0.默认是1.0表示不加速，也不减速.小于1.0表示减速,大于1.0表示加速.精度为小数点后1位.也就是说1.5和1.56其实是一样的.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindowEx (hwnd,"normal","normal","normal","dx.public.hack.speed", 0)//2倍加速dm.HackSpeed 2.0//2.5倍dm.HackSpeed 2.5//10.1倍dm.HackSpeed 10.1//速度降低为原来的一半dm.HackSpeed 0.5//速度降低为原来的十分之一dm.HackSpeed 0.1注意:此接口必须在绑定窗口成功以后调用，而且必须有参数dx. public.hack.speed .不一定对所有窗口有效,具体自己测试.

----

### dm_isBind

**签名**：`long dm_isBind(intptr_t hwnd)`

**简介**：判定指定窗口是否已经被后台绑定. (前台无法判定)

**原型**：long IsBind(hwnd)

**参数定义**

- `hwnd` (整形数)：窗口句柄

**返回值**：整形数: 0:没绑定,或者窗口不存在. 1:已经绑定.

**示例**：dm_ret=dm.IsBind(hwnd)

----

### dm_lockDisplay

**签名**：`long dm_lockDisplay(long enable)`

**简介**：锁定指定窗口的图色数据(不刷新).

**原型**：long LockDisplay (lock)

**参数定义**

- `lock` (整形数)：0关闭锁定1开启锁定

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindow (hwnd,"dx","dx2","dx", 0) dm.LockDisplay 1//这里做需要锁定做的事情dm.LockDisplay 0注意:此接口只对图色为dx.graphic.3d dx.graphic.3d.8 dx.graphic.2d dx.graphic.2d.2 dx.graphic.3d.10plus有效.

----

### dm_lockInput

**签名**：`long dm_lockInput(long enable)`

**简介**：禁止外部输入到指定窗口

**原型**：long LockInput (lock)

**参数定义**

- `lock` (整形数)：0关闭锁定1开启锁定(键盘鼠标都锁定) 2只锁定鼠标3只锁定键盘4同1,但当您发现某些特殊按键无法锁定时,比如(回车，ESC等)，那就用这个模式吧.但此模式会让SendString函数后台失效，或者采用和SendString类似原理发送字符串的其他3方函数失效. 5同3,但当您发现某些特殊按键无法锁定时,比如(回车，ESC等)，那就用这个模式吧.但此模式会让SendString函数后台失效，或者采用和SendString类似原理发送字符串的其他3方函数失效.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.BindWindow (hwnd,"dx","dx2","dx", 0) dm.LockInput 1//这里做需要锁定输入做的事情dm.LockInput 0注意:此接口只针对dx键鼠.普通键鼠无效.有时候，绑定为dx2鼠标模式时(或者没有锁定鼠标位置或状态时)，在脚本处理过程中，在某个时候需要临时锁定外部输入，以免外部干扰，那么这个函数就非常有用.比如某个信息，需要鼠标移动到某个位置才可以获取，但这时，如果外部干扰，那么很可能就会获取失败，所以，这时候就很有必要锁定外部输入.当然，锁定完以后，记得要解除锁定，否则外部永远都无法输入了，除非解除了窗口绑定.

----

### dm_lockMouseRect

**签名**：`long dm_lockMouseRect(long x1, long y1, long x2, long y2)`

**简介**：设置前台鼠标在屏幕上的活动范围.

**原型**：long LockMouseRect (x1, y1, x2, y2)

**参数定义**

- `x1` (整形数)：区域的左上X坐标.屏幕坐标.

- `y1` (整形数)：区域的左上Y坐标.屏幕坐标.

- `x2` (整形数)：区域的右下X坐标.屏幕坐标.

- `y2` (整形数)：区域的右下Y坐标.屏幕坐标.

**返回值**：整形数: 0:失败1:成功

**示例**：//限制鼠标只能在10, 10, 800, 600区域内活动. dm.LockMouseRect 10, 10, 800, 600…Sub OnScriptExit ()//恢复, 4个参数都是0,表示恢复鼠标活动范围为整个屏幕区域. dm.LockMouseRect 0, 0, 0, 0 End Sub

**注**：调用此函数后，一旦有窗口切换或者窗口移动的动作，那么限制立刻失效.如果想一直限制鼠标范围在指定的窗口客户区域，那么你需要启动一个线程，并且时刻监视当前活动窗口，然后根据情况调用此函数限制鼠标范围.

----

### dm_setAero

**签名**：`long dm_setAero(long enable)`

**简介**：设置开启或者关闭系统的Aero效果. (仅对WIN7及以上系统有效)

**原型**：long SetAero (enable)

**参数定义**

- `enable` (整形数)：0关闭1开启

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetAero 0

**注**：如果您发现当图色后台为dx2 gdi dx3时，如果有发现目标窗口刷新速度过慢,那可以考虑关闭系统Aero. (当然这仅仅是可能的原因)

----

### dm_setDisplayDelay

**签名**：`long dm_setDisplayDelay(long delay)`

**简介**：设置dx截图最长等待时间。内部默认是3000毫秒.一般用不到调整这个.

**原型**：long SetDisplayDelay (time)

**参数定义**

- `time` (整形数)：等待时间，单位是毫秒。注意这里不能设置的过小，否则可能会导致截图失败,从而导致图色函数和文字识别失败.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetDisplayDelay 500

**注**：此接口仅对图色为dx.graphic.3d dx.graphic.3d.8 dx.graphic.2d dx.graphic.2d.2有效.其他图色模式无效.默认情况下，截图需要等待一个延时，超时就认为截图失败.这个接口可以调整这个延时.某些时候或许有用.比如当窗口图色卡死(这时获取图色一定都是超时)，并且要判断窗口卡死，那么这个设置就很有用了。

----

### dm_setDisplayRefreshDelay

**签名**：`long dm_setDisplayRefreshDelay(long delay)`

**简介**：设置opengl图色模式的强制刷新窗口等待时间.内置为400毫秒.

**原型**：long SetDisplayRefreshDelay (time)

**参数定义**

- `time` (整形数)：等待时间，单位是毫秒。这个值越小,强制刷新的越频繁，相应的窗口可能会导致闪烁.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetDisplayRefreshDelay 800

**注**：此接口仅对图色为dx.graphic.opengl有效.其他图色模式无效.默认情况下，openg截图时，如果对应的窗口处于不刷新的状态,那么我们的所有图色接口都会无法截图,从而超时导致失效。所以特意设置这个接口，如果截图的时间超过此接口设置的时间,那么插件会对绑定的窗口强制刷新,从而让截图成功.但是强制刷新窗口是有代价的，会造成窗口闪烁。如果您需要截图的窗口，刷新非常频繁，那么一般用不到强制刷新，所以可以用这个接口把等待时间设置大一些，从而避免窗口闪烁.反之,如果您需要截图的窗口偶尔才刷新一次(比如按某个按钮，才刷新一次),那么您就需要用这个接口把等待时间设置小一些，从而提高图色函数的效率，但代价就是窗口可能会闪烁.当这个接口设置的值超过SetDisplayDelay设置的值(默认是3000毫秒)时,那么opengl截图的方式就退化到老版本(大概是6.1540版本)的模式.(也就是不会强制刷新的版本).如果您发现你的程序截图会截取到以前的图片,那么建议把此值加大(建议值2000).如果您发现你的程序偶尔会闪烁,导致窗口出现白色区域,那么可以尝试把此值设置为大于SetDisplayDelay的值(默认是3000毫秒),这样可以彻底杜绝刷新.

----

### dm_setInputDm

**签名**：`long dm_setInputDm(long enable)`

**简介**：设置当前对象用于输入的对象.结合图色对象和键鼠对象,用一个对象完成操作.

**原型**：long SetInputDm(dm_id, rx, ry)

**参数定义**

- `dm_id` (整形数)：接口GetId的返回值

- `rx` (整形数)：两个对象绑定的窗口的左上角坐标的x偏移.是用dm_id对应的窗口的左上角x坐标减去当前窗口左上角坐标的x坐标.一般是0

- `ry` (整形数)：两个对象绑定的窗口的左上角坐标的y偏移.是用dm_id对应的窗口的左上角y坐标减去当前窗口左上角坐标的y坐标.一般是0

**返回值**：整形数: 0:失败1:成功

**示例**：//创建2个对象set dm=createobject("dm.dmsoft") set dm2=createobject("dm.dmsoft") dm.SetPath dm.GetBasePath() hwnd=dm.FindWindowSuper("sub", 0, 1,"subWin", 2, 1) hwnd2=dm.GetWindow(hwnd, 0)//两个对象分别绑定对应的窗口dm_ret=dm.BindWindowEx(hwnd,"dx.graphic.opengl","windows","windows","", 0) dm_ret2=dm2.BindWindowEx(hwnd2,"normal","dx.mouse.position.lock.api","dx.keypad.input.lock.api","", 0) TracePrint dm_ret TracePrint dm_ret2//dm用于图色dm2用于键鼠.结合起来.让dm同时处理图色和键鼠TracePrint dm.SetInputDm(dm2.GetId(), 0, 0)//正常进行处理.比如找图找字等等. dm_ret=dm.FindPic(0, 0, 2000, 2000,"test.bmp","000000", 0.9, 0, x, y) TracePrint cstr(x) if x>0 then dm.MoveTo x, y delay 100 dm.LeftClick end if ...//解绑dm.UnBindWindow dm2.UnBindWindow

**注**：此接口用于特殊用途.比如雷电模拟器.最里层的窗口是一个64位窗口.绑定这个窗口的图色可以用来图色后台.但是这个窗口无法进行键鼠后台.能够键鼠窗口后台的是这个窗口的上一层32位窗口.但这个32位窗口在某些情况下图色会出问题.所以比较好的解决办法是创建2个对象.一个绑定64位的窗口，用来进行图色使用.另一个绑定32位的窗口,用来进行键鼠操作.但是如果对于写好的代码来说,更改起来很麻烦.因为大部分情况下一个对象就够用了.为了让代码不用大幅度更改,就加了这样一个接口.让进行图色绑定的那个对象和进行键鼠操作的那个对象结合起来.这样只用操作一个图色绑定的对象就行了.这里要注意的是,如果2个对象对应的窗口不是一个进程,那么绑定参数上没什么要求.如果是一个进程,那么必须保证只有一个对象能够使用注入的参数.否则会引发冲突导致崩溃.还有rx和ry的具体含义.解释如下:一般来说,我们调用MoveTo或者MoveToEx时,传递进来的x和y坐标都是来自于图色窗口,但是键鼠操作的那个窗口是另一个窗口.如果这2个窗口左上角是重合的,那么无所谓rx和ry就是0.比如我们这里的雷电模拟器等窗口.但是如果不重合,那么我们传递给MoveTo或者MoveToEx的x和y就和键鼠操作的窗口的x和y不对应.所以就必须从图色的x, y减去两个窗口的左上角偏移,这样才能对应键鼠操作的窗口.一般来说, rx和ry都是0.可能有极少数有这种不为0的特例(我是暂时没发现).这里的rx和ry必须是键鼠操作的窗口左上角减去图色窗口的左上角,不能是反的.另外在解绑时,会自动重置.即图色窗口的对象自动和键鼠窗口的对象脱离.需要注意的是,因为两个对象进行了结合,那么就要确保两个对象的生命周期必须是一致的.尤其千万不能在图色窗口操作时,键鼠对象被释放了.那么会导致程序的崩溃.这个接口影响的输入接口如下列表(即图色对象调用了SetDmInput后,以下这些接口统统都是对键鼠对象的调用) GetCursorPos GetCursorShape GetCursorShapeEx GetCursorSpot KeyDown KeyDownChar KeyPress KeyPressChar KeyPressStr KeyUp KeyUpChar LeftClick LeftDoubleClick LeftDown LeftUp MiddleClick MiddleDown MiddleUp MoveR MoveTo MoveToEx RightClick RightDown RightUp SetKeypadDelay SetMouseDelay SetSimMode WheelDown WheelUp EnableFakeActive EnableKeypadMsg EnableKeypadPatch EnableKeypadSync EnableMouseMsg EnableMouseSync EnableRealKeypad EnableRealMouse EnableSpeedDx LockInput

----

### dm_switchBindWindow

**签名**：`long dm_switchBindWindow(intptr_t hwnd)`

**简介**：在不解绑的情况下,切换绑定窗口.(必须是同进程窗口)

**原型**：long SwitchBindWindow (hwnd)

**参数定义**

- `hwnd` (整形数)：需要切换过去的窗口句柄

**返回值**：整形数: 0:失败1:成功

**示例**：//绑定为后台dm_ret=dm.BindWindow (hwnd,"dx","dx","dx", 101)//切换hwnd1=111 dm. SwitchBindWindow (hwnd1)

**注**：此函数一般用在绑定以后，窗口句柄改变了的情况。如果必须不解绑，那么此函数就很有用了。

----

### dm_unBindWindow

**签名**：`long dm_unBindWindow()`

**简介**：解除绑定窗口,并释放系统资源.一般在OnScriptExit调用

**原型**：long UnBindWindow()

**返回值**：整形数: 0:失败1:成功

**示例**：Sub OnScriptExit() dm_ret=dm.UnBindWindow() End Sub

----

### dm_doubleToData

**签名**：`const char* dm_doubleToData(double v)`

**简介**：把双精度浮点数转换成二进制形式.

**原型**：string DoubleToData(value)

**参数定义**

- `value` (双精度浮点数)：需要转化的双精度浮点数

**返回值**：字符串:字符串形式表达的二进制数据.可以用于WriteData FindData FindDataEx等接口.

**示例**：double_data=dm.DoubleToData(1.24) dm_ret=dm.FindData(hwnd,"00000000-7fffffff", double_data)

----

### dm_findData

**签名**：`const char* dm_findData(intptr_t hwnd, const char* addr_range, const char* data)`

**简介**：搜索指定的二进制数据,默认步长是1.默认开启多线程,默认搜索全部内存军类型.如果要定制搜索,请用FindDataEx

**原型**：string FindData(hwnd, addr_range, data)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等.

- `data` (字符串)：要搜索的二进制数据以字符串的形式描述比如"00 01 23 45 67 86 ab ce f1"等.这里也可以支持模糊查找,用??来代替单个字节.比如"00 01????67 86??ce f1"等.注意,这里不支持半个字节,比如3?这种不行.

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindData(hwnd,"00000000-FFFFFFFF","00 01 23 45 67 86 ab ce f1") if len(result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound(result)+1 MessageBox"找到"&count&"个地址"DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_findDataEx

**签名**：`const char* dm_findDataEx(intptr_t hwnd, const char* addr_range, const char* data, long step, long multi_thread, long mode)`

**简介**：搜索指定的二进制数据.

**原型**：string FindDataEx(hwnd, addr_range, data, step, multi_thread, mode)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等.

- `data` (字符串)：要搜索的二进制数据以字符串的形式描述比如"00 01 23 45 67 86 ab ce f1"等这里也可以支持模糊查找,用??来代替单个字节.比如"00 01????67 86??ce f1"等.注意,这里不支持半个字节,比如3?这种不行.

- `step` (整形数)：搜索步长.

- `multi_thread` (整形数)：表示是否开启多线程查找. 0不开启，1开启.开启多线程查找速度较快，但会耗费较多CPU资源.不开启速度较慢，但节省CPU.

- `mode` (整形数)：1表示开启快速扫描(略过只读内存) 0表示所有内存类型全部扫描.

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindDataEx(hwnd,"00000000-FFFFFFFF","00 01 23 45 67 86 ab ce f1", 4, 1, 0) if len(result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound(result)+1 MessageBox"找到"&count&"个地址"

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_findDouble

**签名**：`const char* dm_findDouble(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max)`

**简介**：搜索指定的双精度浮点数,默认步长是1.如果要定制步长，请用FindDoubleEx

**原型**：string FindDouble(hwnd, addr_range, double_value_min, double_value_max)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等.

- `double_value_min` (双精度浮点数)：搜索的双精度数值最小值

- `double_value_max` (双精度浮点数)：搜索的双精度数值最大值最终搜索的数值大与等于double_value_min,并且小于等于double_value_max

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindDouble(hwnd,"00000000-FFFFFFFF", 0.5, 1.0) if len(result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound(result)+1 MessageBox"找到"&count&"个地址"

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_findDoubleEx

**签名**：`const char* dm_findDoubleEx(intptr_t hwnd, const char* addr_range, double double_value_min, double double_value_max, long step, long multi_thread, long mode)`

**简介**：搜索指定的双精度浮点数.

**原型**：string FindDoubleEx(hwnd, addr_range, double_value_min, double_value_max, step, multi_thread, mode)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等.

- `double_value_min` (双精度浮点数)：搜索的双精度数值最小值

- `double_value_max` (双精度浮点数)：搜索的双精度数值最大值最终搜索的数值大与等于double_value_min,并且小于等于double_value_max

- `step` (整形数)：搜索步长.

- `multi_thread` (整形数)：表示是否开启多线程查找. 0不开启，1开启.开启多线程查找速度较快，但会耗费较多CPU资源.不开启速度较慢，但节省CPU.

- `mode` (整形数)：1表示开启快速扫描(略过只读内存) 0表示所有内存类型全部扫描.

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindDoubleEx(hwnd,"00000000-FFFFFFFF", 0.5, 1.0, 8, 1, 0) if len(result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound(result)+1 MessageBox"找到"&count&"个地址"

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_findFloat

**签名**：`const char* dm_findFloat(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max)`

**简介**：搜索指定的单精度浮点数,默认步长是1.如果要定制步长，请用FindFloatEx

**原型**：string FindFloat(hwnd, addr_range, float_value_min, float_value_max)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等.

- `float_value_min` (单精度浮点数)：搜索的单精度数值最小值

- `float_value_max` (单精度浮点数)：搜索的单精度数值最大值最终搜索的数值大与等于float_value_min,并且小于等于float_value_max

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindFloat(hwnd,"00000000-FFFFFFFF", 0.5, 1.0) if len(result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound(result)+1 MessageBox"找到"&count&"个地址"

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_findFloatEx

**签名**：`const char* dm_findFloatEx(intptr_t hwnd, const char* addr_range, float float_value_min, float float_value_max, long step, long multi_thread, long mode)`

**简介**：搜索指定的单精度浮点数.

**原型**：string FindFloatEx(hwnd, addr_range, float_value_min, float_value_max, step, multi_thread, mode)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等.

- `float_value_min` (单精度浮点数)：搜索的单精度数值最小值

- `float_value_max` (单精度浮点数)：搜索的单精度数值最大值最终搜索的数值大与等于float_value_min,并且小于等于float_value_max

- `step` (整形数)：搜索步长.

- `multi_thread` (整形数)：表示是否开启多线程查找. 0不开启，1开启.开启多线程查找速度较快，但会耗费较多CPU资源.不开启速度较慢，但节省CPU.

- `mode` (整形数)：1表示开启快速扫描(略过只读内存) 0表示所有内存类型全部扫描.

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindFloatEx(hwnd,"00000000-FFFFFFFF", 0.5, 1.0, 4, 1, 0) if len(result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound(result)+1 MessageBox"找到"&count&"个地址"

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_findInt

**签名**：`const char* dm_findInt(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max)`

**简介**：搜索指定的整数,默认步长是1.如果要定制步长，请用FindIntEx

**原型**：string FindInt (hwnd, addr_range, int_value_min, int_value_max, type)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等. int_value_min长整形数:搜索的整数数值最小值int_value_max长整形数:搜索的整数数值最大值最终搜索的数值大与等于int_value_min,并且小于等于int_value_max

- `type` (整形数)：搜索的整数类型,取值如下0: 32位1: 16位2: 8位3: 64位

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindInt (hwnd,"00000000-FFFFFFFF", 300, 300, 0) if len (result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound (result)+1 MessageBox"找到"&count&"个地址"

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_findIntEx

**签名**：`const char* dm_findIntEx(intptr_t hwnd, const char* addr_range, long int_value_min, long int_value_max, long step, long multi_thread, long mode)`

**简介**：搜索指定的整数.

**原型**：string FindIntEx (hwnd, addr_range, int_value_min, int_value_max, type, step, multi_thread, mode)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等. int_value_min长整形数:搜索的整数数值最小值i nt_value_max长整形数:搜索的整数数值最大值最终搜索的数值大与等于int_value_min,并且小于等于int_value_max

- `type` (整形数)：搜索的整数类型,取值如下0: 32位1: 16位2: 8位3: 64位

- `step` (整形数)：搜索步长.

- `multi_thread` (整形数)：表示是否开启多线程查找. 0不开启，1开启.开启多线程查找速度较快，但会耗费较多CPU资源.不开启速度较慢，但节省CPU.

- `mode` (整形数)：1表示开启快速扫描(略过只读内存) 0表示所有内存类型全部扫描.

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindIntEx (hwnd,"00000000-FFFFFFFF", 300, 300, 0, 2, 1, 0) if len (result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound (result)+1 MessageBox"找到"&count&"个地址"

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_findString

**签名**：`const char* dm_findString(intptr_t hwnd, const char* addr_range, const char* string_value, long type)`

**简介**：搜索指定的字符串,默认步长是1.如果要定制步长，请用FindStringEx

**原型**：string FindString(hwnd, addr_range, string_value, type)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等.

- `string_value` (字符串)：搜索的字符串

- `type` (整形数)：搜索的字符串类型,取值如下0: Ascii字符串1: Unicode字符串2: UTF8字符串

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindString(hwnd,"00000000-FFFFFFFF","哈哈哈哈", 0) if len(result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound(result)+1 MessageBox"找到"&count&"个地址"

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_findStringEx

**签名**：`const char* dm_findStringEx(intptr_t hwnd, const char* addr_range, const char* string_value, long type, long step, long multi_thread, long mode)`

**简介**：搜索指定的字符串.

**原型**：string FindStringEx(hwnd, addr_range, string_value, type, step, multi_thread, mode)

**参数定义**

- `hwnd` (整形数)：指定搜索的窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr_range` (字符串)：指定搜索的地址集合，字符串类型，这个地方可以是上次FindXXX的返回地址集合,可以进行二次搜索.(类似CE的再次扫描)如果要进行地址范围搜索，那么这个值为的形如如下(类似于CE的新搜索)"00400000-7FFFFFFF""80000000-BFFFFFFF""00000000-FFFFFFFF"等.

- `string_value` (字符串)：搜索的字符串

- `type` (整形数)：搜索的字符串类型,取值如下0: Ascii字符串1: Unicode字符串2: UTF8字符串

- `step` (整形数)：搜索步长.

- `multi_thread` (整形数)：表示是否开启多线程查找. 0不开启，1开启.开启多线程查找速度较快，但会耗费较多CPU资源.不开启速度较慢，但节省CPU.

- `mode` (整形数)：1表示开启快速扫描(略过只读内存) 0表示所有内存类型全部扫描.

**返回值**：字符串:返回搜索到的地址集合，地址格式如下:"addr1|addr2|addr3…|addrn"比如"400050|423435|453430"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：//全局搜索result=dm.FindStringEx(hwnd,"00000000-FFFFFFFF","哈哈哈哈", 0, 2, 1, 1) if len(result)=0 then MessageBox"找不到"EndScript end if result=split(result,"|") count=ubound(result)+1 MessageBox"找到"&count&"个地址"

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_floatToData

**签名**：`const char* dm_floatToData(float v)`

**简介**：把单精度浮点数转换成二进制形式.

**原型**：string FloatToData (value)

**参数定义**

- `value` (单精度浮点数)：需要转化的单精度浮点数

**返回值**：字符串:字符串形式表达的二进制数据.可以用于WriteData FindData FindDataEx等接口.

**示例**：float_data=dm.FloatToData (1.24) dm_ret=dm.FindData(hwnd,"00000000-7fffffff", float_data)

----

### dm_freeProcessMemory

**签名**：`long dm_freeProcessMemory(intptr_t hwnd)`

**简介**：释放指定进程的不常用内存.

**原型**：long FreeProcessMemory(hwnd)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

**返回值**：整形数: 0:失败1:成功

**示例**：dm.FreeProcessMemory hwnd

**注**：此函数的原理并不是真正的释放进程内存，而是把进程中不常用的内存交换到虚拟内存中(硬盘里).这样可以空余出系统ram.此函数会加大系统内存和硬盘之间的数据交换频率，不能频繁调用。一般用法是程序长时间运行一段时间调用一次.

----

### dm_getCommandLine

**签名**：`const char* dm_getCommandLine(intptr_t hwnd)`

**简介**：获取指定窗口所在进程的启动命令行

**原型**：string GetCommandLine(hwnd)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

**返回值**：字符串:读取到的启动命令行

**示例**：command=dm.GetCommandLine(hwnd) MessageBox command

----

### dm_getModuleBaseAddr

**签名**：`long dm_getModuleBaseAddr(intptr_t hwnd, const char* module)`

**简介**：根据指定的窗口句柄，来获取对应窗口句柄进程下的指定模块的基址

**原型**：LONGLONG GetModuleBaseAddr(hwnd, module)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `module` (字符串)：模块名

**返回值**：长整形数:模块的基址

**示例**：base_addr=dm.GetModuleBaseAddr(hwnd,"gdi32.dll") MessageBox base_addr

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_getModuleSize

**签名**：`long dm_getModuleSize(intptr_t hwnd, const char* module)`

**简介**：根据指定的窗口句柄，来获取对应窗口句柄进程下的指定模块的大小

**原型**：long GetModuleSize(hwnd, module)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `module` (字符串)：模块名

**返回值**：整形数:模块的大小

**示例**：module_size=dm.GetModuleSize(hwnd,"gdi32.dll") MessageBox module_size

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_getRemoteApiAddress

**签名**：`long dm_getRemoteApiAddress(intptr_t hwnd, const char* module, const char* api)`

**简介**：根据指定的目标模块地址,获取目标窗口(进程)内的导出函数地址.

**原型**：LONGLONG GetRemoteApiAddress(hwnd, base_addr, fun_name)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . base_addr长整形数:目标模块地址,比如user32.dll的地址,可以通过GetModuleBaseAddr来获取.

- `fun_addr` (字符串)：需要获取的导出函数名.比如"SetWindowTextA".

**返回值**：长整形数:获取的地址.如果失败返回0

**示例**：//此例子用来在目标进程内执行SetWindowTextA来更改窗口标题. hwnd=dm.GetMousePointWindow() user32_base=dm.GetModuleBaseAddr(hwnd,"user32.dll") SetWindowTextA_addr=dm.GetRemoteApiAddress(hwnd, user32_base,"SetWindowTextA") addr=dm.VirtualAllocEx(hwnd, 0, 50, 0) dm.WriteStringAddr hwnd, addr, 0,"哈哈"//64位和32位的汇编代码不同if dm.GetWindowState(hwnd, 9)=0 then dm.AsmClear dm.AsmAdd"mov eax,"&hex(addr) dm.AsmAdd"push eax"dm.AsmAdd"mov eax,"&hex(hwnd) dm.AsmAdd"push eax"dm.AsmAdd"call"&hex(SetWindowTextA_addr) else dm.AsmClear dm.AsmAdd"mov rcx,"&dm.Hex64(hwnd) dm.AsmAdd"mov rdx,"&dm.Hex64(addr) dm.AsmAdd"mov rax,"&dm.Hex64(SetWindowTextA_addr) dm.AsmAdd"sub rsp, 28"dm.AsmAdd"call rax"dm.AsmAdd"add rsp, 28"end if dm.AsmCall hwnd, 1 dm.VirtualFreeEx hwnd, addr

----

### dm_int64ToInt32

**签名**：`const char* dm_int64ToInt32(long v_high, long v_low, long flag)`

**简介**：强制转换64位整数为32位. (这个函数是给按键精灵设计的,由于按键精灵不支持64位自动化变量,某些返回64位的整数的接口会出错)

**原型**：long Int64ToInt32(value)

**返回值**：整形数:返回的32位整数

**示例**：base_addr=dm.Int64ToInt32(dm.GetMoudleBaseAddr(hwnd,"ntdll.dll"))

----

### dm_intToData

**签名**：`const char* dm_intToData(long v, long type)`

**简介**：把整数转换成二进制形式.

**原型**：string IntToData (value, type)

**参数定义**

- `type` (整形数)：取值如下: 0: 4字节整形数(一般都选这个) 1: 2字节整形数2: 1字节整形数3: 8字节整形数

**返回值**：字符串:字符串形式表达的二进制数据.可以用于WriteData FindData FindDataEx等接口.

**示例**：int_data=dm.IntToData (&H12345678, 0) dm_ret=dm.FindData (hwnd,"00000000-7fffffff", int_data)

----

### dm_openProcess

**签名**：`long dm_openProcess(long pid)`

**简介**：根据指定pid打开进程，并返回进程句柄.

**原型**：long OpenProcess(pid)

**参数定义**

- `pid` (整形数)：进程pid

**返回值**：整形数:进程句柄,可用于进程相关操作(读写操作等),记得操作完成以后，自己调用CloseHandle关闭句柄.

**示例**：hwnd=dm.GetMousePointWindow() pid=dm.GetWindowProcessId(hwnd) handle=dm.OpenProcess(pid)……CloseHandle(handle)//这里自己定义导入函数

----

### dm_readData

**签名**：`const char* dm_readData(intptr_t hwnd, long addr, long len)`

**简介**：读取指定地址的二进制数据

**原型**：string ReadData(hwnd, addr, len)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `len` (整形数)：二进制数据的长度

**返回值**：字符串:读取到的数值,以16进制表示的字符串每个字节以空格相隔比如"12 34 56 78 ab cd ef"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadData(hwnd,"4DA678", 10) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。如果要读取的数据长度过长，比如几十K的数据，由于COM组件的限制，可能无法返回如此长的字符串.解决办法是分批读取.

----

### dm_readDataAddr

**签名**：`const char* dm_readDataAddr(intptr_t hwnd, long addr, long len)`

**简介**：读取指定地址的二进制数据

**原型**：string ReadDataAddr(hwnd, addr, len)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `len` (整形数)：二进制数据的长度

**返回值**：字符串:读取到的数值,以16进制表示的字符串每个字节以空格相隔比如"12 34 56 78 ab cd ef"如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadDataAddr(hwnd, 123456, 10) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。如果要读取的数据长度过长，比如几十K的数据，由于COM组件的限制，可能无法返回如此长的字符串.解决办法是分批读取.

----

### dm_readDataAddrToBin

**签名**：`long dm_readDataAddrToBin(intptr_t hwnd, long addr, long len)`

**简介**：读取指定地址的二进制数据,只不过返回的是内存地址,而不是字符串.适合高级用户.

**原型**：long ReadDataAddrToBin(hwnd, addr, len)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `len` (整形数)：二进制数据的长度

**返回值**：整形数:读取到的数据指针.返回0表示读取失败.如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadDataAddrToBin(hwnd, 12341234, 10) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。需要注意的是,调用此接口获取的数据指针保存在当前对象中,到下次调用此接口时,内部就会释放.哪怕是转成字节集,这个地址也还是在此字节集中使用.如果您要此地址一直有效，那么您需要自己拷贝字节集到自己的字节集中.

----

### dm_readDataToBin

**签名**：`long dm_readDataToBin(intptr_t hwnd, long addr, long len)`

**简介**：读取指定地址的二进制数据,只不过返回的是内存地址,而不是字符串.适合高级用户.

**原型**：long ReadDataToBin(hwnd, addr, len)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `len` (整形数)：二进制数据的长度

**返回值**：整形数:读取到的数据指针.返回0表示读取失败.如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadDataToBin(hwnd,"4DA678", 10) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。需要注意的是,调用此接口获取的数据指针保存在当前对象中,到下次调用此接口时,内部就会释放.哪怕是转成字节集,这个地址也还是在此字节集中使用.如果您要此地址一直有效，那么您需要自己拷贝字节集到自己的字节集中.

----

### dm_readDouble

**签名**：`long dm_readDouble(intptr_t hwnd, long addr, double* ret)`

**简介**：读取指定地址的双精度浮点数

**原型**：double ReadDouble(hwnd, addr)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

**返回值**：双精度浮点数:读取到的数值如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadDouble(hwnd,"4DA678") MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_readDoubleAddr

**签名**：`long dm_readDoubleAddr(intptr_t hwnd, long addr, double* ret)`

**简介**：读取指定地址的双精度浮点数

**原型**：double ReadDoubleAddr(hwnd, addr)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

**返回值**：双精度浮点数:读取到的数值如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadDoubleAddr(hwnd, 123456) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_readFloat

**签名**：`long dm_readFloat(intptr_t hwnd, long addr, float* ret)`

**简介**：读取指定地址的单精度浮点数

**原型**：float ReadFloat(hwnd, addr)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

**返回值**：单精度浮点数:读取到的数值如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadFloat(hwnd,"4DA678") MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_readFloatAddr

**签名**：`long dm_readFloatAddr(intptr_t hwnd, long addr, float* ret)`

**简介**：读取指定地址的单精度浮点数

**原型**：float ReadFloatAddr(hwnd, addr)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

**返回值**：单精度浮点数:读取到的数值如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadFloatAddr(hwnd, 123456) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_readInt

**签名**：`long dm_readInt(intptr_t hwnd, long addr, long* ret)`

**简介**：读取指定地址的整数数值，类型可以是8位，16位32位或者64位

**原型**：LONGLONG ReadInt(hwnd, addr, type)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `type` (整形数)：整数类型,取值如下0: 32位有符号1: 16位有符号2: 8位有符号3: 64位4: 32位无符号5: 16位无符号6: 8位无符号

**返回值**：长整形数:读取到的数值如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadInt(hwnd,"4DA678", 0) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_readIntAddr

**签名**：`long dm_readIntAddr(intptr_t hwnd, long addr, long type, long* ret)`

**简介**：读取指定地址的整数数值，类型可以是8位，16位32位或者64位

**原型**：LONGLONG ReadIntAddr(hwnd, addr, type)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `type` (整形数)：整数类型,取值如下0: 32位1: 16位2: 8位3: 64位4: 32位无符号5: 16位无符号6: 8位无符号

**返回值**：长整形数:读取到的数值如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadIntAddr(hwnd, 123456, 0) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_readString

**签名**：`const char* dm_readString(intptr_t hwnd, long addr, long type, long len)`

**简介**：读取指定地址的字符串，可以是GBK字符串或者是Unicode字符串.(必须事先知道内存区的字符串编码方式)

**原型**：string ReadString(hwnd, addr, type, len)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `type` (整形数)：字符串类型,取值如下0: GBK字符串1: Unicode字符串2: UTF8字符串

- `len` (整形数)：需要读取的字节数目.如果为0，则自动判定字符串长度.

**返回值**：字符串:读取到的字符串如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadString(hwnd,"4DA678", 0, 0) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_readStringAddr

**签名**：`const char* dm_readStringAddr(intptr_t hwnd, long addr, long type, long len)`

**简介**：读取指定地址的字符串，可以是GBK字符串或者是Unicode字符串.(必须事先知道内存区的字符串编码方式)

**原型**：string ReadStringAddr(hwnd, addr, type, len)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `type` (整形数)：字符串类型,取值如下0: GBK字符串1: Unicode字符串2: UTF8字符串

- `len` (整形数)：需要读取的字节数目.如果为0，则自动判定字符串长度.

**返回值**：字符串:读取到的字符串如果要想知道函数是否执行成功，请查看GetLastError函数.

**示例**：value=dm.ReadStringAddr(hwnd, 123456, 0, 0) MessageBox value

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_setMemoryFindResultToFile

**签名**：`long dm_setMemoryFindResultToFile(long enable)`

**简介**：设置是否把所有内存查找接口的结果保存入指定文件.

**原型**：long SetMemoryFindResultToFile (file)

**参数定义**

- `file` (字符串)：设置要保存的搜索结果文件名.如果为空字符串表示取消此功能.

**返回值**：整形数: 0:失败1:成功

**示例**：//开启dm.SetPath"d:\test"dm.SetMemoryFindResultToFile"result.dat"//取消dm.SetMemoryFindResultToFile""

**注**：部分高级语言无法接纳FindXXX接口返回的超长字符串，那么需要用这个函数转存入文件,然后再读取分析处理.同时，设置了此文件后，那么当下次调用FindXXX接口传入的地址参数时，并且地址参数不是范围参数,那么地址参数会从设置的文件中读取.如果是范围参数,那么插件不会从设置的文件读取,会认为是首次查找.因为部分高级语言对参数的接收也有长度限制，无法接收超长字符串.

----

### dm_setMemoryHwndAsProcessId

**签名**：`long dm_setMemoryHwndAsProcessId(long enable)`

**简介**：设置是否把所有内存接口函数中的窗口句柄当作进程ID,以支持直接以进程ID来使用内存接口.

**原型**：long SetMemoryHwndAsProcessId (en)

**参数定义**

- `en` (整形数)：取值如下0:关闭1:开启

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetMemoryHwndAsProcessId 1

**注**：默认是当作窗口句柄.

----

### dm_setParam64ToPointer

**签名**：`long dm_setParam64ToPointer(long enable)`

**简介**：这个接口是给E语言设计的.因为E语言的BUG,导致无法对COM对象调用传入长整数参数(被强制截断成整数),特别设计此接口来兼容长整数的处理.

**原型**：long SetParam64ToPointer()

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetParam64ToPointer()

**注**：调用此接口后,所有的调用接口中的长整数,一律当作地址来处理.此接口不需要您手动调用,只要用最新的类库生成工具生成,代码里已经自动处理好了.

----

### dm_stringToData

**签名**：`const char* dm_stringToData(const char* v, long type)`

**简介**：把字符串转换成二进制形式.

**原型**：string StringToData(value, type)

**参数定义**

- `value` (字符串)：需要转化的字符串

- `type` (整形数)：取值如下: 0:返回Ascii表达的字符串1:返回Unicode表达的字符串

**返回值**：字符串:字符串形式表达的二进制数据.可以用于WriteData FindData FindDataEx等接口.

**示例**：string_data=dm.StringToData("12345678", 1) dm_ret=dm.FindData(hwnd,"00000000-7fffffff", string_data)

----

### dm_terminateProcess

**签名**：`long dm_terminateProcess(long pid)`

**简介**：根据指定的PID，强制结束进程.

**原型**：long TerminateProcess (pid)

**参数定义**

- `pid` (整形数)：进程ID.

**返回值**：整形数: 0:失败1:成功

**示例**：hwnd=dm.GetMousePointWindow () pid=dm.GetWindowProcessId (hwnd) dm.TerminateProcess pid

**注**：另外DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_terminateProcessTree

**签名**：`long dm_terminateProcessTree(long pid)`

**简介**：根据指定的PID，强制结束进程以及此进程创建的所有子进程.

**原型**：long TerminateProcessTree(pid)

**参数定义**

- `pid` (整形数)：进程ID.

**返回值**：整形数: 0:失败1:成功

**示例**：hwnd=dm.GetMousePointWindow() pid=dm.GetWindowProcessId(hwnd) dm.TerminateProcessTree pid

**注**：另外DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_virtualAllocEx

**签名**：`long dm_virtualAllocEx(intptr_t hwnd, long size, long type, long protect)`

**简介**：在指定的窗口所在进程分配一段内存.

**原型**：LONGLONG VirtualAllocEx(hwnd, addr, size, type)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:预期的分配地址。如果是0表示自动分配，否则就尝试在此地址上分配内存.

- `size` (整形数)：需要分配的内存大小.

- `type` (整形数)：需要分配的内存类型，取值如下: 0:可读可写可执行1:可读可执行，不可写2:可读可写,不可执行

**返回值**：长整形数:分配的内存地址，如果是0表示分配失败.

**示例**：addr=dm.VirtualAllocEx(hwnd, 0, 50, 0) dm.WriteString hwnd, cstr(hex(addr)), 0,"哈哈"dm.VirtualFreeEx hwnd, addr

**注**：如果正常方式无法分配内存,可以尝试配合DmGuard中的memory护盾,突破部分窗口内存保护。用此函数分配的内存，必须用VirtualFreeEx来释放,以免目标进程内存泄漏.

----

### dm_virtualFreeEx

**签名**：`long dm_virtualFreeEx(intptr_t hwnd, long addr)`

**简介**：释放用VirtualAllocEx分配的内存.

**原型**：long VirtualFreeEx(hwnd, addr)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数: VirtualAllocEx返回的地址

**返回值**：整形数: 0:失败1:成功

**示例**：addr=dm.VirtualAllocEx(hwnd, 0, 50, 0) dm.WriteString hwnd, cstr(hex(addr)), 0,"哈哈"dm.VirtualFreeEx hwnd, addr

**注**：如果正常方式无法分配内存,可以尝试配合DmGuard中的memory护盾,突破部分窗口内存保护。用此函数分配的内存，必须用VirtualFreeEx来释放,以免目标进程内存泄漏.

----

### dm_virtualProtectEx

**签名**：`long dm_virtualProtectEx(intptr_t hwnd, long addr, long size, long type, long* old_protect)`

**简介**：修改指定的窗口所在进程的地址的读写属性,修改为可读可写可执行.

**原型**：long VirtualProtectEx(hwnd, addr, size, type, old_protect)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:需要修改的地址

- `size` (整形数)：需要修改的地址大小.

- `type` (整形数)：修改的地址读写属性类型，取值如下: 0:可读可写可执行,此时old_protect参数无效1:修改为old_protect指定的读写属性

- `old_protect` (整形数)：指定的读写属性

**返回值**：整形数: 0:失败1:修改之前的读写属性

**示例**：//修改地址&H400000为可读可写可执行,并把修改之前的读写属性保存old_protect=dm.VirtualProtectEx(hwnd,&H400000, 5, 0, 0) if old_protect<>0 then dm.AsmClear dm.AsmAdd"lea eax,[400000]"dm.AsmAdd"mov dword ptr[eax], 0"dm.AsmCall hwnd, 1 dm.VirtualProtectEx hwnd,&H400000, 5, 1, old_protect end if

**注**：如果正常方式无法修改内存的读写属性,可以尝试配合DmGuard中的memory护盾,突破部分窗口内存保护。

----

### dm_virtualQueryEx

**签名**：`const char* dm_virtualQueryEx(intptr_t hwnd, long addr, long pmbi)`

**简介**：获取指定窗口，指定地址的内存属性.

**原型**：string VirtualQueryEx(hwnd, addr, pmbi)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:需要查询的地址

- `pmbi` (整形数)：这是一个地址,指向的内容是MEMORY_BASIC_INFORMATION32或者MEMORY_BASIC_INFORMATION64 .取决于要查询的进程是32位还是64位.这个地址可以为0,忽略这个参数.下面是这2个结构体在vc下的定义: typedef struct _MEMORY_BASIC_INFORMATION32{DWORD BaseAddress;DWORD AllocationBase;DWORD AllocationProtect;DWORD RegionSize;DWORD State;DWORD Protect;DWORD Type;}MEMORY_BASIC_INFORMATION32,*PMEMORY_BASIC_INFORMATION32;typedef struct DECLSPEC_ALIGN (16) _MEMORY_BASIC_INFORMATION64{ULONGLONG BaseAddress;ULONGLONG AllocationBase;DWORD AllocationProtect;DWORD __alignment1;ULONGLONG RegionSize;DWORD State;DWORD Protect;DWORD Type;DWORD __alignment2;}MEMORY_BASIC_INFORMATION64,*PMEMORY_BASIC_INFORMATION64;

**返回值**：字符串:查询的结果以字符串形式.内容是"BaseAddress, AllocationBase, AllocationProtect, RegionSize, State, Protect, Type"数值都是10进制表达.

**示例**：这里我们给一个VC的例子.其它语言都差不多. MEMORY_BASIC_INFORMATION32 mbi={0};dm->VirtualQueryEx(hwnd, 0x400000,&mbi);if (mbi. BaseAddress){//做一些你需要的操作}

**注**：如果正常方式无法修改内存的读写属性,可以尝试配合DmGuard中的memory护盾,突破部分窗口内存保护。

----

### dm_writeData

**签名**：`long dm_writeData(intptr_t hwnd, long addr, const char* data)`

**简介**：对指定地址写入二进制数据

**原型**：long WriteData(hwnd, addr, data)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `data` (字符串)：二进制数据，以字符串形式描述，比如"12 34 56 78 90 ab cd"

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteData(hwnd,"4DA678","12 34 56 78 90 ab cd")

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeDataAddr

**签名**：`long dm_writeDataAddr(intptr_t hwnd, long addr, const char* data)`

**简介**：对指定地址写入二进制数据

**原型**：long WriteDataAddr(hwnd, addr, data)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `data` (字符串)：二进制数据，以字符串形式描述，比如"12 34 56 78 90 ab cd"

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteDataAddr(hwnd, 123456,"12 34 56 78 90 ab cd")

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeDataAddrFromBin

**签名**：`long dm_writeDataAddrFromBin(intptr_t hwnd, long addr, long data, long len)`

**简介**：对指定地址写入二进制数据,只不过直接从数据指针获取数据写入,不通过字符串.适合高级用户.

**原型**：long WriteDataAddrFromBin(hwnd, addr, data, len)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `data` (整形数)：数据指针

- `len` (整形数)：数据长度

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteDataAddrFromBin(hwnd, 2934793257239, 1231234, 10)

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeDataFromBin

**签名**：`long dm_writeDataFromBin(intptr_t hwnd, long addr, long data, long len)`

**简介**：对指定地址写入二进制数据,只不过直接从数据指针获取数据写入,不通过字符串.适合高级用户.

**原型**：long WriteDataFromBin(hwnd, addr, data, len)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `data` (整形数)：数据指针

- `len` (整形数)：数据长度

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteDataFromBin(hwnd,"4DA678", 1231234, 10)

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeDouble

**签名**：`long dm_writeDouble(intptr_t hwnd, long addr, double v)`

**简介**：对指定地址写入双精度浮点数

**原型**：long WriteDouble(hwnd, addr, v)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `v` (双精度浮点数)：双精度浮点数

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteDouble(hwnd,"4DA678", 2.34)

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeDoubleAddr

**签名**：`long dm_writeDoubleAddr(intptr_t hwnd, long addr, double v)`

**简介**：对指定地址写入双精度浮点数

**原型**：long WriteDoubleAddr(hwnd, addr, v)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `v` (双精度浮点数)：双精度浮点数

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteDoubleAddr(hwnd, 123456, 2.34)

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeFloat

**签名**：`long dm_writeFloat(intptr_t hwnd, long addr, float v)`

**简介**：对指定地址写入单精度浮点数

**原型**：long WriteFloat(hwnd, addr, v)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `v` (单精度浮点数)：单精度浮点数

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteFloat(hwnd,"4DA678", 2.34)

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeFloatAddr

**签名**：`long dm_writeFloatAddr(intptr_t hwnd, long addr, float v)`

**简介**：对指定地址写入单精度浮点数

**原型**：long WriteFloatAddr(hwnd, addr, v)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `v` (单精度浮点数)：单精度浮点数

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteFloatAddr(hwnd, 123456, 2.34)

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeInt

**签名**：`long dm_writeInt(intptr_t hwnd, long addr, long type, long v)`

**简介**：对指定地址写入整数数值，类型可以是8位，16位32位或者64位

**原型**：long WriteInt(hwnd, addr, type, v)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `type` (整形数)：整数类型,取值如下0: 32位1: 16位2: 8位3: 64位v长整形数:整形数值

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteInt(hwnd,"4DA678", 0, 100)

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeIntAddr

**签名**：`long dm_writeIntAddr(intptr_t hwnd, long addr, long type, long v)`

**简介**：对指定地址写入整数数值，类型可以是8位，16位32位或者64位

**原型**：long WriteIntAddr(hwnd, addr, type, v)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `type` (整形数)：整数类型,取值如下0: 32位1: 16位2: 8位3: 64位v长整形数:整形数值

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteIntAddr(hwnd, 123456, 0, 100)

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeString

**签名**：`long dm_writeString(intptr_t hwnd, long addr, long type, const char* v)`

**简介**：对指定地址写入字符串，可以是Ascii字符串或者是Unicode字符串

**原型**：long WriteString(hwnd, addr, type, v)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId .

- `addr` (字符串)：用字符串来描述地址，类似于CE的地址描述，数值必须是16进制,里面可以用[]+-这些符号来描述一个地址。+表示地址加，-表示地址减模块名必须用<>符号来圈起来例如: 1."4DA678"最简单的方式，用绝对数值来表示地址2."<360SE.exe>+DA678"相对简单的方式，只是这里用模块名来决定模块基址，后面的是偏移3."[4DA678]+3A"用绝对数值加偏移，相当于一级指针4."[<360SE.exe>+DA678]+3A"用模块定基址的方式，也是一级指针5."[[[<360SE.exe>+DA678]+3A]+5B]+8"这个是一个三级指针总之熟悉CE的人应该对这个地址描述都很熟悉,我就不多举例了

- `type` (整形数)：字符串类型,取值如下0: Ascii字符串1: Unicode字符串2: UTF8字符串

- `v` (字符串)：字符串

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteString(hwnd,"4DA678", 0,"我是来测试的")

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_writeStringAddr

**签名**：`long dm_writeStringAddr(intptr_t hwnd, long addr, long type, const char* v)`

**简介**：对指定地址写入字符串，可以是Ascii字符串或者是Unicode字符串

**原型**：long WriteStringAddr(hwnd, addr, type, v)

**参数定义**

- `hwnd` (整形数)：窗口句柄或者进程ID.默认是窗口句柄.如果要指定为进程ID,需要调用SetMemoryHwndAsProcessId . addr长整形数:地址

- `type` (整形数)：字符串类型,取值如下0: Ascii字符串1: Unicode字符串2: UTF8字符串

- `v` (字符串)：字符串

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.WriteStringAddr(hwnd, 123456, 0,"我是来测试的")

**注**：DmGuard中的memory护盾也可以突破部分窗口内存保护，可以尝试使用。

----

### dm_copyFile

**签名**：`long dm_copyFile(const char* src, const char* dest, long over)`

**简介**：拷贝文件.

**原型**：long CopyFile (src_file, dst_file, over)

**参数定义**

- `src_file` (字符串)：原始文件名

- `dst_file` (字符串)：目标文件名.

- `over` (整形数)：取值如下, 0:如果dst_file文件存在则不覆盖返回. 1:如果dst_file文件存在则覆盖.

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.CopyFile"c:\123.txt","d:\456.txt", 1//相对路径dm.SetPath"c:\test_game"dm.CopyFile"123.txt","456.txt", 1

----

### dm_createFolder

**签名**：`long dm_createFolder(const char* folder)`

**简介**：创建指定目录.

**原型**：long CreateFolder (folder)

**参数定义**

- `folder` (字符串)：目录名

**返回值**：整形数: 0:失败1:成功

**示例**：dm.CreateFolder"c:\123\456\789"

----

### dm_decodeFile

**签名**：`long dm_decodeFile(const char* file, const char* pwd)`

**简介**：解密指定的文件.

**原型**：long DecodeFile(file, pwd)

**参数定义**

- `file` (字符串)：文件名.

- `pwd` (字符串)：密码.

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.DecodeFile"c:\test_game\cfg.ini","1234"//相对路径dm.SetPath"c:\test_game"dm.DecodeFile"1.bmp","1234"如果此文件没加密，调用此函数不会有任何效果.插件所有的字库图片ini都是用此接口来解密.

----

### dm_deleteFile

**签名**：`long dm_deleteFile(const char* file)`

**简介**：删除文件.

**原型**：long DeleteFile (file)

**参数定义**

- `file` (字符串)：文件名

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.DeleteFile"c:\123.txt"//相对路径dm.SetPath"c:\test_game"dm.DeleteFile"123.txt"

----

### dm_deleteFolder

**签名**：`long dm_deleteFolder(const char* folder)`

**简介**：删除指定目录.

**原型**：long DeleteFolder (folder)

**参数定义**

- `folder` (字符串)：目录名

**返回值**：整形数: 0:失败1:成功

**示例**：dm.DeleteFolder"c:\123"

----

### dm_deleteIni

**签名**：`long dm_deleteIni(const char* section, const char* key, const char* file)`

**简介**：删除指定的ini小节.

**原型**：long DeleteIni (section, key, file)

**参数定义**

- `section` (字符串)：小节名

- `key` (字符串)：变量名.如果这个变量为空串，则删除整个section小节.

- `file` (字符串)：ini文件名.

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.DeleteIni"Global","var1","c:\test_game\cfg.ini"//相对路径dm.SetPath"c:\test_game"dm.DeleteIni"Global","","cfg.ini"

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.

----

### dm_deleteIniPwd

**签名**：`long dm_deleteIniPwd(const char* section, const char* key, const char* file, const char* pwd)`

**简介**：删除指定的ini小节.支持加密文件

**原型**：long DeleteIniPwd (section, key, file, pwd)

**参数定义**

- `section` (字符串)：小节名

- `key` (字符串)：变量名.如果这个变量为空串，则删除整个section小节.

- `file` (字符串)：ini文件名.

- `pwd` (字符串)：密码.

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.DeleteIniPwd"Global","var1","c:\test_game\cfg.ini","1234"//相对路径dm.SetPath"c:\test_game"dm.DeleteIniPwd"Global","","cfg.ini","1234"

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.但是多进程是不安全的,要避免多进程同时使用此接口,否则会造成数据错乱.如果此文件没加密，调用此函数会自动加密.

----

### dm_downloadFile

**签名**：`long dm_downloadFile(const char* url, const char* save_file, long timeout)`

**简介**：从internet上下载一个文件.

**原型**：long DownloadFile (url, save_file, timeout)

**参数定义**

- `url` (字符串)：下载的url地址.

- `save_file` (字符串)：要保存的文件名.

- `timeout` (整形数)：连接超时时间，单位是毫秒.

**返回值**：整形数: 1:成功-1:网络连接失败-2:写入文件失败

**示例**：dm.DownloadFile"www.sohu.com","sohu.html", 3000 dm.DownloadFile"http://www.sohu.com","d:\sohu.html", 3000

----

### dm_encodeFile

**签名**：`long dm_encodeFile(const char* file, const char* pwd)`

**简介**：加密指定的文件.

**原型**：long EncodeFile(file, pwd)

**参数定义**

- `file` (字符串)：文件名.

- `pwd` (字符串)：密码.

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.EncodeFile"c:\test_game\cfg.ini","1234"//相对路径dm.SetPath"c:\test_game"dm.EncodeFile"1.bmp","1234"如果此文件已经加密，调用此函数不会有任何效果.插件所有的字库图片ini都是用此接口来加密.

----

### dm_enumIniKey

**签名**：`const char* dm_enumIniKey(const char* section, const char* file)`

**简介**：根据指定的ini文件以及section,枚举此section中所有的key名

**原型**：string EnumIniKey(section, file)

**参数定义**

- `section` (字符串)：小节名. (不可为空)

- `file` (字符串)：ini文件名.

**返回值**：字符串:每个key用"|"来连接，如果没有key，则返回空字符串.比如"aaa|bbb|ccc"

**示例**：//绝对路径dm_ret=dm.EnumIniKey("aaa","c:\test_game\cfg.ini")//相对路径dm.SetPath"c:\test_game"dm_ret=dm.EnumIniKey("aaa","cfg.ini") if len(dm_ret)>0 then keys=split(dm_ret,"|") count=ubound(keys)+1 index=0 Do While index<count TracePrint keys(index) index=index+1 Loop end if

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.另外,此函数无法枚举没有section的key.

----

### dm_enumIniKeyPwd

**签名**：`const char* dm_enumIniKeyPwd(const char* section, const char* file, const char* pwd)`

**简介**：根据指定的ini文件以及section,枚举此section中所有的key名.可支持加密文件

**原型**：string EnumIniKeyPwd(section, file, pwd)

**参数定义**

- `section` (字符串)：小节名. (不可为空)

- `file` (字符串)：ini文件名.

- `pwd` (字符串)：密码

**返回值**：字符串:每个key用"|"来连接，如果没有key，则返回空字符串.比如"aaa|bbb|ccc"

**示例**：//绝对路径dm_ret=dm.EnumIniKeyPwd ("aaa","c:\test_game\cfg.ini","123")//相对路径dm.SetPath"c:\test_game"dm_ret=dm.EnumIniKeyPwd ("aaa","cfg.ini","123") if len (dm_ret)>0 then keys=split(dm_ret,"|") count=ubound (keys)+1 index=0 Do While index<count TracePrint keys(index) index=index+1 Loop end if

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.但是多进程是不安全的,要避免多进程同时使用此接口,否则会造成数据错乱.另外,此函数无法枚举没有section的key.如果文件没加密，也可以正常读取.

----

### dm_enumIniSection

**签名**：`const char* dm_enumIniSection(const char* file)`

**简介**：根据指定的ini文件,枚举此ini中所有的Section(小节名)

**原型**：string EnumIniSection(file)

**参数定义**

- `file` (字符串)：ini文件名.

**返回值**：字符串:每个小节名用"|"来连接，如果没有小节，则返回空字符串.比如"aaa|bbb|ccc"

**示例**：//绝对路径dm_ret=dm.EnumIniSection("c:\test_game\cfg.ini")//相对路径dm.SetPath"c:\test_game"dm_ret=dm.EnumIniSection("cfg.ini") if len(dm_ret)>0 then sections=split(dm_ret,"|") count=ubound(sections)+1 index=0 Do While index<count TracePrint sections(index) index=index+1 Loop end if

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.

----

### dm_enumIniSectionPwd

**签名**：`const char* dm_enumIniSectionPwd(const char* file, const char* pwd)`

**简介**：根据指定的ini文件,枚举此ini中所有的Section(小节名)可支持加密文件

**原型**：string EnumIniSectionPwd (file, pwd)

**参数定义**

- `file` (字符串)：ini文件名.

- `pwd` (字符串)：密码

**返回值**：字符串:每个小节名用"|"来连接，如果没有小节，则返回空字符串.比如"aaa|bbb|ccc"

**示例**：//绝对路径dm_ret=dm.EnumIniSectionPwd ("c:\test_game\cfg.ini","123")//相对路径dm.SetPath"c:\test_game"dm_ret=dm.EnumIniSectionPwd ("cfg.ini","123") if len (dm_ret)>0 then sections=split(dm_ret,"|") count=ubound (sections)+1 index=0 Do While index<count TracePrint sections(index) index=index+1 Loop end if

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.但是多进程是不安全的,要避免多进程同时使用此接口,否则会造成数据错乱.如果文件没加密，也可以正常读取.

----

### dm_getFileLength

**签名**：`long dm_getFileLength(const char* file)`

**简介**：获取指定的文件长度.

**原型**：long GetFileLength (file)

**参数定义**

- `file` (字符串)：文件名

**返回值**：整形数:文件长度(字节数)

**示例**：//绝对路径TracePrint dm.GetFileLength ("c:\123.txt")//相对路径dm.SetPath"c:\test_game"TracePrint dm.GetFileLength ("123.txt")

----

### dm_getRealPath

**签名**：`const char* dm_getRealPath(const char* path)`

**简介**：获取指定文件或目录的真实路径

**原型**：string GetRealPath (path)

**参数定义**

- `path` (字符串)：路径名,可以是文件路径，也可以是目录.这里必须是全路径

**返回值**：字符串:真实路径,如果失败,返回空字符串

**示例**：TracePrint dm.GetRealPath ("c:\11\22\3.txt")

**注**：这个功能可以获取到路径中有符号链接之后的，真实路径.

----

### dm_isFileExist

**签名**：`long dm_isFileExist(const char* file)`

**简介**：判断指定文件是否存在.

**原型**：long IsFileExist (file)

**参数定义**

- `file` (字符串)：文件名

**返回值**：整形数: 0:不存在1:存在

**示例**：//绝对路径TracePrint dm.IsFileExist ("c:\123.txt")//相对路径dm.SetPath"c:\test_game"TracePrint dm.IsFileExist("123.txt")

----

### dm_isFolderExist

**签名**：`long dm_isFolderExist(const char* folder)`

**简介**：判断指定目录是否存在.

**原型**：long IsFolderExist (folder)

**参数定义**

- `folder` (字符串)：目录名

**返回值**：整形数: 0:不存在1:存在

**示例**：TracePrint dm.IsFolderExist("c:\test_game")

----

### dm_moveFile

**签名**：`long dm_moveFile(const char* src, const char* dest)`

**简介**：移动文件.

**原型**：long MoveFile (src_file, dst_file)

**参数定义**

- `src_file` (字符串)：原始文件名

- `dst_file` (字符串)：目标文件名.

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.MoveFile"c:\123.txt","d:\456.txt"//相对路径dm.SetPath"c:\test_game"dm.MoveFile"123.txt","456.txt"

----

### dm_readFile

**签名**：`const char* dm_readFile(const char* file)`

**简介**：从指定的文件读取内容.

**原型**：string ReadFile (file)

**参数定义**

- `file` (字符串)：文件

**返回值**：字符串:读入的文件内容

**示例**：//绝对路径TracePrint dm.ReadFile ("c:\123.txt")//相对路径dm.SetPath"c:\test_game"TracePrint dm.ReadFile("123.txt")

----

### dm_readIni

**签名**：`const char* dm_readIni(const char* section, const char* key, const char* file)`

**简介**：从Ini中读取指定信息.

**原型**：string ReadIni(section, key, file)

**参数定义**

- `section` (字符串)：小节名

- `key` (字符串)：变量名.

- `file` (字符串)：ini文件名.

**返回值**：字符串:字符串形式表达的读取到的内容

**示例**：//绝对路径Text=dm.ReadIni("Global","var1","c:\test_game\cfg.ini")//相对路径dm.SetPath"c:\test_game"Text=dm.ReadIni("Global","var1","cfg.ini")

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.

----

### dm_readIniPwd

**签名**：`const char* dm_readIniPwd(const char* section, const char* key, const char* file, const char* pwd)`

**简介**：从Ini中读取指定信息.可支持加密文件

**原型**：string ReadIniPwd (section, key, file, pwd)

**参数定义**

- `section` (字符串)：小节名

- `key` (字符串)：变量名.

- `file` (字符串)：ini文件名.

- `pwd` (字符串)：密码

**返回值**：字符串:字符串形式表达的读取到的内容

**示例**：//绝对路径Text=dm.ReadIniPwd ("Global","var1","c:\test_game\cfg.ini","1234")//相对路径dm.SetPath"c:\test_game"Text=dm.ReadIniPwd ("Global","var1","cfg.ini","1234")

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.但是多进程是不安全的,要避免多进程同时使用此接口,否则会造成数据错乱.如果文件没加密，也可以正常读取.

----

### dm_selectDirectory

**签名**：`const char* dm_selectDirectory()`

**简介**：弹出选择文件夹对话框，并返回选择的文件夹.

**原型**：string SelectDirectory()

**返回值**：字符串:选择的文件夹全路径

**示例**：TracePrint dm.SelectDirectory()

**注**：此接口要求当前线程的COM模型必须是STA.如果当前对象创建于非STA的线程,那么调用此接口有可能会导致程序崩溃.具体的示例,可以查看类库生成工具里产生的例子.

----

### dm_selectFile

**签名**：`const char* dm_selectFile()`

**简介**：弹出选择文件对话框，并返回选择的文件.

**原型**：string SelectFile()

**返回值**：字符串:选择的文件全路径

**示例**：TracePrint dm.SelectFile()

**注**：此接口要求当前线程的COM模型必须是STA.如果当前对象创建于非STA的线程,那么调用此接口有可能会导致程序崩溃.具体的示例,可以查看类库生成工具里产生的例子.

----

### dm_writeFile

**签名**：`long dm_writeFile(const char* file, const char* content)`

**简介**：向指定文件追加字符串.

**原型**：long WriteFile(file, content)

**参数定义**

- `file` (字符串)：文件

- `content` (字符串)：写入的字符串.

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.WriteFile"c:\123.txt","哈哈哈"//相对路径dm.SetPath"c:\test_game"dm.WriteFile"123.txt","哈哈哈"

----

### dm_writeIni

**签名**：`long dm_writeIni(const char* section, const char* key, const char* value, const char* file)`

**简介**：向指定的Ini写入信息.

**原型**：long WriteIni(section, key, value, file)

**参数定义**

- `section` (字符串)：小节名

- `key` (字符串)：变量名.

- `value` (字符串)：变量内容

- `file` (字符串)：ini文件名.

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.WriteIni"Global","var1","123","c:\test_game\cfg.ini"//相对路径dm.SetPath"c:\test_game"dm.WriteIni"Global","var1","123","cfg.ini"

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.

----

### dm_writeIniPwd

**签名**：`long dm_writeIniPwd(const char* section, const char* key, const char* value, const char* file, const char* pwd)`

**简介**：向指定的Ini写入信息.支持加密文件

**原型**：long WriteIniPwd (section, key, value, file, pwd)

**参数定义**

- `section` (字符串)：小节名

- `key` (字符串)：变量名.

- `value` (字符串)：变量内容

- `file` (字符串)：ini文件名.

- `pwd` (字符串)：密码.

**返回值**：整形数: 0:失败1:成功

**示例**：//绝对路径dm.WriteIniPwd"Global","var1","123","c:\test_game\cfg.ini","1234"//相对路径dm.SetPath"c:\test_game"dm.WriteIniPwd"Global","var1","123","cfg.ini","1234"

**注**：此函数是多线程安全的.多线程同时读写同个文件不会造成文件错乱.但是多进程是不安全的,要避免多进程同时使用此接口,否则会造成数据错乱.如果此文件没加密，调用此函数会自动加密.

----

### dm_addDict

**签名**：`long dm_addDict(const char* dict_info)`

**简介**：给指定的字库中添加一条字库信息.

**原型**：long AddDict (index, dict_info)

**参数定义**

- `index` (整形数)：字库的序号,取值为0-99,目前最多支持100个字库

- `dict_info` (字符串)：字库描述串，具体参考大漠综合工具中的字符定义

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.AddDict (0,"081101BF8020089FD 10A 21443F 85038$记$0.0$11")注意:此函数尽量在小字库中使用，大字库中使用AddDict速度比较慢另，此函数是向指定的字库所在的内存中添加,而不是往文件中添加.添加以后立刻就可以用于文字识别。无须再SetDict如果要保存添加进去的字库信息，需要调用SaveDict

----

### dm_clearDict

**签名**：`long dm_clearDict(long index)`

**简介**：清空指定的字库.

**原型**：long ClearDict (index)

**参数定义**

- `index` (整形数)：字库的序号,取值为0-99,目前最多支持100个字库

**返回值**：整形数: 0:失败1:成功

**示例**：dm.ClearDict 0注意:此函数尽量在小字库中使用，大字库中使用AddDict速度比较慢另外，此函数支持清空内存中的字库，而不是字库文件本身.

----

### dm_enableShareDict

**签名**：`long dm_enableShareDict(long enable)`

**简介**：允许当前调用的对象使用全局字库。如果你的程序中对象太多,并且每个对象都用到了同样的字库,可以考虑用全局字库,这样可以节省大量内存.

**原型**：long EnableShareDict (enable)

**参数定义**

- `enable` (整形数)：0关闭1打开

**返回值**：整形数: 0:失败1:成功

**示例**：dm.EnableShareDict 1 dm.SetDict 0,"xxx.txt"

**注**：一旦当前对象开启了全局字库,那么所有的和文字识别，字库相关的接口，通通都认为是对全局字库的操作.如果所有对象都要需要全局字库,可以选一个主对象开启使用全局字库，并且设置好字库，其他对象只需要开启全局字库即可.注意,第一个开启全局字库，并且设置字库的主对象不可以被释放,并且此主对象在修改字库(SetDict AddDict ClearDict SetDictMem)时,其它任何对象都不可以对全局字库进行操作.也就是说,必须是设置好全局字库后,其它对象才可以开启全局字库.同时，设置好全局字库后,任何对象都不可以修改字库.只能使用访问字库这样的接口(UseDict等).

----

### dm_fetchWord

**签名**：`long dm_fetchWord(long x1, long y1, long x2, long y2, const char* color, const char* words)`

**简介**：根据指定的范围,以及指定的颜色描述，提取点阵信息，类似于大漠工具里的单独提取.

**原型**：string FetchWord (x1, y1, x2, y2, color, word)

**参数定义**

- `x1` (整形数)：左上角X坐标

- `y1` (整形数)：左上角Y坐标

- `x2` (整形数)：右下角X坐标

- `y2` (整形数)：右下角Y坐标

- `color` (字符串)：颜色格式串.注意，RGB和HSV,以及灰度格式都支持.

- `word` (字符串)：待定义的文字,不能为空，且不能为关键符号"$"

**返回值**：字符串:识别到的点阵信息，可用于AddDict如果失败，返回空

**示例**：info=dm.FetchWord (200, 200, 250, 220,"abcdef-101010|ffffff-101010","张三") If len (info)>0 Then dm.AddDict 3, info End if info=dm.FetchWord (200, 200, 250, 220,"b@abcdef-101010|ffffff-101010","李四") If len (info)>0 Then dm.AddDict 2, info End if info=dm.FetchWord (200, 200, 250, 220,"b@0.100.100- 0.0.0","张三") If len (info)>0 Then dm.AddDict 4, info End if info=dm.FetchWord (200, 200, 250, 220,"0.100.100- 0.0.0|100.0.0-0.0.0","王") If len (info)>0 Then dm.AddDict 4, info End if

----

### dm_findStr

**签名**：`long dm_findStr(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY)`

**简介**：在屏幕范围(x1, y1, x2, y2)内,查找string(可以是任意个字符串的组合),并返回符合color_format的坐标位置,相似度sim同Ocr接口描述. (多色,差色查找类似于Ocr接口,不再重述)

**原型**：long FindStr (x1, y1, x2, y2, string, color_format, sim, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `intX` (变参指针)：返回X坐标没找到返回-1

- `intY` (变参指针)：返回Y坐标没找到返回-1

**返回值**：整形数:返回字符串的索引没找到返回-1,比如"长安|洛阳",若找到长安，则返回0

**示例**：dm_ret=dm.FindStr (0, 0, 2000, 2000,"长安","9f 2e 3f -000000", 1.0, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If dm_ret=dm.FindStr (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 1.0, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If//查找时,对多行文本进行换行,换行分隔符是"|".语法是在","后增加换行字符串.任意字符串都可以. dm_ret=dm.FindStr (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000,|", 1.0, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If

**注**：此函数的原理是先Ocr识别，然后再查找。所以速度比FindStrFast要慢，尤其是在字库很大，或者模糊度不为1.0时。一般字库字符数量小于100左右，模糊度为1.0时，用FindStr要快一些,否则用FindStrFast .

----

### dm_findStrE

**签名**：`const char* dm_findStrE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim)`

**简介**：在屏幕范围(x1, y1, x2, y2)内,查找string(可以是任意个字符串的组合),并返回符合color_format的坐标位置,相似度sim同Ocr接口描述. (多色,差色查找类似于Ocr接口,不再重述)易语言用不了FindStr可以用此接口来代替

**原型**：string FindStrE (x1, y1, x2, y2, string, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回字符串序号以及X和Y坐标,形式如"id|x|y",比如"0|100|200",没找到时，id和X以及Y均为-1，"-1|-1|-1"

**示例**：pos=dm.FindStrE (0, 0, 2000, 2000,"长安","9f 2e 3f -000000", 1.0) pos=split(pos,"|") If int (pos(0))>=0 Then dm.MoveTo int (pos(1)), int (pos(2)) End If pos=dm.FindStrE (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 1.0) pos=split(pos,"|") If int (pos(0))>=0 Then dm.MoveTo int (pos(1)), int (pos(2)) End If//查找时,对多行文本进行换行,换行分隔符是"|".语法是在","后增加换行字符串.任意字符串都可以. pos=dm.FindStrE (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000,|", 1.0) pos=split(pos,"|") If int (pos(0))>=0 Then dm.MoveTo int (pos(1)), int (pos(2)) End If

**注**：此函数的原理是先Ocr识别，然后再查找。所以速度比FindStrFastE要慢，尤其是在字库很大，或者模糊度不为1.0时。一般字库字符数量小于100左右，模糊度为1.0时，用FindStrE要快一些,否则用FindStrFastE .

----

### dm_findStrEx

**签名**：`const char* dm_findStrEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim)`

**简介**：在屏幕范围(x1, y1, x2, y2)内,查找string(可以是任意字符串的组合),并返回符合color_format的所有坐标位置,相似度sim同Ocr接口描述. (多色,差色查找类似于Ocr接口,不再重述)

**原型**：string FindStrEx (x1, y1, x2, y2, string, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回所有找到的坐标集合,格式如下:"id, x0, y0|id, x1, y1|......|id, xn, yn"比如"0, 100, 20|2, 30, 40"表示找到了两个,第一个,对应的是序号为0的字符串,坐标是(100, 20),第二个是序号为2的字符串,坐标(30, 40)

**示例**：dm_ret=dm.FindStrEx (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 1.0) If len (dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count TracePrint ss (index) sss=split(ss (index),",") id=int (sss (0)) x=int (sss (1)) y=int (sss (2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

**注**：此函数的原理是先Ocr识别，然后再查找。所以速度比FindStrExFast要慢，尤其是在字库很大，或者模糊度不为1.0时。一般字库字符数量小于100左右，模糊度为1.0时，用FindStrEx要快一些,否则用FindStrFastEx .

----

### dm_findStrExS

**签名**：`const char* dm_findStrExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim)`

**简介**：在屏幕范围(x1, y1, x2, y2)内,查找string(可以是任意字符串的组合),并返回符合color_format的所有坐标位置,相似度sim同Ocr接口描述. (多色,差色查找类似于Ocr接口,不再重述).此函数同FindStrEx,只是返回值不同.

**原型**：string FindStrExS (x1, y1, x2, y2, string, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回所有找到的坐标集合,格式如下:"str, x0, y0|str, x1, y1|......|str, xn, yn"比如"长安, 100, 20|大雁塔, 30, 40"表示找到了两个,第一个是长安,坐标是(100, 20),第二个是大雁塔,坐标(30, 40)

**示例**：dm_ret=dm.FindStrExS (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 1.0) If len (dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count TracePrint ss (index) sss=split(ss (index),",") str=sss (0) x=int (sss (1)) y=int (sss (2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

**注**：此函数的原理是先Ocr识别，然后再查找。所以速度比FindStrExFastS要慢，尤其是在字库很大，或者模糊度不为1.0时。一般字库字符数量小于100左右，模糊度为1.0时，用FindStrExS要快一些,否则用FindStrFastExS .

----

### dm_findStrFast

**签名**：`long dm_findStrFast(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY)`

**简介**：同FindStr。

**原型**：long FindStrFast (x1, y1, x2, y2, string, color_format, sim, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `intX` (变参指针)：返回X坐标没找到返回-1

- `intY` (变参指针)：返回Y坐标没找到返回-1

**返回值**：整形数:返回字符串的索引没找到返回-1,比如"长安|洛阳",若找到长安，则返回0

**示例**：dm_ret=dm.FindStrFast (0, 0, 2000, 2000,"长安","9f 2e 3f -000000", 1.0, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If dm_ret=dm.FindStrFast (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 0.9, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If//查找时,对多行文本进行换行,换行分隔符是"|".语法是在","后增加换行字符串.任意字符串都可以. dm_ret=dm.FindStrFast (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000,|", 0.9, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If

**注**：此函数比FindStr要快很多，尤其是在字库很大时，或者模糊识别时，效果非常明显。推荐使用此函数。另外由于此函数是只识别待查找的字符，所以可能会有如下情况出现问题。比如字库中有"张和三"一共3个字符数据，然后待识别区域里是"张和三",如果用FindStr查找"张三"肯定是找不到的，但是用FindStrFast却可以找到，因为"和"这个字符没有列入查找计划中所以，在使用此函数时，也要特别注意这一点。

----

### dm_findStrFastE

**签名**：`const char* dm_findStrFastE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim)`

**简介**：同FindStrE易语言用不了FindStrFast可以用此接口来代替

**原型**：string FindStrFastE (x1, y1, x2, y2, string, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回字符串序号以及X和Y坐标,形式如"id|x|y",比如"0|100|200",没找到时，id和X以及Y均为-1，"-1|-1|-1"

**示例**：pos=dm.FindStrFastE (0, 0, 2000, 2000,"长安","9f 2e 3f -000000", 1.0) pos=split(pos,"|") If int (pos(0))>=0 Then dm.MoveTo int (pos(1)), int (pos(2)) End If pos=dm.FindStrFastE (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 0.9) pos=split(pos,"|") If int (pos(0))>=0 Then dm.MoveTo int (pos(1)), int (pos(2)) End If//查找时,对多行文本进行换行,换行分隔符是"|".语法是在","后增加换行字符串.任意字符串都可以. pos=dm.FindStrFastE (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000,|", 0.9) pos=split(pos,"|") If int (pos(0))>=0 Then dm.MoveTo int (pos(1)), int (pos(2)) End If

**注**：此函数比FindStrE要快很多，尤其是在字库很大时，或者模糊识别时，效果非常明显。推荐使用此函数。另外由于此函数是只识别待查找的字符，所以可能会有如下情况出现问题。比如字库中有"张和三"一共3个字符数据，然后待识别区域里是"张和三",如果用FindStrE查找"张三"肯定是找不到的，但是用FindStrFastE却可以找到，因为"和"这个字符没有列入查找计划中所以，在使用此函数时，也要特别注意这一点。

----

### dm_findStrFastEx

**签名**：`const char* dm_findStrFastEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim)`

**简介**：同FindStrEx

**原型**：string FindStrFastEx (x1, y1, x2, y2, string, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回所有找到的坐标集合,格式如下:"id, x0, y0|id, x1, y1|......|id, xn, yn"比如"0, 100, 20|2, 30, 40"表示找到了两个,第一个,对应的是序号为0的字符串,坐标是(100, 20),第二个是序号为2的字符串,坐标(30, 40)

**示例**：dm_ret=dm.FindStrFastEx (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 0.9) If len (dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count TracePrint ss (index) sss=split(ss (index),",") id=int (sss (0)) x=int (sss (1)) y=int (sss (2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

**注**：此函数比FindStrEx要快很多，尤其是在字库很大时，或者模糊识别时，效果非常明显。推荐使用此函数。另外由于此函数是只识别待查找的字符，所以可能会有如下情况出现问题。比如字库中有"张和三"一共3个字符数据，然后待识别区域里是"张和三",如果用FindStrEx查找"张三"肯定是找不到的，但是用FindStrFastEx却可以找到，因为"和"这个字符没有列入查找计划中所以，在使用此函数时，也要特别注意这一点。

----

### dm_findStrFastExS

**签名**：`const char* dm_findStrFastExS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim)`

**简介**：同FindStrExS .

**原型**：string FindStrFastExS (x1, y1, x2, y2, string, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回所有找到的坐标集合,格式如下:"str, x0, y0|str, x1, y1|......|str, xn, yn"比如"长安, 100, 20|大雁塔, 30, 40"表示找到了两个,第一个是长安,坐标是(100, 20),第二个是大雁塔,坐标(30, 40)

**示例**：dm_ret=dm.FindStrFastExS (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 0.9) If len (dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count TracePrint ss (index) sss=split(ss (index),",") str=sss (0) x=int (sss (1)) y=int (sss (2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

**注**：此函数比FindStrExS要快很多，尤其是在字库很大时，或者模糊识别时，效果非常明显。推荐使用此函数。另外由于此函数是只识别待查找的字符，所以可能会有如下情况出现问题。比如字库中有"张和三"一共3个字符数据，然后待识别区域里是"张和三",如果用FindStrExS查找"张三"肯定是找不到的，但是用FindStrFastExS却可以找到，因为"和"这个字符没有列入查找计划中所以，在使用此函数时，也要特别注意这一点。

----

### dm_findStrFastS

**签名**：`long dm_findStrFastS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY)`

**简介**：同FindStrS .

**原型**：string FindStrFastS (x1, y1, x2, y2, string, color_format, sim, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `intX` (变参指针)：返回X坐标没找到返回-1

- `intY` (变参指针)：返回Y坐标没找到返回-1

**返回值**：字符串:返回找到的字符串.没找到的话返回长度为0的字符串.

**示例**：dm_ret=dm.FindStrFastS (0, 0, 2000, 2000,"长安","9f 2e 3f -000000", 1.0, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If dm_ret=dm.FindStrFastS (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 0.9, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If//查找时,对多行文本进行换行,换行分隔符是"|".语法是在","后增加换行字符串.任意字符串都可以. dm_ret=dm.FindStrFastS (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000,|", 0.9, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If

**注**：此函数比FindStrS要快很多，尤其是在字库很大时，或者模糊识别时，效果非常明显。推荐使用此函数。另外由于此函数是只识别待查找的字符，所以可能会有如下情况出现问题。比如字库中有"张和三"一共3个字符数据，然后待识别区域里是"张和三",如果用FindStrS查找"张三"肯定是找不到的，但是用FindStrFastS却可以找到，因为"和"这个字符没有列入查找计划中所以，在使用此函数时，也要特别注意这一点。

----

### dm_findStrS

**签名**：`long dm_findStrS(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, long* intX, long* intY)`

**简介**：在屏幕范围(x1, y1, x2, y2)内,查找string(可以是任意个字符串的组合),并返回符合color_format的坐标位置,相似度sim同Ocr接口描述. (多色,差色查找类似于Ocr接口,不再重述).此函数同FindStr,只是返回值不同.

**原型**：string FindStrS (x1, y1, x2, y2, string, color_format, sim, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `intX` (变参指针)：返回X坐标没找到返回-1

- `intY` (变参指针)：返回Y坐标没找到返回-1

**返回值**：字符串:返回找到的字符串.没找到的话返回长度为0的字符串.

**示例**：dm_ret=dm.FindStrS (0, 0, 2000, 2000,"长安","9f 2e 3f -000000", 1.0, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If dm_ret=dm.FindStrS (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 1.0, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If//查找时,对多行文本进行换行,换行分隔符是"|".语法是在","后增加换行字符串.任意字符串都可以. dm_ret=dm.FindStrS (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000,|", 1.0, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If

**注**：此函数的原理是先Ocr识别，然后再查找。所以速度比FindStrFastS要慢，尤其是在字库很大，或者模糊度不为1.0时。一般字库字符数量小于100左右，模糊度为1.0时，用FindStrS要快一些,否则用FindStrFastS .

----

### dm_findStrWithFont

**签名**：`long dm_findStrWithFont(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size, long* intX, long* intY)`

**简介**：同FindStr，但是不使用SetDict设置的字库，而利用系统自带的字库，速度比FindStr稍慢.

**原型**：long FindStrWithFont (x1, y1, x2, y2, string, color_format, sim, font_name, font_size, flag, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `font_name` (字符串)：系统字体名,比如"宋体"

- `font_size` (整形数)：系统字体尺寸，这个尺寸一定要以大漠综合工具获取的为准.如果获取尺寸看视频教程.

- `flag` (整形数)：字体类别取值可以是以下值的组合,比如1+2+4+8, 2+4. 0表示正常字体. 1:粗体2:斜体4:下划线8:删除线

- `intX` (变参指针)：返回X坐标没找到返回-1

- `intY` (变参指针)：返回Y坐标没找到返回-1

**返回值**：整形数:返回字符串的索引没找到返回-1,比如"长安|洛阳",若找到长安，则返回0

**示例**：dm_ret=dm.FindStrWithFont (0, 0, 2000, 2000,"长安","9f 2e 3f -000000", 1.0,"宋体", 9, 0, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If dm_ret=dm.FindStrWithFont (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 1.0,"宋体", 9, 1+2, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If//查找时,对多行文本进行换行,换行分隔符是"|".语法是在","后增加换行字符串.任意字符串都可以. dm_ret=dm.FindStrWithFont (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000,|", 1.0,"宋体", 9, 1+2, intX, intY) If intX>=0 and intY>=0 Then dm.MoveTo intX, intY End If

**注**：对于如何获取字体尺寸以及名字等信息，可以参考视频教程，如何使用系统字库.

----

### dm_findStrWithFontE

**签名**：`const char* dm_findStrWithFontE(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size)`

**简介**：同FindStrE，但是不使用SetDict设置的字库，而利用系统自带的字库，速度比FindStrE稍慢

**原型**：string FindStrWithFontE (x1, y1, x2, y2, string, color_format, sim, font_name, font_size, flag)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `font_name` (字符串)：系统字体名,比如"宋体"

- `font_size` (整形数)：系统字体尺寸，这个尺寸一定要以大漠综合工具获取的为准.如果获取尺寸看视频教程.

- `flag` (整形数)：字体类别取值可以是以下值的组合,比如1+2+4+8, 2+4. 0表示正常字体. 1:粗体2:斜体4:下划线8:删除线

**返回值**：字符串:返回字符串序号以及X和Y坐标,形式如"id|x|y",比如"0|100|200",没找到时，id和X以及Y均为-1，"-1|-1|-1"

**示例**：pos=dm.FindStrWithFontE (0, 0, 2000, 2000,"长安","9f 2e 3f -000000", 1.0,"宋体", 9, 0) pos=split(pos,"|") If int (pos(0))>=0 Then dm.MoveTo int (pos(1)), int (pos(2)) End If pos=dm.FindStrWithFontE (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 1.0,"宋体", 9, 1+2) pos=split(pos,"|") If int (pos(0))>=0 Then dm.MoveTo int (pos(1)), int (pos(2)) End If//查找时,对多行文本进行换行,换行分隔符是"|".语法是在","后增加换行字符串.任意字符串都可以. pos=dm.FindStrWithFontE (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000,|", 1.0,"宋体", 9, 1+2) pos=split(pos,"|") If int (pos(0))>=0 Then dm.MoveTo int (pos(1)), int (pos(2)) End If

**注**：对于如何获取字体尺寸以及名字等信息，可以参考视频教程，如何使用系统字库.

----

### dm_findStrWithFontEx

**签名**：`const char* dm_findStrWithFontEx(long x1, long y1, long x2, long y2, const char* str, const char* color_format, long sim, const char* font_name, long font_flag, long font_size)`

**简介**：同FindStrEx，但是不使用SetDict设置的字库，而利用系统自带的字库，速度比FindStrEx稍慢

**原型**：string FindStrWithFontEx (x1, y1, x2, y2, string, color_format, sim, font_name, font_size, flag)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `string` (字符串)：待查找的字符串,可以是字符串组合，比如"长安|洛阳|大雁塔",中间用"|"来分割字符串

- `color_format` (字符串)：颜色格式串,可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `font_name` (字符串)：系统字体名,比如"宋体"

- `font_size` (整形数)：系统字体尺寸，这个尺寸一定要以大漠综合工具获取的为准.如果获取尺寸看视频教程.

- `flag` (整形数)：字体类别取值可以是以下值的组合,比如1+2+4+8, 2+4. 0表示正常字体. 1:粗体2:斜体4:下划线8:删除线

**返回值**：字符串:返回所有找到的坐标集合,格式如下:"id, x0, y0|id, x1, y1|......|id, xn, yn"比如"0, 100, 20|2, 30, 40"表示找到了两个,第一个,对应的是序号为0的字符串,坐标是(100, 20),第二个是序号为2的字符串,坐标(30, 40)

**示例**：dm_ret=dm.FindStrWithFontEx (0, 0, 2000, 2000,"长安|洛阳","9f 2e 3f -000000", 1.0,"宋体", 9, 1+2) If len (dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count TracePrint ss (index) sss=split(ss (index),",") id=int (sss (0)) x=int (sss (1)) y=int (sss (2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

**注**：对于如何获取字体尺寸以及名字等信息，可以参考视频教程，如何使用系统字库.

----

### dm_getDict

**签名**：`const char* dm_getDict(long index, long font_index)`

**简介**：获取指定字库中指定条目的字库信息.

**原型**：string GetDict(index, font_index)

**参数定义**

- `index` (整形数)：字库序号(0-99)

- `font_index` (整形数)：字库条目序号(从0开始计数,数值不得超过指定字库的字库上限,具体参考GetDictCount)

**返回值**：字符串:返回字库条目信息.失败返回空串.

**示例**：s=dm.GetDict(0, 1245) TracePrint s s=dm.GetDict(1, 678) TracePrint s

----

### dm_getDictCount

**签名**：`long dm_getDictCount(long index)`

**简介**：获取指定的字库中的字符数量.

**原型**：long GetDictCount(index)

**参数定义**

- `index` (整形数)：字库序号(0-99)

**返回值**：整形数:字库数量

**示例**：count=dm.GetDictCount(0) TracePrint"0号字库使用的字库数量是:"&count

----

### dm_getDictInfo

**签名**：`const char* dm_getDictInfo(const char* file, long index)`

**简介**：根据指定的文字，以及指定的系统字库信息，获取字库描述信息.

**原型**：string GetDictInfo (str, font_name, font_size, flag)

**参数定义**

- `str` (字符串)：需要获取的字符串

- `font_name` (字符串)：系统字体名,比如"宋体"

- `font_size` (整形数)：系统字体尺寸，这个尺寸一定要以大漠综合工具获取的为准.如何获取尺寸看视频教程.

- `flag` (整形数)：字体类别取值可以是以下值的组合,比如1+2+4+8, 2+4. 0表示正常字体. 1:粗体2:斜体4:下划线8:删除线

**返回值**：字符串:返回字库信息,每个字符的字库信息用"|"来分割

**示例**：//下面的代码是获取"回收站"这3个字符的字库信息，然后加入到字库1中. font_desc=dm.GetDictInfo ("回收站","宋体", 9, 0) font_desc=split(font_desc,"|") count=ubound (font_desc) for i=0 to count TracePrint font_desc (i) dm.AddDict 1, font _desc(i) next

----

### dm_getNowDict

**签名**：`long dm_getNowDict()`

**简介**：获取当前使用的字库序号(0-99)

**原型**：long GetNowDict ()

**返回值**：整形数:字库序号(0-99)

**示例**：index=dm.GetNowDict () TracePrint"当前使用的字库序号是:"&index

----

### dm_getResultCount

**签名**：`long dm_getResultCount(const char* ret)`

**简介**：对插件部分接口的返回值进行解析,并返回ret中的坐标个数

**原型**：long GetResultCount(ret)

**参数定义**

- `ret` (字符串)：部分接口的返回串

**返回值**：整形数:返回ret中的坐标个数

**示例**：s=dm.FindColorEx(0, 0, 2000, 2000,"123456-000000|abcdef-202020", 1.0, 0) count=dm.GetResultCount(s) MessageBox count

----

### dm_getResultPos

**签名**：`long dm_getResultPos(const char* ret, long index, long* intX, long* intY)`

**简介**：对插件部分接口的返回值进行解析,并根据指定的第index个坐标,返回具体的值

**原型**：long GetResultPos(ret, index, intX, intY)

**参数定义**

- `ret` (字符串)：部分接口的返回串

- `index` (整形数)：第几个坐标

- `intX` (变参指针)：返回X坐标

- `intY` (变参指针)：返回Y坐标

**返回值**：整形数: 0:失败1:成功

**示例**：s=dm.FindColorEx(0, 0, 2000, 2000,"123456-000000|abcdef-202020", 1.0, 0) count=dm.GetResultCount(s) index=0 Do While index<count dm_ret=dm.GetResultPos(s, index, intX, intY) MessageBox intX&","&intY index=index+1 Loop

----

### dm_getWordResultCount

**签名**：`long dm_getWordResultCount(const char* str)`

**简介**：在使用GetWords进行词组识别以后,可以用此接口进行识别词组数量的计算.

**原型**：long GetWordResultCount(str)

**参数定义**

- `str` (字符串)：GetWords接口调用以后的返回值

**返回值**：整形数:返回词组数量

**示例**：s=dm.GetWords(0, 0, 2000, 2000,"000000-000000", 1.0) count=dm.GetWordResultCount(s) MessageBox count

----

### dm_getWordResultPos

**签名**：`long dm_getWordResultPos(const char* str, long index, long* intX, long* intY)`

**简介**：在使用GetWords进行词组识别以后,可以用此接口进行识别各个词组的坐标

**原型**：long GetWordResultPos(str, index, intX, intY)

**参数定义**

- `str` (字符串)：GetWords的返回值

- `index` (整形数)：表示第几个词组

- `intX` (变参指针)：返回的X坐标

- `intY` (变参指针)：返回的Y坐标

**返回值**：整形数: 0:失败1:成功

**示例**：s=dm.GetWords(0, 0, 2000, 2000,"000000-000000", 1.0) count=dm.GetWordResultCount(s) index=0 Do While index<count dm_ret=dm.GetWordResultPos(s, index, intX, intY) MessageBox intX&","&intY index=index+1 Loop

----

### dm_getWordResultStr

**签名**：`const char* dm_getWordResultStr(const char* str, long index)`

**简介**：在使用GetWords进行词组识别以后,可以用此接口进行识别各个词组的内容

**原型**：string GetWordResultStr(str, index)

**参数定义**

- `str` (字符串)：GetWords的返回值

- `index` (整形数)：表示第几个词组

**返回值**：字符串:返回的第index个词组内容

**示例**：s=dm.GetWords(0, 0, 2000, 2000,"000000-000000", 1.0) count=dm.GetWordResultCount(s) index=0 Do While index<count word=dm.GetWordResultStr(s, index) MessageBox word index=index+1 Loop

----

### dm_getWords

**签名**：`const char* dm_getWords(long x1, long y1, long x2, long y2, const char* color, const char* words)`

**简介**：根据指定的范围,以及设定好的词组识别参数(一般不用更改,除非你真的理解了)识别这个范围内所有满足条件的词组.比较适合用在未知文字的情况下,进行不定识别.

**原型**：string GetWords (x1, y1, x2, y2, color, sim)

**参数定义**

- `x1` (整形数)：左上角X坐标

- `y1` (整形数)：左上角Y坐标

- `x2` (整形数)：右下角X坐标

- `y2` (整形数)：右下角Y坐标

- `color` (字符串)：颜色格式串.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度0.1-1.0

**返回值**：字符串:识别到的格式串,要用到专用函数来解析

**示例**：s=dm.GetWords (0, 0, 2000, 2000,"000000-000000", 1.0) count=dm.GetWordResultCount (s) index=0 Do While index<count dm_ret=dm.GetWordResultPos (s, index, intX, intY) word=dm.GetWordResultStr (s, index) MessageBox intX&","&intY&","&word index=index+1 Loop

----

### dm_getWordsNoDict

**签名**：`const char* dm_getWordsNoDict(long x1, long y1, long x2, long y2, const char* color)`

**简介**：根据指定的范围,以及设定好的词组识别参数(一般不用更改,除非你真的理解了)识别这个范围内所有满足条件的词组.这个识别函数不会用到字库。只是识别大概形状的位置

**原型**：string GetWordsNoDict (x1, y1, x2, y2, color)

**参数定义**

- `x1` (整形数)：左上角X坐标

- `y1` (整形数)：左上角Y坐标

- `x2` (整形数)：右下角X坐标

- `y2` (整形数)：右下角Y坐标

- `color` (字符串)：颜色格式串.注意，RGB和HSV,以及灰度格式都支持.

**返回值**：字符串:识别到的格式串,要用到专用函数来解析

**示例**：s=dm.GetWordsNoDict (0, 0, 2000, 2000,"000000-000000") count=dm.GetResultCount (s) index=0 Do While index<count dm_ret=dm.GetResultPos (s, index, intX, intY) MessageBox intX&","&intY index=index+1 Loop

----

### dm_ocr

**签名**：`const char* dm_ocr(long x1, long y1, long x2, long y2, const char* color_format, long sim)`

**简介**：识别屏幕范围(x1, y1, x2, y2)内符合color_format的字符串,并且相似度为sim, sim取值范围(0.1-1.0),这个值越大越精确,越大速度越快,越小速度越慢,请斟酌使用!

**原型**：string Ocr(x1, y1, x2, y2, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color_format` (字符串)：颜色格式串.可以包含换行分隔符,语法是","后加分割字符串.具体可以查看下面的示例.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回识别到的字符串

**示例**：//RGB单色识别s=dm.Ocr(0, 0, 2000, 2000,"9f 2e 3f -000000", 1.0) MessageBox s//RGB单色差色识别s=dm.Ocr(0, 0, 2000, 2000,"9f 2e 3f -030303", 1.0) MessageBox s//RGB多色识别(最多支持10种,每种颜色用"|"分割) s=dm.Ocr(0, 0, 2000, 2000,"9f 2e 3f -030303|2d 3f 2f -000000|3f 9e4d-100000", 1.0) MessageBox s//HSV多色识别(最多支持10种,每种颜色用"|"分割) s=dm.Ocr(0, 0, 2000, 2000,"20.30.40 -0.0.0|30.40.50-0.0.0", 1.0) MessageBox s//灰度多色识别(最多支持10种,每种颜色用"|"分割) s=dm.Ocr(0, 0, 2000, 2000,"#40-0|#70-10", 1.0) MessageBox s//识别后,每行字符串用指定字符分割比如用"|"字符分割s=dm.Ocr(0, 0, 2000, 2000,"9f 2e 3f -000000,|", 1.0) MessageBox s//比如用回车换行分割s=dm.Ocr(0, 0, 2000, 2000,"9f 2e 3f -000000,"+vbcrlf, 1.0) MessageBox s//背景色识别//比如要识别背景色为白色,文字颜色未知的字形s=dm.Ocr(0, 0, 2000, 2000,"b@ffffff-000000", 1.0) MessageBox s//

**注**：在color_fomat最前面加上"b@"表示后面的颜色描述是针对背景色,而非字的颜色.

----

### dm_ocrEx

**签名**：`const char* dm_ocrEx(long x1, long y1, long x2, long y2, const char* color_format, long sim)`

**简介**：识别屏幕范围(x1, y1, x2, y2)内符合color_format的字符串,并且相似度为sim, sim取值范围(0.1-1.0),这个值越大越精确,越大速度越快,越小速度越慢,请斟酌使用!这个函数可以返回识别到的字符串，以及每个字符的坐标.

**原型**：string OcrEx (x1, y1, x2, y2, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color_format` (字符串)：颜色格式串.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回识别到的字符串格式如"字符0$x0$y0|…|字符n$xn$yn"

**示例**：和Ocr函数相同，只是结果处理有所不同如下dm_ret=dm.OcrEx (0, 0, 2000, 2000,"ffffff|000000", 1.0) ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count TracePrint ss (index) sss=split(ss (index),"$") ocr_s=int (sss (0)) x=int (sss (1)) y=int (sss (2)) TracePrint ocr_s&","&x&","&y index=index+1 Loop

**注**：OcrEx不再像Ocr一样,支持换行分割了.

----

### dm_ocrExOne

**签名**：`const char* dm_ocrExOne(long x1, long y1, long x2, long y2, const char* color_format, long sim)`

**简介**：识别屏幕范围(x1, y1, x2, y2)内符合color_format的字符串,并且相似度为sim, sim取值范围(0.1-1.0),这个值越大越精确,越大速度越快,越小速度越慢,请斟酌使用!这个函数可以返回识别到的字符串，以及每个字符的坐标.这个同OcrEx,另一种返回形式.

**原型**：string OcrExOne (x1, y1, x2, y2, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `color_format` (字符串)：颜色格式串.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回识别到的字符串格式如"识别到的信息|x0, y0|…|xn, yn"

**示例**：和Ocr函数相同，只是结果处理有所不同如下ss=dm.OcrExOne (0, 0, 2000, 2000,"ffffff|000000", 1.0) ss=split(ss,"|") MessageBox"识别到的字符串:"&ss (0) ss_len=len (ss (0)) for i=1 to ss_len MessageBox"第("&i&")的坐标是"&ss (i) next

----

### dm_ocrInFile

**签名**：`long dm_ocrInFile(long x1, long y1, long x2, long y2, const char* color_format, long sim, const char* file)`

**简介**：识别位图中区域(x1, y1, x2, y2)的文字

**原型**：string OcrInFile (x1, y1, x2, y2, pic_name, color_format, sim)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片文件名

- `color_format` (字符串)：颜色格式串.注意，RGB和HSV,以及灰度格式都支持.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

**返回值**：字符串:返回识别到的字符串

**示例**：s=dm.OcrInFile (0, 0, 2000, 2000,"test.bmp","000000-000000", 1.0) MessageBox s

----

### dm_saveDict

**签名**：`long dm_saveDict(long index, const char* file)`

**简介**：保存指定的字库到指定的文件中.

**原型**：long SaveDict (index, file)

**参数定义**

- `index` (整形数)：字库索引序号取值为0-99对应100个字库

- `file` (字符串)：文件名

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetPath"c:\test_game"dm.AddDict 0,"FFF 00A 7D 49292524A 7D402805FFC$回$0.0.54$11"dm.AddDict 0,"3F 0020087FF08270B 9A 108268708808$收$0.0.43$11"dm.AddDict 0,"2055C 98617420807C 097F 222447C 800$站$0.0.44$11"dm.SaveDict 0,"test.txt"

----

### dm_setColGapNoDict

**签名**：`long dm_setColGapNoDict(long col_gap)`

**简介**：高级用户使用,在不使用字库进行词组识别前,可设定文字的列距,默认列距是1

**原型**：long SetColGapNoDict(col_gap)

**参数定义**

- `col_gap` (整形数)：文字列距

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetColGapNoDict(3)

----

### dm_setDict

**签名**：`long dm_setDict(long index, const char* file)`

**简介**：设置字库文件

**原型**：long SetDict (index, file)

**参数定义**

- `index` (整形数)：字库的序号,取值为0-99,目前最多支持100个字库

- `file` (字符串)：字库文件名

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetDict (0,"test.txt")

**注**：此函数速度很慢，全局初始化时调用一次即可，切换字库用UseDict

----

### dm_setDictMem

**签名**：`long dm_setDictMem(long index, const char* data, long size)`

**简介**：从内存中设置字库.

**原型**：long SetDictMem (index, addr, size)

**参数定义**

- `index` (整形数)：字库的序号,取值为0-99,目前最多支持100个字库

- `addr` (整形数)：数据地址

- `size` (整形数)：字库长度

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetDictMem (0, 234324, 1000)

**注**：此函数速度很慢，全局初始化时调用一次即可，切换字库用UseDict另外，此函数不支持加密的内存字库.

----

### dm_setDictPwd

**签名**：`long dm_setDictPwd(const char* pwd)`

**简介**：设置字库的密码,在SetDict前调用,目前的设计是,所有字库通用一个密码.

**原型**：long SetDictPwd (pwd)

**参数定义**

- `pwd` (字符串)：字库密码

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetDictPwd ("1234")注意:如果使用了多字库,所有字库的密码必须一样.此函数必须在SetDict之前调用,否则会解密失败.

----

### dm_setExactOcr

**签名**：`long dm_setExactOcr(long exact_ocr)`

**简介**：高级用户使用,在使用文字识别功能前，设定是否开启精准识别.

**原型**：long SetExactOcr(exact_ocr)

**参数定义**

- `exact_ocr` (整形数)：0表示关闭精准识别1开启精准识别

**返回值**：整形数: 0:失败1:成功

**示例**：//开启精准识别dm_ret=dm.SetExactOcr(1)注意:精准识别开启后，行间距和列间距会对识别结果造成较大影响，可以在工具中进行测试.

----

### dm_setMinColGap

**签名**：`long dm_setMinColGap(long min_col_gap)`

**简介**：高级用户使用,在识别前,如果待识别区域有多行文字,可以设定列间距,默认的列间距是0,如果根据情况设定,可以提高识别精度。一般不用设定。

**原型**：long SetMinColGap(min_col_gap)

**参数定义**

- `min_col_gap` (整形数)：最小列间距

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetMinColGap(1)注意：此设置如果不为0,那么将不能识别连体字慎用.

----

### dm_setMinRowGap

**签名**：`long dm_setMinRowGap(long min_row_gap)`

**简介**：高级用户使用,在识别前,如果待识别区域有多行文字,可以设定行间距,默认的行间距是1,如果根据情况设定,可以提高识别精度。一般不用设定。

**原型**：long SetMinRowGap(min_row_gap)

**参数定义**

- `min_row_gap` (整形数)：最小行间距

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetMinRowGap(2)

----

### dm_setRowGapNoDict

**签名**：`long dm_setRowGapNoDict(long row_gap)`

**简介**：高级用户使用,在不使用字库进行词组识别前,可设定文字的行距,默认行距是1

**原型**：long SetRowGapNoDict(row_gap)

**参数定义**

- `row_gap` (整形数)：文字行距

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetRowGapNoDict(3)

----

### dm_setWordGap

**签名**：`long dm_setWordGap(long word_gap)`

**简介**：高级用户使用,在识别词组前,可设定词组间的间隔,默认的词组间隔是5

**原型**：long SetWordGap(word_gap)

**参数定义**

- `word_gap` (整形数)：单词间距

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetWordGap(5)

----

### dm_setWordGapNoDict

**签名**：`long dm_setWordGapNoDict(long word_gap)`

**简介**：高级用户使用,在不使用字库进行词组识别前,可设定词组间的间隔,默认的词组间隔是5

**原型**：long SetWordGapNoDict (word_gap)

**参数定义**

- `word_gap` (整形数)：单词间距

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetWordGapNoDict (1)

----

### dm_setWordLineHeight

**签名**：`long dm_setWordLineHeight(long line_height)`

**简介**：高级用户使用,在识别词组前,可设定文字的平均行高,默认的词组行高是10

**原型**：long SetWordLineHeight(line_height)

**参数定义**

- `line_height` (整形数)：行高

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetWordLineHeight(15)

----

### dm_setWordLineHeightNoDict

**签名**：`long dm_setWordLineHeightNoDict(long line_height)`

**简介**：高级用户使用,在不使用字库进行词组识别前,可设定文字的平均行高,默认的词组行高是10

**原型**：long SetWordLineHeightNoDict(line_height)

**参数定义**

- `line_height` (整形数)：行高

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetWordLineHeightNoDict(15)

----

### dm_useDict

**签名**：`long dm_useDict(long index)`

**简介**：表示使用哪个字库文件进行识别(index范围: 0-99)设置之后，永久生效，除非再次设定

**原型**：long UseDict (index)

**参数定义**

- `index` (整形数)：字库编号(0-99)

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.UseDict (1) ss=dm.Ocr (0, 0, 2000, 2000,"FFFFFF-000000", 1.0) dm_ret=dm.UseDict (0)

----

### dm_beep

**签名**：`long dm_beep(long f, long duration)`

**简介**：蜂鸣器.

**原型**：long Beep(f, duration)

**参数定义**

- `f` (整形数)：频率

- `duration` (整形数)：时长(ms).

**返回值**：整形数: 0:失败1:成功

**示例**：dm.Beep 1000, 1000

----

### dm_checkFontSmooth

**签名**：`long dm_checkFontSmooth()`

**简介**：检测当前系统是否有开启屏幕字体平滑.

**原型**：long CheckFontSmooth ()

**返回值**：整形数: 0:系统没开启平滑字体. 1:系统有开启平滑字体.

**示例**：if dm.CheckFontSmooth ()=1 then TracePrint"当前系统有开启平滑字体"end if

----

### dm_checkUAC

**签名**：`long dm_checkUAC()`

**简介**：检测当前系统是否有开启UAC(用户账户控制).

**原型**：long CheckUAC()

**返回值**：整形数: 0:没开启UAC 1:开启了UAC

**示例**：if dm.CheckUAC()=1 then TracePrint"当前系统开启了用户账户控制"end if

**注**：只有WIN7 WIN8 VISTA WIN2008以及以上系统才有UAC设置

----

### dm_delay

**签名**：`long dm_delay(long mis)`

**简介**：延时指定的毫秒,过程中不阻塞UI操作.一般高级语言使用.按键用不到.

**原型**：long Delay(mis)

**参数定义**

- `mis` (整形数)：毫秒数.必须大于0.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.Delay 1000

**注**：由于是com组件,调用此函数必须保证调用线程的模型为MTA.否则此函数可能会失效.

----

### dm_delays

**签名**：`long dm_delays(long mis_min, long mis_max)`

**简介**：延时指定范围内随机毫秒,过程中不阻塞UI操作.一般高级语言使用.按键用不到.

**原型**：long Delays(mis_min, mis_max)

**参数定义**

- `mis_min` (整形数)：最小毫秒数.必须大于0

- `mis_max` (整形数)：最大毫秒数.必须大于0

**返回值**：整形数: 0:失败1:成功

**示例**：dm.Delays 200, 1000

**注**：由于是com组件,调用此函数必须保证调用线程的模型为MTA.否则此函数可能会失效.

----

### dm_disableCloseDisplayAndSleep

**签名**：`long dm_disableCloseDisplayAndSleep(long enable)`

**简介**：设置当前的电源设置，禁止关闭显示器，禁止关闭硬盘，禁止睡眠，禁止待机.不支持XP.

**原型**：long DisableCloseDisplayAndSleep ()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.DisableCloseDisplayAndSleep

----

### dm_disableFontSmooth

**签名**：`long dm_disableFontSmooth()`

**简介**：关闭当前系统屏幕字体平滑.同时关闭系统的ClearType功能.

**原型**：long DisableFontSmooth ()

**返回值**：整形数: 0:失败1:成功

**示例**：if dm.CheckFontSmooth ()=1 then if dm.DisableFontSmooth ()=1 then MessageBox"关闭了当前系统平滑字体,重启生效"dm.ExitOs 2 Delay 2000 EndScript end if end if

**注**：关闭之后要让系统生效，必须重启系统才有效.

----

### dm_disablePowerSave

**签名**：`long dm_disablePowerSave(long enable)`

**简介**：关闭电源管理，不会进入睡眠.

**原型**：long DisablePowerSave ()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.DisablePowerSave

**注**：此函数调用以后，并不会更改系统电源设置.此函数经常用在后台操作过程中.避免被系统干扰.

----

### dm_disableScreenSave

**签名**：`long dm_disableScreenSave(long enable)`

**简介**：关闭屏幕保护.

**原型**：long DisableScreenSave ()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.DisableScreenSave

**注**：调用此函数后，可能在系统中还是看到屏保是开启状态。但实际上屏保已经失效了.系统重启后，会失效。必须再重新调用一次.此函数经常用在后台操作过程中.避免被系统干扰.

----

### dm_enableFontSmooth

**签名**：`long dm_enableFontSmooth()`

**简介**：开启当前系统屏幕字体平滑.同时开启系统的ClearType功能.

**原型**：long EnableFontSmooth ()

**返回值**：整形数: 0:失败1:成功

**示例**：if dm.CheckFontSmooth ()=0 then if dm.EnableFontSmooth ()=1 then MessageBox"开启了当前系统平滑字体,重启生效"d m.ExitOs 2 Delay 2000 EndScript end if end if

**注**：开启之后要让系统生效，必须重启系统才有效.

----

### dm_exitOs

**签名**：`long dm_exitOs(long type)`

**简介**：退出系统(注销重启关机)

**原型**：long ExitOs(type)

**参数定义**

- `type` (整形数)：取值为以下类型0:注销系统1:关机2:重新启动

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.ExitOs(1)

----

### dm_getClipboard

**签名**：`const char* dm_getClipboard()`

**简介**：获取剪贴板的内容

**原型**：string GetClipboard()

**返回值**：字符串:以字符串表示的剪贴板内容

**示例**：TracePrint dm.GetClipboard()

----

### dm_getCpuType

**签名**：`long dm_getCpuType()`

**简介**：获取当前CPU类型(intel或者amd).

**原型**：long GetCpuType()

**返回值**：整形数: 0:未知1: Intel cpu 2: AMD cpu

**示例**：if dm.GetCpuType()<>1 then MessageBox"当前系统CPU不是intel cpu,不支持!"EndScript end if

----

### dm_getCpuUsage

**签名**：`long dm_getCpuUsage()`

**简介**：获取当前CPU的使用率.用百分比返回.

**原型**：long GetCpuUsage ()

**返回值**：整形数: 0-100表示的百分比

**示例**：TracePrint dm.GetCpuUsage()

----

### dm_getDir

**签名**：`const char* dm_getDir(long type)`

**简介**：得到系统的路径

**原型**：string GetDir(type)

**参数定义**

- `type` (整形数)：取值为以下类型0:获取当前路径1:获取系统路径(system32路径) 2:获取windows路径(windows所在路径) 3:获取临时目录路径(temp) 4:获取当前进程(exe)所在的路径

**返回值**：字符串:返回路径

**示例**：path=dm.GetDir(2)

----

### dm_getDiskModel

**签名**：`const char* dm_getDiskModel()`

**简介**：获取本机的指定硬盘的厂商信息.要求调用进程必须有管理员权限.否则返回空串.

**原型**：string GetDiskModel(index)

**参数定义**

- `index` (整形数)：硬盘序号.表示是第几块硬盘.从0开始编号,最小为0,最大为5,也就是最多支持6块硬盘的厂商信息获取.

**返回值**：字符串:字符串表达的硬盘厂商信息

**示例**：//获取第一块硬盘的厂商信息model=dm.GetDiskModel(0)

----

### dm_getDiskReversion

**签名**：`const char* dm_getDiskReversion()`

**简介**：获取本机的指定硬盘的修正版本信息.要求调用进程必须有管理员权限.否则返回空串.

**原型**：string GetDiskReversion (index)

**参数定义**

- `index` (整形数)：硬盘序号.表示是第几块硬盘.从0开始编号,最小为0,最大为5,也就是最多支持6块硬盘的修正版本信息获取.

**返回值**：字符串:字符串表达的修正版本信息

**示例**：//获取第一块硬盘的修正版本信息reversion=dm. GetDiskReversion (0)

----

### dm_getDiskSerial

**签名**：`const char* dm_getDiskSerial()`

**简介**：获取本机的指定硬盘的序列号.要求调用进程必须有管理员权限.否则返回空串.

**原型**：string GetDiskSerial(index)

**参数定义**

- `index` (整形数)：硬盘序号.表示是第几块硬盘.从0开始编号,最小为0,最大为5,也就是最多支持6块硬盘的序列号获取.

**返回值**：字符串:字符串表达的硬盘序列号

**示例**：//获取第一块硬盘的序列号sirial=dm.GetDiskSerial(0)

----

### dm_getDisplayInfo

**签名**：`long dm_getDisplayInfo(long type)`

**简介**：获取本机的显卡信息.

**原型**：string GetDisplayInfo ()

**返回值**：字符串:字符串表达的显卡描述信息.如果有多个显卡,用"|"连接

**示例**：TracePrint dm.GetDisplayInfo()

----

### dm_getDPI

**签名**：`long dm_getDPI()`

**简介**：判断当前系统的DPI(文字缩放)是不是100%缩放.

**原型**：long GetDPI()

**返回值**：整形数: 0:不是1:是

**示例**：if dm.GetDPI()=0 then MessageBox"当前系统文字缩放不是100%,请设置为100%"EndScript end if

----

### dm_getLocale

**签名**：`long dm_getLocale()`

**简介**：判断当前系统使用的非UNICODE字符集是否是GB2312(简体中文) (由于设计插件时偷懒了,使用的是非UNICODE字符集，导致插件必须运行在GB2312字符集环境下).

**原型**：long GetLocale()

**返回值**：整形数: 0:不是GB2312(简体中文) 1:是GB2312(简体中文)

**示例**：if dm.GetLocale()=0 then dm.SetLocale() dm.ExitOs(2) end if

----

### dm_getMachineCode

**签名**：`const char* dm_getMachineCode()`

**简介**：获取本机的机器码.(带网卡).此机器码用于插件网站后台.要求调用进程必须有管理员权限.否则返回空串.

**原型**：string GetMachineCode ()

**返回值**：字符串:字符串表达的机器机器码

**示例**：machine_code=dm.GetMachineCode ()

**注**：此机器码包含的硬件设备有硬盘,显卡,网卡等.其它不便透露.重装系统不会改变此值.另要注意,插拔任何USB设备,(U盘，U盾, USB移动硬盘, USB键鼠等),以及安装任何网卡驱动程序,(开启或者关闭无线网卡等)都会导致机器码改变.

----

### dm_getMachineCodeNoMac

**签名**：`const char* dm_getMachineCodeNoMac()`

**简介**：获取本机的机器码.(不带网卡)要求调用进程必须有管理员权限.否则返回空串.

**原型**：string GetMachineCodeNoMac ()

**返回值**：字符串:字符串表达的机器机器码

**示例**：machine_code=dm.GetMachineCodeNoMac ()

**注**：此机器码包含的硬件设备有硬盘,显卡,等.其它不便透露.重装系统不会改变此值.另要注意,插拔任何USB设备,(U盘，U盾, USB移动硬盘, USB键鼠等),都会导致机器码改变.

----

### dm_getMemoryUsage

**签名**：`long dm_getMemoryUsage()`

**简介**：获取当前内存的使用率.用百分比返回.

**原型**：long GetMemoryUsage()

**返回值**：整形数: 0-100表示的百分比

**示例**：TracePrint dm.GetMemoryUsage()

----

### dm_getNetTime

**签名**：`const char* dm_getNetTime()`

**简介**：从网络获取当前北京时间.

**原型**：string GetNetTime ()

**返回值**：字符串:时间格式.和now返回一致.比如"2001-11-01 23: 14: 08"

**示例**：t=dm.GetNetTime () TracePrint"当前北京时间是:"&t

**注**：如果程序无法访问时间服务器，那么返回"0000-00-00 00: 00: 00"

----

### dm_getNetTimeByIp

**签名**：`const char* dm_getNetTimeByIp(const char* ip)`

**简介**：根据指定时间服务器IP,从网络获取当前北京时间.

**原型**：string GetNetTimeByIp(ip)

**参数定义**

- `ip` (字符串)：IP或者域名,并且支持多个IP或者域名连接

**返回值**：字符串:时间格式.和now返回一致.比如"2001-11-01 23: 14: 08"

**示例**：t=dm.GetNetTimeByIp("210.72.145.44|ntp.sjtu.edu.cn") TracePrint"当前北京时间是:"&t

**注**：如果程序无法访问时间服务器，那么返回"0000-00-00 00: 00: 00"时间服务器的IP可以从网上查找NTP服务器.

----

### dm_getNetTimeSafe

**签名**：`const char* dm_getNetTimeSafe(long time_out)`

**简介**：服务器压力太大,此函数不再支持。请使用GetNetTimeByIp

**原型**：string GetNetTimeSafe()

**返回值**：字符串:时间格式.和now返回一致.比如"2001-11-01 23: 14: 08"

**示例**：t=dm.GetNetTimeSafe() TracePrint"当前北京时间是:"&t

**注**：此接口不支持简单游平台.如果程序无法访问时间服务器，那么空串.

----

### dm_getOsBuildNumber

**签名**：`long dm_getOsBuildNumber()`

**简介**：得到操作系统的build版本号.比如win10 16299,那么返回的就是16299.其他类似

**原型**：long GetOsBuildNumber ()

**返回值**：整形数: build版本号失败返回0

**示例**：os_build_number=dm.GetOsBuildNumber () WIN11的BuildNumber从22000开始.如果要判断是不是WIN11,直接判断BuildNumber是否大于等于22000即可.

----

### dm_getOsType

**签名**：`long dm_getOsType()`

**简介**：得到操作系统的类型

**原型**：long GetOsType()

**返回值**：整形数: 0: win95/98/me/nt4.0 1: xp/2000 2: 2003/2003 R2/xp-64 3: vista/2008 4: win7/2008 R2 5: win8/2012 6: win8.1/2012 R2 7: win10/2016 TP/win11

**示例**：os_type=dm.GetOsType()

----

### dm_getScreenDepth

**签名**：`long dm_getScreenDepth()`

**简介**：获取屏幕的色深.

**原型**：long GetScreenDepth()

**返回值**：整形数:返回系统颜色深度.(16或者32等)

**示例**：Depth=dm.GetScreenDepth()

----

### dm_getScreenHeight

**签名**：`long dm_getScreenHeight()`

**简介**：获取屏幕的高度.

**原型**：long GetScreenHeight()

**返回值**：整形数:返回屏幕的高度

**示例**：ScreenH=dm.GetScreenHeight()

----

### dm_getScreenWidth

**签名**：`long dm_getScreenWidth()`

**简介**：获取屏幕的宽度.

**原型**：long GetScreenWidth()

**返回值**：整形数:返回屏幕的宽度

**示例**：ScreenW=dm.GetScreenWidth()

----

### dm_getSystemInfo

**签名**：`const char* dm_getSystemInfo(long type, long method)`

**简介**：获取指定的系统信息.

**原型**：string GetSystemInfo(type, method)

**参数定义**

- `type` (字符串)：取值如下"cpuid":表示获取cpu序列号. method可取0和1"disk_volume_serial id":表示获取分区序列号. id表示分区序号. 0表示C盘.1表示D盘.以此类推.最高取到5.也就是6个分区. method可取0"bios_vendor":表示获取bios厂商信息. method可取0和1"bios_version":表示获取bios版本信息. method可取0和1"bios_release_date":表示获取bios发布日期. method可取0和1"bios_oem":表示获取bios里的oem信息. method可取0"board_vendor":表示获取主板制造厂商信息. method可取0和1"board_product":表示获取主板产品信息. method可取0和1"board_version":表示获取主板版本信息. method可取0和1"board_serial":表示获取主板序列号. method可取0"board_location":表示获取主板位置信息. method可取0"system_manufacturer":表示获取系统制造商信息. method可取0和1"system_product":表示获取系统产品信息. method可取0和1"system_serial":表示获取bios序列号. method可取0"system_uuid":表示获取bios uuid. method可取0"system_version":表示获取系统版本信息. method可取0和1"system_sku":表示获取系统sku序列号. method可取0和1"system_family":表示获取系统家族信息. method可取0和1"product_id":表示获取系统产品id. method可取0"system_identifier":表示获取系统标识. method可取0"system_bios_version":表示获取系统BIOS版本号. method可取0.多个结果用"|"连接."system_bios_date":表示获取系统BIOS日期. method可取0

- `method` (整形数)：获取方法.一般从0开始取值.

**返回值**：字符串:字符串表达的系统信息.

**示例**：//获取系统所有特征信息TracePrint dm.GetSystemInfo("cpuid", 0) TracePrint dm.GetSystemInfo("cpuid", 1) TracePrint dm.GetSystemInfo("bios_vendor", 0) TracePrint dm.GetSystemInfo("bios_vendor", 1) TracePrint dm.GetSystemInfo("bios_version", 0) TracePrint dm.GetSystemInfo("bios_version", 1) TracePrint dm.GetSystemInfo("bios_release_date", 0) TracePrint dm.GetSystemInfo("bios_release_date", 1) TracePrint dm.GetSystemInfo("bios_oem", 0) TracePrint dm.GetSystemInfo("board_vendor", 0) TracePrint dm.GetSystemInfo("board_vendor", 1) TracePrint dm.GetSystemInfo("board_product", 0) TracePrint dm.GetSystemInfo("board_product", 1) TracePrint dm.GetSystemInfo("board_version", 0) TracePrint dm.GetSystemInfo("board_version", 1) TracePrint dm.GetSystemInfo("board_serial", 0) TracePrint dm.GetSystemInfo("board_location", 0) TracePrint dm.GetSystemInfo("system_manufacturer", 0) TracePrint dm.GetSystemInfo("system_manufacturer", 1) TracePrint dm.GetSystemInfo("system_product", 0) TracePrint dm.GetSystemInfo("system_product", 1) TracePrint dm.GetSystemInfo("system_serial", 0) TracePrint dm.GetSystemInfo("system_uuid", 0) TracePrint dm.GetSystemInfo("system_version", 0) TracePrint dm.GetSystemInfo("system_version", 1) TracePrint dm.GetSystemInfo("system_sku", 0) TracePrint dm.GetSystemInfo("system_sku", 1) TracePrint dm.GetSystemInfo("system_family", 0) TracePrint dm.GetSystemInfo("system_family", 1) TracePrint dm.GetSystemInfo("product_id", 0) TracePrint dm.GetSystemInfo("system_identifier", 0) TracePrint dm.GetSystemInfo("system_bios_version", 0) TracePrint dm.GetSystemInfo("system_bios_date", 0) TracePrint dm.GetSystemInfo("disk_volume_serial 0", 0) TracePrint dm.GetSystemInfo("disk_volume_serial 1", 0) TracePrint dm.GetSystemInfo("disk_volume_serial 2", 0) TracePrint dm.GetDiskSerial(0) TracePrint dm.GetDiskModel(0) TracePrint dm.GetDiskReversion(0) TracePrint dm.GetMac()

----

### dm_getTime

**签名**：`long dm_getTime()`

**简介**：获取当前系统从开机到现在所经历过的时间，单位是毫秒

**原型**：long GetTime()

**返回值**：整形数:时间(单位毫秒)

**示例**：t1=dm.GetTime() dm_ret=dm.FindPic(0, 0, 2000, 2000,"test.bmp","000000", 1.0, 0, x, y) t2=dm.GetTime() MessageBox (t2-t1)

----

### dm_is64Bit

**签名**：`long dm_is64Bit()`

**简介**：判断当前系统是否是64位操作系统

**原型**：long Is64Bit()

**返回值**：整形数: 0:不是64位系统1:是64位系统

**示例**：if dm.Is64Bit()=1 then MessageBox"64位系统"else MessageBox"不是64位系统"end if

----

### dm_isSurrpotVt

**签名**：`long dm_isSurrpotVt()`

**简介**：判断当前CPU是否支持vt,并且是否在bios中开启了vt.仅支持intel的CPU.

**原型**：long IsSurrpotVt()

**返回值**：整形数: 0:当前cpu不是intel的cpu,或者当前cpu不支持vt,或者bios中没打开vt. 1:支持

**示例**：if dm.IsSurrpotVt()=1 then MessageBox"当前系统可以开启vt功能"else MessageBox"不支持vt"end if

----

### dm_play

**签名**：`long dm_play(const char* media_file)`

**简介**：播放指定的MP3或者wav文件.

**原型**：long Play(media_file)

**参数定义**

- `media_file` (字符串)：指定的音乐文件，可以采用文件名或者绝对路径的形式.

**返回值**：整形数: 0:失败非0表示当前播放的ID。可以用Stop来控制播放结束.

**示例**：//test.mp3放于d:\test目录下dm.SetPath"d:\test"id=dm.Play("test.mp3")//绝对路径id=dm.Play("d:\test\test.mp3") Delay 1000 dm.Stop id

----

### dm_runApp

**签名**：`long dm_runApp(const char* app_path, long mode)`

**简介**：运行指定的应用程序.

**原型**：long RunApp (app_path, mode)

**参数定义**

- `app_path` (字符串)：指定的可执行程序全路径.

- `mode` (整形数)：取值如下0:普通模式1:加强模式

**返回值**：整形数: 0:失败1:成功

**示例**：dm.RunApp"c:\windows\notepad.exe", 0 dm.RunApp"notepad", 1

----

### dm_setClipboard

**签名**：`long dm_setClipboard(const char* value)`

**简介**：设置剪贴板的内容

**原型**：long SetClipboard(value)

**参数定义**

- `value` (字符串)：以字符串表示的剪贴板内容

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetClipboard"abcd"

----

### dm_setDisplayAcceler

**签名**：`long dm_setDisplayAcceler(long flag)`

**简介**：设置当前系统的硬件加速级别.

**原型**：long SetDisplayAcceler (level)

**参数定义**

- `level` (整形数)：取值范围为0-5. 0表示关闭硬件加速。5表示完全打开硬件加速.

**返回值**：整形数: 0:失败. 1:成功.

**示例**：//关闭硬件加速TracePrint SetDisplayAcceler (0)

**注**：此函数只在XP 2003系统有效.

----

### dm_setScreen

**签名**：`long dm_setScreen(long width, long height, long depth)`

**简介**：设置系统的分辨率系统色深

**原型**：long SetScreen(width, height, depth)

**参数定义**

- `width` (整形数)：屏幕宽度

- `height` (整形数)：屏幕高度

- `depth` (整形数)：系统色深

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetScreen(1024, 768, 16)

----

### dm_setUAC

**签名**：`long dm_setUAC(long enable)`

**简介**：设置当前系统的UAC(用户账户控制).

**原型**：long SetUAC(enable)

**参数定义**

- `enable` (整形数)：取值如下0:关闭UAC 1:开启UAC

**返回值**：整形数: 0:操作失败1:操作成功

**示例**：if dm.SetUAC(0)=1 then TracePrint"成功关闭了当前系统UAC设置"end if

**注**：只有WIN7 WIN8 VISTA WIN2008以及以上系统才有UAC设置.关闭UAC以后，必须重启系统才会生效.如果关闭了UAC，那么默认启动所有应用程序都是管理员权限，就不会再发生绑定失败这样的尴尬情况了.

----

### dm_showTaskBarIcon

**签名**：`long dm_showTaskBarIcon(intptr_t hwnd, long show)`

**简介**：显示或者隐藏指定窗口在任务栏的图标.

**原型**：long ShowTaskBarIcon (hwnd, is_show)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄

- `is_show` (整形数)：0为隐藏, 1为显示

**返回值**：整形数: 0:失败1:成功

**示例**：//显示dm.ShowTaskBarIcon hwnd, 1//隐藏dm.ShowTaskBarIcon hwnd, 0

----

### dm_stop

**签名**：`long dm_stop(long id)`

**简介**：停止指定的音乐.

**原型**：long Stop(id)

**参数定义**

- `id` (整形数)：Play返回的播放id.

**返回值**：整形数: 0:失败1:成功.

**示例**：//test.mp3放于d:\test目录下dm.SetPath"d:\test"id=dm.Play("test.mp3")//绝对路径id=dm.Play("d:\test\test.mp3") Delay 1000 dm.Stop id

----

### dm_activeInputMethod

**签名**：`long dm_activeInputMethod(intptr_t hwnd, const char* input_method)`

**简介**：激活指定窗口所在进程的输入法.

**原型**：long ActiveInputMethod(hwnd, input_method)

**参数定义**

- `hwnd` (整形数)：窗口句柄

- `input_method` (字符串)：输入法名字。具体输入法名字对应表查看注册表中以下位置: HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts下面的每一项下的Layout Text的值就是输入法名字比如"中文- QQ拼音输入法"以此类推.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.ActiveInputMethod(hwnd,"中文- QQ拼音输入法") if dm_ret=1 then msgbox"QQ输入法开启成功"end if

----

### dm_checkInputMethod

**签名**：`long dm_checkInputMethod(intptr_t hwnd, const char* input_method)`

**简介**：检测指定窗口所在线程输入法是否开启

**原型**：long CheckInputMethod(hwnd, input_method)

**参数定义**

- `hwnd` (整形数)：窗口句柄

- `input_method` (字符串)：输入法名字。具体输入法名字对应表查看注册表中以下位置: HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts下面的每一项下的Layout Text的值就是输入法名字比如"中文- QQ拼音输入法"以此类推.

**返回值**：整形数: 0:未开启1:开启

**示例**：dm_ret=dm.CheckInputMethod(hwnd,"中文- QQ拼音输入法") if dm_ret=1 then msgbox"QQ输入法开启拉"end if

----

### dm_enterCri

**签名**：`long dm_enterCri()`

**简介**：检测是否可以进入临界区,如果可以返回1,否则返回0.此函数如果返回1，则调用对象就会占用此互斥信号量,直到此对象调用LeaveCri,否则不会释放.注意:如果调用对象在释放时，会自动把本对象占用的互斥信号量释放.

**原型**：long EnterCri ()

**返回值**：整形数: 0:不可以1:已经进入临界区

**示例**：do if dm.EnterCri ()=1 then exit do end if delay 100 loop关于如何前台多开,点这里.

----

### dm_executeCmd

**签名**：`const char* dm_executeCmd(const char* cmd, long time_out)`

**简介**：执行指定的CMD指令,并返回cmd的输出结果.

**原型**：string ExecuteCmd(cmd, current_dir, time_out)

**参数定义**

- `cmd` (字符串)：需要执行的CMD指令.比如"dir"

- `current_dir` (字符串)：执行此cmd命令时,所在目录.如果为空，表示使用当前目录.比如""或者"c:"

- `time_out` (整形数)：超时设置,单位是毫秒. 0表示一直等待.大于0表示等待指定的时间后强制结束,防止卡死.

**返回值**：字符串: cmd指令的执行结果.返回空字符串表示执行失败.

**示例**：TracePrint dm.ExecuteCmd("dir","", 0) TracePrint dm.ExecuteCmd("dir","c:", 2000) TracePrint dm.ExecuteCmd("dir","c:\windows", 3000)介于很多人不会用命令行操作CMD,这里写一份常用的adb命令来给大家参考.首先, adb.exe是一个操作android系统的应用程序，一般都在你安装的模拟器的对应的目录下.比如雷电模拟器,我们假如安装再d:\dnplayer2,那么adb.exe一般就位于这个目录.其它模拟器同理.知道adb.exe的路径，那么我们就开始调用adb来实现一些常用的功能.以下所有的例子，都假定adb.exe位于d:\dnplayer2 1.查看adb的版本信息.这个可以用于测试adb.exe是否是你想要的版本,如下: adb_version=dm.ExecuteCmd("adb.exe version","d:\dnplayer2", 0) TracePrint adb_version比如我的机器的返回值是以下内容Android Debug Bridge version 1.0.31 2.接下来我们开始对模拟器里的东东做一些操作.比如安装APK，拷贝文件之类的。我们首先要先列出当前系统的所有device(可以是模拟器，也可以是用USB连接的手机),例子如下: adb_devices=dm.ExecuteCmd("adb.exe devices","d:\dnplayer2", 0) TracePrint adb_devices比如我的机器的返回值如下:(我打开了2个模拟器) List of devices attached 127.0.0.1: 5555 device 127.0.0.1: 5557 device这里要说明一下,前面这个IP地址和端口号，就标识了一个device,我们后面要操作这些devcie，必须依赖于这个标识.有的时候，这个标识不一定是ip地址和端口号，也可能是序列号之类的东西.但意思都一样. 3.接下来我们来对127.0.0.1: 5555这个device来查看下安装的应用,例子如下:(这里我们要用到adb shell命令,顾名思义，这个shell的意思就是去device上去执行命令,这里的语法都和linux的语法一样) adb_device_1_apps=dm.ExecuteCmd("adb.exe -s 127.0.0.1: 5555 shell pm list packages","d:\dnplayer2", 0) TracePrint adb_device_1_apps这里输出的内容比较多，我就不列举了简单的说一下，这里的-s设备标识的意思就是对这台device来执行命令.设备标识在之前adb devices中有列出来.那么我们要执行其它的操作，也是如此,比如"adb.exe -s设备标识命令"比如安装apk dm.ExecuteCmd("adb.exe -s 127.0.0.1: 5555 install -r d:\xxx.apk","d:\dnplayer2", 0)比如卸载某apk dm.ExecuteCmd("adb.exe -s 127.0.0.1: 5555 uninstall com.qihoo360.mobilesafe","d:\dnplayer2", 0)好到此为止，如何操作adb去控制模拟器，就说到这里。这里贴一份常用详细的adb中文说明给大家参考https://blog.csdn.net/u010375364/article/details/52344120

----

### dm_findInputMethod

**签名**：`long dm_findInputMethod(const char* input_method)`

**简介**：检测系统中是否安装了指定输入法

**原型**：long FindInputMethod(input_method)

**参数定义**

- `input_method` (字符串)：输入法名字。具体输入法名字对应表查看注册表中以下位置: HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Keyboard Layouts下面的每一项下的Layout Text的值就是输入法名字比如"中文- QQ拼音输入法"以此类推.

**返回值**：整形数: 0:未安装1:安装了

**示例**：dm_ret=dm.FindInputMethod("中文- QQ拼音输入法") if dm_ret=1 then msgbox"QQ输入法安装啦"end if

----

### dm_initCri

**签名**：`long dm_initCri()`

**简介**：初始化临界区,必须在脚本开头调用一次.这个函数是强制把插件内的互斥信号量归0,无论调用对象是否拥有此信号量.

**原型**：long InitCri()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.InitCri关于如何前台多开,点这里.

----

### dm_leaveCri

**签名**：`long dm_leaveCri()`

**简介**：和EnterCri对应,离开临界区。此函数是释放调用对象占用的互斥信号量.注意，只有调用对象占有了互斥信号量，此函数才会有作用.否则没有任何作用.如果调用对象在释放时，会自动把本对象占用的互斥信号量释放.

**原型**：long LeaveCri()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.LeaveCri关于如何前台多开,点这里.

----

### dm_releaseRef

**签名**：`long dm_releaseRef()`

**简介**：强制降低对象的引用计数。此接口为高级接口，一般使用在高级语言，比如E vc等.

**原型**：long ReleaseRef()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.ReleaseRef每个对象内部，系统都会维护一个引用计数,当计数为0时，才可以释放对象。由于在某些高级语言中使用时，会由于各种原因，导致对象的引用计数出现异常，最后导致对象无法被释放.比如一个对象在使用过程中（比如调用了对象的某个接口过程中），突然所在线程被强制结束，导致对象引用计数无法被释放.然后这个对象就永远无法被释放掉了。最后导致资源泄漏.还有可能是，由于不正确的复制了对象，但复制过后的对象又没有正确的释放，也会导致计数异常。为了让对象能够正确的被释放，提供这个接口，强制释放引用计数。一般用在对象释放之前，并且此时不能有任何线程去调用此对象的任何接口。具体的使用例子，在最新版本的类库生成工具，生成以后有相对应平台的多线程模板，里面有详细介绍.

----

### dm_setExitThread

**签名**：`long dm_setExitThread(long mode)`

**简介**：设置当前对象的退出线程标记，之后除了调用此接口的线程之外，调用此对象的任何接口的线程会被强制退出.此接口为高级接口，一般用在高级语言,比如e vc等.

**原型**：long SetExitThread(mode)

**参数定义**

- `enable` (整形数)：1和2都为开启标记, 0为关闭标记。1和2的区别是, 1会解绑当前对象的绑定, 2不会.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetExitThread 1一般我们在写多线程程序时，如何正确的结束线程是个难题.脚本语言一般没这种烦恼，但高级语言比如E vc等就很麻烦.一般来说，让线程自然的结束，那是最好的结果.但是事实上，高级语言中很难做到。因为调用的函数是一层套一层，很难返回.所以，我们退而求其次，让线程自己调用退出，这样虽然也有一定的资源泄漏（主要是线程中创建的局部变量，比如类对象等),但总比强制结束线程要好的多.所以，我们这个接口的目的也很明显，设置以后，除了调用线程之外的线程，如果调用到插件，那么线程就自己退出了。具体的使用例子，在最新版本的类库生成工具，生成以后有相对应平台的多线程模板，里面有详细介绍.

----

### dm_asmAdd

**签名**：`long dm_asmAdd(const char* asm_ins)`

**简介**：添加指定的MASM汇编指令.支持标准的masm汇编指令.

**原型**：long AsmAdd (asm_ins)

**返回值**：整形数: 0:失败1:成功

**示例**：dm.AsmAdd"push 100"dm.AsmAdd"push 60304d"dm.AsmAdd"emit 90 90 90"dm.AsmAdd"push dword ptr[112233bb]dm.AsmAdd"call 678fed"//下面是一个跳转指令的例子dm.AsmAdd"mov eax, 1"dm.AsmAdd"cmp eax, 1"dm.AsmAdd"je Label1"dm.AsmAdd"mov eax, 3"dm.AsmAdd"jmp Exit"dm.AsmAdd": Label1"dm.AsmAdd"mov eax, 2"dm.AsmAdd": Exit"dm.AsmCall hwnd, 1

**注**：有些人用老版本的插件，省略了dword qword word byte等前缀,事实上老版本语法规则不完善，容易出问题,从新版本7.1818开始，所有语法都必须是规范的masm语法.大家可以参考od里的汇编语法.以下的都是一些错误的写法push[112233bb]mov ecx,[aabbccdd]等。以下的才是正确的写法push dword ptr[112233bb]mov ecx, dword ptr[aabbccdd]mov cx, word ptr[aabbccdd]mov cl, byte ptr[aabbccdd]等

----

### dm_asmCall

**签名**：`long dm_asmCall(intptr_t hwnd, long mode)`

**简介**：执行用AsmAdd加到缓冲中的指令.

**原型**：LONGLONG AsmCall(hwnd, mode)

**参数定义**

- `hwnd` (整形数)：窗口句柄

- `mode` (整形数)：模式，取值如下0:在本进程中进行执行，这时hwnd无效.

**注**：此模式会创建线程. 1:对hwnd指定的进程内执行,注入模式为创建远程线程2：必须在对目标窗口进行注入绑定后,才可以用此模式(直接在目标进程创建线程).此模式下的call的执行是排队的,如果同时有多个call在此窗口执行,那么必须排队.所以执行效率不如模式1.同时此模式受目标窗口刷新速度的影响,目标窗口刷新太慢，也会影响此模式的速度.

**注**：此模式会创建线程. 3：同模式2,但是此模式不会创建线程,而直接在hwnd所在线程执行. 4：同模式0,但是此模式不会创建线程,直接在当前调用AsmCall的线程内执行. 5:对hwnd指定的进程内执行,注入模式为APC.此模式必须开启memory盾。任意一个memory盾都可以. 6:直接hwnd所在线程执行.

**返回值**：长整形数:获取执行汇编代码以后的EAX的值(32位进程),或者RAX的值(64位进程).一般是函数的返回值.如果要想知道函数是否执行成功，请查看GetLastError函数. -200:执行中出现错误. -201:使用模式5时，没有开启memory盾.

**示例**：dm.AsmClear dm.AsmAdd"mov eax, 1"dm.AsmAdd"push 0123456"dm.AsmAdd"call 0343434"dm.AsmCall hwnd, 1另要注意的是，AsmAdd里所有的数值都是16进制

**注**：有些时候有保护的时候，此函数执行会失败，那么此时可以尝试用memory保护盾来试试看.这里特别对64位的汇编执行做一个简单的说明.因为64位寻址的限制,那么类似下面的call可能会无法正确寻址. call 1234aabbccdd因为call绝对地址只能寻址上下2G的范围,超过以后就无法寻址.所以类似这样的语句，我们要改为下面的方式,比如mov rax, 1234aabbccdd call rax另外，由于64位调用的约定,前4个参数通过rcx rdx r8 r9来传递(浮点参数通过xmm0, xmm1, xmm2, xmm3),后面的参数通过栈来传递,同时要给call预留28h字节的栈空间.比如上面的call正确的写法如下: mov rax, 1234aabbccdd sub rsp, 28 call rax add rsp, 28也就说，所有的call前后，一定得有sub rsp, 28和add rsp, 28如果要传递超过4个参数，则按照从右往左的顺序压栈.具体以MoveWindow这个接口为例. BOOL WINAPI MoveWindow(__in HWND hWnd,__in int X,__in int Y,__in int nWidth,__in int nHeight,__in BOOL bRepaint) MoveWindow这个API是6个参数,由于多了2个参数，所以这里的sub rsp, 28也要改变，每个参数多8个字节(无论参数是不是8个字节).也就是这里变成sub rsp, 38另外要注意, push的参数必须从10h开始push.具体原因我也不知道.看这里的例子mov rcx, hWnd这里传入第一个参数hWnd mov rdx, X这里传入第二个参数X mov r8d, Y这里传入第三个参数Y mov r9d, nWidth这里传入第四个参数nWidth mov r11, rsp保存原始的rsp,方便后面传递参数sub rsp, 38 mov dword ptr[r11-10], bRepaint这里传入第六个参数.(从右往左) mov dword ptr[r11-18], nHeight这里传入第五个参数. call MoveWindow add rsp, 38完整的测试代码如下(必须是64位的顶级窗口) set dm=CreateObject("dm.dmsoft") hwnd=dm.GetMousePointWindow() user32_base=dm.GetModuleBaseAddr(hwnd,"user32.dll") MoveWindow_addr=dm.GetRemoteApiAddress(hwnd, user32_base,"MoveWindow") if dm.GetWindowState(hwnd, 9)=1 then dm.AsmClear dm.AsmAdd"mov rcx,"&hex(hwnd) dm.AsmAdd"mov rdx,"&hex(0) dm.AsmAdd"mov r8,"&hex(0) dm.AsmAdd"mov r9,"&hex(300) dm.AsmAdd"mov r11, rsp"dm.AsmAdd"sub rsp, 38"dm.AsmAdd"mov dword ptr[r11-10],"&hex(1) dm.AsmAdd"mov dword ptr[r11-18],"&hex(400) dm.AsmAdd"mov rax,"&hex(MoveWindow_addr) dm.AsmAdd"call rax"dm.AsmAdd"add rsp, 38"end if dm.AsmCall hwnd, 1过

----

### dm_asmCallEx

**签名**：`long dm_asmCallEx(intptr_t hwnd, long mode, long base_addr, long call_addr)`

**简介**：执行用AsmAdd加到缓冲中的指令.这个接口同AsmCall,但是由于插件内部在每次AsmCall时,都会有对目标进程分配内存的操作,这样会不够效率.所以增加这个接口，可以让调用者指定分配好的内存,并在此内存上执行call的操作.

**原型**：LONGLONG AsmCallEx(hwnd, mode, base_addr)

**参数定义**

- `hwnd` (整形数)：窗口句柄

- `mode` (整形数)：模式，取值如下0:在本进程中进行执行，这时hwnd无效.

**注**：此模式会创建线程. 1:对hwnd指定的进程内执行,注入模式为创建远程线程2：必须在对目标窗口进行注入绑定后,才可以用此模式(直接在目标进程创建线程).此模式下的call的执行是排队的,如果同时有多个call在此窗口执行,那么必须排队.所以执行效率不如模式1.同时此模式受目标窗口刷新速度的影响,目标窗口刷新太慢，也会影响此模式的速度.

**注**：此模式会创建线程. 3：同模式2,但是此模式不会创建线程,而直接在hwnd所在线程执行. 4：同模式0,但是此模式不会创建线程,直接在当前调用AsmCall的线程内执行. 5:对hwnd指定的进程内执行,注入模式为APC.此模式必须开启memory盾。任意一个memory盾都可以. 6:直接hwnd所在线程执行. base_addr字符串: 16进制格式.比如"45A 00000",此参数指定的地址必须要求有可读可写可执行属性.并且内存大小最少要200个字节.模式6要求至少400个字节.如果Call的内容较多,那么长度相应也要增加.如果此参数为空,那么效果就和AsmCall一样.

**返回值**：长整形数:获取执行汇编代码以后的EAX的值(32位进程),或者RAX的值(64位进程).一般是函数的返回值.如果要想知道函数是否执行成功，请查看GetLastError函数. -200:执行中出现错误. -201:使用模式5时，没有开启memory盾.

**示例**：base_addr=dm.VirtualAllocEx(hwnd, 0, 200, 0) dm.AsmClear dm.AsmAdd"mov eax, 1"dm.AsmAdd"push 0123456"dm.AsmAdd"call 0343434"dm.AsmCallEx hwnd, 1, hex(base_addr)另要注意的是，AsmAdd里所有的数值都是16进制.

**注**：有些时候有保护的时候，此函数执行会失败，那么此时可以尝试用memory保护盾来试试看.这里特别对64位的汇编执行做一个简单的说明.因为64位寻址的限制,那么类似下面的call可能会无法正确寻址. call 1234aabbccdd因为call绝对地址只能寻址上下2G的范围,超过以后就无法寻址.所以类似这样的语句，我们要改为下面的方式,比如mov rax, 1234aabbccdd call rax另外，由于64位调用的约定,前4个参数通过rcx rdx r8 r9来传递,后面的参数通过栈来传递,同时要给call预留28h字节的栈空间.比如上面的call正确的写法如下: mov rax, 1234aabbccdd sub rsp, 28 call rax add rsp, 28也就说，所有的call前后，一定得有sub rsp, 28和add rsp, 28如果要传递超过4个参数，则按照从右往左的顺序压栈.具体以MoveWindow这个接口为例. BOOL WINAPI MoveWindow(__in HWND hWnd,__in int X,__in int Y,__in int nWidth,__in int nHeight,__in BOOL bRepaint) MoveWindow这个API是6个参数,由于多了2个参数，所以这里的sub rsp, 28也要改变，每个参数多8个字节(无论参数是不是8个字节).也就是这里变成sub rsp, 38另外要注意, push的参数必须从10h开始push.具体原因我也不知道.看这里的例子mov rcx, hWnd这里传入第一个参数hWnd mov rdx, X这里传入第二个参数X mov r8d, Y这里传入第三个参数Y mov r9d, nWidth这里传入第四个参数nWidth mov r11, rsp保存原始的rsp,方便后面传递参数sub rsp, 38 mov dword ptr[r11-10], bRepaint这里传入第六个参数.(从右往左) mov dword ptr[r11-18], nHeight这里传入第五个参数. call MoveWindow add rsp, 38完整的测试代码如下(必须是64位的顶级窗口) set dm=CreateObject("dm.dmsoft") hwnd=dm.GetMousePointWindow() user32_base=dm.GetModuleBaseAddr(hwnd,"user32.dll") MoveWindow_addr=dm.GetRemoteApiAddress(hwnd, user32_base,"MoveWindow") if dm.GetWindowState(hwnd, 9)=1 then dm.AsmClear dm.AsmAdd"mov rcx,"&hex(hwnd) dm.AsmAdd"mov rdx,"&hex(0) dm.AsmAdd"mov r8,"&hex(0) dm.AsmAdd"mov r9,"&hex(300) dm.AsmAdd"mov r11, rsp"dm.AsmAdd"sub rsp, 38"dm.AsmAdd"mov dword ptr[r11-10],"&hex(1) dm.AsmAdd"mov dword ptr[r11-18],"&hex(400) dm.AsmAdd"mov rax,"&hex(MoveWindow_addr) dm.AsmAdd"call rax"dm.AsmAdd"add rsp, 38"end if dm.AsmCall hwnd, 1

----

### dm_asmClear

**签名**：`long dm_asmClear()`

**简介**：清除汇编指令缓冲区用AsmAdd添加到缓冲的指令全部清除

**原型**：long AsmClear ()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.AsmClear

----

### dm_asmSetTimeout

**签名**：`long dm_asmSetTimeout(long timeout)`

**简介**：此接口对AsmCall和AsmCallEx中的模式5和6中内置的一些延时参数进行设置.

**原型**：long AsmSetTimeout (time_out, param)

**参数定义**

- `time_out` (整形数)：具体含义看以下说明.(默认值10000)单位毫秒

- `param` (整形数)：具体含义看以下说明. (默认值100)单位毫秒

**返回值**：整形数: 0:失败1:成功

**示例**：dm.AsmSetTimeout 10000, 100

**注**：time_out同时影响模式5和6.单位是毫秒。表示执行此AsmCall时，最长的等待时间.超过此时间后，强制结束.如果是-1，表示无限等待.比如，当执行某个寻路call时,需要到寻路结束，call才会返回.那么就需要把此参数设置大一些，甚至设置为-1. param仅影响模式6.这个值越大,越不容易引起目标进程崩溃，同时call的执行速度相对慢一些.越小越容易崩溃,同时call的执行速度快一些.可根据自己情况设置.一般默认的就可以了.

----

### dm_assemble

**签名**：`const char* dm_assemble(long base_addr, long is_asm_code)`

**简介**：把汇编缓冲区的指令转换为机器码并用16进制字符串的形式输出

**原型**：string Assemble(base_addr, is_64bit)

**参数定义**

- `is_64bit` (整形数)：表示缓冲区的指令是32位还是64位. 32位表示为0, 64位表示为1

**返回值**：字符串:机器码，比如"aa bb cc"这样的形式

**示例**：code=dm.Assemble(&H405940, 1) MessageBox code

----

### dm_disAssemble

**签名**：`const char* dm_disAssemble(long asm_code, long base_addr, long is_asm_code)`

**简介**：把指定的机器码转换为汇编语言输出

**原型**：string DisAssemble (asm_code, base_addr, is_64bit)

**参数定义**

- `asm_code` (字符串)：机器码，形式如"aa bb cc"这样的16进制表示的字符串(空格无所谓) base_addr长整形数:指令所在的地址

- `is_64bit` (整形数)：表示asm_code表示的指令是32位还是64位. 32位表示为0, 64位表示为1

**返回值**：字符串: MASM汇编语言字符串.如果有多条指令，则每条指令以字符"|"连接.

**示例**：dm_ret=dm.DisAssemble ("81 05 E0 5A 47 00 01 00 00 00",&H435fde, 0) MessageBox dm_ret

----

### dm_setAsmHwndAsProcessId

**签名**：`long dm_setAsmHwndAsProcessId(long enable)`

**简介**：使用AsmCall时的hwnd参数当作进程pid.

**注**：仅对AsmCall的模式1起作用,因为其它模式都需要窗口.

**原型**：long SetAsmHwndAsProcessId(enable)

**参数定义**

- `enable` (整形数)：0关闭, 1打开

**返回值**：整形数: 0:失败1:成功

**示例**：dm.SetAsmHwndAsProcessId 1 dm.AsmCall pid, 1

----

### dm_setShowAsmErrorMsg

**签名**：`long dm_setShowAsmErrorMsg(long show)`

**简介**：设置是否弹出汇编功能中的错误提示,默认是打开.

**原型**：long SetShowAsmErrorMsg (show)

**参数定义**

- `show` (整形数)：0表示不打开, 1表示打开

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.SetShowAsmErrorMsg (0)

----

### dm_aiEnableFindPicWindow

**签名**：`long dm_aiEnableFindPicWindow(long enable)`

**简介**：设置是否在调用AiFindPicXX系列接口时,是否弹出找图结果的窗口.方便调试.默认是关闭的.

**原型**：long AiEnableFindPicWindow(enable)

**参数定义**

- `enable` (整形数)：0关闭1开启

**返回值**：整形数: 0:失败1:成功

**示例**：set dm=CreateObject("dm.dmsoft") TracePrint dm.Ver() dm.AiEnableFindPicWindow 1 ai_path="D:\ai.module"dm_ret=dm.LoadAi(ai_path) TracePrint dm_ret dm.SetPath dm.GetBasePath() dm_ret=dm.FreePic("souce.bmp") dm_ret=dm.SetDisplayInput("pic: souce.bmp") dm_ret=dm.AiFindPic(0, 0, 2000, 2000,"test.bmp", 0.8, 0, x, y) TracePrint x&","&y dm_ret=dm.AiFindPicEx(0, 0, 2000, 2000,"test.bmp", 0.8, 0) TracePrint dm_ret dm_ret=dm.SetDisplayInput("screen")这是一个从图片中找图片的例子.

----

### dm_aiFindPic

**签名**：`long dm_aiFindPic(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir, long* intX, long* intY)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个找到的X Y坐标.此接口使用Ai模块来实现,比传统的FindPic的效果更好.不需要训练

**原型**：long AiFindPic(x1, y1, x2, y2, pic_name, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

- `intX` (变参指针)：返回图片左上角的X坐标

- `intY` (变参指针)：返回图片左上角的Y坐标

**返回值**：整形数:返回找到的图片的序号,从0开始索引.如果没找到返回-1

**示例**：dm_ret=dm.AiFindPic(0, 0, 2000, 2000,"1.bmp|2.bmp|3.bmp", 0.9, 0, intX, intY) If intX>=0 and intY>=0 Then MessageBox"找到"End If此接口需要ai.module 4.0及其之后的版本.

----

### dm_aiFindPicEx

**签名**：`const char* dm_aiFindPicEx(long x1, long y1, long x2, long y2, const char* pic_name, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,并且返回所有找到的图像的坐标.此接口使用Ai模块来实现,比传统的FindPicEx的效果更好.不需要训练

**原型**：string AiFindPicEx(x1, y1, x2, y2, pic_name, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_name` (字符串)：图片名,可以是多个图片,比如"test.bmp|test2.bmp|test3.bmp"

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回的是所有找到的坐标格式如下:"id, x, y|id, x, y..|id, x, y"(图片左上角的坐标)比如"0, 100, 20|2, 30, 40"表示找到了两个,第一个,对应的图片是图像序号为0的图片,坐标是(100, 20),第二个是序号为2的图片,坐标(30, 40) (由于内存限制,返回的图片数量最多为1500个左右)

**示例**：dm_ret=dm.AiFindPicEx(0, 0, 2000, 2000,"test.bmp|test2.bmp|test3.bmp|test4.bmp|test5.bmp", 1.0, 0) If len(dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound(ss)+1 Do While index<count TracePrint ss(index) sss=split(ss(index),",") id=int(sss(0)) x=int(sss(1)) y=int(sss(2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If此接口需要ai.module 4.0及其之后的版本.

----

### dm_aiFindPicMem

**签名**：`long dm_aiFindPicMem(long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir, long* intX, long* intY)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,只返回第一个找到的X Y坐标.这个函数要求图片是数据地址.此接口使用Ai模块来实现,比传统的FindPicMem的效果更好.不需要训练

**原型**：long AiFindPicMem(x1, y1, x2, y2, pic_info, sim, dir, intX, intY)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_info` (字符串)：图片数据地址集合.格式为"地址1,长度1|地址2,长度2.....|地址n,长度n".可以用AppendPicAddr来组合.地址表示24位位图资源在内存中的首地址，用十进制的数值表示长度表示位图资源在内存中的长度，用十进制数值表示.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

- `intX` (变参指针)：返回图片左上角的X坐标

- `intY` (变参指针)：返回图片左上角的Y坐标

**返回值**：整形数:返回找到的图片的序号,从0开始索引.如果没找到返回-1

**示例**：pic_info=""pic_info=dm.AppendPicAddr(pic_info, 12034, 643) pic_info=dm.AppendPicAddr(pic_info, 328435, 8935) pic_info=dm.AppendPicAddr(pic_info, 809234, 789) dm_ret=dm.AiFindPicMem(0, 0, 2000, 2000, pic_info, 0.9, 0, intX, intY) If intX>=0 and intY>=0 Then MessageBox"找到"End If

**注**：内存中的图片格式必须是24位色，并且不能加密.此接口需要ai.module 4.0及其之后的版本.

----

### dm_aiFindPicMemEx

**签名**：`const char* dm_aiFindPicMemEx(long x1, long y1, long x2, long y2, const char* pic_info, long sim, long dir)`

**简介**：查找指定区域内的图片,位图必须是24位色格式,支持透明色,当图像上下左右4个顶点的颜色一样时,则这个颜色将作为透明色处理.这个函数可以查找多个图片,并且返回所有找到的图像的坐标.这个函数要求图片是数据地址.此接口使用Ai模块来实现,比传统的FindPicMemEx的效果更好.不需要训练

**原型**：string AiFindPicMemEx(x1, y1, x2, y2, pic_info, sim, dir)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `pic_info` (字符串)：图片数据地址集合.格式为"地址1,长度1|地址2,长度2.....|地址n,长度n".可以用AppendPicAddr来组合.地址表示24位位图资源在内存中的首地址，用十进制的数值表示长度表示位图资源在内存中的长度，用十进制数值表示.

- `sim` (双精度浮点数)：相似度,取值范围0.1-1.0

- `dir` (整形数)：查找方向0:从左到右,从上到下1:从左到右,从下到上2:从右到左,从上到下3:从右到左,从下到上

**返回值**：字符串:返回的是所有找到的坐标格式如下:"id, x, y|id, x, y..|id, x, y"(图片左上角的坐标)比如"0, 100, 20|2, 30, 40"表示找到了两个,第一个,对应的图片是图像序号为0的图片,坐标是(100, 20),第二个是序号为2的图片,坐标(30, 40) (由于内存限制,返回的图片数量最多为1500个左右)

**示例**：pic_info=""pic_info=dm.AppendPicAddr(pic_info, 12034, 643) pic_info=dm.AppendPicAddr(pic_info, 328435, 8935) pic_info=dm.AppendPicAddr(pic_info, 809234, 789) dm_ret=dm.AiFindPicMemEx(0, 0, 2000, 2000, pic_info, 1.0, 0) If len(dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound(ss)+1 Do While index<count TracePrint ss(index) sss=split(ss(index),",") id=int(sss(0)) x=int(sss(1)) y=int(sss(2)) dm.MoveTo x, y Delay 1000 index=index+1 Loop End If

**注**：内存中的图片格式必须是24位色，并且不能加密.此接口需要ai.module 4.0及其之后的版本.

----

### dm_aiYoloDetectObjects

**签名**：`const char* dm_aiYoloDetectObjects(long x1, long y1, long x2, long y2, float prob, float iou)`

**简介**：需要先加载Ai模块.在指定范围内检测对象.

**原型**：string AiYoloDetectObjects(x1, y1, x2, y2, prob, iou)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `prob` (双精度浮点数)：置信度,也可以认为是相似度.超过这个prob的对象才会被检测

- `iou` (双精度浮点数)：用于对多个检测框进行合并.越大越不容易合并(很多框重叠).越小越容易合并(可能会把正常的框也给合并).所以这个值一般建议0.4-0.6之间.可以在Yolo综合工具里进行测试.

**返回值**：字符串:返回的是所有检测到的对象.格式是"类名,置信度, x, y, w, h|....".如果没检测到任何对象,返回空字符串.

**示例**：dm.AiYoloUseModel 0 objects=dm.AiYoloDetectObjects(0, 0, 2000, 2000, 0.5, 0.45) if len(objects)>0 then ss=split(objects,"|") index=0 count=UBound(ss)+1 Do While index<count TracePrint ss(index) sss=split(ss(index),",") class_info=int(sss(0)) prob_info=Csng(sss(1)) x=int(sss(2)) y=int(sss(3)) w=int(sss(4)) h=int(sss(5)) index=index+1 Loop end if

**注**：模块内部是全局的,所以调用此接口时得确保没有其它接口去访问此模型.如果多个线程里, UseModel的序号是相同的,那么如果同时执行此接口时,会排队执行.

----

### dm_aiYoloDetectObjectsToDataBmp

**签名**：`long dm_aiYoloDetectObjectsToDataBmp(long x1, long y1, long x2, long y2, float prob, float iou, long data, long size, long mode)`

**简介**：需要先加载Ai模块.在指定范围内检测对象,把结果输出到BMP图像数据.用于二次开发.

**原型**：long AiYoloDetectObjectsToDataBmp(x1, y1, x2, y2, prob, iou, data, size, mode)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `prob` (双精度浮点数)：置信度,也可以认为是相似度.超过这个prob的对象才会被检测

- `iou` (双精度浮点数)：用于对多个检测框进行合并.越大越不容易合并(很多框重叠).越小越容易合并(可能会把正常的框也给合并).所以这个值一般建议0.4-0.6之间.可以在Yolo综合工具里进行测试.

- `data` (变参指针)：返回图片的数据指针

- `size` (变参指针)：返回图片的数据长度

- `mode` (整形数)：0表示绘制的文字信息里包含置信度. 1表示不包含.

**返回值**：整形数: 0:失败1:成功

**示例**：以下是在E语言中的示例. .局部变量data,整数型.局部变量size,整数型dm.数值方法(“AiYoloDetectObjectsToDataBmp”, 0, 0, 100, 200, 0.5, 0.45, data, size, 0)图片框1.图片＝指针到字节集(data, size)

**注**：模块内部是全局的,所以调用此接口时得确保没有其它接口去访问此模型.如果多个线程里, UseModel的序号是相同的,那么如果同时执行此接口时,会排队执行.

----

### dm_aiYoloDetectObjectsToFile

**签名**：`long dm_aiYoloDetectObjectsToFile(long x1, long y1, long x2, long y2, float prob, float iou, const char* file, long mode)`

**简介**：需要先加载Ai模块.在指定范围内检测对象,把结果输出到指定的BMP文件.

**原型**：long AiYoloDetectObjectsToFile(x1, y1, x2, y2, prob, iou, file, mode)

**参数定义**

- `x1` (整形数)：区域的左上X坐标

- `y1` (整形数)：区域的左上Y坐标

- `x2` (整形数)：区域的右下X坐标

- `y2` (整形数)：区域的右下Y坐标

- `prob` (双精度浮点数)：置信度,也可以认为是相似度.超过这个prob的对象才会被检测

- `iou` (双精度浮点数)：用于对多个检测框进行合并.越大越不容易合并(很多框重叠).越小越容易合并(可能会把正常的框也给合并).所以这个值一般建议0.4-0.6之间.可以在Yolo综合工具里进行测试.

- `file` (字符串)：图片名,比如"test.bmp"

- `mode` (整形数)：0表示绘制的文字信息里包含置信度. 1表示不包含.

**返回值**：整形数: 0:失败1:成功

**示例**：dm.AiYoloUseModel 0 dm_ret=dm.AiYoloDetectObjectsToFile(0, 0, 2000, 2000, 0.5, 0.45,"test.bmp", 0)

**注**：模块内部是全局的,所以调用此接口时得确保没有其它接口去访问此模型.如果多个线程里, UseModel的序号是相同的,那么如果同时执行此接口时,会排队执行.

----

### dm_aiYoloFreeModel

**签名**：`long dm_aiYoloFreeModel()`

**简介**：需要先加载Ai模块.卸载指定的模型

**原型**：long AiYoloFreeModel(index)

**参数定义**

- `index` (整形数)：模型的序号.最多支持20个.从0开始

**返回值**：整形数: 1表示成功0失败

**示例**：dm.AiYoloFreeModel 0 dm.AiYoloFreeModel 1

**注**：模型内部是全局的,所以调用此接口时得确保没有其它接口去访问此模型.

----

### dm_aiYoloObjectsToString

**签名**：`const char* dm_aiYoloObjectsToString(const char* objects)`

**简介**：需要先加载Ai模块.把通过AiYoloDetectObjects或者是AiYoloSortsObjects的结果,按照顺序把class信息连接输出.

**原型**：string AiYoloObjectsToString(objects)

**参数定义**

- `objects` (字符串)：AiYoloDetectObjects或者AiYoloSortsObjects的返回值.

**返回值**：字符串:返回的是class信息连接后的信息.

**示例**：dm.AiYoloUseModel 0 objects=dm.AiYoloDetectObjects(0, 0, 2000, 2000, 0.5, 0.45) sorted_objects=dm.AiYoloSortsObjects(objects) TracePrint dm.AiYoloObjectsToString(sorted_objects)

----

### dm_aiYoloSetModel

**签名**：`long dm_aiYoloSetModel(const char* model_file, const char* model_type)`

**简介**：需要先加载Ai模块.从文件加载指定的模型.

**原型**：long AiYoloSetModel(index, file, pwd)

**参数定义**

- `index` (整形数)：模型的序号.最多支持20个.从0开始

- `file` (字符串)：模型文件名.比如"xxxx.onnx"或者"xxxx. dmx"

- `pwd` (字符串)：模型的密码.仅对dmx格式有效.

**返回值**：整形数: 1表示成功0失败

**示例**：dm.AiYoloSetModel 0,"xxxx.onnx",""dm.AiYoloSetModel 1,"xxxx.dmx","123"

**注**：模块内部是全局的,所以调用此接口时得确保没有其它接口去访问此模型.另外,加载onnx时得确保和这个onnx同名的class文件也在同目录下.比如加载xxxx.onnx,那么必须得有个相应的xxxx. class .

----

### dm_aiYoloSetModelMemory

**签名**：`long dm_aiYoloSetModelMemory(long data, long size, const char* model_type)`

**简介**：需要先加载Ai模块.从内存加载指定的模型.仅支持dmx格式的内存

**原型**：long AiYoloSetModelMemory(index, data, size, pwd)

**参数定义**

- `index` (整形数)：模型的序号.最多支持20个.从0开始

- `data` (整形数)：dmx模型的内存地址

- `size` (整形数)：dmx模型的大小

- `pwd` (字符串)：dmx模型的密码

**返回值**：整形数: 1表示成功0失败

**示例**：dm.AiYoloSetModelMemory 0, 2343253, 23432432,"123"

**注**：模块内部是全局的,所以调用此接口时得确保没有其它接口去访问此模型.

----

### dm_aiYoloSetVersion

**签名**：`long dm_aiYoloSetVersion(const char* version)`

**简介**：需要先加载Ai模块.设置Yolo的版本

**原型**：long AiYoloSetVersion(ver)

**参数定义**

- `ver` (字符串)：Yolo的版本信息.需要在加载Ai模块后,第一时间调用.目前可选的值只有"v5-7.0"

**返回值**：整形数: 1表示成功0失败

**示例**：dm.AiYoloSetVersion"v5-7.0"

----

### dm_aiYoloSortsObjects

**签名**：`const char* dm_aiYoloSortsObjects(const char* objects, long sort_type)`

**简介**：需要先加载Ai模块.把通过AiYoloDetectObjects的结果进行排序.排序按照从上到下,从左到右.

**原型**：string AiYoloSortsObjects(objects, height)

**参数定义**

- `objects` (字符串)：AiYoloDetectObjects的返回值

- `height` (整形数)：行高信息.排序时需要使用此行高.用于确定两个检测框是否处于同一行.如果两个框的Y坐标相差绝对值小于此行高,认为是同一行.

**返回值**：字符串:返回的是所有检测到的对象.格式是"类名,置信度, x, y, w, h|....".如果没检测到任何对象,返回空字符串.

**示例**：dm.AiYoloUseModel 0 objects=dm.AiYoloDetectObjects(0, 0, 2000, 2000, 0.5, 0.45) sorted_objects=dm.AiYoloSortsObjects(objects)

----

### dm_aiYoloUseModel

**签名**：`long dm_aiYoloUseModel(long index)`

**简介**：需要先加载Ai模块.切换当前使用的模型序号.用于AiYoloDetectXX等系列接口.

**原型**：long AiYoloUseModel(index)

**参数定义**

- `index` (整形数)：模型的序号.最多支持20个.从0开始

**返回值**：整形数: 1表示成功0失败

**示例**：dm.AiYoloUseModel 0

----

### dm_loadAi

**签名**：`long dm_loadAi(const char* file)`

**简介**：加载Ai模块. Ai模块从后台下载.模块加载仅支持所有的正式版本。具体可以看DmGuard里系统版本的说明.

**原型**：long LoadAi(file)

**参数定义**

- `file` (字符串)：ai模块的路径.比如绝对路径c:\ai.module或者相对路径ai.module等.

**返回值**：整形数: 1表示成功-1打开文件失败-2内存初始化失败.如果是正式版本,出现这个错误可以联系我解决. -3参数错误-4加载错误-5 Ai模块初始化失败-6内存分配失败

**示例**：dm.SetPath dm.GetBasePath() dm_ret=dm.LoadAi("ai.module") TracePrint dm_ret

----

### dm_loadAiMemory

**签名**：`long dm_loadAiMemory(long data, long size)`

**简介**：从内存加载Ai模块. Ai模块从后台下载.模块加载仅支持所有的正式版本。具体可以看DmGuard里系统版本的说明.

**原型**：long LoadAiMemory(data, size)

**参数定义**

- `data` (整形数)：ai模块在内存中的地址

- `size` (整形数)：ai模块在内存中的大小

**返回值**：整形数: 1表示成功-1打开文件失败-2内存初始化失败.如果是正式版本,出现这个错误可以联系我解决. -3参数错误-4加载错误-5 Ai模块初始化失败-6内存分配失败

**示例**：//先获取ai.module的内存地址dm_ret=dm.LoadAiMemory(234735, 32948) TracePrint dm_ret

----

### dm_createFoobarCustom

**签名**：`long dm_createFoobarCustom(intptr_t hwnd, long x, long y, long w, long h, const char* pic)`

**简介**：根据指定的位图创建一个自定义形状的窗口

**原型**：long CreateFoobarCustom (hwnd, x, y, pic_name, trans_color, sim)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄,如果此值为0,那么就在桌面创建此窗口

- `x` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `pic_name` (字符串)：位图名字.如果第一个字符是@,则采用指针方式.@后面是指针地址和大小.必须是十进制.具体看下面的例子

- `trans_color` (字符串)：透明色(RRGGBB)

- `sim` (双精度浮点数)：透明色的相似值0.1-1.0

**返回值**：整形数:创建成功的窗口句柄

**示例**：foobar=dm . CreateFoobarCustom (hwnd, 10, 10,"菜单.bmp","FF00FF", 1.0) foobar=dm . CreateFoobarCustom (hwnd, 10, 10,"@9237392578, 2345","FF00FF", 1.0)

**注**：foobar不能在本进程窗口内创建.

----

### dm_createFoobarEllipse

**签名**：`long dm_createFoobarEllipse(intptr_t hwnd, long x, long y, long w, long h)`

**简介**：创建一个椭圆窗口

**原型**：long CreateFoobarEllipse (hwnd, x, y, w, h)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄,如果此值为0,那么就在桌面创建此窗口

- `x` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `w` (整形数)：矩形区域的宽度

- `h` (整形数)：矩形区域的高度

**返回值**：整形数:创建成功的窗口句柄

**示例**：foobar=dm . CreateFoobarEllipse (hwnd, 10, 10, 200, 200)

**注**：foobar不能在本进程窗口内创建.

----

### dm_createFoobarRect

**签名**：`long dm_createFoobarRect(intptr_t hwnd, long x, long y, long w, long h)`

**简介**：创建一个矩形窗口

**原型**：long CreateFoobarRect (hwnd, x, y, w, h)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄,如果此值为0,那么就在桌面创建此窗口

- `x` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `w` (整形数)：矩形区域的宽度

- `h` (整形数)：矩形区域的高度

**返回值**：整形数:创建成功的窗口句柄

**示例**：foobar=dm.CreateFoobarRect (hwnd, 10, 10, 200, 200)

**注**：foobar不能在本进程窗口内创建.

----

### dm_createFoobarRoundRect

**签名**：`long dm_createFoobarRoundRect(intptr_t hwnd, long x, long y, long w, long h, long rw, long rh)`

**简介**：创建一个圆角矩形窗口

**原型**：long CreateFoobarRoundRect (hwnd, x, y, w, h, rw, rh)

**参数定义**

- `hwnd` (整形数)：指定的窗口句柄,如果此值为0,那么就在桌面创建此窗口

- `x` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `w` (整形数)：矩形区域的宽度

- `h` (整形数)：矩形区域的高度

- `rw` (整形数)：圆角的宽度

- `rh` (整形数)：圆角的高度

**返回值**：整形数:创建成功的窗口句柄

**示例**：foobar=dm.CreateFoobarRoundRect (hwnd, 10, 10, 200, 200, 30, 30)

**注**：foobar不能在本进程窗口内创建.

----

### dm_foobarClearText

**签名**：`long dm_foobarClearText(intptr_t hwnd)`

**简介**：清除指定的Foobar滚动文本区

**原型**：long FoobarClearText (hwnd)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarClearText (foobar)

----

### dm_foobarClose

**签名**：`long dm_foobarClose(intptr_t hwnd)`

**简介**：关闭一个Foobar,注意,必须调用此函数来关闭窗口,用SetWindowState也可以关闭,但会造成内存泄漏.

**原型**：long FoobarClose (hwnd)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarClose (foobar)

----

### dm_foobarDrawLine

**签名**：`long dm_foobarDrawLine(intptr_t hwnd, long x1, long y1, long x2, long y2, long color, long style)`

**简介**：在指定的Foobar窗口内部画线条.

**原型**：long FoobarDrawLine (hwnd, x1, y1, x2, y2, color, style, width)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口,注意,此句柄必须是通过CreateFoobarxxxx系列函数创建出来的

- `x1` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y1` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `x2` (整形数)：右下角X坐标(相对于hwnd客户区坐标)

- `y2` (整形数)：右下角Y坐标(相对于hwnd客户区坐标)

- `color` (字符串)：填充的颜色值

- `style` (整形数)：画笔类型. 0为实线. 1为虚线

- `width` (整形数)：线条宽度.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarDrawLine (foobar, 0, 0, 200, 200,"FF0000", 1, 1)

**注**：当style为1时，线条宽度必须也是1.否则线条是实线.

----

### dm_foobarDrawPic

**签名**：`long dm_foobarDrawPic(intptr_t hwnd, long x, long y, const char* pic)`

**简介**：在指定的Foobar窗口绘制图像

**原型**：long FoobarDrawPic (hwnd, x, y, pic_name, trans_color)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口,注意,此句柄必须是通过CreateFoobarxxxx系列函数创建出来的

- `x` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `pic_name` (字符串)：图像文件名如果第一个字符是@,则采用指针方式.@后面是指针地址和大小.必须是十进制.具体看下面的例子

- `trans_color` (字符串)：图像透明色

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarDrawPic (foobar, 0, 0,"menu.bmp","FF0000") dm_ret=dm.FoobarDrawPic (foobar, 0, 0,"@32432525, 23435","FF0000")

----

### dm_foobarDrawText

**签名**：`long dm_foobarDrawText(intptr_t hwnd, long x, long y, long w, long h, const char* text, long color, long align)`

**简介**：在指定的Foobar窗口绘制文字

**原型**：long FoobarDrawText(hwnd, x, y, w, h, text, color, align)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口,注意,此句柄必须是通过CreateFoobarxxxx系列函数创建出来的

- `x` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `w` (整形数)：矩形区域的宽度

- `h` (整形数)：矩形区域的高度

- `text` (字符串)：字符串

- `color` (字符串)：文字颜色值

- `align` (整形数)：取值定义如下1:左对齐2:中间对齐4:右对齐

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarDrawText(foobar, 0, 0, 200, 30,"测试","FF0000", 1)

----

### dm_foobarFillRect

**签名**：`long dm_foobarFillRect(intptr_t hwnd, long x1, long y1, long x2, long y2, long color)`

**简介**：在指定的Foobar窗口内部填充矩形

**原型**：long FoobarFillRect(hwnd, x1, y1, x2, y2, color)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口,注意,此句柄必须是通过CreateFoobarxxxx系列函数创建出来的

- `x1` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y1` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `x2` (整形数)：右下角X坐标(相对于hwnd客户区坐标)

- `y2` (整形数)：右下角Y坐标(相对于hwnd客户区坐标)

- `color` (字符串)：填充的颜色值

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarFillRect(foobar, 0, 0, 200, 200,"FF0000")

----

### dm_foobarLock

**签名**：`long dm_foobarLock(intptr_t hwnd)`

**简介**：锁定指定的Foobar窗口,不能通过鼠标来移动

**原型**：long FoobarLock(hwnd)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarLock(foobar)

----

### dm_foobarPrintText

**签名**：`long dm_foobarPrintText(intptr_t hwnd, const char* text, long color)`

**简介**：向指定的Foobar窗口区域内输出滚动文字

**原型**：long FoobarPrintText(hwnd, text, color)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

- `text` (字符串)：文本内容

- `color` (字符串)：文本颜色

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarPrintText(foobar,"大漠测试","ff0000")//用红色文字向滚动区域输出文字信息

----

### dm_foobarSetFont

**签名**：`long dm_foobarSetFont(intptr_t hwnd, const char* font_name, long size, long flag)`

**简介**：设置指定Foobar窗口的字体

**原型**：long FoobarSetFont(hwnd, font_name, size, flag)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

- `font_name` (字符串)：系统字体名,注意,必须保证系统中有此字体

- `size` (整形数)：字体大小

- `flag` (整形数)：取值定义如下0:正常字体1:粗体2:斜体4:下划线文字可以是以上的组合比如粗斜体就是1+2,斜体带下划线就是: 2+4等.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarSetFont(foobar,"宋体", 25, 2+4)

----

### dm_foobarSetSave

**签名**：`long dm_foobarSetSave(intptr_t hwnd, const char* file, long enable)`

**简介**：设置保存指定的Foobar滚动文本区信息到文件.

**原型**：long FoobarSetSave(hwnd, file, enable, header)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

- `file` (字符串)：保存的文件名

- `enable` (整形数)：取值如下0:关闭向文件输出(默认是0) 1:开启向文件输出

- `header` (字符串)：输出的附加头信息. (比如行数日期时间信息)格式是如下格式串的顺序组合.如果为空串，表示无附加头."%L0nd%"表示附加头信息带有行号，并且是按照十进制输出. n表示按多少个十进制数字补0对齐.比如"%L04d%",输出的行号为0001 0002 0003等."%L03d",输出的行号为001 002 003..等."%L0nx%"表示附加头信息带有行号，并且是按照16进制小写输出. n表示按多少个16进制数字补0对齐.比如"%L04x%",输出的行号为0009 000a 000b等."%L03x",输出的行号为009 00a 00b..等."%L0nX%"表示附加头信息带有行号，并且是按照16进制大写输出. n表示按多少个16进制数字补0对齐.比如"%L04X%",输出的行号为0009 000A 000B等."%L03X",输出的行号为009 00A 00B..等."%yyyy%"表示年.比如2012"%MM%"表示月.比如12"%dd%"表示日.比如28"%hh%"表示小时.比如13"%mm%"表示分钟.比如59"%ss%"表示秒.比如48.

**返回值**：整形数: 0:失败1:成功

**示例**：//开启输出，并且按原始信息输出.无附加头. dm.FoobarSetSave foobar,"output.txt", 1,""//开启输出，并且按行号信息输出. dm.FoobarSetSave foobar,"output.txt", 1,"【%L04d%】"//这个输出的信息类似如下【0001】…【0002】…【0003】…//开启输出，并且按年月日的方式输出. dm.FoobarSetSave foobar,"output.txt", 1,"{%yyyy%-%MM%-%dd%}"//这个输出的信息类似如下{2008-01-01}…{2008-01-01}…{2008-01-01}…{2008-01-01}…//开启输出，并且按时分秒的方式输出. dm.FoobarSetSave foobar,"output.txt", 1,"{%hh%-%mm%-%ss%}"//这个输出的信息类似如下{13-04-45}…{13-04-48}…{13-04-50}…//开启输出，并且按照行号时分的方式输出dm.FoobarSetSave foobar,"output.txt", 1,"[%L03d%](%hh%-%mm%)"//这个输出的信息类似如下[001](23-16)…[002](23-17)…[003](23-20)…

----

### dm_foobarSetTrans

**签名**：`long dm_foobarSetTrans(intptr_t hwnd, long trans, long color)`

**简介**：设置指定Foobar窗口的是否透明

**原型**：long FoobarSetTrans (hwnd, is_trans, color, sim)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

- `is_trans` (整形数)：是否透明. 0为不透明(此时, color和sim无效)，1为透明.

- `color` (字符串)：透明色(RRGGBB)

- `sim` (双精度浮点数)：透明色的相似值0.1-1.0

**返回值**：整形数: 0:失败1:成功

**示例**：foobar=dm.CreateFoobarRoundRect (hwnd, 1, 1, 300, 300, 100, 100) dm_ret=dm.FoobarSetFont (foobar,"宋体", 50, 0) dm.FoobarSetTrans foobar, 1,"000000", 1.0 do dm_ret=dm.FoobarFillRect (foobar, 0, 0, 300, 300,"000000") dm_ret=dm.FoobarDrawText (foobar, 0, 0, 300, 100,"测试","FF0000", 1) dm.foobarupdate foobar delay 100 Loop EndScript

**注**：调用此接口，最好打开windows的dwm .否则可能会卡.

----

### dm_foobarStartGif

**签名**：`long dm_foobarStartGif(intptr_t hwnd, const char* pic, long x, long y)`

**简介**：在指定的Foobar窗口绘制gif动画.

**原型**：long FoobarStartGif (hwnd, x, y, pic_name, repeat_limit, delay)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口,注意,此句柄必须是通过CreateFoobarxxxx系列函数创建出来的

- `x` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `pic_name` (字符串)：图像文件名如果第一个字符是@,则采用指针方式.@后面是指针地址和大小.必须是十进制.具体看下面的例子

- `repeat_limit` (整形数)：表示重复GIF动画的次数，如果是0表示一直循环显示.大于0，则表示循环指定的次数以后就停止显示.

- `delay` (整形数)：表示每帧GIF动画之间的时间间隔.如果是0，表示使用GIF内置的时间，如果大于0，表示使用自定义的时间间隔.

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarStartGif (foobar, 0, 0,"警报.gif", 0, 0) dm_ret=dm.FoobarStartGif (foobar, 0, 0,"@23432525, 2345", 0, 0)

**注**：当foobar关闭时，所有播放的gif也会自动关闭，内部资源也会自动释放，没必要一定去调用FoobarStopGif函数.另外，所有gif动画是在顶层显示，在默认绘图层和Print层之上. gif之间的显示顺序按照调用FoobarStartGif的顺序决定.

----

### dm_foobarStopGif

**签名**：`long dm_foobarStopGif(intptr_t hwnd)`

**简介**：停止在指定foobar里显示的gif动画.

**原型**：long FoobarStopGif (hwnd, x, y, pic_name)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口,注意,此句柄必须是通过CreateFoobarxxxx系列函数创建出来的

- `x` (整形数)：左上角X坐标(相对于hwnd客户区坐标)

- `y` (整形数)：左上角Y坐标(相对于hwnd客户区坐标)

- `pic_name` (字符串)：图像文件名

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarStopGif (foobar, 0, 0,"警报.gif")

**注**：当foobar关闭时，所有播放的gif也会自动关闭，内部资源也会自动释放，没必要一定去调用FoobarStopGif函数.另外，对于在不同的坐标显示的gif动画，插件内部会认为是不同的GIF.所以停止GIF时，一定要和FoobarStartGif时指定的x, y坐标一致.

----

### dm_foobarTextLineGap

**签名**：`long dm_foobarTextLineGap(intptr_t hwnd, long line_gap)`

**简介**：设置滚动文本区的文字行间距,默认是3

**原型**：long FoobarTextLineGap(hwnd, line_gap)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

- `line_gap` (整形数)：文本行间距

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarTextLineGap(foobar, 5)

----

### dm_foobarTextPrintDir

**签名**：`long dm_foobarTextPrintDir(intptr_t hwnd, long dir)`

**简介**：设置滚动文本区的文字输出方向,默认是0

**原型**：long FoobarTextPrintDir (hwnd, dir)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

- `dir` (整形数)：0表示向下输出: 1表示向上输出

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarTextPrintDir (foobar, 1)

----

### dm_foobarTextRect

**签名**：`long dm_foobarTextRect(intptr_t hwnd, long x, long y, long w, long h)`

**简介**：设置指定Foobar窗口的滚动文本框范围,默认的文本框范围是窗口区域

**原型**：long FoobarTextRect(hwnd, x, y, w, h)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

- `x` (整形数)：x坐标

- `y` (整形数)：y坐标

- `w` (整形数)：宽度

- `h` (整形数)：高度

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarTextRect(foobar, 10, 10, 100, 200)

----

### dm_foobarUnlock

**签名**：`long dm_foobarUnlock(intptr_t hwnd)`

**简介**：解锁指定的Foobar窗口,可以通过鼠标来移动

**原型**：long FoobarUnlock(hwnd)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口句柄,此句柄必须是通过CreateFoobarxxx创建而来

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarUnlock(foobar)

----

### dm_foobarUpdate

**签名**：`long dm_foobarUpdate(intptr_t hwnd)`

**简介**：刷新指定的Foobar窗口

**原型**：long FoobarUpdate(hwnd)

**参数定义**

- `hwnd` (整形数)：指定的Foobar窗口,注意,此句柄必须是通过CreateFoobarxxxx系列函数创建出来的

**返回值**：整形数: 0:失败1:成功

**示例**：dm_ret=dm.FoobarUpdate(foobar)注意：所有绘制完成以后,必须通过调用此函数来刷新窗口,否则窗口内容不会改变.

----

### dm_faqCancel

**签名**：`long dm_faqCancel()`

**简介**：可以把上次FaqPost的发送取消,接着下一次FaqPost

**原型**：long FaqCancel()

**返回值**：整形数: 0:失败1:成功

**示例**：//调用FaqPost异步发送，必须先取消,否则FaqCapture会因为上一次FaqPost未处理完毕而失败. dm.FaqCancel//截取这个范围内, 3秒动画,图像质量为中等50,动画帧率间隔为100ms handle=dm.FaqCapture(50, 50, 300, 400, 50, 100, 3000) dm_ret=dm.FaqPost("192.168.1.100: 12345", handle, 1, 3*60*1000) If dm_ret=0 Then MessageBox"发送失败，可能上个FaqPost还未处理完毕"EndScript End If//不影响脚本运行Do result=dm.FaqFetch() If len(result)>0 Then MessageBox"服务器返回结果="&result End If//做其他的事情这里就假设为延时Delay 1000 Loop

----

### dm_faqCapture

**签名**：`long dm_faqCapture(long x1, long y1, long x2, long y2, long quality, long delay, long time)`

**简介**：截取指定范围内的动画或者图像,并返回此句柄.

**原型**：long FaqCapture(x1, y1, x2, y2, quality, delay, time)

**参数定义**

- `x1` (整形数)：左上角X坐标

- `y1` (整形数)：左上角Y坐标

- `x2` (整形数)：右下角X坐标

- `y2` (整形数)：右下角Y坐标

- `quality` (整形数)：图像或动画品质,或者叫压缩率,此值越大图像质量越好取值范围（1-100或者250）当此值为250时，那么会截取无损bmp图像数据.

- `delay` (整形数)：截取动画时用,表示相隔两帧间的时间间隔,单位毫秒（如果只是截取静态图像,这个参数必须是0）

- `time` (整形数)：表示总共截取多久的动画,单位毫秒（如果只是截取静态图像,这个参数必须是0）

**返回值**：整形数:图像或者动画句柄

**示例**：//截取这个范围内, 3秒动画,图像质量为中等50,动画帧率间隔为100ms handle=dm.FaqCapture(intX - 50, intY - 232, intX+272, intY-12, 50, 100, 3000)

**注**：从插件版本2.1119之后，接口FaqCapture返回handle，不需要再手动调用FaqRelease释放了。插件已经自动释放了.另外,如果上一次的FaqPost还没有处理完毕,那么此函数调用会失败,要释放上一次的FaqPost,请调用FaqCancel函数

----

### dm_faqCaptureFromFile

**签名**：`long dm_faqCaptureFromFile(const char* file, long quality, long delay, long time)`

**简介**：截取指定图片中的图像,并返回此句柄.

**原型**：long FaqCaptureFromFile(x1, y1, x2, y2, file, quality)

**参数定义**

- `x1` (整形数)：左上角X坐标

- `y1` (整形数)：左上角Y坐标

- `x2` (整形数)：右下角X坐标

- `y2` (整形数)：右下角Y坐标

- `file` (字符串)：图片文件名,图像格式基本都支持.

- `quality` (整形数)：图像或动画品质,或者叫压缩率,此值越大图像质量越好取值范围（1-100或者250）.当此值为250时,会截取无损bmp图像数据.

**返回值**：整形数:图像或者动画句柄

**示例**：handle=dm.FaqCaptureFromFile(0, 0, 2000, 2000,"c:\test.bmp", 50)

**注**：如果上一次的FaqPost还没有处理完毕,那么此函数调用会失败,要释放上一次的FaqPost,请调用FaqCancel函数

----

### dm_faqCaptureString

**签名**：`const char* dm_faqCaptureString(long x1, long y1, long x2, long y2, long quality, long delay, long time)`

**简介**：从给定的字符串(也可以算是文字类型的问题),获取此句柄.（此接口必须配合答题器v30以后的版本）

**原型**：long FaqCaptureString (str)

**参数定义**

- `str` (字符串)：文字类型的问题.比如(桃园三结义指的是哪些人?)

**返回值**：整形数:文字句柄

**示例**：handle=dm.FaqCaptureString ("汉朝的帝都是哪里?")//后面调用FaqSend或者FaqPost发送问题.

**注**：如果上一次的FaqPost还没有处理完毕,那么此函数调用会失败,要释放上一次的FaqPost,请调用FaqCancel函数活用此函数，可以配合答题器，实现自动回答文字问题.答题器从v30后，可以实现自动采集问题到题库，并根据题库自动答题的功能.当文字类型的问题到达答题器,答题器可以先运行指定的EXE(A),(此EXE自己实现,可以查询题库,如果查到答案，把答案传递给答题器),如果获取不到，可以自动转到人工答题,然后人工答题成功后，会自动调用指定的EXE(B),(此EXE自己实现，答题器会把问题以及答案传递给它，EXE中记录问题和答案到题库).这样周而复始多次后，题库中就会自动充实。具体可能还有别的用途，大家自己去挖掘吧(比如自动获取帐号信息??)

----

### dm_faqFetch

**签名**：`long dm_faqFetch(long time_out)`

**简介**：获取由FaqPost发送后，由服务器返回的答案.

**原型**：string FaqFetch ()

**返回值**：字符串:如果此函数调用失败,那么返回值如下"Error:错误描述"如果函数调用成功,那么返回值如下"OK:答案"根据FaqPost中request_type取值的不同,返回值不同当request_type为0时,答案的格式为"x, y"(不包含引号)当request_type为1时,答案的格式为"1""2""3""4""5""6"(不包含引号)当request_type为2时,答案就是要求的答案比如"李白"(不包含引号)当request_type为3时,答案的格式为"x1, y1|..|xn, yn"比如"20, 30|78, 68|33, 33"(不包含引号)如果返回为空字符串，表示FaqPost还未处理完毕,或者没有调用过FaqPost .

**示例**：//截取这个范围内, 3秒动画,图像质量为中等50,动画帧率间隔为100ms handle=dm.FaqCapture (50, 50, 300, 400, 50, 100, 3000)//调用FaqPost异步发送dm_ret=dm.FaqPost ("192.168.1.100: 12345", handle, 1, 3*60*1000) If dm_ret=0 Then MessageBox"发送失败，可能上个FaqPost还未处理完毕"EndScript End If//不影响脚本运行Do result=dm.FaqFetch () If len (result)>0 Then MessageBox"服务器返回结果="&result End If//做其他的事情这里就假设为延时Delay 1000 Loop

**注**：如果此函数调用成功后，插件内部状态会再次重置为未处理状态，可以接着处理FaqPost接口.

----

### dm_faqGetSize

**签名**：`long dm_faqGetSize(long handle)`

**简介**：获取句柄所对应的数据包的大小,单位是字节

**原型**：long FaqGetSize(handle)

**参数定义**

- `handle` (整形数)：由FaqCapture返回的句柄

**返回值**：整形数:数据包大小,一般用于判断数据大小,选择合适的压缩比率.

**示例**：//截取这个范围内, 3秒动画,图像质量为中等50,动画帧率间隔为100ms handle=dm.FaqCapture(intX - 50, intY - 232, intX+272, intY-12, 50, 100, 3000) packet_size=dm.FaqGetSize(handle) MessageBox packet_size

----

### dm_faqIsPosted

**签名**：`long dm_faqIsPosted()`

**简介**：用于判断当前对象是否有发送过答题(FaqPost)

**原型**：long FaqIsPosted()

**返回值**：整形数: 0:没有1:有发送过

**示例**：//判断是否已经发过题目了if dm.FaqIsPosted()=0 then//截取这个范围内, 3秒动画,图像质量为中等50,动画帧率间隔为100ms handle=dm.FaqCapture(50, 50, 300, 400, 50, 100, 3000)//调用FaqPost异步发送dm_ret=dm.FaqPost("192.168.1.100: 12345", handle, 1, 3*60*1000) If dm_ret=0 Then MessageBox"发送失败，可能上个FaqPost还未处理完毕"EndScript End If end if//不影响脚本运行Do result=dm.FaqFetch() If len(result)>0 Then MessageBox"服务器返回结果="&result End If//做其他的事情这里就假设为延时Delay 1000 Loop

----

### dm_faqPost

**签名**：`long dm_faqPost(const char* server, long handle, long request_type, long time_out)`

**简介**：发送指定的图像句柄到指定的服务器,并立即返回(异步操作).

**原型**：long FaqPost(server, handle, request_type, time_out)

**参数定义**

- `server` (字符串)：服务器地址以及端口,格式为(ip: port),例如"192.168.1.100: 12345"

- `handle` (整形数)：由FaqCapture获取到的句柄

- `request_type` (整形数)：取值定义如下0:要求获取坐标1:要求获取选项,比如(ABCDE) 2:要求获取文字答案3:要求获取N个坐标.此功能要求答题器必须是v15之后的版本.

- `time_out` (整形数)：表示等待多久,单位是毫秒

**返回值**：整形数: 0:失败，一般情况下是由于上个FaqPost还没有处理完毕(服务器还没返回) 1:成功

**示例**：//截取这个范围内,静态图片,图像质量为中等50 handle=dm.FaqCapture(50, 50, 300, 400, 50, 0, 0)//调用FaqPost异步发送dm_ret=dm.FaqPost("192.168.1.100: 12345", handle, 1, 3*60*1000) If dm_ret=0 Then MessageBox"发送失败，可能上个FaqPost还未处理完毕"EndScript End If//不影响脚本运行Do result=dm.FaqFetch() If len(result)>0 Then result=split(result,":") If result(0)="OK"Then MessageBox result(1) Else MessageBox"错误:"&result(1) End If End If//做其他的事情这里就假设为延时Delay 1000 Loop

**注**：从插件版本2.1119之后，接口FaqCapture返回handle，不需要再手动调用FaqRelease释放了。插件已经自动释放了.本接口不支持多ip发送.

----

### dm_faqSend

**签名**：`long dm_faqSend(long handle, long time_out)`

**简介**：发送指定的图像句柄到指定的服务器,并等待返回结果(同步等待).

**原型**：string FaqSend(server, handle, request_type, time_out)

**参数定义**

- `server` (字符串)：服务器地址以及端口,格式为(ip: port),例如"192.168.1.100: 12345"多个地址可以用"|"符号连接。比如"192.168.1.100: 12345|192.168.1.101: 12345"。

- `handle` (整形数)：由FaqCapture获取到的句柄

- `request_type` (整形数)：取值定义如下0:要求获取坐标1:要求获取选项,比如(ABCDE) 2:要求获取文字答案3:要求获取N个坐标.此功能要求答题器必须是v15之后的版本.

- `time_out` (整形数)：表示等待多久,单位是毫秒

**返回值**：字符串:如果此函数调用失败,那么返回值如下"Error:错误描述"如果函数调用成功,那么返回值如下"OK:答案"根据request_type取值的不同,返回值不同当request_type为0时,答案的格式为"x, y"(不包含引号)当request_type为1时,答案的格式为"1""2""3""4""5""6"(不包含引号)当request_type为2时,答案就是要求的答案比如"李白"(不包含引号)当request_type为3时,答案的格式为"x1, y1|. ..|xn, yn|"比如"20, 30|78, 68|33, 33"(不包含引号)

**示例**：//截取这个范围内, 3秒动画,图像质量为中等50,动画帧率间隔为100ms handle=dm.FaqCapture(20, 20, 100, 100, 50, 100, 3000)//等待3分钟,答案要求是选项result=dm.FaqSend("192.168.1.100: 12345|192.168.1.101: 12345", handle, 1, 3*60*1000) result=split(result,":") If result(0)="OK"Then If result(1)="1"Then MessageBox"1"ElseIf result(1)="2"Then MessageBox"2"ElseIf result(1)="3"Then MessageBox"3"ElseIf result(1)="4"Then MessageBox"4"End If Else MessageBox"错误:"&result(1) End If

**注**：从插件版本2.1119之后，接口FaqCapture返回handle，不需要再手动调用FaqRelease释放了。插件已经自动释放了.另外，当向多个地址发送题目时，只要有任意一个服务器返回答案，函数就返回结果。

----

### dm_excludePos

**签名**：`const char* dm_excludePos(const char* all_pos, long type, long x1, long y1, long x2, long y2)`

**简介**：根据部分Ex接口的返回值，排除指定范围区域内的坐标.

**原型**：string ExcludePos(all_pos, type, x1, y1, x2, y2)

**参数定义**

- `all_pos` (字符串)：坐标描述串。一般是FindStrEx, FindStrFastEx, FindStrWithFontEx, FindColorEx, FindMultiColorEx,和FindPicEx的返回值.

- `type` (整形数)：取值为0或者1如果all_pos的内容是由FindPicEx, FindPicMemEx, FindStrEx, FindStrFastEx, FindStrWithFontEx返回，那么取值为0如果all_pos的内容是由FindColorEx, FindMultiColorEx, FindColorBlockEx, FindShapeEx返回，那么取值为1如果all_pos的内容是由OcrEx返回，那么取值为2如果all_pos的内容是由FindPicExS, FindStrExS, FindStrFastExS返回，那么取值为3

- `x1` (整形数)：左上角横坐标

- `y1` (整形数)：左上角纵坐标

- `x2` (整形数)：右下角横坐标

- `y2` (整形数)：右下角纵坐标

**返回值**：字符串:经过筛选以后的返回值，格式和type指定的一致.

**示例**：ret=dm.FindColorEx(0, 0, 2000, 2000,"aaaaaa-000000", 1.0, 0) ret=dm.ExcludePos(ret, 1, 100, 100, 300, 400) TracePrint ret ret=dm.FindPicEx(0, 0, 2000, 2000,"a.bmp","000000", 1.0, 0) ret=dm.ExcludePos(ret, 0, 100, 100, 300, 400) TracePrint ret ret=dm.OcrEx(0, 0, 2000, 2000,"ffffff", 1.0) ret=dm.ExcludePos(ret, 2, 100, 100, 300, 400) TracePrint ret ret=dm.FindPicExS(0, 0, 2000, 2000,"test.bmp|test2.bmp","020202", 1.0, 0) ret=dm.ExcludePos(ret, 3, 100, 100, 300, 400) TracePrint ret

----

### dm_findNearestPos

**签名**：`const char* dm_findNearestPos(const char* all_pos, long type, long x, long y)`

**简介**：根据部分Ex接口的返回值，然后在所有坐标里找出距离指定坐标最近的那个坐标.

**原型**：string FindNearestPos(all_pos, type, x, y)

**参数定义**

- `all_pos` (字符串)：坐标描述串。一般是FindStrEx, FindStrFastEx, FindStrWithFontEx, FindColorEx, FindMultiColorEx,和FindPicEx的返回值.

- `type` (整形数)：取值为0或者1如果all_pos的内容是由FindPicEx, FindStrEx, FindStrFastEx, FindStrWithFontEx返回，那么取值为0如果all_pos的内容是由FindColorEx, FindMultiColorEx, FindColorBlockEx返回，那么取值为1如果all_pos的内容是由OcrEx返回，那么取值为2如果all_pos的内容是由FindPicExS, FindStrExS, FindStrFastExS返回，那么取值为3

- `x` (整形数)：横坐标

- `y` (整形数)：纵坐标

**返回值**：字符串:返回的格式和type有关，如果type为0，那么返回的格式是"id, x, y"如果type为1,那么返回的格式是"x, y".

**示例**：ret=dm.FindColorEx(0, 0, 2000, 2000,"aaaaaa-000000", 1.0, 0) ret=dm.FindNearestPos(ret, 1, 100, 100) TracePrint ret ret=dm.FindPicEx(0, 0, 2000, 2000,"a.bmp","000000", 1.0, 0) ret=dm.FindNearestPos(ret, 0, 100, 100) TracePrint ret ret=dm.OcrEx(0, 0, 2000, 2000,"ffffff", 1.0) ret=dm.FindNearestPos(ret, 2, 100, 100) TracePrint ret ret=dm.FindPicExS(0, 0, 2000, 2000,"test.bmp|test2.bmp","020202", 1.0, 0) ret=dm.FindNearestPos(ret, 3, 100, 100) TracePrint ret

----

### dm_sortPosDistance

**签名**：`const char* dm_sortPosDistance(const char* all_pos, long type, long x, long y)`

**简介**：根据部分Ex接口的返回值，然后对所有坐标根据对指定坐标的距离(或者指定X或者Y)进行从小到大的排序.

**原型**：string SortPosDistance (all_pos, type, x, y)

**参数定义**

- `all_pos` (字符串)：坐标描述串。一般是FindStrEx, FindStrFastEx, FindStrWithFontEx, FindColorEx, FindMultiColorEx,和FindPicEx的返回值.

- `type` (整形数)：取值为0或者1如果all_pos的内容是由FindPicEx, FindStrEx, FindStrFastEx, FindStrWithFontEx返回，那么取值为0如果all_pos的内容是由FindColorEx, FindMultiColorEx, FindColorBlockEx返回，那么取值为1如果all_pos的内容是由OcrEx返回，那么取值为2如果all_pos的内容是由FindPicExS, FindStrExS, FindStrFastExS返回，那么取值为3

- `x` (整形数)：横坐标

- `y` (整形数)：纵坐标注意:如果x为65535并且y为0时，那么排序的结果是仅仅对x坐标进行排序,如果y为65535并且x为0时，那么排序的结果是仅仅对y坐标进行排序.

**返回值**：字符串:返回的格式和type指定的格式一致.

**示例**：ret=dm.FindColorEx (0, 0, 2000, 2000,"aaaaaa-000000", 1.0, 0) ret=dm.SortPosDistance (ret, 1, 100, 100) TracePrint ret ret=dm.FindPicEx (0, 0, 2000, 2000,"a.bmp","000000", 1.0, 0) ret=dm.SortPosDistance (ret, 0, 65535, 0) TracePrint ret ret=dm.OcrEx (0, 0, 2000, 2000,"ffffff", 1.0) ret=dm.SortPosDistance (ret, 2, 65535, 0) TracePrint ret ret=dm.FindPicExS (0, 0, 2000, 2000,"test.bmp|test2.bmp","020202", 1.0, 0) ret=dm.SortPosDistance (ret, 3, 65535, 0) TracePrint ret

----

### dm_dmGuard

**签名**：`long dm_dmGuard(long enable, const char* type)`

**简介**：针对部分检测措施的保护盾.前面有五角星的表示同时支持32位和64位,否则就仅支持64位.驱动功能支持的系统版本号为(win7/win8/win8.1/win10(10240)/win10(10586)/win10(14393)/win10(15063)/win10(16299)/win10(17134)/win10(17763)/win10(18362)/win10(18363)/win10(19041)/win10(19042)/win10(19043)/win10(19044)/win10(19045)/win11(22000)/win11(22621)/win11(22631)/win11(26100)不支持所有的预览版本,仅仅支持正式版本.新点的WIN10和WIN11必须要关闭内核隔离.否则会无法加载驱动,或者加载某些功能蓝屏.

**原型**：long DmGuard(enable, type)

**参数定义**

- `enable` (整形数)：0表示关闭保护盾(仅仅对memory memory2 memory3 memory4 memory5 memory6 b2 b3 display3 display4起作用) 1表示打开保护盾

- `type` (字符串)：参数具体内容可以是以下任意一个.★"np":这个是防止NP检测(这个盾已经过时,不建议使用).★"memory":这个保护内存系列接口和汇编接口可以正常运行. (此模式需要加载驱动)★"memory2":这个保护内存系列接口和汇编接口可以正常运行. (此模式需要加载驱动)"memory3 pid addr_start addr_end":这个保护内存系列接口和汇编接口可以正常运行.pid表示要操作内存的进程ID,指定了以后,所有内存系列接口仅能对此pid进程进行操作,其他进程无效.但此盾速度较快。addr_start表示起始地址(此参数可以忽略), addr_end表示结束地址(此参数可以忽略).另外，如果你发现有地址读写不到，可以尝试重新调用一次此盾.此盾是对指定的PID，指定的地址范围做快照. (此模式需要加载驱动)"memory4":这个保护内存系列接口和汇编接口可以正常运行. (此模式需要加载驱动)"memory5":这个保护内存系列接口和汇编接口可以正常运行. (此模式需要加载驱动,直接读写物理内存,所以对于地址空间不在物理内存里的地址,就会无法读写.)"memory6":这个保护内存系列接口和汇编接口可以正常运行. (此模式是memory5的加强版本,需要加载驱动,直接读写物理内存,所以对于地址空间不在物理内存里的地址,就会无法读写.)"phide[pid]":隐藏指定进程,保护指定进程以及进程内的窗口不被非法访问. pid为可选参数.如果不指定pid，默认保护当前进程. (此模式需要加载驱动,目前仅支持32位系统)"phide2[pid]":同phide.只是进程不隐藏(可在任务管理器中操作) (此模式需要加载驱动,目前仅支持32位系统)"phide3[pid]":只隐藏进程(在任务管理器看不到),但不保护进程和窗口. (此模式需要加载驱动,目前仅支持32位系统)★"display2":同display,但此模式用在一些极端的场合.比如用任何截图软件也无法截图时，可以考虑这个盾.★"display3<hwnd>":此盾可以保护当前进程指定的窗口(和子窗口)，无法被用正常手段截图. hwnd是必选参数.并且必须是和当前调用进程相同进程的顶级窗口.此盾有限制,具体查看下方的备注."display4<hwnd>":此盾可以保护指定的窗口(和子窗口)，无法被用正常手段截图. hwnd是必选参数.并且必须是顶级窗口. (此模式需要加载驱动,此盾和display3类似.只是使用了驱动来实现.并且没有display3的同进程限制.在高版本的WIN10上效果会更好.)★"block[pid]":保护指定进程不被非法访问. pid为可选参数.如果不指定pid，默认保护当前进程,另种实现方式.（此模式需要加载驱动,另外此盾在64位系统下无法隐藏驱动,调用后会让驱动无法隐藏,所以64位系统下,不太建议使用此盾)★"b2[pid]":保护指定进程不被非法访问. pid为可选参数.如果不指定pid，默认保护当前进程,另种实现方式. (此模式需要加载驱动),另外, b2盾有副作用，会导致任何和音频输出的函数无声音(比如，Play和Beep函数，或者类似此函数实现的方式.解决办法是另外创建一个进程用来播放音乐).另外要特别注意，个别系统上，会出现保护进程退出时，导致系统蓝屏，解决办法是在进程结束前，关闭b2盾即可.另外测试下来新版本的WIN10也会触发pg."b3[pid]":保护指定进程不被非法访问. pid为可选参数.如果不指定pid，默认保护当前进程,另种实现方式. (此模式需要加载驱动),另外, b3盾有副作用，会导致无法创建线程，无法结束线程,无法操作某些系统API(比如打开文件对话框)，无法绑定目标窗口等等,解决办法是，临时关闭b3，进行你的操作,然后再打开b3。"f1[pid]":把当前进程伪装成pid指定的进程，可以保护进程路径无法被获取到.如果省略pid参数，则伪装成svchost.exe进程. (此模式需要加载驱动),另外，简单游平台专用版本无法使用此盾，原因是和简单游有冲突。还有，使用此盾后，别人无法获取到你的进程的真实路径，但自己也同样无法获取，所以如果要获取真实路径，请务必在获取到路径后保存，再调用此盾. pid参数如果有效，那必须是一个真实存在的pid,否则会失败.如果被伪装的进程关闭了，那么当前进程也会立刻失去伪装.还有最重要的一点，伪装的进程和目的进程，占用内存要差不多，最好目的进程占用内存要大于被伪装进程，否则可能会导致进程崩溃!!!有些编译平台编译出的程序,貌似开这个盾会导致异常，可以尝试f2盾.★"d1[cls][add dll_name exact]":阻止指定的dll加载到本进程.这里的dll_name不区分大小写.具体调用方法看下面的例子.★"f2<target_process><protect_process>":把protect_process伪装成target_process运行.此盾会加载target_process运行,然后用protect_process来替换target_process,从而达到伪装自身的目的.此盾不加载驱动.这个protect_process也可以使用内存地址的形式，不用路径.写法是这样<addr, size>, addr是内存地址, size是大小,都是10进制.后面有例子(使用此盾后，别人无法获取到你的进程的真实路径，但自己也同样无法获取，所以如果要获取真实路径，请务必在获取到路径后保存后,通过共享内存等方式传递给保护进程).返回值为伪装后的进程ID"hm module unlink":防止当前进程中的指定模块被非法访问. module为模块名(为0表示EXE模块),比如dm.dll。unlink取0或者1，1表示是否把模块在进程模块链表中擦除, 0表示不擦除. (此模式需要加载驱动)"inject mode pid<param1><param2>":注入指定的DLL到指定的进程中. mode表示注入模式. pid表示需要注入进去的进程ID . param1和param2参数含义根据mode决定. (此模式需要加载驱动) mode取值0 1 2 3，具体含义如下: 0:此时param1表示需要注入的dll全路径. param2表示为<unlink erase>.注入方式是通过创建线程注入. unlink(取值0和1)，表示是否从进程模块链表中断链,取1表示断链. erase(取值0和1),表示是否擦除PE头,取1表示擦除. 1:此时param1表示需要注入的dll全路径. param2表示为<unlink erase>.注入方式是通过APC注入. unlink(取值0和1)，表示是否从进程模块链表中断链,取1表示断链. erase(取值0和1),表示是否擦除PE头,取1表示擦除. 2:此时param1表示需要注入的dll全路径. param2表示为<hidevad erase nothread noimportdll>.注入方式是内存加载DLL. hidevad(取值0和1),表示是否擦除vad,取1表示擦除.注意,此参数在对部分系统上,对64位进程注入时,会导致进程崩溃. erase(取值0和1),表示是否擦除PE头,取1表示擦除. nothread(取值0和1),表示是否在注入时创建线程,取1表示不创建.选1表示使用APC注入.选0表示使用线程注入. noimportdll(取值0和1),表示是否再次加载此DLL中的导入表DLL,取1表示不加载,直接使用.除非你真的理解这个参数的意义,否则不要轻易设置为1,会导致进程崩溃. 3:此时param1表示需要注入的dll的地址和大小. param1表示为<addr, size>. param2表示为<hidevad erase nothread noimportdll>.注入方式是内存加载DLL. addr表示DLL的起始地址, 10进制表示.这个同模式2.只不过DLL使用地址的方式来传递. size表示DLL的大小，10进制表示hidevad(取值0和1),同模式2 erase(取值0和1),同模式2 nothread(取值0和1),同模式2 noimportdll(取值0和1),同模式2以上几种注入模式,最隐蔽的是2和3,并且param2取为<1, 1, 1, 1>.可以自己测试使用.另外2和3对DLL是由要求的,尽可能要让DLL的导入表简单.也就是说尽可能不要让DLL依赖太多DLL。越简单越好."del<path>":强制删除指定的文件. path表示需要删除的文件的全路径.当path为0时,表示为当前dm.dll的路径,当path为1时,表示为当前EXE的全路径. (此模式需要加载驱动)其它后续开发."★cl pid type name":关闭指定进程中，对象名字中含有name的句柄. pid表示进程PID. type表示需要关闭的句柄类型.比如Section Event Mutant等.具体的类型可以用pchunter查看. name表示需要查找的对象名.注意type和name都是大小写敏感的."hl[pid]":隐藏指定进程中的句柄,无法用正常手段获取到. pid为可选参数.如果不指定pid，默认为当前进程. (此模式需要加载驱动,另外此盾会在WIN10上触发pg蓝屏.win10系统必须过pg才可以使用)"gr":开启句柄操作，具体查看DmGuardParams相关说明. (此模式需要加载驱动)"th":开启线程操作，具体查看DmGuardParams相关说明. (此模式需要加载驱动)

**返回值**：整形数: 0:不支持的保护盾类型1:成功-1: 32位平台不支持-2:驱动释放失败. (可能原因是权限不够) -3:驱动加载失败,可能是权限不够.参考UAC权限设置.或者是被安全软件拦截.如果是在64位系统下返回此错误，需要安装补丁KB3033929.如果是WIN10 1607之后的系统，出现这个错误，可参考这里还有一个可能，如果使用了定制的插件，如果原先加载了老的驱动，那么新的定制可能会无法加载，返回-3.只需要重启系统即可.(必须是重启，不是关机再开机) -555: f2盾的返回值, protect_process路径无法访问. -666: f2盾的返回值, target_process无法创建进程.(可能路径错误?) -777: f2盾的返回值,其它异常错误. - 4 -5 -6都是异常错误. -7:一般是系统版本不支持导致,用winver可以查看系统内部版本号.驱动只支持正式发布的版本，所有预览版本都不支持. -8:驱动加载失败.检查要加载的盾需要的条件. -9:表示参数错误. -10:表示此盾的功能失败了. -11:表示分配内存失败. -14:无效的窗口句柄-16:此功能依赖的驱动没有先启动-20:此功能不可重复加载-30:通信模式1和2出此错误是异常错误. -31:通信模式1和2出此错误是异常错误. -32:通信模式1和2出此错误是异常错误.

**示例**：dm.DmGuard 1,"np"dm.DmGuard 1,"memory"dm.DmGuard 1,"display2"dm.DmGuard 1,"block"dm.DmGuard 1,"block 1044"dm.DmGuard 1,"b2"dm.DmGuard 1,"b2 1044"dm.DmGuard 0,"b2"dm.DmGuard 1,"f1"dm.DmGuard 1,"f1 2358"dm.DmGuard 1,"f2<c:\windows\system32\calc.exe><d:\test\my.exe>"dm.DmGuard 1,"f2<d:\test\my_cheate.exe><d:\test\my.exe>"dm.DmGuard 1,"f2<d:\test\aaa.dat><d:\test\my.exe>"dm.DmGuard 1,"f2<c:\windows\system32\calc.exe><293478325735, 234356>"dm.DmGuard 1,"b3"dm.DmGuard 1,"b3 1044"dm.DmGuard 0,"b3"dm.DmGuard 1,"memory2"dm.DmGuard 1,"memory3 1044"dm.DmGuard 1,"hm dm.dll 1"dm.DmGuard 1,"hm dm.dll 0"//这个是隐藏exe模块dm.DmGuard 1,"hm 0 1"dm.DmGuard 1,"hm xxx.dll 1"dm.DmGuard 1,"inject 0 1044<c:\test.dll><1 1>"dm.DmGuard 1,"inject 1 1044<c:\test.dll><1 0>"dm.DmGuard 1,"inject 2 1044<c:\test.dll><1 1 1 0>"dm.DmGuard 1,"inject 3 1044<239458, 568><1 1 1 1>"dm.DmGuard 1,"del<c:\test.dll>"dm.DmGuard 1,"del<0>dm.DmGuard 1,"del<1>"dm.DmGuard 1,"display3 1188"dm.DmGuard 0,"display3 1188"//关闭进程1024中,类型为Mutant的，名字中含有test123的句柄. dm.DmGuard 1,"cl 1024 Mutant test123"//关闭进程1024中,类型为Event的，名字中含有abc的句柄. dm.DmGuard 1,"cl 1024 Event abc"//清除拦截列表dm.DmGuard 1,"d1 cls"//拦截dll名字中含有antiphinshing的所有dll dm.DmGuard 1,"d1 add antiphinshing 0"//拦截dll名字完全等同于abc.dll的DLL加载dm.DmGuard 1,"d1 add abc.dll 1"//拦截所有DLL的加载dm.DmGuard 1,"d1 add all"//隐藏句柄dm.DmGuard 1,"hl"dm.DmGuard 1,"hl 1024"//开启句柄操作dm.DmGuard 1,"gr"

**注**：此函数最好在目标进程打开之前调用即可。调用一次即可。尽量保证此函数第一个被执行，以免和其他驱动冲突.此函数最好在绑定之前执行.可多个组合调用.另外np和display2盾有点特殊,必须保证调用此对象的dm对象一直处于存在状态,不可以释放.一旦释放,等于没调用.比如有些人喜欢在按键的OnScriptLoad中调用这2个盾,实际上OnScriptLoad执行完以后,对象会自动被释放掉. f2盾特别注意, protect_process为你的真正需要运行的程序, target_process为任意一个可执行的exe,最后protect_process被伪装成target_process运行.但由于兼容性问题, target_process的选取并不是任意的,请自己做好测试.有些target_process会出现让你的程序UI不太正常等异常问题.经过我测试,最好是使用自己平台编译出的exe最好.当然,如果你使用系统exe或者其它EXE，只要测试没问题，也是OK的.另外,这2个EXE必须都是32位的,否则会加载失败!调用此盾的程序相当于是个EXE加载器,加载成功后,自己就可以退出了. f2盾使用至少需要3个EXE, A B C,其中A是加载器，里面执行f2盾, B是被伪装成的程序，C是你的真正的程序. A执行f2盾类似这样, DmGuard 1,"f2 B C"f2盾有个限制，如果C是使用f2启动的，那么C里面会无法加载驱动。比如我的DmGuard函数在C中会失败.解决办法是所有在C中需要加载的盾，都必须在A里面也加载一次。另外5个memory盾是可以切换的,切换方式是重新调用另一个memory盾即可,只有最后调用的memory盾生效.盾都要求在目标进程开启之前开启,但memory3比较特殊,因为他必须接一个真实存在的pid。对于这种情况，只需要提前加载memory2,等目标进程打开以后，再调用memory3即可. 6个memory盾在相同接口下的速度如下memory3>memory4>memory>memory5=memory6>memory2 6个memory盾突破防护的能力如下memory5=memory6>memory2>memory3>memory4>memory另外,如果您需要极限读写速度,请使用带Addr系列的读写接口,比相同系列不带Addr的接口速度要快30%.如果还要加快,那必须要定制非COM版本的DLL. COM版本天生速度就慢一些.并且使用SetMemoryHwndAsProcessId接口,直接使用pid来进行内存接口的访问.如果您要使用memory系列盾,请让它第一个被加载,这样可以让效率最大化. display3盾仅仅支持win7以上系统.并且系统必须开启了DWM.否则会返回失败. win10以下的系统可以手动设置DWM的开关, win10以上的系统是强制打开的,无法关闭.所以此盾在win10系统上效果很好.下图是win7如何打开DWM,也就是说只要勾选了启用桌面组合和在窗口和按钮上使用视觉样式就会开启dwm. win8应该也是类似.如果您开启了dwm，并且调用成功了display3盾后,如果中途关闭了dwm,那么保护就会失效.另外此盾只能保护当前进程的窗口.

----

### dm_dmGuardExtract

**签名**：`long dm_dmGuardExtract(const char* type, const char* file)`

**简介**：释放插件用的驱动.可以自己拿去签名.防止有人对我的签名进行检测.强烈推荐使用驱动的用户使用.仅释放64位系统的驱动.

**原型**：long DmGuardExtract (type, path)

**参数定义**

- `type` (字符串)：需要释放的驱动类型.这里写"common"即可.

- `path` (字符串)：释放出的驱动文件全路径.比如"c:\test.sys".

**返回值**：整形数: 0:不支持的type 1:成功-2:释放失败

**示例**：dm.DmGuardExtract"common","c:\test.sys"

**注**：释放出的文件进行签名后,可以再用DmGuardLoadCustom来进行加载.

----

### dm_dmGuardLoadCustom

**签名**：`long dm_dmGuardLoadCustom(const char* type, const char* data)`

**简介**：加载用DmGuardExtract释放出的驱动.建议自己签名后,然后找个自己喜欢的路径加载.仅支持64位系统的驱动加载.加载成功后,就可以正常调用DmGuard了.

**原型**：long DmGuardLoadCustom (type, path)

**参数定义**

- `type` (字符串)：需要释放的驱动类型.这里写"common"即可.

- `path` (字符串)：驱动文件全路径.比如"c:\test.sys".

**返回值**：整形数:返回值请参考DmGuard的返回值.一样的含义.

**示例**：dm.DmGuardLoadCustom"common","c:\test.sys"

**注**：这个路径只是演示.实际上最好不要放在这么随意的位置.一般驱动文件都在c:\windows\system32目录下.

----

### dm_dmGuardParams

**签名**：`const char* dm_dmGuardParams(const char* cmd, const char* param)`

**简介**：DmGuard的加强接口,用于获取一些额外信息.具体看下面参数介绍

**原型**：string DmGuardParams (cmd, subcmd, param)

**参数定义**

- `cmd` (字符串)：盾类型.这里取值为"gr"或者"th"(以后可能会有扩充).这里要注意的是,如果要获取指定的盾类型信息,必须先成功用DmGuard开启此盾.比如这里的"gr"必须dm.DmGuard 1,"gr"开启成功才可以

- `subcmd` (字符串)：针对具体的盾类型，需要获取的具体信息.如果cmd为"gr"时,那么subcmd取值如下定义:"enum":枚举指定进程的所有句柄."get":获取指定进程的指定句柄信息.(类型,名字,权限值)"set":设置指定进程的指定句柄的权限值."close":关闭指定进程的指定句柄.如果cmd为"th"时,那么subcmd取值如下定义:"enum":枚举指定进程的所有线程."get":获取指定线程的详细信息.(句柄值,优先级, ETHREAD, TEB, win32StartAddresss,线程地址所在的模块名,交换次数,线程状态,挂起次数)"resume":恢复指定的线程.不可操作本线程.当suspend_count大于0时,每调用一次resume,都会让suspend_count减1,直到0为止才会真正的恢复线程."suspend":挂起指定线程.不可操作本线程."terminate":结束指定线程.不可操作本线程.

- `param` (字符串)：参数信息,这里具体的含义取决于cmd和subcmd .如果cmd为"gr"时, subcmd取值为如下时，具体的参数含义:"enum": param为"pid", pid为进程pid, 10进制形式."get": param为"pid handle", pid为进程pid, 10进制形式, handle为句柄值, 10进制形式."set": param为"pid handle access", pid为进程pid, 10进制形式, handle为句柄值, 10进制形式, access为权限值, 10进制形式."close": param为"pid handle", pid为进程pid, 10进制形式, handle为句柄值, 10进制形式.如果cmd为"th"时, subcmd取值为如下时，具体的参数含义:"enum": param为"pid", pid为进程pid, 10进制形式."get": param为"tid", tid为线程tid, 10进制形式."resume": param为"tid", tid为线程tid, 10进制形式."suspend": param为"tid", tid为线程tid, 10进制形式."terminate": param为"tid", tid为线程tid, 10进制形式.

**返回值**：字符串:根据不同的cmd和subcmd,返回值不同.如果cmd为"gr"时, subcmd取值为如下时，具体的返回值:"enum":"handle1|handle2|.....|handlen",每个handle都是10进制长整数.如果失败返回空字符串"get":"type|name|access". type表示句柄的类型，比如"Event","File"等之类的. name表示句柄的名字,有些句柄的名字可能是空的. access10进制整数,表示此句柄的权限值.如果失败返回空字符串"set":成功返回"ok",否则为空字符串."close":成功返回"ok",否则为空字符串.如果cmd为"th"时, subcmd取值为如下时，具体的返回值:"enum":"tid1|tid2|.....|tidn",每个tid都是10进制整数.如果失败返回空字符串"get":"tid|prority|ethread|teb|win32StartAddress|module_name|switch_count|state|suspend_count".如果失败返回空字符串. tid:线程tid, 10进制整数prority:线程优先级, 10进制整数ethread:线程内核对象ETHREAD指针. 64位16进制整数. teb:线程内核对象TEB指针. 64位16进制整数win32StartAddress:线程起始地址. 64位16进制整数module_name:线程起始地址所在的模块名. switch_count:线程切换次数. 10进制整数state:线程状态. 10进制整数. 0:初始化1:准备2:运行3:待机4:结束5:等待6:转换.这个地方其实只需要关心是不是4就行了.如果是4表示线程结束了.其它的状态都可以认为是运行状态.不用太关心. suspend_count:线程挂起次数. 10进制整数.线程挂起次数,当此值大于0时,表示线程处于挂起状态.等于0表示处于运行状态."resume":成功返回"ok",否则为空字符串."suspend":成功返回"ok",否则为空字符串."terminate":成功返回"ok",否则为空字符串.

**示例**：//枚举进程的所有句柄,并获取此句柄的信息,并打印出来dm_ret=dm.DmGuard (1,"gr") dm_ret=dm.DmGuardParams ("gr","enum","1024") If len (dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count dm_ret=dm.DmGuardParams ("gr","get","1024"&ss (index)) TracePrint dm_ret index=index+1 Loop End If//关闭指定进程的句柄dm_ret=dm.DmGuard (1,"gr") dm_ret=dm.DmGuardParams ("gr","close","1024 240") TracePrint dm_ret//修改指定进程的句柄的权限dm_ret=dm.DmGuard (1,"gr") dm_ret=dm.DmGuardParams ("gr","set","1024 240 12345") TracePrint dm_ret//枚举进程的所有线程,并把线程的信息都打印出来dm_ret=dm.DmGuard (1,"th") dm_ret=dm.DmGuardParams ("th","enum","1024") If len (dm_ret)>0 Then ss=split(dm_ret,"|") index=0 count=UBound (ss)+1 Do While index<count dm_ret=dm.DmGuardParams ("th","get", ss (index)) TracePrint dm_ret index=index+1 Loop End If//结束指定线程.注意后面的338是线程tid .不是进程pid dm_ret=dm.DmGuardParams ("th","terminate","338")//挂起指定线程dm_ret=dm.DmGuardParams ("th","suspend","338")//恢复指定线程dm_ret=dm.DmGuardParams ("th","resume","338")

**注**：gr盾对应的这些subcmd,其实最主要的目的是"set",设置句柄权限.比如场景1:我们需要打开进程，或者打开线程，文件等等，如果在打开进程的权限里写了最高权限，那么由于一些保护措施，往往会失败.但是如果你降低一些权限，比如用最低权限，反而会成功.所以我们可以先用低权限打开对应的句柄,然后用"set"来提权，这样就相当于用间接的方式用最高权限打开了句柄.再比如场景2:某些时候，有坏人打开我们自己的进程来做一些非法的事情,这时候我们可以通过扫描("enum"功能),来检查是否有关我们进程的句柄,如果有的话，我们可以考虑直接关闭句柄("close"),或者给他降低权限("set").其它场景，请自行脑补.可能又有人会问,我怎么知道设定的权限值到底是多少呢？这个很简单。每种句柄都对应不同的权限，我们可以自己创建一个最高权限的句柄，来获取("get")这个句柄的信息，就知道最高权限对应的权限到底是多少.同样最低权限也一样。这里一定要注意,不同类型的句柄,权限定义是不同的.比如"Event"和"File"就是完全不同的权限值.这里对进程句柄举个例子.比如我们想获取最高权限的进程句柄权限值,那么操作如下//我们打开进程id为1024的句柄, 1024只是一个例子，必须换成真实存在的pid handle=dm.OpenProcess (1024) dm_ret=dm.DmGuard (1,"gr") my_pid=GetCurrentProcessId ()//这个GetCurrentProcessId是调用系统API，获取当前进程ID，因为打开的句柄在本进程里info=dm.DmGuardParams ("gr","get", cstr (my_pid)&""&cstr (handle)) ss=split(info,"|") access=clng (ss (2))//那么这里的access就是最高权限的值了.一般来说,每种句柄权限在不同系统里的定义都是相同的.所以你可以在一个系统里获取到后，在其它系统里也是通用的. ps:上次有人问我，如何把一个别人正在读写的文件给强行打开读写，其实也是一个权限的事,增加一个SHARE_READ和SHARE_WRITE就OK了.具体方法和上面这个进程句柄大同小异.

----

### dm_unLoadDriver

**签名**：`long dm_unLoadDriver()`

**简介**：卸载插件相关的所有驱动.仅对64位系统的驱动生效.

**原型**：long UnLoadDriver ()

**返回值**：整形数: 0:失败1:成功

**示例**：dm.UnLoadDriver

**注**：此接口一般不建议使用.除非你真的知道自己在干什么.

----

## 覆盖统计

- 基本设置：13 个
- 窗口：40 个
- 键鼠：34 个
- 图色：57 个
- 后台设置：28 个
- 内存：54 个
- 文件：26 个
- 文字识别：46 个
- 系统：44 个
- 杂项：9 个
- 汇编：9 个
- Ai：17 个
- Foobar：22 个
- 答题：9 个
- 算法：3 个
- 防护盾：5 个

> 未能匹配到 dm.h 的 htm：
> 基本设置/SetDisplayInput.htm, 基本设置/SetEnumWindowDelay.htm, 基本设置/SpeedNormalGraphic.htm, 基本设置/函数调用约定.htm, 键鼠/键码对应表.htm, 系统/SetLocale.htm, Ai/Ai接口综述.htm, 答题/答题接口详细描述(C语言).htm
