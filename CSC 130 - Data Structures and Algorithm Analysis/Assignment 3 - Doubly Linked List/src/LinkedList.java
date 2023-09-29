public class LinkedList
{
    private Node<String> head;
    private Node<String> tail;


    void insert(String value)
    {
        Node<String> new_node = new Node(value);

        if ( head == null )
        {
            head = tail = new_node;
            return;
        }

        if ( value.compareToIgnoreCase( head.get_value() ) < 0 )
        {
            head.set_prev( new_node );
            new_node.set_next( head );
            head = new_node;

            return;
        }

        if ( value.compareToIgnoreCase( tail.get_value() ) > 0 )
        {
            tail.set_next( new_node );
            new_node.set_prev( tail );
            tail = new_node;

            return;
        }

        Node<String> current = head.get_next();

        while (
                value.compareToIgnoreCase( current.get_value() ) > 0 &&
                current != null
        )
            current = current.get_next();

        new_node.set_next( current.get_prev().get_next() );
        new_node.set_prev( current.get_prev() );
        new_node.get_prev().set_next( new_node );
        new_node.get_next().set_prev( new_node );
    }

    void remove(String value)
    {
        Node<String> current = head;

        if ( current == null )
        {
            System.out.println("Linked List is empty...");
            throw new RuntimeException(ExceptionMessage.FAIL_TO_DELETE);
        }

        while ( current != null )
        {
            if ( value.compareToIgnoreCase(current.get_value() ) != 0 )
            {
                current = current.get_next();
                continue;
            }


            /// Between `head` and `tail`
            if ( current.get_next() != null && current.get_prev() != null )
            {
                current.get_prev().set_next( current.get_next() );
                current.get_next().set_prev( current.get_prev() );
            }
            /// Is Head
            else if ( current.get_prev() == null )
            {
                current.get_next().set_prev( null );
                head = current.get_next();
            }
            /// Is Tail
            else if ( current.get_next() == null )
            {
                current.get_prev().set_next( null );
                tail = current.get_prev();
            }

            return;
        }

        System.out.println("No String with following criteria: `" + value + "`");
    }

    void traverse()
    {
        this.traverse(OrderingType.ASCENDING);
    }

    void traverse(OrderingType order)
    {
        Node<String> current;

        switch (order)
        {
            case ASCENDING:
            {
                current = head;
                while (current != null)
                {
                    System.out.println( current.get_value() );
                    current = current.get_next();
                }
                break;
            }

            case DESCENDING:
            {
                current = tail;
                while (current != null)
                {
                    System.out.println( current.get_value() );
                    current = current.get_prev();
                }
                break;
            }

            default:
                throw new RuntimeException(ExceptionMessage.UNKNOWN_ORDER);
        }
    }
}
