// ----------------------------------------------------
//     2022/4/13  by mky
// ----------------------------------------------------
1.参阅doc目录相关文档和说明书
  阅读顺序
  1) cmake基础入门.pdf
  2）用cmake优雅的构建大型项目.pdf
  3）项目组件结构.html
  4）组件式开发实践_v0.0.1a.html
  5）git远程仓库工作流程.pdf
  6）gtest单元测试和集成测试.pdf
  7）git submodule常用指令.pdf
  8）软件版本控制草案.docx

// ----------------------------------------------------
2.编译方式
  1）原始编译
     mkdir build
     cd  build
     cmake ..
     make -j install
  2) 脚本编译（注意给脚本运行权限）
     ./scripts/base_scripts/build.sh

// ----------------------------------------------------
3.配置文件
  .env
  [pro]
        pro_name=demo //工程名称
  [conf]
        custom_profile=default//默认配置文件路径
  [user]
        ip_address=192.168.30.215//目标终端ip
        user_name=root//账户
        user_password=rockchip//密码
  [pack]
        pack_path=installation_package//安装包路径
  [build]
        build_platforms= //工具链文件路径 例:../platforms/linux/rv1126-toolchain-mky.cmake
        build_type=Release//编译debug/release
        build_tests=ON//是否编译测试单元
  [vers]
        ver_major=0//主版本号
        ver_minor=0//次版本号
        ver_patch=1//修正版本号
        ver_cycle=a//版本类型 a内测 参阅版本控制文档
  [book]
        author=mky//工程建造者
        email=18645528660//联系方式

// ----------------------------------------------------