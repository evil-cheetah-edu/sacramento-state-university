import java.util.Comparator;


public class MergeSort
{
   static LinkedList<Integer> sort(LinkedList<Integer> items, Comparator<Integer> comparator)
   {
        if (items.size() == 0 || items.size() == 1)
            return items;

        LinkedList<Integer> left  = items.split();
        LinkedList<Integer> right = items;

        left  = sort(left,  comparator);
        right = sort(right, comparator);

        return merge(left, right, comparator);
   }


    private static LinkedList<Integer> merge(
        LinkedList<Integer> left,
        LinkedList<Integer> right,
        Comparator<Integer> comparator
    )
    {
        LinkedList<Integer> result = new LinkedList<>();

        while ( !left.is_empty() && !right.is_empty() )
        {
            if ( comparator.compare(left.get_head_value(), right.get_head_value()) < 0 )
            {
                result.add_to_back(left.get_head_value());
                left.pop_in_front();
            }
            else
            {
                result.add_to_back(right.get_head_value());
                right.pop_in_front();
            }
        }

        if ( left != null )
            result.merge(left);

        if ( right != null )
            result.merge(right);

        return result;
    }
}
