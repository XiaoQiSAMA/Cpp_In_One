# B-树与B+树

寻址效率的优化

## 1. BTree

![BTree](bTree.png)

Example:
![Example](bTree_example.png)

Code: bTree.c

### 1.1 BTree的添加

在添加key的时候，如果满足节点>M阶的时候，需先分裂再添加。

分裂的两种情况：

1. 只有根节点的时候，1分裂三个节点
2. 其余的情况，1分裂2个节点

Code: bTree.c/btree_split_child

结点的插入：
Code: bTree.c/btree_insert

1. 找到对应的结点
2. 对结点的key对比，找到合适的位置

插入的数据是插在叶子节点上面

Code: bTree.c/btree_insert_nonfull

### 1.2 BTree的删除

结点的删除：

删除的操作也是在将其下沉到叶子结点上操作

1. idx的子树满足ceil(M / 2) - 1 (idx此时表示出问题子树的父亲)

    A. 借位

        a. 从idx-1处. 大于 ceil(M / 2) - 1

        b. 从idx+1处. 大于 ceil(M / 2) - 1

    B. 合并

        {idx.childs.keys} & {idx.keys} & {(idx+1).childs.keys}

2. idx的子树 . ceil(M / 2) - 1

    直接删除
