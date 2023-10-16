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
}
