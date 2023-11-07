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

        System.out.println("Original Linked List:");
        list.traverse();
        System.out.println();

        System.out.println("Sorted Linked List(Ascending):");
        list = MergeSort.sort(list, Comparator.naturalOrder());
        list.traverse();
        System.out.println();

        System.out.println("Sorted Linked List(Descending):");
        list = MergeSort.sort(list, Comparator.reverseOrder());
        list.traverse();
    }
}
