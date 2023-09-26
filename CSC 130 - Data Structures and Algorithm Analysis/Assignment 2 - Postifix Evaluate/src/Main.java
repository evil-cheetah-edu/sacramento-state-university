import java.util.Scanner;

public class Main
{
    public static void main(String[] args)
    {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Enter an expression:");
        String input = scanner.next();

        System.out.println(
                "Original Expression: " + input
        );
        System.out.println(
                "Postfix Notation:    " + Postfix.to_postfix(input)
        );
        System.out.println(
                "Result:              " + Integer.toString(Postfix.evaluate((input)))
        );

        scanner.close();
    }
}