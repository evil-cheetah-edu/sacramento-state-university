import java.io.FileNotFoundException;

public class Main
{
    public static void main(String[] args)
    {
        LinkedList list = new LinkedList();

        try
        {
            list.read_file("input.txt");
        }
        catch (FileNotFoundException e)
        {
            System.out.println("File not found!");
            throw new RuntimeException(e);
        }

        LinkedList list2 = list.split();

        System.out.println("List 1:");
        list.traverse();

        System.out.println();
        System.out.println("List 2:");
        list2.traverse();

        list.merge(list2);
        System.out.println();
        System.out.println("Merged List:");
        list.traverse();
    }
}
