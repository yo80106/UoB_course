import com.alexmerz.graphviz.*;
import com.alexmerz.graphviz.objects.*;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;

public class GraphParser {
    LocationList locationlist = new LocationList();

    public GraphParser() {
    }
    public LocationList Parse(String filename) {
        try {
            Parser parser = new Parser();
            FileReader reader = new FileReader(filename);
            parser.parse(reader);
            ArrayList<Graph> graphs = parser.getGraphs();
            ArrayList<Graph> subGraphs = graphs.get(0).getSubgraphs();
            for(Graph g : subGraphs){
                ArrayList<Graph> subGraphs1 = g.getSubgraphs();
                for (Graph g1 : subGraphs1){
                    ArrayList<Node> nodesLoc = g1.getNodes(false);
                    Node nLoc = nodesLoc.get(0);
                    Locations location = new Locations(nLoc.getId().getId(), nLoc.getAttribute("description"));
                    ArrayList<Graph> subGraphs2 = g1.getSubgraphs();
                    for (Graph g2 : subGraphs2) {
                        ArrayList<Node> nodesEnt = g2.getNodes(false);
                        for (Node nEnt : nodesEnt) {
                            switch (g2.getId().getId()) {
                                case "artefacts":
                                    location.addArtefacts(nEnt.getId().getId(), nEnt.getAttribute("description"));
                                    break;
                                case "furniture":
                                    location.addFurniture(nEnt.getId().getId(), nEnt.getAttribute("description"));
                                    break;
                                case "characters":
                                    location.addCharacters(nEnt.getId().getId(), nEnt.getAttribute("description"));
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    locationlist.addLoctions(location);
                }
                ArrayList<Edge> edges = g.getEdges();
                for (Edge e : edges){
                    String source = e.getSource().getNode().getId().getId();
                    String target = e.getTarget().getNode().getId().getId();

                    if (locationlist.checkLocations(source)) {
                        locationlist.getLocations(source).addPaths(target);
                    }
                }
            }
        } catch (FileNotFoundException fnfe) {
            System.out.println(fnfe);
        } catch (com.alexmerz.graphviz.ParseException pe) {
            System.out.println(pe);
        }
        return locationlist;
    }
}
