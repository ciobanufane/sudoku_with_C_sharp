namespace sudokuWPF.Models
{

    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct BoardCell
    {
        public int originalValue;
        public int currentValue;
        public int solutionValue;
    }

    class Board : BindableBase
    {
        [DllImport(@"sudoku.dll")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool solveBoard([In, Out] BoardCell[,] board, int boxLength, int boardPosition, ref int solutions, bool uniqueFlag);

        [DllImport(@"sudoku.dll")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool populateBoard([In, Out] BoardCell[,] board, int boxLength);

        [DllImport(@"sudoku.dll")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool createBoard([In, Out] BoardCell[,] board, int boxLength, int pairCluesRemoved);

        [DllImport(@"sudoku.dll")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool resetBoard([In, Out] BoardCell[,] board, int boxLength);

        [DllImport(@"sudoku.dll")]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool checkBoard(BoardCell[,] board, int boxLength);

        private BoardCell[,] _board;
        private readonly int _boxLength;

        public Board(int boxLength)
        {
            int boardLength = boxLength * boxLength;

            _boxLength = boxLength;
            _board = new BoardCell[boardLength, boardLength];
            createBoard(_board, boxLength, 20);
        }

        public bool reset()
        {
            return resetBoard(_board, _boxLength);
        }

        public bool newBoard(){
            return createBoard(_board, _boxLength, 20);
        }

        public bool check()
        {
            return checkBoard(_board, _boxLength);
        }

        public int boxLength
        {
            get => _boxLength;
        }

        public int this[int i, int j]
        {
            get => _board[i, j].currentValue;
            set => _board[i, j].currentValue = value;
        }

    }
}
