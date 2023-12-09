public class Main
{
    public static void main(String[] args)
    {
        Tree tree = new Tree();

        tree.read_file("input.txt");

        System.out.println("Breadth First:");
        tree.traverse(TraversalType.BREADTH_FIRST);
    }
}