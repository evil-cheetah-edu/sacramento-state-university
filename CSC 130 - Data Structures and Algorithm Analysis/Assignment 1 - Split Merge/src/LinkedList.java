import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class LinkedList
{
    private Node<String> head;

    void add_to_front(String value)
    {
        Node<String> new_node = new Node<>();

        new_node.set_value(value);
        new_node.set_next(head);

        head = new_node;
    }

    void traverse()
    {
        Node<String> temp = head;

        while ( temp != null )
        {
            System.out.println(temp.get_value());
            temp = temp.get_next();
        }
    }

    void read_file(String path)
            throws FileNotFoundException
    {
        File file  = new File(path);
        Scanner input = new Scanner(file);

        while ( input.hasNext() )
        {
            this.add_to_front( input.next() );
        }

        input.close();
    }
}
