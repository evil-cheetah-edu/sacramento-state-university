public class Postfix
{
    static int evaluate(String expression)
    {
        Stack<Integer> stack = new Stack<>();
        String postfix = to_postfix(expression);

        for (char c : postfix.toCharArray())
        {
            if (Character.isDigit(c))
            {
                stack.push(Character.getNumericValue(c));
                continue;
            }

            if (stack.peek() == '=')
            {
                if (stack.is_empty())
                    throw new RuntimeException(ExceptionMessage.EMPTY_STACK_EXCEPTION_MESSAGE);

                return stack.pop();
            }

            int a, b;

            a = stack.pop();
            b = stack.pop();

            int result = calculate(a, b, c);
            stack.push(result);
        }

        return stack.pop();
    }

    static String to_postfix(String expression)
    {
        Stack<Character> stack = new Stack<>();
        String result = "";

        for (char c : expression.toCharArray())
        {
            if ( is_operand(c) )
            {
                result += c;
                continue;
            }


            if (stack.is_empty())
                stack.push(c);

            else if (c == '(')
                stack.push(c);

            else if (c == ')')
                result += stack.pop();

            else if (precedence(c) > precedence(stack.peek()))
                stack.push(c);

            else if (precedence(c) <= precedence(stack.peek()))
            {
                while (!stack.is_empty() && precedence(c) <= precedence(stack.peek()))
                    if (!(stack.peek() == '(' || stack.peek() == ')'))
                        result += stack.pop();

                stack.push(c);
            }
        }

        while (!stack.is_empty())
        {
            if (!(stack.peek() == '(' || stack.peek() == ')'))
                result += stack.peek();

            stack.pop();
        }

        return result;
    }

    private static boolean is_operand(char c)
    {
        return (
                Character.isDigit(c)
        );
    }

    private static int precedence(char c)
    {
        switch (c)
        {
            case ('+'):
            case ('-'):
                return 1;

            case ('*'):
            case ('/'):
                return 2;

            default:
                return 0;
        }
    }

    private static int calculate(int a, int b, char operator)
    {
        switch (operator)
        {
            case ('+'):
                return b + a;

            case ('-'):
                return b - a;

            case ('*'):
                return b * a;

            case ('/'):
                return b / a;

            default:
                System.out.println("Specified Operator `" + operator + '`');
                throw new RuntimeException(ExceptionMessage.UNKNOWN_OPERATOR);
        }
    }
}
