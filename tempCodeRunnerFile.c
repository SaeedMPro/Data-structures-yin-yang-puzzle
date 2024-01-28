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
