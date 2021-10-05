import java.io.*;
import java.net.*;
import java.util.ArrayList;

class StagServer
{
    // locationlist_reserve is for keeping entity list
    Playerslist playerslist = new Playerslist();
    LocationList locationlist = new LocationList();
    LocationList locationlist_reserve = new LocationList();
    ActionsList actionlist = new ActionsList();
    Players player;
    public static void main(String args[])
    {
        if(args.length != 2) System.out.println("Usage: java StagServer <entity-file> <action-file>");
        else new StagServer(args[0], args[1], 8888);
    }

    public StagServer(String entityFilename, String actionFilename, int portNumber)
    {
        GraphParser graphParser = new GraphParser();
        GraphParser graphParser_reserve = new GraphParser();
        JsonParser jsonParser = new JsonParser();
        try {
            ServerSocket ss = new ServerSocket(portNumber);
            System.out.println("Server Listening");
            locationlist = graphParser.Parse(entityFilename);
            locationlist_reserve = graphParser_reserve.Parse(entityFilename);
            actionlist = jsonParser.Parse(actionFilename);

            while(true) acceptNextConnection(ss);
        } catch(IOException ioe) {
            System.err.println(ioe);
        }
    }

    private void acceptNextConnection(ServerSocket ss)
    {
        try {
            // Next line will block until a connection is received
            Socket socket = ss.accept();
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            BufferedWriter out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            processNextCommand(in, out);
            out.close();
            in.close();
            socket.close();
        } catch(IOException ioe) {
            System.err.println(ioe);
        }
    }

    private void processNextCommand(BufferedReader in, BufferedWriter out) throws IOException
    {
        String line = in.readLine();
        Tokenizer tokenizer = new Tokenizer(line);
        String username;
        String currentplace;
        Locations location;

        tokenizer.tmpCommands = tokenizer.splitCommand();
        tokenizer.commands = tokenizer.tokenize();

        // Check if user has entered any commands
        if (tokenizer.commands.size() == 1) {
            return;
        }

        username = tokenizer.commands.get(0).replace(":", "");

        if (playerslist.checkPlayer(username) == false) {
            player = new Players(username, locationlist.getStartLocation());
            playerslist.addPlayer(player);
        }else {
            player = playerslist.getPlayer(username);
        }

        currentplace = player.getCurrentPlace();
        location = locationlist.getLocations(currentplace);
        

        switch (tokenizer.commands.get(1)) {
            case "look":
                out.write(ViewPlace(currentplace));
                break;
            case "get":
                out.write(GetCommand(location, player, tokenizer.commands.get(2)));
                break;
            case "inv":
                out.write("You have: " + 
                        makeDescList(player.getInventory()));
                break;
            case "drop":
                out.write(DropCommand(location, player, tokenizer.commands.get(2)));
                break;
            case "goto":
                out.write(GotoCommand(location, player, tokenizer.commands.get(2)));
                break;
            case "health":
                out.write("Your health: " + player.getHealth());
                break;
            default:
                if (tokenizer.commands.size() <= 2) {
                    out.write("Unknown subject for your action.");
                } else {
                    out.write(ActionsCommand(location, player, tokenizer.commands.get(1), tokenizer.commands.get(2)));
                }
                break;
        }
    }

    public String ViewPlace(String currentplace) {
        String output;
        ArrayList<String[]> artefacts = locationlist.getLocations(currentplace).getArtefacts();
        ArrayList<String[]> furniture = locationlist.getLocations(currentplace).getFurniture();
        ArrayList<String[]> characters = locationlist.getLocations(currentplace).getCharacters();
        ArrayList<String> paths = locationlist.getLocations(currentplace).getPaths();
        String description = locationlist.getLocations(currentplace).getLocationDesc();
        ArrayList<String> players = new ArrayList<String>();

        // Check if there is other players in this location
        if (playerslist.getPlayerInLocation(player.getPlayerName(), currentplace).size() != 0) {
            players = playerslist.getPlayerInLocation(player.getPlayerName(), currentplace);
        }

        output = "You are in " + description + ". You can see:" + "\n" +
                makeDescStr(players) +
                makeDescList(artefacts) + 
                makeDescList(furniture) +
                makeDescList(characters) +
                "You can access from here:" + "\n" +
                makeDescStr(paths);
        return output;
    }
    
    public String makeDescList(ArrayList<String[]> list) {
        String tmp = "";
        for (String[] item : list) {
            tmp += item[1] + "\n";
        }
        return tmp;
    }

    public String makeDescStr(ArrayList<String> list) {
        String tmp = "";
        for (String str : list) {
            tmp += str + "\n";
        }
        return tmp;
    }

    public String GetCommand(Locations location, Players player, String item) {
        if (location.checkArtefacts(item)) {
            player.addArtefacts(location.selectArtefacts(item));
            location.removeArtefacts(item);
            return "You picked up a " + item + "\n";
        }
        return "Cannot find " + item + " in this place.";
    }

    public String DropCommand(Locations location, Players player, String item) {
        String[] tmp;

        if (player.checkInventory(item)) {
            tmp = player.selectInventory(item);
            location.addArtefacts(tmp[0], tmp[1]);
            player.removeInventory(item);
            
            return "You dropped a " + item + "\n";
        }
        return "Cannot find " + item + " in your inventory.";
    }

    public String GotoCommand(Locations location, Players player, String place) {
        if (location.checkPaths(place)) {
            player.changePlace(place);
            return ViewPlace(player.getCurrentPlace());
        }
        return "No path to " + place;
    }

    public String ActionsCommand(Locations location, Players player, String command, String input_subject) {
        Actions action;
        String dead_message = "";

        if (actionlist.checkActions(command) == false) {
            return "Unknown action.";
        } else {
            action = actionlist.getActions(command, input_subject);
        }

        // Check if user's inventory or current loaction has this object
        if (action == null) {
            return "There is no " + input_subject + " in your inventory or in this location.";
        }

        for (String subject : action.getSubjects()) {
            if (player.checkInventory(subject) == false) {
                if (location.checkArtefacts(subject) == false & 
                    location.checkFurniture(subject) == false & 
                    location.checkCharacters(subject) == false) {
                    return "There is no " + input_subject + " in your inventory or in this location.";
                }
            } 
        }

        // Check consumed item is in inventory, at current location, or health
        for (String consumed : action.getConsumed()) {
            if (player.checkInventory(consumed)) {
                player.removeInventory(consumed);
            } else if (location.checkArtefacts(consumed)) {
                location.removeArtefacts(consumed);
            } else if (location.checkFurniture(consumed)) {
                location.removeFurniture(consumed);
            } else if (consumed.equals("health")) {
                player.minusHealth();
                if (player.checkIfDead()) {
                    // Player will lose all the items in the inventory
                    if (player.checkInventoryEmpty() == false) {
                        for (String[] item : player.getInventory()) {
                            location.addArtefacts(item[0], item[1]);
                        }
                        player.removeAllInventory();
                    }
                    // Player will return to start location
                    player.changePlace(locationlist.getStartLocation());
                    dead_message = "\nYou are dead.";
                    player.resetHealth();
                }
            }
        }

        // Check produced item is an artefact, a piece of furniture, a location, an character, or health
        // if the produced item did not exist in anywhere before, just add it to current location 
        for (String produced : action.getProduced()) {
            if (locationlist.checkLocations(produced)) {
                location.addPaths(produced);
            } else if (produced.equals("health")) {
                player.addHealth();
            } else if (locationlist_reserve.getArtefacts(produced) != null) {
                location.addArtefacts(locationlist_reserve.getArtefacts(produced)[0], locationlist_reserve.getArtefacts(produced)[1]);
            } else if (locationlist_reserve.getFurniture(produced) != null) {
                location.addFurniture(locationlist_reserve.getFurniture(produced)[0], locationlist_reserve.getFurniture(produced)[1]);
            } else if (locationlist_reserve.getCharater(produced) != null) {
                location.addCharacters(locationlist_reserve.getCharater(produced)[0], locationlist_reserve.getCharater(produced)[1]);
            } else {
                location.addArtefacts(produced, produced);
            }
        }
        return action.getNarration() + dead_message;
    }
}