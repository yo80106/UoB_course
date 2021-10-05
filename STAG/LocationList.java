import java.util.ArrayList;

public class LocationList {
    ArrayList<Locations> locationlist = new ArrayList<Locations>();
    
    public LocationList() {
    }

    public String getStartLocation() {
        return locationlist.get(0).getLocationName();
    }
    
    public void addLoctions(Locations location) {
        locationlist.add(location);
    }

    public boolean checkLocations(String name) {
        for (Locations location : locationlist) {
            if (location.getLocationName().equals(name)) {
                return true;
            }
        }
        return false;
    }

    public Locations getLocations(String name) {
        for (Locations location : locationlist) {
            if (location.getLocationName().equals(name)) {
                return location;
            }
        }
        return null;
    }

    public String[] getArtefacts(String entity) {
        ArrayList<String[]> artefacts;

        for (Locations location : locationlist) {
            artefacts = location.getArtefacts();

            for (String[] item : artefacts) {
                if (item[0].equals(entity)) {
                    return item;
                }
            }        
        }
        return null;
    }

    public String[] getFurniture(String entity) {
        ArrayList<String[]> furniture;

        for (Locations location : locationlist) {
            furniture = location.getArtefacts();

            for (String[] item : furniture) {
                if (item[0].equals(entity)) {
                    return item;
                }
            }        
        }
        return null;
    }

    public String[] getCharater(String entity) {
        ArrayList<String[]> character;

        for (Locations location : locationlist) {
            character = location.getCharacters();

            for (String[] item : character) {
                if (item[0].equals(entity)) {
                    return item;
                }
            }        
        }
        return null;
    }
}
