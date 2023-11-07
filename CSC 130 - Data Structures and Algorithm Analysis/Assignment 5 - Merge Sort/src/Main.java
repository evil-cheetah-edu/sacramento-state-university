import java.util.Comparator;


public class Main
{
    public static void main(String[] args)
    {
        LinkedList<Integer> list = new LinkedList<>();

        list.add_to_back(10);
        list.add_to_back(-1);
        list.add_to_back(3);
        list.add_to_back(20);
        list.add_to_back(2);

        list.traverse();
        System.out.println();

        list = MergeSort.sort(list, Comparator.naturalOrder());
        list.traverse();
    }
}
