import java.util.ArrayList;

public class Playerslist {
    ArrayList<Players> playerlist = new ArrayList<Players>();

    public Playerslist() {
    }

    public void addPlayer(Players player) {
        playerlist.add(player);
    }

    public boolean checkPlayer(String name) {
        for (Players player : playerlist) {
            if (player.getPlayerName().equals(name)) {
                return true;
            }
        }
        return false;
    }

    public ArrayList<String> getPlayerInLocation(String name, String place) {
        ArrayList<String> players = new ArrayList<String>();

        for (Players player : playerlist) {
            if (player.getCurrentPlace().equals(place) & !player.getPlayerName().equals(name)) {
                players.add(player.getPlayerName());
            }
        }
        return players;
    }

    public Players getPlayer(String name) {
        for (Players player : playerlist) {
            if (player.getPlayerName().equals(name)) {
                return player;
            }
        }
        return null;
    }
}
