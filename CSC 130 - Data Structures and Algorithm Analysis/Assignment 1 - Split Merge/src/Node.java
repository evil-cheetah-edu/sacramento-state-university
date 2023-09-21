public class Node<T>
{
    private T       value;
    private Node<T> next;


    Node ()
    {
        this.value = null;
    }

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

    Node<T> get_next()
    {
        return next;
    }

    void set_next(Node next)
    {
        this.next = next;
    }
}