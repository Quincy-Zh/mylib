/**
 * @file linkedlist.h
 * @author  Qyincy-Zh (wangqy31@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-09-20
 * 
 * 通用单链表
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#ifdef __cplusplus
extern "C" {
#endif

struct LinkedList;
typedef struct LinkedList sLinkedList;

/**
 * @brief 遍历链表的回调函数
 * 
 * @param data 链表节点内容
 * @param ctx 回调函数上下文指针
 * @return 返回值非0时，停止遍历
 */
typedef int (*fnForeach)(void *data, void *ctx);

/**
 * @brief 创建单链表
 * 
 * @return 单链表
 */
sLinkedList *linkedlist_create(int option);

/**
 * @brief 销毁单链表
 * 
 * @param llist 链表指针
 */
void linkedlist_destroy(sLinkedList **plist);

/**
 * @brief 检查链表元素个数
 * 
 * @param lst 
 * @return int 
 */
int linkedlist_size(sLinkedList *lst);

/**
 * @brief 追加元素
 * 
 * @param lst 单链表
 * @param data 数据指针
 * @return 成功返回0，其他表示失败
 */
int linkedlist_append(sLinkedList *lst, void *data);

/**
 * @brief 删除元素
 * 
 * @param lst 链表
 * @param index 元素索引
 * @return 成功返回0，其他表示失败 
 */
int linkedlist_remove(sLinkedList *lst, int index);

/**
 * @brief 遍历链表
 * 
 * @param lst 链表
 * @param fn 回调函数
 * @param ctx 上下文指针
 * @return 成功返回0，其他表示失败 
 */
int linkedlist_foreach(sLinkedList *lst, fnForeach fn, void *ctx);

#ifdef __cplusplus
}
#endif

#endif // LINKEDLIST_H
// EOF
