// 栈

#include <stdio.h>
#include <stdlib.h>
// 1、结构设计
typedef struct Node{
    // void *data;
    int data;
    struct Node *next;
}Node;
// 栈头设计为一个指针，一个大小
typedef struct Stack{
    int size;
    Node *top;
}Stack;
// 2、功能设计
// 初始化
Stack *initStack(){
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->size=0;
    stack->top = NULL;
    return stack; 
}
Node *initNode(int data){
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node; 
}
// 入栈
Stack *pushTop(Stack *stack,Node *node){
    if(stack->top == NULL){
        stack->top = node;
    }else{
        Node *oNode = stack->top;
        stack->top = node;
        node->next = oNode;
    }
    stack->size++;
    return stack;
}

// 删除（头删）
Node *popTop(Stack *stack){
    if(stack->top == NULL){
        return NULL;
    }
    Node *node = stack->top;
    stack->top = node->next;
    stack->size--;
    return node;
}

// 3、功能测试
int PStack(Stack *stack){
    if(stack->size==0||stack == NULL){
        printf("NULL\n");
        return 0;
    }
    Node *pcur = stack->top;
    for(int i=0;i<stack->size;i++){
        printf("%d--",pcur->data);
        pcur = pcur->next;
    }

    printf("\nsize:%d\n",stack->size);
    
    return 0;
}



// stack的应用：
// 1、括号匹配(伪代码)

// 2、后缀表达式求值


int main(){
    Stack *s = initStack();
    Node *node = initNode(10);
    s = pushTop(s,node);
    PStack(s);
    node = initNode(11);
    s = pushTop(s,node);
    node = initNode(12);
    s = pushTop(s,node);
    node = initNode(13);
    s = pushTop(s,node);
    PStack(s);
    Node *topNode = popTop(s);
    printf("stack top is %d\n",topNode->data);
    PStack(s);

}