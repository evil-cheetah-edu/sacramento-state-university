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
        this.matrix = new int[matrix.length][];

        for( int i = 0; i < matrix.length; ++i )
            this.matrix[i] = matrix[i].clone();
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
