public class Node<T>
{
    private T    value;
    private Node next;

    Node(T value)
    {
        this.value = value;
    }

    T get_value()
    {
        return value;
    }

    void set_value(T value)
    {
        this.value = value;
    }

    Node get_next()
    {
        return next;
    }

    void set_next(Node next)
    {
        this.next = next;
    }
}