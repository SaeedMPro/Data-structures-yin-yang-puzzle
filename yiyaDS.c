#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define SIZE 4

int count = 0;
typedef uint32_t Board;

typedef struct TreeNode
{
    Board state;
    struct TreeNode **children;
    int num_children;
} TreeNode;

// Check if board satisfies conditions
bool is_valid(Board board)
{
    // // Check 2x2 squares
    // for (int y = 0; y < SIZE - 1; y++)
    // {
    //     for (int x = 0; x < SIZE - 1; x++)
    //     {
    //         int index = y * SIZE + x;
    //         if (((board & (1 << index)) && (board & (1 << (index + 1)))) &&
    //             ((board & (1 << (index + SIZE))) && (board & (1 << (index + SIZE + 1)))))
    //         {
    //             if ((board & (1 << (index + 16))) == (board & (1 << (index + 1 + 16))) &&
    //                 (board & (1 << (index + SIZE + 16))) == (board & (1 << (index + SIZE + 1 + 16))))
    //             {
    //                 return false;
    //             }
    //         }
    //         if (((!(board & (1 << index))) && (!(board & (1 << (index + 1))))) &&
    //             ((!(board & (1 << (index + SIZE)))) && (!(board & (1 << (index + SIZE + 1))))))
    //         {
    //             if ((board & (1 << (index + 16))) == (board & (1 << (index + 1 + 16))) &&
    //                 (board & (1 << (index + SIZE + 16))) == (board & (1 << (index + SIZE + 1 + 16))))
    //             {
    //                 return false;
    //             }
    //         }
    //     }
    // }

    return true;
}

// Print board
void print_board(Board board)
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            int index = y * SIZE + x;

            if (board & (1 << index))
            {
                // Painted
                if (board & (1 << (index + 16)))
                {
                    printf("B "); // Black
                }
                else
                {
                    printf("W "); // White
                }
            }
            else
            {
                printf("- "); // Not painted
            }
        }
        printf("\n");
    }
    printf("------------------------Number of Board: %d-----------------------\n\n\n", ++count);
}

// Create empty board
Board create_board()
{
    return 0;
}

// Set cell (x,y) to be painted white
void set_white(Board *board, int x, int y)
{
    int index = y * SIZE + x;
    *board |= (1 << index);         // Set paint bit
    *board &= ~(1 << (index + 16)); // Clear black bit
}

// Set cell (x,y) to be painted black
void set_black(Board *board, int x, int y)
{
    int index = y * SIZE + x;
    *board |= (1 << index);        // Set paint bit
    *board |= (1 << (index + 16)); // Set black bit
}

// DFS algorithm
void dfs(TreeNode *root)
{
    if (root == NULL)
        return;

    if (is_valid(root->state))
    {
        // Found a valid state
        print_board(root->state);
    }

    // Generate next states and add them as children
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            Board next_state = root->state;

            // Check if the cell is already painted
            int index = y * SIZE + x;
            if (!(next_state & (1 << index)))
            {
                // Try painting white
                set_white(&next_state, x, y);

                // Create a new child node for the painted state
                TreeNode *child = malloc(sizeof(TreeNode));
                child->state = next_state;
                child->children = NULL;
                child->num_children = 0;

                // Add the child node
                root->children = realloc(root->children, (root->num_children + 1) * sizeof(TreeNode *));
                root->children[root->num_children++] = child;

                // Recursively explore the child node
                dfs(child);

                // Remove the child node if it leads to an invalid state
                if (!is_valid(child->state))
                {
                    free(child);
                    root->num_children--;
                }

                // Try painting black
                set_black(&next_state, x, y);

                // Create a new child node for the painted state
                child = malloc(sizeof(TreeNode));
                child->state = next_state;
                child->children = NULL;
                child->num_children = 0;

                // Add the child node
                root->children = realloc(root->children, (root->num_children + 1) * sizeof(TreeNode *));
                root->children[root->num_children++] = child;

                // Recursively explore the child node
                dfs(child);

                // Remove the child node if it leads to an invalid state
                if (!is_valid(child->state))
                {
                    free(child);
                    root->num_children--;
                }
            }
        }
    }
}

// Breadth-First Search algorithm
void bfs(TreeNode *root)
{
    if (root == NULL)
        return;

    // Queue for BFS
    TreeNode **queue = malloc(sizeof(TreeNode *) * SIZE * SIZE);
    int front = 0, rear = 0;

    // Enqueue the root
    queue[rear++] = root;

    while (front < rear)
    {
        // Dequeue a node
        TreeNode *current = queue[front++];

        if (is_valid(current->state))
        {
            // Found a valid state
            print_board(current->state);
        }

        // Enqueue children
        for (int i = 0; i < current->num_children; ++i)
        {
            queue[rear++] = current->children[i];
        }
    }

    free(queue);
}

// Free the memory used by the tree
void free_tree(TreeNode *root)
{
    if (root == NULL)
        return;

    for (int i = 0; i < root->num_children; ++i)
    {
        free_tree(root->children[i]);
    }

    free(root->children);
    free(root);
}

int main()
{
    Board initial = create_board();

    TreeNode *root = malloc(sizeof(TreeNode));
    root->state = initial;
    root->children = NULL;
    root->num_children = 0;

    printf("BFS:\n");
    bfs(root);

    count = 0;
    
    printf("DFS:\n");
    dfs(root);

    free_tree(root);

    return 0;
}
