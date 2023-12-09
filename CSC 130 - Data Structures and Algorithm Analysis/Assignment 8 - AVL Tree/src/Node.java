public class Node<T>
{
    private T       value;
    private Node<T> left;
    private Node<T> right;

    private int     height;


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

    int get_height()
    {
        return height;
    }

    void set_left(Node<T> node)
    {
        this.left = node;
    }

    void set_right(Node<T> node)
    {
        this.right = node;
    }

    void set_height(int height)
    {
        this.height = height;
    }
}