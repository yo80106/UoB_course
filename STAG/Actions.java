import java.util.ArrayList;

public class Actions {
    ArrayList<String> triggers = new ArrayList<String>();
    ArrayList<String> subjects = new ArrayList<String>();
    ArrayList<String> consumed = new ArrayList<String>();
    ArrayList<String> produced = new ArrayList<String>();
    String narration = new String();

    public Actions() {
    }

    public void addTriggers(String item) {
        triggers.add(item);
    }

    public void addSubjects(String item) {
        subjects.add(item);
    }

    public void addConsumed(String item) {
        consumed.add(item);
    }

    public void addProduced(String item) {
        produced.add(item);
    }

    public void addNarration(String narr) {
        narration = narr;
    }

    public ArrayList<String> getTriggers() {
        return triggers;
    }

    public ArrayList<String> getSubjects() {
        return subjects;
    }

    public ArrayList<String> getConsumed() {
        return consumed;
    }

    public ArrayList<String> getProduced() {
        return produced;
    }

    public String getNarration() {
        return narration;
    }
}
