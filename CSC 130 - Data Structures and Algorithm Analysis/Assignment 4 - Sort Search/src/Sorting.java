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
