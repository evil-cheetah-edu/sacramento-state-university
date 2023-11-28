import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;


public class BinaryTree
{
    private Node<Integer> root;


    BinaryTree()
    {
        root = null;
    }


    Node<Integer> get_root()
    {
        return root;
    }

    void set_root(Node<Integer> node)
    {
        root = node;
    }


    void read_file(String filename)
    {
        Scanner scanner;

        try
        {
            File file = new File(filename);
            scanner   = new Scanner(file);
        }

        catch (FileNotFoundException e)
        {
            throw new RuntimeException(ExceptionMessage.FILE_NOT_FOUND);
        }

        while ( scanner.hasNext() )
        {
            String next = scanner.next();

            if ( next.equalsIgnoreCase("Delete") )
            {
                delete( scanner.nextInt() );
                continue;
            }

            insert( Integer.parseInt(next) );
        }
    }


    void insert(int value)
    {
            root = insert(root, new Node<>(value));
    }

    void delete(int value)
    {
        root = delete(root, value);
    }


    void traverse(TraversalType type)
    {
        switch (type)
        {
            case PRE_ORDER:
                pre_order_traversal(root);
                return;

            case IN_ORDER:
                in_order_traversal(root);
                return;

            case POST_ORDER:
                post_order_traversal(root);
                return;

            default:
                throw new RuntimeException(ExceptionMessage.UNKNOWN_TRAVERSAL_TYPE);
        }
    }


    private Node<Integer> insert(Node<Integer> current, Node<Integer> new_node)
    {
        if ( current == null )
            return new_node;

        if ( new_node.get_value() < current.get_value() )
            current.set_left( insert(current.get_left(), new_node) );

        else if ( new_node.get_value() > current.get_value() )
            current.set_right( insert(current.get_right(), new_node) );

        return current;
    }


    private void pre_order_traversal(Node<Integer> current)
    {
        if ( current == null)
            return;

        System.out.println(Integer.toString( current.get_value() ) + ' ');
        pre_order_traversal(current.get_left());
        pre_order_traversal(current.get_right());
    }


    private void in_order_traversal(Node<Integer> current)
    {
        if ( current == null )
            return;

        in_order_traversal(current.get_left());
        System.out.println(Integer.toString( current.get_value() ) + ' ');
        in_order_traversal(current.get_right());
    }


    private void post_order_traversal(Node<Integer> current)
    {
        if ( current == null )
            return;

        post_order_traversal(current.get_left());
        post_order_traversal(current.get_right());
        System.out.println(Integer.toString( current.get_value() ) + ' ');
    }


    private Node<Integer> delete(Node<Integer> current, int value)
    {
        if ( current == null )
            return null;

        if ( value < current.get_value() )
            current.set_left( delete(current.get_left(), value) );

        else if ( current.get_value() < value )
            current.set_right( delete(current.get_right(), value) );

        else
        {
            if ( current.get_left() == null || current.get_right() == null )
                if ( current.get_left() == null )
                    return current.get_right();

                else
                    return current.get_left();

            else
            {
                current.set_value( this.min( current.get_right() ) );
                current.set_right( delete(current.get_right(), current.get_value()) );
            }
        }

        return current;
    }


    private int min(Node<Integer> current)
    {
        if ( current.get_left() == null )
            return current.get_value();

        return min(current.get_left());
    }
}
