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
