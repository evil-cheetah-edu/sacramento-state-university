public class Queue<T>
{
    private Node<T> front;
    private Node<T> rear;


    void enqueue(T value)
    {
        Node<T> new_node = new Node<>();

        new_node.set_value(value);
        new_node.set_next(null);

        if ( front == null )
        {
            front = rear = new_node;
            return;
        }

        rear.set_next(new_node);
        rear = new_node;
    }

    T dequeue()
    {
        if ( is_empty() )
        {
            rear = null;
            throw new RuntimeException(ExceptionMessage.EMPTY_QUEUE_EXCEPTION_MESSAGE);
        }

        Node<T> temp = front;
        front = front.get_next();
        temp.set_next(null);

        return temp.get_value();
    }

    T peek()
    {
        if ( is_empty() )
        {
            rear = null;
            throw new RuntimeException(ExceptionMessage.EMPTY_QUEUE_EXCEPTION_MESSAGE);
        }

        return front.get_value();
    }

    String to_string()
    {
        String result = "";

        Node<T> current = front;

        while ( current != null )
        {
            result  += current.get_value() + " ";
            current  = current.get_next();
        }

        return result;
    }


    boolean is_empty()
    {
        return ( front == null );
    }
}