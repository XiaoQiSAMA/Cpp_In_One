#define M 6
#define DEGREE (M/2)

typedef int KEY_VALUE;

struct bTree_node
{
    KEY_VALUE *keys;    // malloc
    struct bTree_node **childrens;

    int num;
    int leaf;   //bool 是否是叶子节点
};

struct bTree
{
    struct bTree_node *root;
};

struct bTree_node *btree_create_node(struct bTree *T){
};


void btree_split_child(struct btree *T, struct bTree_node *x, int i){
    // x: 承受分裂的父节点  i: x的第i个儿子需要分裂
    struct bTree_node *y = x->childrens[i];

    // 构建新节点来接受分裂的节点
    struct bTree_node *z = btree_create_node(T);

    for (int j = 0; j < DEGREE - 1; j++)
        z->keys[j] = y->keys[j];
    
    // 分裂的节点有children，则继承他们
    if (y->leaf == 0){
        for (int j = 0; j < DEGREE; j++)
            z->childrens[j] = y->childrens[j];
    } 
}

void btree_insert_nonfull(struct bTree *T, struct bTree_node *x, KEY_VALUE k){
    int i = x->num;
    // 只往叶子结点里插入
    if (x->leaf == 1){
        btree_insert(T, k);
    } else {
        while (i >= 0 && x->keys[i] > k)    i--;
        // 递归过程中需要插入的子结点中有full的结点，需先分裂再插入
        if (x->childrens[i+1]->num == M-1)
            btree_split_child(T, x, i+1);
        btree_insert_nonfull(T, x->childrens[i+1], k);
    }
}

void btree_insert(struct bTree *T, KEY_VALUE key){
    struct bTree_node *r = T->root;
    // r的孩子要分裂
    if (r->num == M-1){
        struct bTree_node *node = btree_create_node(T);

        T->root = node;
        node->childrens[0] = r;

        btree_split_child(T, node, 0);
    }
    // 插入结点
}

void btree_merge(struct bTree *T, struct bTree_node *x, int idx){
    // 合并{x->childrens[idx]->keys}{x->keys[idx]}{x->childrens[idx+1]->keys}这三个集合
}

void btree_delete_key(struct bTree *T, struct bTree_node *x, KEY_VALUE key){
    int idx = 0;
    while (idx < x->num && key > x->keys[idx])
        idx++;
    
    if (key == x->keys[idx]){
        if (x->leaf){
            // 删除key
            delete(x->keys[idx]);

        }   else if (x->childrens[idx]->num >= DEGREE){
            // 向idx的结点借位
        }   else if (x->childrens[idx+1]->num >= DEGREE){
            // 向idx的后一位借位
        }   else {
            // 合并
            btree_merge(T, x, idx);
            // 递归
            btree_delete_key(T, x->childrens[idx], key);
        }

    }
}
 