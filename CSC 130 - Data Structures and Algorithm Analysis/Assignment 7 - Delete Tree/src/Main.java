public class Main
{
    public static void main(String[] args)
    {
        BinaryTree tree = new BinaryTree();

        tree.read_file("input.txt");

        System.out.println("Pre Order:");
        tree.traverse(TraversalType.PRE_ORDER);

        System.out.println("In Order:");
        tree.traverse(TraversalType.IN_ORDER);

        System.out.println("Post Order:");
        tree.traverse(TraversalType.POST_ORDER);

        System.out.println("Breadth First:");
        tree.traverse(TraversalType.BREADTH_FIRST);
    }
}