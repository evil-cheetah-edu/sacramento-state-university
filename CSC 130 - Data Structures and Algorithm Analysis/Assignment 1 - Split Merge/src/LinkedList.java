public class LinkedList
{
    private Node<String> head;

    void add_to_front(String value)
    {
        Node<String> new_node = new Node<>();

        new_node.set_value(value);
        new_node.set_next(head);

        head = new_node;
    }
}
