import java.util.ArrayList;

public class Players {
    String name = new String();
    int health = 3;
    String currentPlace = new String();
    ArrayList<String[]> inventory = new ArrayList<String[]>();

    public Players(String name, String place) {
        this.name = name;
        this.currentPlace = place;
    }

    public void addArtefacts(String[] item) {
        inventory.add(item);
    }

    public String getPlayerName() {
        return name;
    }

    public String getCurrentPlace() {
        return currentPlace;
    }

    public ArrayList<String[]> getInventory() {
        return inventory;
    }

    public boolean checkInventoryEmpty() {
        System.out.println("Inventoy size: " + inventory.size());
        if (inventory.size() == 0) {
            return true;
        }
        return false;
    }

    public boolean checkInventory(String checkitem) {
        for (String[] item : inventory) {
            if (item[0].equals(checkitem)) {
                return true;
            }
        }
        return false;
    }

    public void removeInventory(String reomveitem) {
        int i;

        for (i = 0; i < inventory.size(); i++) {
            if (inventory.get(i)[0].equals(reomveitem)) {
                inventory.remove(i);
            }
        }
    }

    public void removeAllInventory() {
        ArrayList<String[]> removelist = new ArrayList<String[]>();

        for (String[] item : inventory) {
            removelist.add(item);
        }
        inventory.removeAll(removelist);
    }

    public String[] selectInventory(String name) {
        for (String[] item : inventory) {
            if (item[0].equals(name)) {
                return item;
            }
        }
        return null;
    }

    public void changePlace(String place) {
        currentPlace = place;
    }

    public int getHealth() {
        return health;
    }
    
    public void addHealth() {
        health += 1;
    }

    public void minusHealth() {
        health -= 1;
    }

    public boolean checkIfDead() {
        if (health == 0) {
            return true;
        }
        return false;
    }

    public void resetHealth() {
        health = 3;
    }

}
