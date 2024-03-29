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
    for (int i = 0; i < 16; ++i)
    {
        if ((board & (1 << i)) != 0)
        {
            num++;
        }
    }

    return num;
}

// Helper function to check if an array contains a specific element
bool contains(int arr[], int size, int element)
{
    for (int i = 0; i < size; ++i)
    {
        if (arr[i] == element)
        {
            return true;
        }
    }
    return false;
}
// Check connectivity of cells using DFS
bool check_connectivity(Board board, int target)
{
    int visited[SIZE * SIZE];
    for (int i = 0; i < SIZE * SIZE; ++i)
    {
        visited[i] = 0;
    }

    int cells[SIZE * SIZE];
    int num_cells = 0;

    // Collect cell indices
    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            int index = y * SIZE + x;
            if (target)
            {
                if (!(board & (1 << index + 16)) == !target)
                {
                    cells[num_cells++] = index;
                }
            }
            else
            {
                if ((board & (1 << index + 16)) == target)
                {
                    cells[num_cells++] = index;
                }
            }
        }
    }

    if (num_cells > 0)
    {
        // Perform DFS
        int stack[SIZE * SIZE];
        int top = -1;

        int startCell = cells[0];
        stack[++top] = startCell;

        while (top >= 0)
        {
            int currentCell = stack[top--];
            visited[currentCell] = 1;

            // Get the indices of neighboring cells
            int currentX = currentCell % SIZE;
            int currentY = currentCell / SIZE;
            int indexUpCell = currentCell - SIZE;
            int indexDownCell = currentCell + SIZE;
            int indexLeftCell = currentCell - 1;
            int indexRightCell = currentCell + 1;

            // Check if any neighboring cell is in the same set of cells
            if (contains(cells, num_cells, indexUpCell) && !visited[indexUpCell])
                stack[++top] = indexUpCell;
            if (contains(cells, num_cells, indexDownCell) && !visited[indexDownCell])
                stack[++top] = indexDownCell;
            if (contains(cells, num_cells, indexLeftCell) && !visited[indexLeftCell] && currentCell % SIZE != 0)
                stack[++top] = indexLeftCell;
            if (contains(cells, num_cells, indexRightCell) && !visited[indexRightCell] && (currentCell + 1) % SIZE != 0)
                stack[++top] = indexRightCell;
        }

        for (int i = 0; i < num_cells; ++i)
        {
            if (!visited[cells[i]])
            {
                return false;
            }
        }

        return true;
    }
    else
    {
        return false; // No cells with the target color, not connected.
    }
}

// Check 2*2 square
bool check_square(Board board)
{
    // Iterate over each possible 2x2 square
    for (int y = 0; y < SIZE - 1; y++)
    {
        for (int x = 0; x < SIZE - 1; x++)
        {
            int index = y * SIZE + x;

            // Extract the colors of the cells in the square
            uint32_t colorTopLeft = (board >> (index + 16)) & 1;
            uint32_t colorTopRight = (board >> (index + 1 + 16)) & 1;
            uint32_t colorBottomLeft = (board >> (index + SIZE + 16)) & 1;
            uint32_t colorBottomRight = (board >> (index + SIZE + 1 + 16)) & 1;

            // Check if the colors are the same for all cells
            if (colorTopLeft == colorTopRight && colorTopLeft == colorBottomLeft && colorTopLeft == colorBottomRight)
            {
                // Colors are the same, return false (violation)
                return false;
            }
        }
    }
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
    if (check_square(board) && check_connectivity(board, 0) && check_connectivity(board, 1))
    {
        return true;
    }

    return false;
}

// bool condition_continue(Board board)
// {
//     if (check_square(board))
//     {
//         return true;
//     }
//     else
//         return false;

//     return true;
// }

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

    printf("==========================Number of Board: %d==========================\n\n\n", ++count);
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
        print_board(root->state);
    }

    // Generate next states and add them as children
    for (int i = 0; i < SIZE * SIZE; i++)
    {
        Board next_state = root->state;

        // Check if the cell is already painted
        if (!(next_state & (1 << i)))
        {
            // Try painting black
            set_black(&next_state, i % SIZE, i / SIZE);

            // Create a new child node for the painted state
            TreeNode *child = malloc(sizeof(TreeNode));
            child->state = next_state;
            child->children = NULL;
            child->num_children = 0;

            // Add the child node
            root->children = realloc(root->children, (root->num_children + 1) * sizeof(TreeNode *));
            root->children[root->num_children++] = child;

            // Remove the child node if it leads to an invalid state
            // if (!condition_continue(child->state))
            // {
            //     free(child);
            //     root->num_children--;
            //     printf("in1");
            // }
            // else 
            dfs(child);    // Recursively explore the child node

            // Try painting white
            set_white(&next_state, i % SIZE, i / SIZE);

            // Create a new child node for the painted state
            child = malloc(sizeof(TreeNode));
            child->state = next_state;
            child->children = NULL;
            child->num_children = 0;

            // Add the child node
            root->children = realloc(root->children, (root->num_children + 1) * sizeof(TreeNode *));
            root->children[root->num_children++] = child;

            // Remove the child node if it leads to an invalid state
            // if (!condition_continue(child->state))
            // {
            //     free(child);
            //     root->num_children--;
            //     printf("in2");

            // }
            // else 
            dfs(child);    // Recursively explore the child node
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

        // Process the current node
        if (is_valid(root->state))
        {
            print_board(root->state);
        }

        // Generate next states and add them as children
        for (int i = 0; i < SIZE * SIZE; i++)
        {
            Board next_state = current->state;

            // Check if the cell is already painted
            if (!(next_state & (1 << i)))
            {
                // Try painting black
                set_black(&next_state, i % SIZE, i / SIZE);

                // Create a new child node for the painted state
                TreeNode *child = malloc(sizeof(TreeNode));
                child->state = next_state;
                child->children = NULL;
                child->num_children = 0;

                // Add the child node
                current->children = realloc(current->children, (current->num_children + 1) * sizeof(TreeNode *));
                current->children[current->num_children++] = child;

                // Enqueue the child node
                queue[rear++] = child;

                // Try painting white
                set_white(&next_state, i % SIZE, i / SIZE);

                // Create a new child node for the painted state
                child = malloc(sizeof(TreeNode));
                child->state = next_state;
                child->children = NULL;
                child->num_children = 0;

                // Add the child node
                current->children = realloc(current->children, (current->num_children + 1) * sizeof(TreeNode *));
                current->children[current->num_children++] = child;

                // Enqueue the child node
                queue[rear++] = child;
            }
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
    free_tree(root);

    count = 0;

    printf("DFS:\n");
    dfs(root);
    free_tree(root);
    return 0;
}
