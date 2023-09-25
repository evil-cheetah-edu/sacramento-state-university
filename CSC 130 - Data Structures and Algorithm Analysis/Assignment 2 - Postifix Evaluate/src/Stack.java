public class Stack<T>
{
    private Node<T> head;


    void push(T value)
    {
        Node<T> new_node = new Node<>();

        new_node.set_value(value);
        new_node.set_next(head);

        head = new_node;
    }

    T pop()
    {
        if ( is_stack_empty() )
            throw new RuntimeException( ExceptionMessage.EMPTY_STACK_EXCEPTION_MESSAGE );

        Node<T> temp = head;

        head = head.get_next();
        temp.set_next(null);

        return temp.get_value();
    }


    T peek()
    {
        if ( is_stack_empty() )
            throw new RuntimeException( ExceptionMessage.EMPTY_STACK_EXCEPTION_MESSAGE );

        return head.get_value();
    }

    private boolean is_stack_empty()
    {
        return (head == null);
    }
}
