public class Main
{
    public static void main(String[] args)
    {
        Stack<String> stack = new Stack<>();

        stack.push("Hello!");
        stack.push("World!");
        stack.push("I am in Stack!");

        System.out.println(stack.peek());
        System.out.println(stack.pop());
        System.out.println(stack.pop());

        // # Successfully Throws
        // System.out.println(stack.pop());
    }
}