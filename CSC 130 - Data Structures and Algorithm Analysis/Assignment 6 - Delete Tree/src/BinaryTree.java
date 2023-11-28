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

    void insert(int value)
    {
            root = insert(root, new Node<>(value));
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
}
