public class Main
{
    public static void main(String[] args)
    {
        Node<String> node1 = new Node<>("Hello");
        Node<String> node2 = new Node<>("World");

        node1.set_next(node2);

        Node<String> temp = node1;

        while (temp != null)
        {
            System.out.println(temp.get_value());
            temp = temp.get_next();
        }
    }
}
