/// Constants.java
public class Constants
{
    public static int[][] BACK_UP_MATRIX = {
        {5, 3,  2, 16},
        {9, 8, 10, 17},
        {4, 7, 11, 18},
        {2, 5,  9, 12},
        {7, 9,  4, 10}
    };
}



/// Matrix.java
public class Matrix
{
    private int[][] matrix;


    Matrix(int rows, int columns)
    {
        matrix = new int[rows][columns];

        for( int i = 0; i < rows; ++i )
            for( int j = 0; j < columns; ++j)
                this.matrix[i][j] = 0;
    }


    Matrix(int[][] matrix)
    {
        this.set_matrix(matrix);
    }


    void traverse()
    {
        int max_length = this.max_length();

        for( int[] row : matrix )
        {
            for( int item : row )
                System.out.print(
                    String.format('%' + Integer.toString(max_length) + "d ", item)
                );

            System.out.println();
        }
    }

    void traverse_column(int column_index)
    {
        int max_length = this.max_length();

        for( int[] row : matrix )
            System.out.println(
                    String.format('%' + Integer.toString(max_length) + "d ", row[column_index])
            );
    }

    int[][] get_matrix()
    {
        int[][] matrix_copy = new int[this.matrix.length][];

        for ( int i = 0; i < this.matrix.length; ++i )
            matrix_copy[i] = this.matrix[i].clone();
        
        return this.matrix;
    }

    void set_matrix(int[][] matrix)
    {
        this.matrix = new int[matrix.length][];

        for( int i = 0; i < matrix.length; ++i )
            this.matrix[i] = matrix[i].clone();
    }


    private int max_length()
    {
        int max_length = 0;
        for( int[] row : matrix )
        {
            for( int item : row )
            {
                int item_length = Integer.toString(item).length();

                if ( item_length > max_length )
                    max_length = item_length;
            }
        }

        return max_length;
    }
}



/// Searching.java
public class Searching
{
    static int binary_search(Matrix m, int row_index, int key)
    {
        int[][] matrix = m.get_matrix();

        int low = 0,
            high = matrix[row_index].length - 1,
            mid;

        while( high >= low )
        {
            mid = low / 2 + high / 2;

            if ( key == matrix[row_index][mid] )
                return mid;

            if ( key > matrix[row_index][mid] )
            {
                low = mid + 1;
                continue;
            }

            high = mid - 1;
        }

        return -1;
    }
}



/// Sorting.java
import java.util.Comparator;


public class Sorting
{
    static Matrix bubble_sort(Matrix m, int column_index, Comparator<Integer> comparator)
    {
        int[][] matrix = m.get_matrix();
        int     limit  = matrix.length - 1;

        for(; limit > 0; --limit )
            for( int i = 0; i < matrix.length - 1; ++i )
                if (
                    comparator.compare( matrix[i][column_index], matrix[i + 1][column_index] ) > 0
                )
                {
                    int temporary               = matrix[  i  ][column_index];
                    matrix[  i  ][column_index] = matrix[i + 1][column_index];
                    matrix[i + 1][column_index] = temporary;
                }

        return new Matrix(matrix);
    }


    static Matrix selection_sort(Matrix m, int column_index, Comparator<Integer> comparator)
    {
        int[][] matrix = m.get_matrix();
        int     limit  = matrix.length - 1;

        for(; limit > 0; --limit)
        {
            int largest_index = 0;

            for( int i = 0; i <= limit; ++i )
                if (
                    comparator.compare( matrix[i][column_index], matrix[largest_index][column_index] ) > 0
                )
                    largest_index = i;

            if ( limit != largest_index )
            {
                int temporary                       = matrix[limit][column_index];
                matrix[limit][column_index]         = matrix[largest_index][column_index];
                matrix[largest_index][column_index] = temporary;
            }
        }

        return new Matrix(matrix);
    }


    static Matrix insertion_sort(Matrix m, int row_index, Comparator<Integer> comparator)
    {
        int[][] matrix = m.get_matrix();

        for ( int i = 1; i < matrix[row_index].length; ++i )
            for (
                int j = i;
                (j > 0) && (comparator.compare( matrix[row_index][j - 1], matrix[row_index][j] ) > 0);
                --j
            )
            {
                int temporary            = matrix[row_index][j];
                matrix[row_index][  j  ] = matrix[row_index][j - 1];
                matrix[row_index][j - 1] = temporary;
            }

        return new Matrix(matrix);
    }

    static Matrix shell_sort(Matrix m, int column_index, Comparator<Integer> comparator)
    {
        int[][] matrix = m.get_matrix();

        for( int gap = matrix.length / 2; gap >= 1; gap /= 2)
            for( int i = gap; i < matrix.length; ++i )
                for(
                    int j = i;
                    (
                        (j >= gap) &&
                        (comparator.compare(matrix[j - gap][column_index], matrix[j][column_index]) > 0)
                    );
                    j -= gap
                )
                {
                    int temporary                 = matrix[j - gap][column_index];
                    matrix[j - gap][column_index] = matrix[   j   ][column_index];
                    matrix[   j   ][column_index] = temporary;
                }

        return new Matrix(matrix);
    }
}



/// Main.java
import java.util.Comparator;
import java.util.Scanner;


public class Main
{
    public static void main(String[] args)
    {
        /// Init
        System.out.println("Initial Matrix:");
        Matrix matrix = new Matrix(Constants.BACK_UP_MATRIX);
        matrix.traverse();

        System.out.println();
        matrix.set_matrix(Constants.BACK_UP_MATRIX);

        /// Bubble Sort
        System.out.println("Matrix After Bubble Sort:");
        matrix = Sorting.bubble_sort(matrix, 0, Comparator.naturalOrder());
        matrix.traverse();

        System.out.println();
        matrix.set_matrix(Constants.BACK_UP_MATRIX);

        /// Selection Sort
        System.out.println("Matrix After Selection Sort:");
        matrix = Sorting.selection_sort(matrix, 1, Comparator.reverseOrder());
        matrix.traverse();

        System.out.println();
        matrix.set_matrix(Constants.BACK_UP_MATRIX);

        /// Insertion Sort
        System.out.println("Matrix After Insertion Sort:");
        matrix = Sorting.insertion_sort(matrix, 4, Comparator.naturalOrder());
        matrix.traverse();

        System.out.println();
        matrix.set_matrix(Constants.BACK_UP_MATRIX);

        /// Shell Sort
        System.out.println("Matrix After Shell Sort:");
        matrix = Sorting.shell_sort(matrix, 2, Comparator.naturalOrder());
        matrix.traverse();

        System.out.println();
        matrix.set_matrix(Constants.BACK_UP_MATRIX);


        /// Binary Search on Insertion Sort Matrix
        System.out.println("Binary Search on Matrix After Insertion Sort:");
        matrix           = Sorting.insertion_sort(matrix, 4, Comparator.naturalOrder());
        Scanner input    = new Scanner(System.in);
        int column_index = Searching.binary_search(matrix, 4, input.nextInt());
        matrix.traverse_column(column_index);
    }
}
