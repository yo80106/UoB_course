import java.util.ArrayList;

public class Tokenizer {
    String incomingCommand;
    String[] tmpCommands;
    ArrayList<String> commands = new ArrayList<String>();

    public Tokenizer(String incomingCommand) {
        this.incomingCommand = incomingCommand;
    }

    public String[] splitCommand() {
        // incomingCommand = incomingCommand.replaceAll("[^a-zA-Z0-9]", "");
        tmpCommands = incomingCommand.split(" ");
        return tmpCommands;
    }

    public ArrayList<String> tokenize() {
        for (String w : tmpCommands) {
            commands.add(w);
        }
        return commands;
    }
}
