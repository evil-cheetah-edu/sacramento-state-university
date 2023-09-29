public class Node<T>
{
    private Node<T> prev;
    private T       value;
    private Node<T> next;


    Node()
    {
        value = null;
        prev  = null;
        next  = null;
    }

    Node(T value)
    {
        this.value = value;
        this.prev  = null;
        this.next  = null;
    }


    T get_value()
    {
        return value;
    }

    Node<T> get_prev()
    {
        return prev;
    }

    Node<T> get_next()
    {
        return next;
    }


    void set_value(T value)
    {
        this.value = value;
    }

    void set_prev(Node<T> prev)
    {
        this.prev = prev;
    }

    void set_next(Node<T> next)
    {
        this.next = next;
    }
}
