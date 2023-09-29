public class Main
{
    public static void main(String[] args)
    {
        Node<String> node1 = new Node<>("Adam");
        Node<String> node2 = new Node<>("James");

        node1.set_next(node2);
        node2.set_prev(node1);

        System.out.println("Node 1:");
        System.out.println("\tAddress: " + node1);
        System.out.println("\tPrev:    " + node1.get_prev());
        System.out.println("\tValue:   " + node1.get_value());
        System.out.println("\tNext:    " + node1.get_next());

        System.out.println();

        System.out.println("Node 2:");
        System.out.println("\tAddress: " + node2);
        System.out.println("\tPrev:    " + node2.get_prev());
        System.out.println("\tValue:   " + node2.get_value());
        System.out.println("\tNext:    " + node2.get_next());

    }
}
