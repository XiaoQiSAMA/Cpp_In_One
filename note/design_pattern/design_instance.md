# 设计模式实例

## 单例模式

一个类不管创建多少次对象，永远只能得到该类型的一个对象实例

例如: 日志模块(统一由一个日志对象管理)、数据库模块

[code_for_Singleton](../../src/design_pattern/singleton.cpp)

* 饿汉式单例模式: 还没有获取实例对象，实例对象就已经产生了
* 懒汉式单例模式: 唯一的实例对象，知道第一次获取他的时候，才创建

## 工厂模式

* 简单工厂  Simple Factory
* 工厂方法  Factory Method
* 抽象工厂  Abstract Factory

MainIdea: 封装了对象的创建

[code_for_Factory](../../src/design_pattern/Factory.cpp)