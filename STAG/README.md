# STAG
This is the final project (assignment) for the Java programming course at UoB of Computer Science MSc. The focus of this assignment is to build a general-purpose socket-server game-engine for text adventure games. A typical game of this genre is illustrated in the screenshot below.

The following sections illustrate different requirements for building a functional system.

## Game Server
Your aim is to build a game engine server that communicates with one or more game clients. Your server must use a ServerSocket to listen on port 8888 for incoming socket connections.

Upon receiving a connection, your server should accept an incoming command from the client and process the actions that have been requested. The server should process the command, change any game state that is required and send a suitable response back to the client. After processing the command from the client, your server MUST close the connection and then listen again for the next connection on port 8888.

It is intended for your game engine to be versatile so that it is able to play any game (that conforms to certain rules). To support this versatility, two configuration files (entities and actions) are passed into the game server when it is run. For example:

```
java StagServer entities.dot actions.json
```

The server will load the game scenario in from these two files, allowing a range of different games to be played.

## Configuration Files
The game description files passed into the server contain the following two aspects of the game:

- Entities: "things" in the game, their structural layout and the relationships between them
- Actions: the dynamic behaviours of the entities within the game

Because these two types of data are very different in nature, we have chosen two different documents formats to represent them:

- DOT: A language for expressing graphs (which is basically what a text adventure game is !)
- JSON: A language for expressing structured data (which we will use to store the actions)

For parsing DOT files you should use the JPGD library. For parsing JSON files you should use the JSON-simple library.

## Game Entities
Each game consists of a number of different "entities" (as described in the "entities.dot" file). There are a number of different types of entity, including:

- Locations: Rooms or places within the game
- Artefacts: Physical "things" within the game (that can be collected by the player)
- Furniture: Physical "things" that are an integral part of a location (they can NOT be collected by the player)
- Characters: Creatures or people involved in game
- Players: A special kind of character that represents the user !

All entities need at least a name and a description, some may need additional attributes as well. It is worth mentioning that any entity names defined in the configuration files will be unique.

It is worth noting that "Locations" are complex constructs and as such have various different attributes in their own right, including:

- Paths to other Locations (note: it is possible for paths to be one-way !)
- Characters that are currently at a Location
- Artefacts that are currently present in a Location
- Furniture that belongs in a Location

Note that every game has a "special" location that is the starting point for an adventure. This starting point is always the first location that is encountered when reading in the "entities" file.

There is another special location called "unplaced" that can be found in the entities file. This location does not appear in the game world, but is rather a container for all of the entities that have no initial location. They need to exist somewhere in the game structure so that they can be defined, but they do not enter the game until an action places then in another location within the game.

The image below visually shows the structure of the basic entities file. As you can see, each location is represented by a box containing a number of different entities (each type of entity being represented by a different shape). The paths between locations are also presented as directed arrows.

## Game Actions
Dynamic behaviours within the game are represented by "Actions", each of which has following elements:

- A set of possible "trigger" words (ANY of which can be used to initiate the action)
- A set of "subjects" entities that are acted upon (ALL of which need to be present to perform the action)
- A set of "consumed" entities that are all removed ("eaten up") by the action
- A set of "produced" entities that are all created ("generated") by the action

Note that "being present" requires the entity to either be in the inventory of the player invoking the action or for that entity to be in the room/location where the action is being performed.

It is worth noting that action names are NOT unique - for example there may be multiple "open" actions that act on different entities. So be careful when storing and accessing actions.

## Communication
In order to communicate with the server, we need an agreed language (otherwise the user won't know what to type to interact with the game !) There are a number of standard "built-in" gameplay commands that your game engine should respond to:

- "inventory" (or "inv" for short): lists all of the artefacts currently being carried by the player
- "get": picks up a specified artefact from current location and puts it into player's inventory
- "drop": puts down an artefact from player's inventory and places it into the current location
- "goto": moves from one location to another (if there is a path between the two)
- "look": describes the entities in the current location and lists the paths to other locations

In addition to the standard "built-in" commands, your game engine should also accept ANY of the trigger keywords from the loaded-in game actions file. You should first verify that the conditions hold to perform the action (i.e. ALL "Subject" entities are available to the player). You must then undertake the relevant additions/removals (consumption/production). Be sure to make your command interpreter as flexible and robust as possible (to deal with "varied" input from the user !)

## Multiplayer
As an extension to the basic game, you might like to add a "health level" feature. Each player should start with a health level of 3. Consumption of "Poisons \& Potions" or interaction with beneficial or dangerous characters will increase or decrease a player's health. You will see in the "extended" actions file the use of the `health` keyword in the `consumed` and `produced` fields.

When a player's health runs out (i.e. reaches zero) they should lose all of the items in their inventory (which are dropped in the location where they ran out of health) and then they should return to the start location. In order to implement these features in your game engine, you should also add a new `health` command keyword that reports back the player's current health level (so the player can keep track of it).

## Results
There were three main data structures to keep the data, which were Actions, Locations, and Players. These data structures were classes that encapsulated all the required variables and methods. For example, in the Player class, the constructer created a name and a place where the player was currently based. Also, I designed three corresponding ArrayList classes to gather individual objects of Actions, Locations, and Players. As for parsing the input files, I developed two parsers, which deal with DOT and JSON format.

To run the programme, users first execute the server side by performing:
```
java StagServer <entities DOT file> <actions JSON file>
```

Next, run the client side by entering:
```
java StagClient <username>
```
The outcome will look like:
