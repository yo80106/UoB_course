import java.util.ArrayList;

public class Locations {
    String name;
    String description;
    ArrayList<String> paths = new ArrayList<String>();
    ArrayList<String[]> artefacts = new ArrayList<String[]>();
    ArrayList<String[]> furniture = new ArrayList<String[]>();
    ArrayList<String[]> characters = new ArrayList<String[]>();

    public Locations(String name, String description) {
        this.name = name;
        this.description = description;
    }
    
    public String getLocationName() {
        return name;
    }

    public String getLocationDesc() {
        return description;
    }

    public void addPaths(String target) {
        paths.add(target);
    }

    public void addArtefacts(String name, String description) {
        String art[] = new String[2]; 
        art[0] = name;
        art[1] = description;
        artefacts.add(art);
    }

    public void addFurniture(String name, String description) {
        String furn[] = new String[2]; 
        furn[0] = name;
        furn[1] = description;
        furniture.add(furn);
    }

    public void addCharacters(String name, String description) {
        String chara[] = new String[2]; 
        chara[0] = name;
        chara[1] = description;
        characters.add(chara);
    }

    public ArrayList<String[]> getArtefacts() {
        return artefacts;
    }

    public ArrayList<String[]> getFurniture() {
        return furniture;
    }

    public ArrayList<String[]> getCharacters() {
        return characters;
    }

    public ArrayList<String> getPaths() {
        return paths;
    }
    
    public String[] selectArtefacts(String name) {
        for (String[] item : artefacts) {
            if (item[0].equals(name)) {
                return item;
            }
        }
        return null;
    }

    public boolean checkArtefacts(String checkitem) {
        for (String[] item : artefacts) {
            if (item[0].equals(checkitem)) {
                return true;
            }
        }
        return false;
    }

    public void removeArtefacts(String reomveitem) {
        int i;

        for (i = 0; i < artefacts.size(); i++) {
            if (artefacts.get(i)[0].equals(reomveitem)) {
                artefacts.remove(i);
            }
        }
    }

    public boolean checkFurniture(String checkitem) {
        for (String[] item : furniture) {
            if (item[0].equals(checkitem)) {
                return true;
            }
        }
        return false;
    }

    public void removeFurniture(String reomveitem) {
        int i;

        for (i = 0; i < furniture.size(); i++) {
            if (furniture.get(i)[0].equals(reomveitem)) {
                furniture.remove(i);
            }
        }
    }

    public boolean checkCharacters(String checkitem) {
        for (String[] item : characters) {
            if (item[0].equals(checkitem)) {
                return true;
            }
        }
        return false;
    }

    public boolean checkPaths(String checkitem) {
        for (String item : paths) {
            if (item.equals(checkitem)) {
                return true;
            }
        }
        return false;
    }
}
