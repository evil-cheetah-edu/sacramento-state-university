public class Main
{
    public static void main(String[] args)
    {
        BinaryTree tree = new BinaryTree();

        tree.insert(10);
        tree.insert(6);
        tree.insert(2);
        tree.insert(9);
        tree.insert(15);
        tree.insert(11);
        tree.insert(19);

        System.out.println("Pre Order:");
        tree.traverse(TraversalType.PRE_ORDER);

        System.out.println("In Order:");
        tree.traverse(TraversalType.IN_ORDER);

        System.out.println("Post Order:");
        tree.traverse(TraversalType.POST_ORDER);

        System.out.println();
        tree.delete(10);

        System.out.println("Pre Order:");
        tree.traverse(TraversalType.PRE_ORDER);

        System.out.println("In Order:");
        tree.traverse(TraversalType.IN_ORDER);

        System.out.println("Post Order:");
        tree.traverse(TraversalType.POST_ORDER);
    }
}