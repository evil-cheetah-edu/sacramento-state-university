public class Main
{
    public static void main(String[] args)
    {
        Queue<String> queue = new Queue<>();

        queue.enqueue("Hello!");
        queue.enqueue("World!");
        queue.enqueue("I am in Queue!");

        System.out.println(queue.peek());
        System.out.println();

        System.out.println(queue.dequeue());
        System.out.println(queue.dequeue());
        System.out.println(queue.dequeue());
    }
}