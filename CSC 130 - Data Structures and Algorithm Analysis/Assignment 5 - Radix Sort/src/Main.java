import java.util.Arrays;
import java.util.Comparator;


public class Main
{
    public static void main(String[] args)
    {
        int[] numbers = {21, 111, 22, 8, 3333, 4, 77, 55555, 1, 99999};

        int[] numbers2;

        System.out.println("Original Array:");
        System.out.println( '\t' + Arrays.toString(numbers) );
        System.out.println();

        System.out.println("Sorted Array (Ascending Order):");
        System.out.println(
                '\t'                                                              +
                Arrays.toString( Radix.sort(numbers, Comparator.naturalOrder()) ) +
                '\n'
        );

        System.out.println("Sorted Array (Descending Order):");
        System.out.println(
                '\t'                                                              +
                Arrays.toString( Radix.sort(numbers, Comparator.reverseOrder()) ) +
                '\n'
        );
    }
}