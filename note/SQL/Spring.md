#IoC容器
- Spring通过IoC容器来管理
- 功能
	- 所有Java对象的实例化和初始化
	- 控制对象之间的依赖关系

- Spring Bean: IoC容器管理的Java对象没有区别
- IoC使用map集合来储存bean对象

- 工作机制
	- 在xml中配置Bean的定义信息(BeanDefinition)
	- 通过BeanDefinition的实现类将Bean定义信息加载到IoC
		- 例如ApplicationContext context = new ClassPathXmlApplicationContext("bean.xml");
	- IoC依据Bean定义信息使用BeanFactory和反射创建对象，并初始化得到最终对象
		- context.getBean("xxx");


# 基于xml管理Bean
- 定义Bean 
	在bean.xml下添加</br>
	```
	<bean id="user" class="indi.beta.spring6.iocxml.User"/>
	```
- 获取Bean
	<pre>
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("bean.xml");

        User user1 = (User) context.getBean("user");
        User user2 = context.getBean(User.class);
        User user3 = context.getBean("user", User.class);
	</pre>
- 注意
	- 在bean.xml中创建相同class的多个对象时，不得使用getBean(User.class)来获取Bean(获取是无歧义的)
	- 如果bean.xml定义的bean的class是接口，且当该接口只有一个实现类的时候，可以通过getBean("接口bean名")来获取对象
	- 如果bean.xml定义的bean的class是接口，且当该接口有多个实现类的时候，不能使用class来获取

- 依赖注入(Dependency Injection)
	- 基于setter
		- 通过bean.xml下的bean下的property注入</br>
			```
			<bean id="user1" class="indi.beta.spring6.iocxml.User">
		        <property name="age" value="15"/>
		        <property name="name" value="BBB"/>
		    </bean>
			```
	- 基于构造器
		- 通过有参构造器注入</br>
			```
			<bean id="user2" class="indi.beta.spring6.iocxml.User">
		        <constructor-arg name="age" value="64"/>
		        <constructor-arg name="name" value="GGG"/>
		    </bean>
			```
		- 会依据给定的属性名自动选择相应的构造器
	- 使用util注入集合
			
		```
	    <bean id="cat" class="indi.beta.spring6.iocxml.Pet">
	        <property name="name" value="catAAA"/>
	    </bean>
	    <bean id="dog" class="indi.beta.spring6.iocxml.Pet">
	        <property name="name" value="dogBBB"/>
	    </bean>
	
	    <bean id="user" class="indi.beta.spring6.iocxml.User">
	        <property name="pet" ref="map1"/>
	    </bean>
	
	    <util:map id="map1">
	        <entry key="cat" value-ref="cat"/>
	        <entry key="dog" value-ref="dog" />
	    </util:map>
		```
	- 使用p命名空间注入
		```
	    <bean id="cat" class="indi.beta.spring6.iocxml.Pet">
	        <property name="name" value="catAAA"/>
	    </bean>
	    <bean id="dog" class="indi.beta.spring6.iocxml.Pet">
	        <property name="name" value="dogBBB"/>
	    </bean>
	
	    <bean id="user" class="indi.beta.spring6.iocxml.User" p:pet-ref="map1"/>
	
	    <util:map id="map1">
	        <entry key="cat" value-ref="cat"/>
	        <entry key="dog" value-ref="dog" />
	    </util:map>
			```

	- 特殊值处理
		- 空</br>
			```
			<bean id="user1" class="iocxml.User">
		        <null/>
		    </bean>
			```
		- CDATA节(用于处理xml冲突的字符)</br>
			```
			<bean id="user1" class="iocxml.User">
		        <property name="age">
					<value><![CDATA[xx]]></value>
				</property>
		    </bean>
			```
		- 对象
			- 引用外部Bean</br>
			```
		    <bean id="pet" class="indi.beta.spring6.iocxml.Pet">
		        <property name="name" value="catAA"/>
		    </bean>
		
		    <bean id="user" class="indi.beta.spring6.iocxml.User">
		        <property name="pet" ref="pet"/>
		    </bean>
			```
				- 需要先创建需要被引用的bean，然后是哟个**ref**而非value来引用
			- 内部Bean
			```
	        <bean id="user" class="indi.beta.spring6.iocxml.User">
		        <property name="pet">
		            <bean id="pet" class="indi.beta.spring6.iocxml.Pet">
		                <property name="name" value="dogVV"/>
		            </bean>
		        </property>
		    </bean>
			```
			- 级联赋值</br>
			```
            <bean id="pet" class="indi.beta.spring6.iocxml.Pet">
		    </bean>
		
		    <bean id="user" class="indi.beta.spring6.iocxml.User">
		        <property name="pet" ref="pet"/>
		        <property name="pet.name" value="birdDDD"/>
		 	</bean>
			```
				- 需要在外部bean的基础上实现。实际就是对外部bean的属性的赋值

		- 数组</br>
			```
		    <bean id="user" class="indi.beta.spring6.iocxml.User">
		        <property name="hobbit">
		            <array>
		                <value>scdad</value>
		                <value>bcbc</value>
		                <value>tfj</value>
		            </array>
		        </property>
		    </bean>
			```
		- 集合</br>
			```
		    <bean id="user" class="indi.beta.spring6.iocxml.User">
		        <property name="hobbit">
		            <list>
		                <value>dsadsa</value>
		                <value>daewq</value>
		                <value>yiukj</value>
		            </list>
		        </property>
		    </bean>
			```
			```
		    <bean id="user" class="indi.beta.spring6.iocxml.User">
		        <property name="debt">
		            <map>
		                <entry>
		                    <key>
		                        <value>key 1</value>
		                    </key>
		                    <value>value 1</value>
		                </entry>
		            </map>
		        </property>
		    </bean>
			```
- 引入外部属性文件