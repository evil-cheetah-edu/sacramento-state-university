import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import queue.Queue;


public class Tree
{
    private Node<Integer> root;


    Tree()
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

            case BREADTH_FIRST:
                breadth_first_traversal(root);
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

        return rebalance(current);
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

        return rebalance(current);
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


    private void breadth_first_traversal(Node<Integer> root)
    {
        Queue<Node<Integer>> current_level = new Queue<>();
        Queue<Node<Integer>> next_level    = new Queue<>();

        current_level.enqueue(root);

        while ( !current_level.is_empty() )
        {
            Node<Integer> current = current_level.dequeue();
            System.out.printf(
                "%d(%d, %d) ",
                current.get_value(),
                height(current),
                balance(current)
            );

            if ( current.get_left() != null )
                next_level.enqueue( current.get_left() );

            if ( current.get_right() != null )
                next_level.enqueue( current.get_right() );

            if ( current_level.is_empty() )
            {
                current_level = next_level;
                next_level    = new Queue<>();
                System.out.println();
            }
        }
    }


    private Node<Integer> rebalance(Node<Integer> node)
    {
        if ( node == null )
            return null;

        node.set_height(
            Math.max( height(node.get_left()), height(node.get_right()) ) + 1
        );

        if ( balance(node) == 2 )
            if ( balance(node.get_left()) == -1 )
            {
                node.set_left( rotate_left(node.get_left()) );
                return rotate_right(node);
            }
            else
            {
                return rotate_right(node);
            }

        if ( balance(node) == -2 )
            if ( balance(node.get_right()) == 1 )
            {
                node.set_right( rotate_right(node.get_right()) );
                return rotate_left(node);
            }
            else
            {
                return rotate_left(node);
            }

        return node;
    }


    private Node<Integer> rotate_left(Node<Integer> X)
    {
        /*
         *    X            Y 
         *     \          /
         *      Y   ->   X
         *     /          \
         *    Z            Z
        **/
        
        Node<Integer> Y = X.get_right();
        Node<Integer> Z = Y.get_left();

        Y.set_left(  X );
        X.set_right( Z );

        X.set_height(
                Math.max( height(X.get_left()), height(X.get_right()) ) + 1
        );
        Y.set_height(
                Math.max( height(Y.get_left()), height(Y.get_right()) ) + 1
        );

        return Y;
    }


    private Node<Integer> rotate_right(Node<Integer> X)
    {
        /*
         *      X        Y
         *     /          \
         *    Y     ->     X
         *     \          /
         *      Z        Z
        **/

        Node<Integer> Y = X.get_left();
        Node<Integer> Z = Y.get_right();

        Y.set_right( X );
        X.set_left(  Z );

        X.set_height(
            Math.max( height(X.get_left()), height(X.get_right()) ) + 1
        );
        Y.set_height(
            Math.max( height(Y.get_left()), height(Y.get_right()) ) + 1
        );

        return Y;
    }


    private int min(Node<Integer> current)
    {
        if ( current.get_left() == null )
            return current.get_value();

        return min(current.get_left());
    }


    private int balance(Node<Integer> node)
    {
        return height(node.get_left()) - height(node.get_right());
    }

    private int height(Node<Integer> node)
    {
        if ( node == null )
            return 0;

        return node.get_height();
    }
}
