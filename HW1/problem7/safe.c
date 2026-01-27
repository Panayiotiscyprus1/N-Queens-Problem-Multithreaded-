#define N 8

int safe(int row, int col, int **board){
    // Check this row on left side
    for(int j=col; j>=0; j--){
        if(board[row][j] == 1) return 0;
    }
    // Check upper diagonal on left side
    for(int i=row, j=col; i>=0 && j>=0; i--, j--){
        if(board[i][j]==1) return 0;
    }
    // Check lower diagonal on left side
    for(int i=row, j=col; i<N && j>=0; i++, j--){
        if(board[i][j]==1) return 0;
    }
    return 1;
}