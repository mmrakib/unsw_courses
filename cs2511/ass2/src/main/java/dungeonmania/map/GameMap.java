package dungeonmania.map;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import dungeonmania.Game;
import dungeonmania.entities.Entity;
import dungeonmania.entities.Player;
import dungeonmania.util.Direction;
import dungeonmania.util.Position;

public class GameMap {
    private Game game;
    private Player player;

    private NodeRegistry nodeRegistry = new NodeRegistry();

    /**
     * Initialise the game map
     * 1. pair up portals
     * 2. register all movables
     * 3. register all spawners
     * 4. register bombs and switches
     * 5. more...
     */
    public void init() {
        GameMapInitializer.initialise(game, this);
    }

    public void moveTo(Entity entity, Position position) {
        if (!canMoveTo(entity, position))
            return;

        triggerMovingAwayEvent(entity);
        removeNode(entity);
        entity.setPosition(position);
        addEntity(entity);
        triggerOverlapEvent(entity);
    }

    public void moveTo(Entity entity, Direction direction) {
        if (!canMoveTo(entity, Position.translateBy(entity.getPosition(), direction)))
            return;
        triggerMovingAwayEvent(entity);
        removeNode(entity);
        entity.translate(direction);
        addEntity(entity);
        triggerOverlapEvent(entity);
    }

    private void triggerMovingAwayEvent(Entity entity) {
        List<Runnable> callbacks = new ArrayList<>();
        getEntities(entity.getPosition()).forEach(e -> {
            if (e != entity)
                callbacks.add(() -> e.onMovedAway(this, entity));
        });
        callbacks.forEach(callback -> {
            callback.run();
        });
    }

    private void triggerOverlapEvent(Entity entity) {
        List<Runnable> overlapCallbacks = new ArrayList<>();
        getEntities(entity.getPosition()).forEach(e -> {
            if (e != entity)
                overlapCallbacks.add(() -> e.onOverlap(this, entity));
        });
        overlapCallbacks.forEach(callback -> {
            callback.run();
        });
    }

    public boolean canMoveTo(Entity entity, Position position) {
        Map<Position, GraphNode> nodes = nodeRegistry.getNodes();
        return !nodes.containsKey(position) || nodes.get(position).canMoveOnto(this, entity);
    }

    public Position dijkstraPathFind(Position src, Position dest, Entity entity) {
        return Pathfinder.djikstra(src, dest, entity, this);
    }

    public void removeNode(Entity entity) {
        nodeRegistry.unregister(entity);
    }

    public void destroyEntity(Entity entity) {
        removeNode(entity);
        entity.onDestroy(this);
    }

    public void addEntity(Entity entity) {
        nodeRegistry.register(entity);
    }

    public void addNode(GraphNode node) {
        nodeRegistry.register(node);
    }

    public Entity getEntity(String id) {
        return nodeRegistry.getEntity(id);
    }

    public List<Entity> getEntities(Position p) {
        return nodeRegistry.getEntities(p);
    }

    public List<Entity> getEntities() {
        return nodeRegistry.getEntities();
    }

    public <T extends Entity> List<T> getEntities(Class<T> type) {
        return nodeRegistry.getEntities(type);
    }

    public Player getPlayer() {
        return player;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }

    public Game getGame() {
        return game;
    }

    public void setGame(Game game) {
        this.game = game;
    }

    public NodeRegistry getNodeRegistry() {
        return nodeRegistry;
    }
}
