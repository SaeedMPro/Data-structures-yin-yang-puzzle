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

// Count number of cell that paint
int count_cell(Board board)
{
    int num = 0;
    for (int i = 0; i < SIZE * SIZE; ++i)
    {
        if ((board & (1 << i)) != 0)
        {
            num++;
        }
    }

    return num;
}


// Check if cell i is connected to any other cell of the same color
bool connected(Board board, int visited[], int i)
{
    if (visited[i])
    {
        return false;
    }

    visited[i] = 1;

    int x = i % SIZE, y = i / SIZE;
    int index = y * SIZE + x;

    // Check neighboring cells
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }

            int new_x = x + dx, new_y = y + dy;
            if (new_x >= 0 && new_x < SIZE && new_y >= 0 && new_y < SIZE)
            {
                int neighbor_index = new_y * SIZE + new_x;
                if (((board & (1 << index)) && (board & (1 << neighbor_index))) ||
                    ((board & (1 << (index + 16))) && (board & (1 << (neighbor_index + 16)))))
                {
                    if (connected(board, visited, neighbor_index))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

// Check if board satisfies continuity condition
bool check_connectivity(Board board)
{
    // Count number of white and black cells
    int white_cells = 0, black_cells = 0;
    for (int i = 0; i < SIZE * SIZE; ++i)
    {
        if ((board & (1 << i)) != 0)
        {
            white_cells++;
        }
        else if ((board & (1 << (i + 16))) != 0)
        {
            black_cells++;
        }
    }

    if (white_cells == 0 || black_cells == 0)
    {
        // If there are no white or black cells, the condition is satisfied
        return true;
    }

    // Check if white and black cells are connected
    int visited[SIZE * SIZE] = {0};
    for (int i = 0; i < SIZE * SIZE; ++i)
    {
        if (connected(board, visited, i))
        {
            return true;
        }
    }

    return false;
}
// Check 2*2 square 
bool check_square(Board board ){

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
// Check if board satisfies conditions
bool is_valid(Board board)
{
    // Checking the condition that the board is completely full
    if (count_cell(board) != 16)
    {
        return false;
    }
    // Check condition 1 & 2
    if (check_square(board) && check_connectivity(board))
    {   
        return true;
    }

    return false;
}

bool condition_continue(Board board)
{

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
    printf("DFS:\n");

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
                if (!condition_continue(child->state))
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
                if (!condition_continue(child->state))
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
    printf("BFS:\n");
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

        // Generate next states and add them as children
        for (int x = 0; x < SIZE; x++)
        {
            for (int y = 0; y < SIZE; y++)
            {
                Board next_state = current->state;

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
                    current->children = realloc(current->children, (current->num_children + 1) * sizeof(TreeNode *));
                    current->children[current->num_children++] = child;

                    // Recursively explore the child node
                    if (is_valid(child->state))
                    {
                        // Found a valid state
                        print_board(child->state);
                    }

                    // Try painting black
                    set_black(&next_state, x, y);

                    // Create a new child node for the painted state
                    child = malloc(sizeof(TreeNode));
                    child->state = next_state;
                    child->children = NULL;
                    child->num_children = 0;

                    // Add the child node
                    current->children = realloc(current->children, (current->num_children + 1) * sizeof(TreeNode *));
                    current->children[current->num_children++] = child;

                    // Recursively explore the child node
                    if (is_valid(child->state))
                    {
                        // Found a valid state
                        print_board(child->state);
                    }
                }
            }
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

    printf("DFS:\n");
    dfs(root);
    count = 0;
    printf("BFS:\n");
    bfs(root);

    free_tree(root);

    return 0;
}
