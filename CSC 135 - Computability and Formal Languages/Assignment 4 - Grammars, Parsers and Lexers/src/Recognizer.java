import org.antlr.v4.runtime.*;

import java.io.*;


public class Recognizer
{
    public static boolean is_valid(String filename)
    {
        try
        {
            CharStream input = CharStreams.fromFileName(filename);

            JSONLexer lexer = new JSONLexer(input);
            lexer.removeErrorListeners();

            CommonTokenStream tokens = new CommonTokenStream(lexer);

            JSONParser parser = new JSONParser(tokens);
            parser.removeErrorListeners();

            parser.json();

            return parser.getNumberOfSyntaxErrors() == 0;
        }
        catch (IOException e)
        {
            System.err.printf("Error reading file %s...\n", filename);
            System.err.println("Check `error.log` for more details");

            Recognizer._save_exception_to_log(e);

            return false;
        }
    }


    private static void _save_exception_to_log(Exception error)
    {
        try (
            PrintStream w =
                new PrintStream("error.log")
        )
        {
            error.printStackTrace(w);
        }
        catch (Exception e)
        {
            System.err.println("An Error occurred while saving error to file...");
        }
    }
}
