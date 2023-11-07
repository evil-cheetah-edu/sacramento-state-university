/// Node.java
public class Node<T>
{
    private T       value;
    private Node<T> next;


    Node()
    {
        value = null;
        next  = null;
    }

    Node(T value)
    {
        this.value = value;
        this.next  = null;
    }


    T get_value()
    {
        return value;
    }

    void set_value(T value)
    {
        this.value = value;
    }


    Node<T> get_next()
    {
        return next;
    }

    void set_next(Node<T> next)
    {
        this.next = next;
    }
}



/// LinkedList.java
public class LinkedList<T>
{
    private Node<T> head;

    Node<T> get_head()
    {
        return head;
    }

    T get_head_value()
    {
        return head.get_value();
    }

    void set_head(Node<T> head)
    {
        this.head = head;
    }

    void add_to_front(T value)
    {
        Node<T> new_node = new Node<>();

        new_node.set_value(value);
        new_node.set_next(head);

        head = new_node;
    }

    void add_to_back(T value)
    {
        Node<T> new_node = new Node<>(value);

        if ( head == null )
        {
            head = new_node;
            return;
        }

        Node<T> current = head;
        while ( current.get_next() != null )
            current = current.get_next();
        current.set_next( new_node );
    }

    void pop_in_front()
    {
        if ( head == null )
            return;

        Node<T> temp = head;
        head = head.get_next();
        temp.set_next(null);
    }

    void traverse()
    {
        Node<T> current = head;

        while ( current != null )
        {
            System.out.println( current.get_value() );
            current = current.get_next();
        }
    }

    LinkedList<T> split()
    {
        Node<T> middle_node = get_pre_last_middle_node();
        Node<T> current     = middle_node.get_next();

        LinkedList<T> list = new LinkedList();

        while ( current != null)
        {
            list.add_to_back( current.get_value() );
            current = current.get_next();
        }

        middle_node.set_next( null );

        return list;
    }

    int size()
    {
        Node<T> current = head;

        int count = 0;
        while (current != null)
        {
            current = current.get_next();
            ++count;
        }

        return count;
    }

    boolean is_empty()
    {
        return head == null;
    }

    void merge(LinkedList list)
    {
        if ( list == null )
            return;

        Node<T> current = list.get_head();

        while ( current != null )
        {
            add_to_back( current.get_value() );
            current = current.get_next();
        }
    }

    private Node<T> get_pre_last_middle_node()
    {
        Node<T> slow = head,
                fast = head;

        while (
                fast.get_next()            != null &&
                fast.get_next().get_next() != null
        )
        {
            slow = slow.get_next();
            fast = fast.get_next().get_next();
        }

        return slow;
    }
}



/// MergeSort.java
import java.util.Comparator;


public class MergeSort
{
   static LinkedList<Integer> sort(LinkedList<Integer> items, Comparator<Integer> comparator)
   {
        if (items.size() == 0 || items.size() == 1)
            return items;

        LinkedList<Integer> left  = items.split();
        LinkedList<Integer> right = items;

        left  = sort(left,  comparator);
        right = sort(right, comparator);

        return merge(left, right, comparator);
   }


    private static LinkedList<Integer> merge(
        LinkedList<Integer> left,
        LinkedList<Integer> right,
        Comparator<Integer> comparator
    )
    {
        LinkedList<Integer> result = new LinkedList<>();

        while ( !left.is_empty() && !right.is_empty() )
        {
            if ( comparator.compare(left.get_head_value(), right.get_head_value()) < 0 )
            {
                result.add_to_back(left.get_head_value());
                left.pop_in_front();
            }
            else
            {
                result.add_to_back(right.get_head_value());
                right.pop_in_front();
            }
        }

        if ( left != null )
            result.merge(left);

        if ( right != null )
            result.merge(right);

        return result;
    }
}



/// Driver.java
import java.util.Comparator;


public class Driver
{
    public static void main(String[] args)
    {
        LinkedList<Integer> list = new LinkedList<>();

        list.add_to_back(10);
        list.add_to_back(-1);
        list.add_to_back(3);
        list.add_to_back(20);
        list.add_to_back(2);

        System.out.println("Original Linked List:");
        list.traverse();
        System.out.println();

        System.out.println("Sorted Linked List(Ascending):");
        list = MergeSort.sort(list, Comparator.naturalOrder());
        list.traverse();
        System.out.println();

        System.out.println("Sorted Linked List(Descending):");
        list = MergeSort.sort(list, Comparator.reverseOrder());
        list.traverse();
    }
}
