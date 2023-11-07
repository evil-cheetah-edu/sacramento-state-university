public class Main
{
    public static void main(String[] args)
    {
        LinkedList<Integer> list = new LinkedList<>();

        list.add_to_back(2);
        list.add_to_back(3);
        list.add_to_back(10);
        list.add_to_back(20);

        LinkedList<Integer> list2 = list.split();

        list.traverse();
        System.out.println();
        list2.traverse();
    }
}
