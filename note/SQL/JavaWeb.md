# Servlet
Servlet是java与浏览器之间交互的桥梁

### 从浏览器获取post类请求: doPost
	- doPost(HttpServletRequest req, HttpServletResponse res)
		- req是客户端请求封装成的对象


### 用户动作与servlet的对应关系: web.xml
	- servlet: 
        - servlet-name
        - servlet-class
    - servlet-mapping
        - servlet-name
        - url-pattern
    - 过程
	    - 用户请求，action=xxx，提交到xxx
	    - 依据web.xml中的url-pattern
	    - 找到servlet-name
	    - 依据mapping找到servlet
	    - 在servlet中依据servlet-class找到类
	    - 调用类中的相应方法(post或get或指定名称方法)