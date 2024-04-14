/**
 * Grammar: json
 * Grammar Path: https://github.com/antlr/grammars-v4/tree/master/json
 * Reasoning: JSON (JavaScript Object Notation) is the most common
 *            format used for API communication
 * Explanation: The second input is not accepted because glossary is
 *              not enclosed in quotes, causing it not to match any of
 *              the acceptable types specified in
 *              `value` (JSON.g4, Line 28).
**/


public class Main
{
    public static void main(String[] args)
    {
        /// Iterates over the filenames passed as args
        /// Can be initialized manually
        for (String arg : args)
        {
            String status = Recognizer.is_valid(arg) ? "accepted" : "rejected";

            System.out.printf("File: %s is %s\n", arg, status);
        }
    }
}