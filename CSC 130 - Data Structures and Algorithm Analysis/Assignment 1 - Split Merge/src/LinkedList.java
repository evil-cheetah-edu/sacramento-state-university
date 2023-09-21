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

    void add_to_back(String value)
    {
        Node<String> new_node = new Node<>(value);

        if ( head == null )
        {
            head = new_node;
            return;
        }

        Node<String> current = head;
        while ( current.get_next() != null )
            current = current.get_next();
        current.set_next( new_node );
    }

    void traverse()
    {
        Node<String> current = head;

        while ( current != null )
        {
            System.out.println( current.get_value() );
            current = current.get_next();
        }
    }

    void read_file(String path)
            throws FileNotFoundException
    {
        File file  = new File(path);
        Scanner input = new Scanner(file);

        while ( input.hasNext() )
        {
            this.add_to_back( input.next() );
        }

        input.close();
    }
}
