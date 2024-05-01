import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

import java.io.FileInputStream;
import java.io.InputStream;

public class Glory {
    public static void main(String[] args) throws Exception {

        ANTLRInputStream inputStream = new ANTLRInputStream(
                new FileInputStream(args[0]));

        try {
            // Get our lexer
            GloryLexer lexer = new GloryLexer(inputStream);
            // Get a list of matched tokens
            CommonTokenStream tokens = new CommonTokenStream(lexer);

            // Pass the tokens to the parsercatcat
            GloryParser parser = new GloryParser(tokens);

            // Specify our entry point
            GloryParser.StatementContext drinkSentenceContext = parser.statement();


            // Walk it and attach our listener
            ParseTreeWalker walker = new ParseTreeWalker();
            DirectiveListener listener = new DirectiveListener();
            walker.walk(listener, drinkSentenceContext);
            LEROptimizeListener optimizeListener = new LEROptimizeListener();
            walker.walk(optimizeListener, drinkSentenceContext);
            
        } catch (Exception e) {
            System.out.println(e.getMessage());
            System.out.println("Invalid Input");
        }
    }


}
