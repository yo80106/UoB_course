import org.json.simple.*;
import org.json.simple.parser.*;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class JsonParser {
    public JsonParser() {
    }
    public ActionsList Parse(String filename) 
    {    
        ActionsList actionlist = new ActionsList();
        try {
            JSONParser jsonParser = new JSONParser();
            FileReader reader = new FileReader(filename);

            JSONObject jsonObject = (JSONObject) jsonParser.parse(reader);
            JSONArray actionsList = (JSONArray) jsonObject.get("actions");
            
            for (Object item : actionsList) {
                Actions action = new Actions();
                JSONObject itemJSON = (JSONObject) item;
                //System.out.println(itemJSON);
                action = parseAction(itemJSON);
                actionlist.addActions(action);
            }
 
        } catch (FileNotFoundException e) {
            System.out.println(e);
        } catch (IOException e) {
            System.out.println(e);
        } catch (ParseException e) {
            System.out.println(e);
        }
        return actionlist;
    }
 
    public Actions parseAction(JSONObject itemJSON) 
    {
        Actions action = new Actions();
        int i;

        JSONArray triggers = (JSONArray) itemJSON.get("triggers");
        for (i = 0; i < triggers.size(); i++) {
            action.addTriggers((String) triggers.get(i));
        }
        
        JSONArray subjects = (JSONArray) itemJSON.get("subjects");
        for (i = 0; i < subjects.size(); i++) {
            action.addSubjects((String) subjects.get(i));
        }
         
        JSONArray consumed = (JSONArray) itemJSON.get("consumed");
        for (i = 0; i < consumed.size(); i++) {
            action.addConsumed((String) consumed.get(i));
        }

        JSONArray produced = (JSONArray) itemJSON.get("produced");
        for (i = 0; i < produced.size(); i++) {
            action.addProduced((String) produced.get(i));
        }

        String narration = (String) itemJSON.get("narration");
        action.addNarration(narration);
        
        return action;
    }
}
