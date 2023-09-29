import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;


public class Main
{
    public static void main(String[] args)
    {
        LinkedList list = new LinkedList();
        Scanner scanner;

        try
        {
            File    file = new File("input.txt");
                 scanner = new Scanner(file);
        }
        catch (FileNotFoundException e)
        {
            System.out.println("Specified file was not found...");
            throw new RuntimeException(ExceptionMessage.FILE_NOT_FOUND);
        }

        while (scanner.hasNext())
        {
            String next = scanner.next();

            if ( next.equalsIgnoreCase("Delete") )
            {
                list.remove(scanner.next());
                continue;
            }
            list.insert(next);
        }

        System.out.println("Ascending Order:");
        list.traverse();

        System.out.println("\nDescending Order...");
        list.traverse(OrderingType.DESCENDING);
    }
}
