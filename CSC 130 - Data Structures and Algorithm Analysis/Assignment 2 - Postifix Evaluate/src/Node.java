public class Node<T>
{
    private T       value;
    private Node<T> next;


    Node()
    {
        value = null;
        next  = null;
    }

    Node(T value)
    {
        this.value = value;
        this.next  = null;
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

    void set_next(Node<T> next)
    {
        this.next = next;
    }
}
