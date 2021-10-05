import java.util.ArrayList;

public class ActionsList {
    ArrayList<Actions> actionlist = new ArrayList<Actions>();

    public ActionsList() {
    }

    public void addActions(Actions action) {
        actionlist.add(action);
    }

    public boolean checkActions(String name) {
        for (Actions action : actionlist) {
            for (String item : action.getTriggers()) {
                if (item.equals(name)) {
                    return true;
                }
            }
        }
        return false;
    }

    public Actions getActions(String name, String input_subject) {
        for (Actions action : actionlist) {
            for (String trigger : action.getTriggers()) {
                if (trigger.equals(name)) {
                    for (String subject : action.getSubjects()) {
                        if (subject.equals(input_subject)) {
                            return action;
                        }
                    }
                }
            }
        }
        return null;
    }
    
}
