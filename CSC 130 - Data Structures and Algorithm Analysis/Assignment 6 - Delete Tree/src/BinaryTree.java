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
        root = insert(root, new Node<Integer>(value));
    }


    private Node<Integer> insert(Node<Integer> current, Node<Integer> new_node)
    {
        if ( current == null )
            return new_node;

        if ( new_node.get_value() < current.get_value() )
            current.set_left( new_node );

        else if ( new_node.get_value() > current.get_value() )
            current.set_right( new_node );

        return current;
    }
}
