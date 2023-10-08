# JDBC
- 什么是JDBC
	- 在JAVA代码中，使用JDBC提供的方法，发送字符串类型的SQL语句到数据库管理软件，并获取语句的执行结果，进而实现数据库数据的CRUD
	- **JDBC=接口+实现**。JAVA提供接口(规范)，数据库厂商依据规范进行实现。实现的代码可以不同，但方法一定相同。

- JDCB核心类和接口
	- DriverManger
		- 将第三方的jar包注册到程序
		- 依据数据库连接信息获取connection
	- Connection
		- 与数据库建立连接，在连接的对象上可以多次执行CRUD
		- 可以获取statement和preparedStatement, callableStatement对象
	- Statement | PreparedStatement | CallableStatement
		- 具体发送SQL语句到DBM 
	- Result
		- 面向对思维的产物
		- 存储DQL查询数据库结果的对象
		- 需要进行解析次啊能获取具体的数据库数据

# JDBC核心API
- 导入jdbc.jar
- 基于Statement的基本操作
	- 注册驱动
		<pre>
		DriverManager.registerDriver(new Driver());
		</pre>
		- Driver静态代码块和DriverManager都会注册驱动
			<pre>
			# 通过反射实现仅1次注册，同时提升程序的适应性
			Class.forName("com.mysql.gj.jdbc.Driver");
			</pre>
	- 获取连接
		- 通过给定的信息连接数据库
		<pre>
		Connection conn = DriverManager.getConnection(url, user, password);
		</pre>
		- url格式
			- "jdbc:数据库厂商名://ip|主机名:port/数据库名?key1=value1&key2=value2"
				- 例如: "jdbc:mysql://127.0.0.1:3306/db_beta_test"
	- 创建sql语句对象
		<pre>
		Statement statement = conn.createStatement();
		</pre>
	- 发送sql语句，并获取返回结果
		<pre>
		String sql = "SELECT * FROM jdbc_table";
        ResultSet resultSet = statement.executeQuery(sql);	# 执行查询
		# ResultSet resultSet = statement.executeUpdate(sql);	# 执行增删改等
		</pre>
	- 结果集解析
		<pre>
		while (resultSet.next()){
            int id = resultSet.getInt("id");
            String account = resultSet.getString("account");
            String password = resultSet.getString("pswd");
            String nickname = resultSet.getString("nickname");

            System.out.println(id + account + password + nickname);
        }
		</pre>
	- 资源关闭
		<pre>
		resultSet.close();
        statement.close();
        conn.close();
		</pre>


- Statement(不推荐的方式)
	- 适合执行没有动态语句的查询
	- 可能出现SQL注入
		- 强行将查询条件变为true是一种攻击方式
	- statement.executeUpdate(sql)
		- 非查询语句都可用
		- 返回值为int型，值为受影响的行
	- statement.executeQuery(sql)
		- 用于查询
		- 返回值为ResultSet对象，值为封装相应数据的集合
			- RsultSet对象通过游标来控制
				- 游标默认在第一行之前
				- 游标具有很多移动方法
				- 游标的方法next()有返回值，指明是否已经完成查询。
			- 通过字段名查询
				<pre>
				resultSet.getXxx("attr_name");
				</pre>
			- 通过字段下标查询
				<pre>
				resultSet.getXxx(attr_index);
				</pre>

- PreparedStatement
	- 有效防止SQL注入
		- 原因是创建preparedstatement时已经确定了sql语句的结构，避免了动态值充当sql语句结构的情况
	- PreperedStatement与Statement的区别
		- statement
			- 创建statement
			- 拼接sql
			- 发送sql
		- preparedstatement
			- 编写sql查询语句的结构，其中动态字段值使用？占位
			- 创建preparedstatement
			- 使用setObject传入动态值
				<pre>
				ps.setObject(index, value);
				</pre>
			- 发送sql

- ResultSet转List
	- 获取ResultSet的属性
		<pre>
		ResultSetMetaData metaData = resultSet.getMetaData();
		</pre>
	- 获取字段名
		<pre>
		metaData.getColumnLabel(index); # 不推荐使用getColumnName，因为它无法获取别名
		</pre>
	- 完整的
		<pre>
		ResultSetMetaData metaData = resultSet.getMetaData();

        List<Map> list = new ArrayList<>();

        while(resultSet.next()){
            Map map = new HashMap();
            for (int i = 1; i < metaData.getColumnCount() + 1; i++) {
                map.put(metaData.getColumnLabel(i), resultSet.getObject(i));
            }
            list.add(map);
        }

        for (Map<String, Object> map: list){
            for (String key: map.keySet())
                System.out.print(key + ": " + map.get(key) + ", ");
            System.out.println();
        }
		</pre>

- 其他操作
	- 主键回显: 查看自增的主键值
		- 在声明preparedstatement时指明需要返回主键
			<pre>
			ResultSet primaryKey = ps.getGeneratedKeys();
			</pre>
		- 建立一个单独的结果集接收主键、
			<pre>
			ResultSet primaryKey = ps.getGeneratedKeys();
			</pre>
	- 批量插入数据的优化
		- 对每个条目都添加到statement中
			<pre>
			statement.addBatch();
			</pre>
		- 使用批量添加而单条目添加
			<pre>
			statement.executeBatch();
			</pre>
		- 注意
			- 连接数据库时，必须声明允许批量添加，即设置rewriteBatchedStatements=true
			- 预先声明的sql语句不能以分号结束

- 事物
	- 使用java的try-catch实现事物
		<pre>
		connection.setAutoCommit(false);	 # 必须先将自动提交关闭，防止try内部动作被逐个提交
		try{
			动作1;
			...
			动作n;
		} catch (Exception e){		# 一旦出现错误，说明事物没有完成，则回滚
			connection.rollback();
		} finally{
			onnection.close();
		}
		</pre>
	- 注意
		- **不同的动作需要使用同一个connection**

- Druid
	- 连接池的原理和优势
		- 将连接放入连接池中，实现连接的复用
		- 对于每次使用连接，直接从连接池调用连接，避免了重新建立连接的开销

- DAO(Data Access Object)思想
	- 由于通常JDBC的代码是相似的，因此可以将其封装成方法。可以极大地简化代码
	- 对于每一个特定的数据表，都有其特定的DAO
	- 由于DQL的返回值是不确定类型的，可以考虑使用反射