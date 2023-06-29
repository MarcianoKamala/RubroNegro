#include <stdio.h>
#include <stdlib.h>
	
	typedef enum { RED, BLACK } Color;     // Cor do nó (0 = vermelho, 1 = preto)

typedef struct Node {
    int data;
    Color color;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

// Função para criar um novo nó
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = RED;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void insert(Node** root, int data) {
    Node* newNode = createNode(data);
    Node* current = *root;
    Node* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;

    if (parent == NULL)
        *root = newNode;
    else if (data < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;
}
// Função auxiliar para realizar uma rotação para a esquerda
void leftRotate(Node **root, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}
// Função auxiliar para realizar uma rotação para a direita
void rightRotate(Node **root, Node *x) {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

// Função para contar a quantidade de nós na árvore Rubro-Negra
int countNodes(struct Node* root) {
    if (root == NULL)
        return 0;
    else
        return 1 + countNodes(root->left) + countNodes(root->right);
}

// Função auxiliar para obter a altura negra de um nó
int blackHeight(Node* node) {
    if (node == NULL) {
        return 1;
    }

    int leftHeight = blackHeight(node->left);
    int rightHeight = blackHeight(node->right);

    if (leftHeight != rightHeight) {
        return -1;
    }

    if (node->color == 'B') {
        return leftHeight + 1;
    } else {
        return leftHeight;
    }
}

// Função para verificar se a árvore é Rubro-Negra
int isRedBlackTree(Node* root) {
    if (root == NULL) {
        return 1;
    }

    // Verifica as propriedades da árvore Rubro-Negra
    if (root->color != 'B') {
        return 0;
    }

    if (blackHeight(root->left) != blackHeight(root->right)) {
        return 0;
    }

    return isRedBlackTree(root->left) && isRedBlackTree(root->right);
}
// Função para remover um nó na árvore rubro-negra
void removeNode(Node** root, int key) {
    Node* node = *root;
    Node* parent = NULL;
    Node* temp = NULL;

    while (node != NULL) {
        if (node->data == key)
            break;

        parent = node;

        if (key < node->data)
            node = node->left;
        else
            node = node->right;
    }

    if (node == NULL)
        return;

    if (node->left == NULL || node->right == NULL)
        temp = node;
    else {
        temp = node->right;
        while (temp->left != NULL)
            temp = temp->left;
    }

    Node* child = NULL;
    if (temp->left != NULL)
        child = temp->left;
    else
        child = temp->right;

    if (child != NULL)
        child->parent = temp->parent;

    if (temp->parent == NULL)
        *root = child;
    else if (temp == temp->parent->left)
        temp->parent->left = child;
    else
        temp->parent->right = child;

    if (temp != node)
        node->data = temp->data;

    if (temp->color == 0)
        fixDelete(root, child);

    free(temp);
}
// Função auxiliar para consertar a árvore rubro-negra após a remoção de um nó
void fixDelete(Node** root, Node* node) {
    if (node == NULL)
        return;

    if (node == *root) {
        *root = NULL;
        return;
    }

    if (node->color == 1 || (node->left != NULL && node->left->color == 1) || (node->right != NULL && node->right->color == 1)) {
        Node* child = (node->left != NULL) ? node->left : node->right;

        if (node == node->parent->left) {
            node->parent->left = child;
            if (child != NULL)
                child->parent = node->parent;
            node->color = 0;
            free(node);
        } else {
            node->parent->right = child;
            if (child != NULL)
                child->parent = node->parent;
            node->color = 0;
            free(node);
        }
    } else {
        Node* sibling = getSibling(node);

        if (sibling != NULL && sibling->color == 1) {
            node->parent->color = 1;
            sibling->color = 0;
            if (isLeftChild(node))
                rotateLeft(root, node->parent);
            else
                rotateRight(root, node->parent);
        }

        sibling = getSibling(node);

        if (node->parent->color == 0 && (sibling == NULL || sibling->color == 0) &&
            (sibling == NULL || sibling->left == NULL || sibling->left->color == 0) &&
            (sibling == NULL || sibling->right == NULL || sibling->right->color == 0)) {
            sibling->color = 1;
            fixDelete(root, node->parent);
        } else if (sibling != NULL && sibling->color == 0) {
            if (isLeftChild(node) &&
                (sibling->right == NULL || sibling->right->color == 0) &&
                (sibling->left != NULL && sibling->left->color == 1)) {
                sibling->color = 1;
                sibling->left->color = 0;
                rotateRight(root, sibling);
            } else if (!isLeftChild(node) &&
                       (sibling->left == NULL || sibling->left->color == 0) &&
                       (sibling->right != NULL && sibling->right->color == 1)) {
                sibling->color = 1;
                sibling->right->color = 0;
                rotateLeft(root, sibling);
            }
        }

        sibling = getSibling(node);

        if (sibling != NULL) {
            sibling->color = node->parent->color;
            node->parent->color = 0;

            if (isLeftChild(node)) {
                if (sibling->right != NULL)
                    sibling->right->color = 0;
                rotateLeft(root, node->parent);
            } else {
                if (sibling->left != NULL)
                    sibling->left->color = 0;
                rotateRight(root, node->parent);
            }
        }
        if (node->parent != NULL)
            fixDelete(root, node->parent);
    }
}
void printTree(Node* root) {
    if (root == NULL)
        return;

    printTree(root->left);
    printf("%d ", root->data);
    printTree(root->right);
}

int main() {
    Node* root = NULL;

    // Exemplo de inserção
    insert(&root, 1000);
    insert(&root, 100);
    insert(&root, 10);
    insert(&root, 30);
    insert(&root, 130);
    insert(&root, 400);
    insert(&root, 20);

    printf("Árvore Rubro-Negra: ");
    printTree(root);
    
    return 0;
}
