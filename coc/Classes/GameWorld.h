#pragma once

class MapManager;
class Npc;
class GameObjectManager;
class GameObjectSelectBox;
class BulletManager;
class SpecialEffectManager;
class ForceManager;
class GameUICallbackFunctionsManager;

struct DebugInfo
{
    MapDebugInfo mapDebugInfo;

    int gameObjectCount = 0;
};

class GameWorld : public Node
{
public:
    ~GameWorld();
	CREATE_FUNC(GameWorld);

    GameObject* createGameObject(GameObjectType gameObjectType, ForceType forceType, const string& jobName, const Vec2& position);
    void removeGameObjectBy(int uniqueID);

    void createBullet(BulletType bulletType, int attackerID, int attackTargetID);
    void createSpecialEffect(const string& templateName, const Vec2& inMapPosition, bool isRepeat);

    void onMouseLeftButtonDown();
    void onMouseLeftButtonUp();
    void onMouseRightButtonDown();
    void onMouseRightButtonUp();
    void onMouseMove(EventCustom* eventCustom);
    void onClearDebugDraw();

    void syncCursorPoint(const Vec2& cursorPoint);

    list<Vec2>  computePathList(const Vec2& inMapStartPosition, const Vec2& inMapEndPosition, bool isAllowEndTileNodeToMoveIn = false);
    MapManager* getMapManager();
    const DebugInfo& getDebugInfo();
    void createNpcAroundBaseCamp(ForceType forceType, const string& npcTemplateName, int npcCount);    

    int getEnemyBaseCampUniqueID();
    int getPlayerBaseCampUniqueID();

    void setShiftKeyStatus(bool isPressed);

    bool isMouseClick();
private:
    bool init() override;
    void initEditedGameObjects();
    void initMapGIDTable();

    void update(float deltaTime) override;
    void onMouseScroll(Event* event);
    void constructBuilding();
    void cancelConstructBuilding();

    vector<Vec2> computeNpcCreatePointList(ForceType forceType, int readyToCreateNpcCount);

    bool isLeftButtonMultyClick();

    GameObjectManager* _gameObjectManager = nullptr;
    GameObjectSelectBox* _gameObjectSelectBox = nullptr;
    BulletManager* _bulletManager = nullptr;
    SpecialEffectManager* _specialEffectManager = nullptr;
    MapManager* _mapManager = nullptr;
    ForceManager* _forceManager = nullptr;
    GameUICallbackFunctionsManager* _gameUI = nullptr;

    Vec2 _cursorPoint;
    Vec2 _previousClickedCursorPoint;

    bool _isLeftMouseButtonDown = false;
    bool _isShiftKeyPressed = false;

    int _holdingBuildingID = GAME_OBJECT_UNIQUE_ID_INVALID;

    DebugInfo _debugInfo;

    int _playerBaseCampUniqueID = GAME_OBJECT_UNIQUE_ID_INVALID;
    int _aiBaseCampUniqueID = GAME_OBJECT_UNIQUE_ID_INVALID;
    vector<vector<int>> _mapGIDTable;   // ���������Ѱ��npc������λ�ã������������npc����ô��npc��ռ�õĸ��������ϰ���
};