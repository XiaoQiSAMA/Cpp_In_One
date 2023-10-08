# Maven的功能
- 构建
	- 清理: 删除上一次构建的结果
	- 编译: 编译main目录下代码
	- 测试: 编译和执行测试目录下代码
	- 报告: 对测试给出全面的信息
	- 打包: java -> jar, Web -> war
	- 安装: 导入jar和war到Maven本地仓
	- 部署: 部署到服务器
- 依赖管理

# Maven命令行
- 坐标
	- 唯一定位jar的标识
		- groupId: 公司或组织或其项目的id
		- artifactId: 项目中模块的id
		- version: 版本号
- Maven命令格式
	- mvn  插件:目标
- 生成maven工程
	<pre>
	mvn archetype:gengerate	
	</pre>
- pom.xml
	- project: 根标签
	- modelVesion: 代表当前pom.xml的结构
	- groupId, artifactId, version: 坐标信息
	- packaging: 打包方式
		- jar: java工程
		- war: web工程
		- pom: 其他工程
	- properties: 定义属性值
		- project.build.sourceEncoding: 读取源码时用的字符集
	- dependencies: 配置具体的依赖信息
		- dependency: 依赖项
			- groupId, artifactId, version: 依赖项的坐标信息
			- scope： 当前依赖项的范围

# Maven核心概念
- POM
	- 含义: 项目对象模型
	- 模型化思想: POM将工程抽象为一个模型，再用程序中的对象来描述这个模型
	
- 约定的目录结构
	- **src**: 源码
		- **main**: 主体程序目录
			- **java**: java代码目录
				- **com**: package目录
			- **resources**: 配置文件
		- **test**: 测试程序目录
			- **java**: java源代码
				- **com**: package目录
- 构建
	- **与构建相关操作必须在pom.xml的目录下**
	- 清理: mvn clear
	- 编译: mvn compile
		- 编译工程: mvn compile
		- 编译测试工程: ，mvn test-compile
		- *编译结果在pom同级target目录下*
	- 测试: mvn test
		- 测试报告路径: target/surefire-reports
	- 打包: mvn package
		- 打包前会自动执行测试，通过时才会打包
		- 测试程序不会被打包
	- 安装: mvn install
		- 安装会自动执行打包
		- 工程系的pom.xml会被转成.pom文件一同安装到仓库中，二者内容相同

- 依赖的有效范围(dependencies/dependency/scope标签下的参数控制)
	- compile(默认的)
		- main/test目录有效
		- 开发/部署有效
	- test
		- main目录无效/test目录有效
		- 开发有效/部署无效
	- provide
		- main/test目录有效
		- 开发有效/部署无效

- 依赖的传递性
	- scope为compile时传递性不会被阻断
	- score为test/provide时传递性被阻断，此时需要手动配置依赖
	- 禁止出现循环依赖

- 依赖的排除
	- 目的: 防止多个包需要的依赖项存在版本冲突
	- dependencies/denpendency/exclusions/exclusion标签添加条目
	- 只需要提供groupId和artifactId，不需要提供version

- 继承
	- 工程之间的pom继承
	- 目的
		- 对大项目的模块进行拆分
		- 通过父工程统一控制子工程的依赖
	- 子工程pom
		- parent标签指定父工程的groupId, artifactId和version
		- *子工程与父工程的version或groupId相同时可以省略*
		- 即使父工程管理了包，子工程pom依然需要在dependencies下添加
			- 如果子工程缺省了version，则采用父工程的版本
			- 如果填写了version且与父工程一致，则采用父工程的版本
			- 否则，子工程单独导入一个自己指定的包
	- 父工程pom
		- dependencyManagement/dependency标签进行指定
		- 
- 自定义属性(properties下)
	- 定义
		- <my.attr>my_value</my_attr>
	- 解析
		- ${my.attr}
	- 使用
		- <version>${my.attr}<version>

- 聚合(modules/module)
	- 实现总工程中一键执行
	- 优先构建依赖工程

- Maven生命周期
	- Clean: 清理操作相关
	- Site: 生成站点相关
	- Default: 主要构建相关

- 插件与目标
	- 一个插件可以对应多个目标
	- 一个目标对应一个插件生命周期中的一环

- 仓库
	- 本地仓库
	- 远程仓库
		- 私服: 使用Nexus搭建maven私服
		- Internet
			- 中央仓库
			- 镜像仓库 

# 架构
- 架构: 项目的结构
	- 单一架构: 所有代码、配置、资源都在同一个工程下
		- 一个项目包含一个工程
		- 导出一个war包
		- 放在一个Tomcat上运行

- 架构设计
	- 确定技术选型
	- 组件依赖列表
	- 项目划分模块