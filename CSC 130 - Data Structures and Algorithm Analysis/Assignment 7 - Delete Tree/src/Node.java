public class Node<T>
{
    private T       value;
    private Node<T> left;
    private Node<T> right;


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

    Node<T> get_left()
    {
        return left;
    }

    Node<T> get_right()
    {
        return right;
    }

    void set_left(Node<T> node)
    {
        this.left = node;
    }

    void set_right(Node<T> node)
    {
        this.right = node;
    }
}