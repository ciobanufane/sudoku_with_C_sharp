using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Controls.Primitives;

/*
 * References
 * https://stackoverflow.com/questions/54055388/sudoku-in-wpf-what-base-element-should-i-use-for-the-table
 * https://stackoverflow.com/questions/36149863/how-to-write-a-viewmodelbase-in-mvvm
 */

namespace sudokuWPF
{ 
    public partial class MainWindow : Window
    {

        private const int InnerWidth = 3;
        private const int OuterWidth = InnerWidth * InnerWidth;

        private const int Thin = 1;
        private const int Thick = 3;

        public MainWindow()
        {
            InitializeComponent();
            InitializeViewModel();
            InitializeSudokuTable();
        }

        public SudokuViewModel ViewModel => (SudokuViewModel)DataContext;

        private void InitializeViewModel()
        { 
            DataContext = new SudokuViewModel(InnerWidth);
        }

        private void InitializeSudokuTable()
        {
            var grid = new UniformGrid
            {
                Rows = OuterWidth,
                Columns = OuterWidth
            };

            for (var i = 0; i < OuterWidth; i++)
            {
                for (var j = 0; j < OuterWidth; j++)
                {
                    var border = CreateBorder(i, j);
                    
                    var viewBox = new Viewbox { Stretch = Stretch.Fill };
                    viewBox.Child = CreateTextBox(i, j);
                    border.Child = viewBox;

                    grid.Children.Add(border);
                }
            }

            SudokuTable.Child = grid;
        }

        private static Border CreateBorder(int i, int j)
        {
            var left = j % InnerWidth == 0 ? Thick : Thin;
            var top = i % InnerWidth == 0 ? Thick : Thin;
            var right = j == OuterWidth - 1 ? Thick : 0;
            var bottom = i == OuterWidth - 1 ? Thick : 0;

            return new Border
            {
                BorderThickness = new Thickness(left, top, right, bottom),
                BorderBrush = Brushes.Black
            };
        }

        private TextBox CreateTextBox(int i, int j)
        {
            var readOnly = ViewModel[i, j] != "";
            var color = ViewModel[i, j] != "" ? Brushes.Red : Brushes.Black;

            var textBox = new TextBox
            {
                VerticalAlignment = VerticalAlignment.Stretch,
                HorizontalAlignment = HorizontalAlignment.Stretch,
                TextAlignment = TextAlignment.Center,
                AcceptsReturn = false,
                FontSize = 15,
                IsReadOnly = readOnly,
                Foreground = color
            };

            var binding = new Binding
            {
                Source = ViewModel,
                Path = new PropertyPath($"[{i},{j}]"),
                Mode = BindingMode.TwoWay
            };

            textBox.SetBinding(TextBox.TextProperty, binding);

            return textBox;
        }

        private void NewBoardButton_Click(object sender, RoutedEventArgs e)
        {
            ViewModel.newBoard();
            InitializeSudokuTable();
        }

        private void ResetBoardButton_Click(object sender, RoutedEventArgs e)
        {
            ViewModel.reset();
        }

        private void CheckBoardButton_Click(object sender, RoutedEventArgs e)
        {
            if (ViewModel.check())
            {
                MessageBox.Show("Correct Sudoku");
            } 
            else
            {
                MessageBox.Show("Incorrect Sudoku");
            }
        }
    }
}
