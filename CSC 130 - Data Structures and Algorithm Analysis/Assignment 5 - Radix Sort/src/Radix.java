import java.util.Comparator;


public class Radix
{
    static int[] sort(int[] numbers, Comparator<Integer> comparator)
    {
        int digits = get_max_digits(numbers);


        Queue<Integer>[] buckets = new Queue[10];

        for (int i = 0; i < buckets.length; ++i)
            buckets[i] = new Queue<>();


        for (int d = 0; d < digits; ++d)
        {
            for (int number : numbers)
            {
                int digit = ( number / (int) Math.pow(10, d) ) % 10;
                buckets[
                    comparator.compare(Integer.MIN_VALUE, Integer.MAX_VALUE) > 0
                        ? 9 - digit
                        : digit
                ].enqueue( number );
            }

            int index = 0;
            for (Queue<Integer> bucket : buckets)
            {
                while ( !bucket.is_empty() )
                    numbers[index++] = bucket.dequeue();
            }
        }

        return numbers;
    }

    private static int get_max_digits(int[] numbers)
    {
        int max = Integer.MIN_VALUE;

        for ( int number : numbers )
        {
            if ( Math.abs(number) > max )
            {
                max = Math.abs(number);
            }
        }

        return ( (int) Math.log10(max) ) + 1;
    }
}
