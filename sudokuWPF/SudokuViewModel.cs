using System;
using System.Runtime.InteropServices;
using sudokuWPF.Models;

namespace sudokuWPF
{

    public class SudokuViewModel : BindableBase
    {

        private Board _board;

        public SudokuViewModel(int boxLength)
        {
            _board = new Board(boxLength);
        }

        public bool reset()
        {
            var result = _board.reset();
            NotifyPropertyChanged("");
            return result;
        }

        public bool newBoard()
        {
            var result = _board.newBoard();
            NotifyPropertyChanged("");
            return result;
        }

        public bool check()
        {
            return _board.check();
        }

        public string this[int i, int j]
        {
            get
            {
                if (_board[i, j] == 0)
                {
                    return "";
                }
                return _board[i, j].ToString();
            }
            set
            {
                int val = 0;
                if (Int32.TryParse(value, out val))
                {
                    int prevValue = _board[i, j];
                    if (val < 0 || val > _board.boxLength * _board.boxLength)
                    {
                        //int x = _board[i, j];
                        //SetProperty(ref x, prevValue);
                        _board[i, j] = prevValue;
                    }
                    else
                    {
                        //int x = _board[i, j];
                        //SetProperty(ref x, val);
                        _board[i, j] = val;
                    }
                }
                else
                {
                    int x = _board[i, j];
                    SetProperty(ref x, 0);
                }
            }
        }
    }
}
