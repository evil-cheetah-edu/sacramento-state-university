import java.util.Comparator;

public class Main
{
    public static void main(String[] args)
    {
        System.out.println("Initial Matrix:");
        Matrix matrix = new Matrix(Constants.BACK_UP_MATRIX);
        matrix.traverse();

        System.out.println();
        matrix.set_matrix(Constants.BACK_UP_MATRIX);

        System.out.println("Matrix After Bubble Sort:");
        matrix = Sorting.bubble_sort(matrix, 0, Comparator.naturalOrder());
        matrix.traverse();

        System.out.println();
        matrix.set_matrix(Constants.BACK_UP_MATRIX);

        System.out.println("Matrix After Selection Sort:");
        matrix = Sorting.selection_sort(matrix, 1, Comparator.reverseOrder());
        matrix.traverse();

        System.out.println();
        matrix.set_matrix(Constants.BACK_UP_MATRIX);

        System.out.println("Matrix After Insertion Sort:");
        matrix = Sorting.insertion_sort(matrix, 4, Comparator.naturalOrder());
        matrix.traverse();

        System.out.println();
    }
}
